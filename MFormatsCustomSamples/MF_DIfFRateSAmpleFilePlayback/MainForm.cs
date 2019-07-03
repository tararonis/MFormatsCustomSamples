using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
using MCHROMAKEYLib;
using MFORMATSLib;
using Microsoft.Win32;
using MLCHARGENLib;
using eMFCC = MFORMATSLib.eMFCC;
using eMFrameClone = MFORMATSLib.eMFrameClone;
using eMFrameFlags = MFORMATSLib.eMFrameFlags;
using eMVideoFormat = MFORMATSLib.eMVideoFormat;
using M_AV_PROPS = MFORMATSLib.M_AV_PROPS;
using M_TIME = MFORMATSLib.M_TIME;

namespace SampleFilePlayback
{
    public partial class MainForm : Form
    {

        enum eState
        {
            eST_PlayFwd,
            eST_Pause,
            eST_PlayRev,
            eST_StepFwd,
            eST_StepRev
        };

        class PlayerState
        {
            public Object stateLock = new Object();
            public eState state;
            public double dblFrameRequest;
            public double dblDuration;
            public double dblRate;
            public string strFileName;
            public bool bLoop;
        }


        private MFReaderClass m_objMFReader;            //MFormats Reader object
        private MFPreviewClass m_objPreview;            //Preview object
        private PlayerState m_playerState;              //Playback state
        private MFRendererClass m_objRenderer;          //MRenderer object
        private MFSinkClass m_objMFSink;                //MFSink object

        private CoMLCharGenClass m_objCharGen;          //CG plugin
        private MCCDisplayClass m_objCCDisplay;         //MCCDisplay plugin
        private MFOverlayHTMLClass m_objOverlayHTML;    //MFOverlayHTML plugin
        private Object m_objHTMLLock = new Object();

        private MFFrame m_frOverlay;            //Frame to overlay
        private MFFactory m_objFactory;			//Frames class used to create MFrame from file
        private Thread m_threadWorker;	//Working thread
        private Object m_objLock = new Object();
        private bool m_bWork = false;
        private bool m_bThread = false;

        private M_AV_PROPS m_avProps;
        private M_TIME mTime;

        private string[] m_arrArgs;

        private string m_strDemoURL = "";

        private string m_CEFResourcesPath = "";

        public MainForm(string[] args)
        {
            m_arrArgs = args;
            InitializeComponent();
        }

        //////////////////////////////////////////////////////////////////////////
        // BG Worker methods

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (NextFrame() == false) return;
            }
        }

        M_AV_PROPS currentProps;
        private bool NextFrame()
        {
            // Get request pos and set pause and reverse flags
            double dblRequest = -1.0;
            string strParams = string.Empty;

            // Update player state
            lock (m_playerState.stateLock)
            {
                if (m_playerState.state == eState.eST_Pause)
                    strParams = " pause=true";
                else if (m_playerState.state == eState.eST_PlayRev || m_playerState.state == eState.eST_StepRev)
                    strParams = " reverse=true";

                // Update player state
                if (m_playerState.state == eState.eST_StepFwd || m_playerState.state == eState.eST_StepRev)
                {
                    // Pause on next iteration - because single frame was requested
                    m_playerState.state = eState.eST_Pause;
                }

                // Get request time and set next cycle request to next frame 
                // -1 as first parameter means "give me next frame", -5 means "give me next next 5th frame" etc,
                // this works accordingly when the reverse=true parameter is set.
                // positive values are uninterpreted as "give me frame at position"
                dblRequest = m_playerState.dblFrameRequest;
                m_playerState.dblFrameRequest = -1 * (int)(m_playerState.dblRate);
            }


            // Next frame cycle:
            // Get frame from reader and send to preview
            // Note: Preview keep frame according to frame time 

            MFFrame pFrame = null;
            lock (m_objLock) // For prevent reader replace in OpenFile() and overlay change
            {
                // Get next frame or frame by position
                // -1 as first parameter means "give me next frame", -5 means "give me next next 5th frame" etc,
                // this works accordingly when the reverse=true parameter is set.
                // positive values are uninterpreted as "give me frame at position"
                try
                {
                    if (m_objMFReader != null)
                        m_objMFReader.SourceFrameConvertedGetByTime(ref m_avProps, dblRequest, -1, out pFrame, strParams + " rate=0.1");
                    int samples;
                    pFrame.MFAVPropsGet(out currentProps, out samples);
                }
                catch (System.Exception ex)
                {
                    MessageBox.Show("Error occurs during file decoding:\n\n" + ex.Message, m_playerState.strFileName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return false;
                }
            }
            
            if (pFrame == null)
            {
                Thread.Sleep(5);
                return true;
            }

            try
            {
                // Demonstrate overlay - overlay another frame if any
                if (m_frOverlay != null)
                    pFrame.MFOverlay(m_frOverlay, null, 50, 100, 0.5, "", "");

                // Show CG object
                if (checkBoxCG.Checked)
                {
                    ShowPluggin(m_objCharGen, ref pFrame);
                    if (cgEditor != null)
                    {
                        cgEditor.m_objEditor_FrameProcessed(pFrame, null);
                    }
                }

                // Show CC
                if (checkBoxCC.Checked)
                {
                    if (m_objCCDisplay != null)
                        ShowPluggin(m_objCCDisplay, ref pFrame);
                }

                // Show HTML
                if (checkBoxHTML.Checked)
                {
                    lock (m_objHTMLLock)
                    {
                        if (m_objOverlayHTML != null)
                        {
                            ShowPluggin(m_objOverlayHTML, ref pFrame);
                            if (overlayHTMLWind != null && !overlayHTMLWind.IsDisposed)
                            {
                                overlayHTMLWind.UpdatePreview(pFrame);
                            }
                        }
                    }
                }

                mAudioMeter1.pFrame = pFrame;
                mAudioMeter1.SetValues();
                // Get frame timings
                pFrame.MFTimeGet(out mTime);

                double dblDur = 0;
                m_objMFReader.ReaderDurationGet(out dblDur);
                if (m_playerState.dblDuration != dblDur)
                    m_playerState.dblDuration = dblDur;

                // Send frame to Renderer
                SendToRenderer(pFrame);

                //Send frame to the preview
                m_objPreview.ReceiverFramePut(pFrame, checkBoxRenderer.Checked ? 0 : -1, "");

                //Send to Sink
                SendToSink(pFrame);

                //Release frame - DO NOT FORGOT TO DO THIS !!!
                releaseComObj(pFrame);
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("Error occurs during frame processing:\n\n" + ex.Message, m_playerState.strFileName, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return false;
            }

            // Check for the last frame
            if ((mTime.eFFlags & eMFrameFlags.eMFF_Last) != 0)
            {
                lock (m_playerState.stateLock)
                {
                    // Pause playback at the end of the file if loop is disabled
                    if (!m_playerState.bLoop)
                        pause();

                    if (m_playerState.state == eState.eST_PlayRev)
                    {
                        // Rewind to end in case of reverse playback
                        rewindToEnd();
                    }
                    else if (m_playerState.state == eState.eST_PlayFwd)
                    {
                        // Rewind to start in case of direct playback
                        rewindToStart();
                    }
                }
            }
            
            return true;
        }

        private void ShowPluggin(object _obj, ref MFFrame pFrame)
        {
            int nFramesRes;
            MFFrame pFrameOut = null;
            ((MFORMATSLib.IMFProcess)_obj).ProcessFrame(pFrame, out pFrameOut, out nFramesRes, "");
            if (pFrameOut != null)
            {
                releaseComObj(pFrame);
                pFrame = pFrameOut;
            }
        }

        private void SendToSink(MFFrame pFrame)
        {
            if (checkBoxVirtDev.Checked)
            {
                if (m_objMFSink != null)
                {
                    lock (m_objLock)
                    {
                        // Send pFrame to Sink
                        try
                        {
                            m_objMFSink.ReceiverFramePut(pFrame, -1, "");
                        }
                        catch
                        { }
                    }
                }
            }
        }

        private void SendToRenderer(MFFrame pFrame)
        {
            if (checkBoxRenderer.Checked)
            {
                if (m_objRenderer != null && comboBoxRenrerers.SelectedItem != null)
                {
                    lock (m_objLock)
                    {
                        // Send pFrame to renderer
                        try
                        {
                            m_objRenderer.ReceiverFramePut(pFrame, -1, "");
                        }
                        catch
                        { }
                    }
                }
            }
        }

        private MAudioChannel[] arrChannels;

        //////////////////////////////////////////////////////////////////////////
        // Form handlers
        private CancellationTokenSource cancelSource;
        private void MainForm_Load(object sender, EventArgs e)
        {

            // Create MF objects
            m_objFactory = new MFFactory();
            m_objPreview = new MFPreviewClass();
            m_playerState = new PlayerState();
            m_objRenderer = new MFRendererClass();
            m_objCharGen = new CoMLCharGenClass();
            m_objMFSink = new MFSinkClass();

            try
            {
                m_objCCDisplay = new MCCDisplayClass();
            }
            catch
            {
                MessageBox.Show("MCCDisplay module was not found.");
            }


            m_CEFResourcesPath = (string)Registry.GetValue(@"HKEY_CURRENT_USER\SOFTWARE\Medialooks\MFormats", "resources.path", null);

            if (File.Exists(m_CEFResourcesPath + "\\CEF\\demo.html"))
            {
                m_strDemoURL = Path.GetFullPath(m_CEFResourcesPath + "\\CEF\\demo.html");
                textBoxHTMLURL.Text = "demo";
            }

            m_playerState.bLoop = checkBoxLoop.Checked;

            // First file start with pause, next file open in same state
            pause();

            //Configure preview
            m_objPreview.PreviewWindowSet("", panelPreview.Handle.ToInt32());
            m_objPreview.PreviewEnable("", Convert.ToInt32(checkBoxAudio.Checked), Convert.ToInt32(checkBoxVideo.Checked));

            FillVideoFormats();

            // Fill renderer devices
            FillRenderers();

            // Fill audio renderers
            FillAudioRenderers();
            CheckForIllegalCrossThreadCalls = false;   //Need for use Combobox in multithreading 

            mAudioMeter1.ChannelsUpdated += MAudioMeter1_ChannelsUpdated;
            // Create thread worker
            //m_bThread = true;
            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();

            if (m_arrArgs.Length > 0 && File.Exists(m_arrArgs[0]))
                OpenFile(m_arrArgs[0]);
        }

        private void MAudioMeter1_ChannelsUpdated(object sender, EventArgs e)
        {
            panelPreview.Size = new Size(mAudioMeter1.Location.X - 6 - panelPreview.Location.X, panelPreview.Size.Height);
        }

        private void FillAudioRenderers()
        {
            int nCount;
            m_objPreview.PropsOptionGetCount("audio_renderer", out nCount);

            if (nCount > 0)
            {
                // Auto width of combobox
                int width = comboBoxAudioRenderers.DropDownWidth;
                Graphics g = comboBoxAudioRenderers.CreateGraphics();
                Font font = comboBoxAudioRenderers.Font;
                int newWidth = 0;

                //Fill the List Box with renderer names
                for (int i = 0; i < nCount; i++)
                {

                    string name, help;
                    m_objPreview.PropsOptionGetByIndex("audio_renderer", i, out name, out help);

                    comboBoxAudioRenderers.Items.Add(name);

                    newWidth = (int)g.MeasureString(name, font).Width;
                    if (width < newWidth)
                    {
                        width = newWidth;
                    }
                }

                comboBoxAudioRenderers.DropDownWidth = width;

                if (comboBoxAudioRenderers.Items.Count > 0)
                    comboBoxAudioRenderers.SelectedIndex = 0;
            }
        }

        private void FillVideoFormats()
        {
            comboBoxVideoFormat.Items.Clear();

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_Custom);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_NTSC);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_NTSC_2398);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_NTSC_16x9);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_PAL);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_PAL_16x9);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD720_50p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD720_5994p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD720_60p);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_2398p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_24p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_25p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_2997p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_30p);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_50i);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_5994i);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_60i);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_50p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_5994p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_HD1080_60p);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_DCI_2398p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_DCI_24p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_DCI_25p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_DCI_50p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_DCI_5994p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_DCI_60p);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_2398p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_24p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_2K_25p);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_DCI_2398p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_DCI_24p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_DCI_25p);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_UHD_50i);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_UHD_5994i);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_UHD_60i);

            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_UHD_50p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_UHD_5994p);
            comboBoxVideoFormat.Items.Add(eMVideoFormat.eMVF_4K_UHD_60p);
            comboBoxVideoFormat.SelectedIndex = 0;

            m_avProps = new M_AV_PROPS();
            m_avProps.vidProps.eVideoFormat = (eMVideoFormat)comboBoxVideoFormat.SelectedItem;
        }

        /// <summary>
        /// fill renderers
        /// </summary>
        private void FillRenderers()
        {
            comboBoxRenrerers.Items.Clear();

            int nCount = 0;

            m_objRenderer.DeviceGetCount(eMFDeviceType.eMFDT_Video, out nCount);

            if (nCount > 0)
            {
                comboBoxRenrerers.Enabled = true;
                checkBoxRenderer.Enabled = true;

                for (int y = 0; y < nCount; y++)
                {
                    string strName;
                    int nBusy;
                    m_objRenderer.DeviceGetByIndex(eMFDeviceType.eMFDT_Video, y, out strName, out nBusy);
                    comboBoxRenrerers.Items.Add(strName);
                }

                // Choose 0 index of renderer
                comboBoxRenrerers.SelectedIndex = 0;
            }
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            cancelSource.Cancel();
            // Release Reader (the worker could be in running state at this moment -> use lock)

            m_threadWorker.Join(1000);

            if (m_objMFReader != null)
            {
                m_objMFReader.ReaderClose();
                releaseComObj(m_objMFReader);
            }

            m_objMFSink.SinkClose();
        }

        private void buttonLoadFile_Click(object sender, EventArgs e)
        {
            // Open next file
            openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == DialogResult.OK && openFileDialog.FileName != string.Empty)
                OpenFile(openFileDialog.FileName);
        }

        private void OpenFile(string _filename)
        {
            mAudioMeter1.SizeChanged -= mAudioMeter1_SizeChanged;
            mAudioMeter1.SizeChanged += mAudioMeter1_SizeChanged;
            // Open next file
            // Change current reader with new one
            lock (m_objLock) // For preview access from worker thread
            {
                try
                {
                    if (m_objMFReader == null)
                        m_objMFReader = new MFReaderClass();

                    m_avProps = new M_AV_PROPS();
                    m_avProps.vidProps.eVideoFormat = (eMVideoFormat)comboBoxVideoFormat.SelectedItem;

                    m_objMFReader.ReaderOpen(_filename, "");
                    checkBoxCG.Enabled = true;
                    checkBoxHTML.Enabled = true;
                    textBoxHTMLURL.Enabled = true;
                    pause();
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error open file:" + _filename + "\n\n" + ex.Message);
                    return;
                }

                //Get file duration
                m_objMFReader.ReaderDurationGet(out m_playerState.dblDuration);
                m_playerState.strFileName = _filename;


                // Update audio track & TS programm Combo
                getAudioTracks();
                getTSPrograms();
                MFFrame pFrame;
                m_objMFReader.SourceFrameGetByTime(-1, -1, out pFrame, "");
                if (pFrame != null)
                {
                    mAudioMeter1.pFrame = pFrame;
                    mAudioMeter1.UpdateControl();
                    mAudioMeter1.SetValues();
                }

                if (m_threadWorker == null || !m_threadWorker.IsAlive )
                {
                    m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
                    m_threadWorker.Name = "thread_DoWork";
                    m_threadWorker.Start();
                }
            }

            rewindToStart();

            GC.Collect();
        }

        private void comboBoxAudioTrack_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Set audio track by index
            m_objMFReader.PropsSet("audio_track", comboBoxAudioTrack.SelectedIndex.ToString());

            MFFrame pFrame;
            m_objMFReader.SourceFrameGetByTime(-1, -1, out pFrame, "");
            mAudioMeter1.pFrame = pFrame;
            mAudioMeter1.UpdateControl();
            mAudioMeter1.SetValues();
        }


        private void comboBoxTSProg_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Set TS program by index
            m_objMFReader.PropsSet("ts_program", comboBoxTSProg.SelectedIndex.ToString());
            MFFrame pFrame;
            m_objMFReader.SourceFrameGetByTime(-1, -1, out pFrame, "");
            mAudioMeter1.pFrame = pFrame;
            mAudioMeter1.UpdateControl();
            mAudioMeter1.SetValues();
        }

        private void buttonPlay_Click(object sender, EventArgs e)
        {
            play();

            // Start worker if not started yet
            //m_bWork = true;
        }

        private void buttonStop_Click(object sender, EventArgs e)
        {
            pause();
            rewindToStart();
        }

        private void buttonFrFwd_Click(object sender, EventArgs e)
        {
            stepFwd();
        }

        private void buttonFrBack_Click(object sender, EventArgs e)
        {
            stepBack();
        }

        private void trackBarSeek_ValueChanged(object sender, EventArgs e)
        {
            // Seek to position
            double dblPos = ((double)trackBarSeek.Value / (double)trackBarSeek.Maximum) * m_playerState.dblDuration;

            if (dblPos < m_playerState.dblDuration)
                seek(dblPos);
        }

        private void checkBoxLoop_CheckedChanged(object sender, EventArgs e)
        {
            m_playerState.bLoop = checkBoxLoop.Checked;
        }

        private void buttonPause_Click(object sender, EventArgs e)
        {
            pause();
        }

        private void buttonReverse_Click(object sender, EventArgs e)
        {
            reverse();
        }

        private void buttonFastBackward_Click(object sender, EventArgs e)
        {
            fastBackw();
        }

        private void buttonFastForward_Click(object sender, EventArgs e)
        {
            fastFwd();
        }

        private void buttonGoToPos_Click(object sender, EventArgs e)
        {
            // Parse string to position and seek to pos
            string strTime = textBoxToPos.Text;
            double dblPos = ParseTime(ref strTime);
            seek(dblPos);
        }

        private void textBoxToPos_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return || e.KeyCode == Keys.Tab)
            {
                // Parse string to position and seek to pos
                string strTime = textBoxToPos.Text;
                double dblPos = ParseTime(ref strTime);
                if (dblPos >= 0)
                {
                    textBoxToPos.Text = strTime;
                    seek(dblPos);
                }

                e.Handled = true;
                e.SuppressKeyPress = true;
            }
        }

        private void trackBarSeek_MouseDown(object sender, MouseEventArgs e)
        {
            // Jump to the clicked location
            double dblValue = ((double)(e.X) / (double)(trackBarSeek.Width)) * (trackBarSeek.Maximum - trackBarSeek.Minimum);
            if (dblValue >= 0)
                trackBarSeek.Value = Convert.ToInt32(dblValue);
        }

        private void panelPreview_SizeChanged(object sender, EventArgs e)
        {
            if (m_objMFReader != null && m_objPreview != null)
            {
                // Update preview size
                m_objPreview.PreviewWindowSet("", panelPreview.Handle.ToInt32());
            }
        }

        private void buttonLogo_Click(object sender, EventArgs e)
        {
            openFileDialog = new System.Windows.Forms.OpenFileDialog();
            openFileDialog.Filter = "Image Files |*.jpeg; *.jpg; *.bmp; *.png";
            if (openFileDialog.ShowDialog() == DialogResult.OK && openFileDialog.FileName != string.Empty)
            {
                // Create new MFrame from image file
                MFFrame frOverlay;
                m_objFactory.MFFrameCreateFromFile(openFileDialog.FileName, out frOverlay, "");

                lock (m_objLock)
                {
                    if (m_frOverlay != null)
                    {
                        releaseComObj(m_frOverlay);
                        m_frOverlay = null;
                    }

                    // Resize Overlay to 300x200 (for example) 
                    frOverlay.MFResize(eMFCC.eMFCC_Default, 300, 200, 0, out m_frOverlay, "", "");

                    releaseComObj(frOverlay);
                }
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // Player control

        private void play()
        {
            lock (m_playerState.stateLock)
            {
                // Set direct playback state
                m_playerState.dblRate = 1;
                m_playerState.state = eState.eST_PlayFwd;
            }
        }

        private void reverse()
        {
            lock (m_playerState.stateLock)
            {
                // Set reverse playback state
                m_playerState.dblRate = 1;
                m_playerState.state = eState.eST_PlayRev;
            }
        }

        private void pause()
        {
            lock (m_playerState.stateLock)
            {
                // Set pause state
                m_playerState.dblRate = 1;
                m_playerState.state = eState.eST_Pause;
            }
        }

        private void stepFwd()
        {
            lock (m_playerState.stateLock)
            {
                // Set single frame forward state
                m_playerState.state = eState.eST_StepFwd;
            }
        }

        private void stepBack()
        {
            lock (m_playerState.stateLock)
            {
                // Set single frame backward state
                m_playerState.state = eState.eST_StepRev;
            }
        }

        private void fastFwd()
        {
            lock (m_playerState.stateLock)
            {
                // Set needed rate and forward playback state
                if (m_playerState.state == eState.eST_PlayFwd)
                {
                    m_playerState.dblRate += 1.0;
                }
                else
                {
                    m_playerState.dblRate = 2.0;
                    m_playerState.state = eState.eST_PlayFwd;
                }
            }
        }

        private void fastBackw()
        {
            lock (m_playerState.stateLock)
            {
                // Set needed rate and reverse playback state
                if (m_playerState.state == eState.eST_PlayRev)
                {
                    m_playerState.dblRate += 1.0;
                }
                else
                {
                    m_playerState.dblRate = 2.0;
                    m_playerState.state = eState.eST_PlayRev;
                }
            }
        }

        private void seek(double Pos)
        {
            lock (m_playerState.stateLock)
            {
                // Request frame at the specified position
                m_playerState.dblFrameRequest = Pos;
            }

            // Start worker (e.g. if error during playback occurs)
            //m_bWork = true;
        }

        private void rewindToStart()
        {
            // Seek to zero position
            seek(0);
        }

        private void rewindToEnd()
        {
            // Seek to duration
            seek(m_playerState.dblDuration);
        }

        //////////////////////////////////////////////////////////////////////////
        // Update controls methods

        private void getAudioTracks()
        {
            // Get audio tracks count
            string strTracksCount;
            m_objMFReader.PropsGet("info::audio_tracks", out strTracksCount);
            if (strTracksCount == null) return;
            int nTracksCount = Int32.Parse(strTracksCount);

            comboBoxAudioTrack.Items.Clear();
            for (int i = 0; i < nTracksCount; i++)
            {
                comboBoxAudioTrack.Items.Add(i.ToString());
            }

            // Get current audio track
            string strCurrTrack;
            m_objMFReader.PropsGet("audio_track", out strCurrTrack);
            int nCurrTrack = Int32.Parse(strCurrTrack);

            if (nTracksCount > 0 && nCurrTrack >= 0 && nCurrTrack <= nTracksCount)
                comboBoxAudioTrack.SelectedIndex = nCurrTrack;
        }

        private void getTSPrograms()
        {
            // Get TS programs count
            string strProgramsCount;
            m_objMFReader.PropsGet("info::ts_programs", out strProgramsCount);
            if (strProgramsCount == null) return;
            int nProgramsCount = Int32.Parse(strProgramsCount);

            comboBoxTSProg.Items.Clear();
            for (int i = 0; i < nProgramsCount; i++)
            {
                comboBoxTSProg.Items.Add(i.ToString());
            }

            // Get current TS program
            string strCurrProgram;
            m_objMFReader.PropsGet("ts_program", out strCurrProgram);
            int nCurrProgram = Int32.Parse(strCurrProgram);

            if (nProgramsCount > 0 && nCurrProgram >= 0 && nCurrProgram <= nProgramsCount)
                comboBoxTSProg.SelectedIndex = nCurrProgram;
        }

        double dblPos = 0;
        private void update_UI(M_TIME mTime)
        {
            if(currentProps.vidProps.dblRate!=0)
                dblPos = (double)mTime.tcFrame.nExtraCounter / currentProps.vidProps.dblRate;            

            labelPos.Width = (int)((trackBarSeek.Width - 16) * dblPos / m_playerState.dblDuration);

            // Update fps
            string strFPS;
            ((IMFProps)m_objPreview).PropsGet("stat::fps_in", out strFPS);
            string strAudioDrop;
            ((IMFProps)m_objPreview).PropsGet("stat::audio_drop", out strAudioDrop);
            string strAudioDup;
            ((IMFProps)m_objPreview).PropsGet("stat::audio_dup", out strAudioDup);
            string strHWAcc = " ";
            if (m_objMFReader != null) ((IMFProps)m_objMFReader).PropsGet("info::hw_acceleration", out strHWAcc);
            labelStatus.Text = Path.GetFileName(m_playerState.strFileName) + " | PlayerState = " + m_playerState.state + " | Pos = " + (secTime2DTime(dblPos)).ToString("HH:mm:ss.fff") + " / " + (secTime2DTime(m_playerState.dblDuration)).ToString("HH:mm:ss.fff") + "\r\n" +
                "Audio Drops: " + strAudioDrop + "   Audio Dups: " + strAudioDup + "   Fps:" + strFPS + "   Rate:" + m_playerState.dblRate + "   Decoding mode: " + (String.IsNullOrEmpty(strHWAcc) ? "CPU" : strHWAcc);
        }



        //////////////////////////////////////////////////////////////////////////
        // Helper methods

        private DateTime secTime2DTime(double dblTime)
        {
            DateTime res = new DateTime();
            // Convert time in seconds to DateTime            
            res = res.AddMilliseconds(Math.Abs(dblTime) * 1000);
            return res;
        }

        private void releaseComObj(object comObj)
        {
            if (comObj != null)
                Marshal.ReleaseComObject(comObj);
        }

        // Time in next format:
        // SS / SS.MSec
        // MM:SS  / MM:SS.MSec
        // HH:MM:SS / HH:MM:SS.MSec
        public double ParseTime(ref string _strTime)
        {
            double dblMSec = 0;
            int nSec = 0, nMinutes = 0, nHours = 0;

            // Get msec and cut-off last part
            int nMsecPos = _strTime.LastIndexOf('.');
            if (nMsecPos >= 0)
            {
                Double.TryParse(_strTime.Substring(nMsecPos), out dblMSec);
                _strTime = _strTime.Substring(0, nMsecPos);
            }

            // Now parse something like HH:MM:SS or MM:SS or SS 
            string[] arrParts = _strTime.Split(':');
            if (arrParts.Length > 0)
            {
                Array.Reverse(arrParts); // SS:MM:HH or SS:MM or SS 

                Int32.TryParse(arrParts[0], out nSec);
                if (arrParts.Length > 1)
                    Int32.TryParse(arrParts[1], out nMinutes);
                if (arrParts.Length > 2)
                    Int32.TryParse(arrParts[2], out nHours);
            }

            double dblPos = nHours * 3600 + nMinutes * 60 + nSec + dblMSec;

            _strTime = (secTime2DTime(dblPos)).ToString("HH:mm:ss.fff");

            return dblPos;
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            // Volume in dB
            // 0 - full volume, -100 silence
            double dblPos = (double)trackBarVolume.Value / trackBarVolume.Maximum;
            m_objPreview.PreviewAudioVolumeSet("", -1, -60 * (1 - dblPos));
        }

        private void mAudioMeter1_SizeChanged(object sender, EventArgs e)
        {
            mAudioMeter1.Location = new Point(this.Width - 28 - mAudioMeter1.Size.Width, mAudioMeter1.Location.Y);
            //
        }

        private void checkBoxAudio_CheckedChanged(object sender, EventArgs e)
        {
            m_objPreview.PreviewEnable("", Convert.ToInt32(checkBoxAudio.Checked), Convert.ToInt32(checkBoxVideo.Checked));
        }

        private void label3_Click(object sender, EventArgs e)
        {
            if (checkBoxAudio.Checked)
                checkBoxAudio.Checked = false;
            else
                checkBoxAudio.Checked = true;
        }

        private void checkBox_renderer_CheckedChanged(object sender, EventArgs e)
        {
            if (!checkBoxRenderer.Checked)
                m_objRenderer.DeviceClose();
            else
            {
                string name;
                int nBusy;
                m_objRenderer.DeviceGetByIndex(eMFDeviceType.eMFDT_Video, comboBoxRenrerers.SelectedIndex, out name,
                    out nBusy);

                if (nBusy < 1)
                {
                    m_objRenderer.DeviceSet(eMFDeviceType.eMFDT_Video, comboBoxRenrerers.SelectedIndex, "");

                }
                else
                {
                    checkBoxRenderer.Checked = false;
                    MessageBox.Show("Device is busy.");
                }
            }
        }

        private void comboVF_SelectedIndexChanged(object sender, EventArgs e)
        {
            lock (m_objLock)
            {
                m_avProps = new M_AV_PROPS();
                m_avProps.vidProps.eVideoFormat = (eMVideoFormat)comboBoxVideoFormat.SelectedItem;
            }
        }

        private void checkBoxVideo_CheckedChanged(object sender, EventArgs e)
        {
            m_objPreview.PreviewEnable("", Convert.ToInt32(checkBoxAudio.Checked), Convert.ToInt32(checkBoxVideo.Checked));
        }

        private void renderersCombobox_SelectedIndexChanged(object sender, EventArgs e)
        {
            lock (m_objLock)
            {
                if (checkBoxRenderer.Checked)
                {
                    string name;
                    int nBusy;
                    m_objRenderer.DeviceGetByIndex(eMFDeviceType.eMFDT_Video, comboBoxRenrerers.SelectedIndex, out name,
                        out nBusy);

                    if (nBusy < 1)
                    {
                        m_objRenderer.DeviceClose();
                        m_objRenderer.DeviceSet(eMFDeviceType.eMFDT_Video, comboBoxRenrerers.SelectedIndex, "");
                    }
                }
            }
        }


        private void buttonCGProps_Click(object sender, EventArgs e)
        {
            if (m_objCharGen != null)
            {
                m_objCharGen.ShowPropertiesPage(0);
            }
        }

        private void checkBoxCG_CheckedChanged(object sender, EventArgs e)
        {
            buttonCGProps.Enabled = checkBoxCG.Checked;
            CGEditorButton.Enabled = checkBoxCG.Checked;
        }

        private void timerUpdatePos_Tick(object sender, EventArgs e)
        {
            // Update positions
            //if (m_bWork)
            update_UI(mTime);
        }

        private void MainForm_SizeChanged(object sender, EventArgs e)
        {
            if (m_objPreview != null)
                m_objPreview.PreviewWindowSet("", panelPreview.Handle.ToInt32());
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_objPreview.PropsSet("audio_renderer", comboBoxAudioRenderers.SelectedItem.ToString());
        }

        private CGEditor_WinForms.MainWindow cgEditor;
        private void CGEditorButton_Click(object sender, EventArgs e)
        {
            if (m_objCharGen != null)
            {
                cgEditor = new CGEditor_WinForms.MainWindow();
                cgEditor.SetSourceObject(m_objMFReader, m_objCharGen);
                cgEditor.ShowDialog(this);
                cgEditor = null;
            }
        }

        private void checkBoxVirtDev_CheckedChanged(object sender, EventArgs e)
        {
            lock (m_objLock)
            {
                if (checkBoxVirtDev.Checked)
                {
                    m_objMFSink.SinkInit("MFReaderSink", "MFormats VSource", null);
                }
                else
                {
                    m_objMFSink.SinkClose();
                }
            }
        }

        private void checkBoxHTML_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxHTML.Checked)
            {
                if (m_objOverlayHTML == null)
                {
                    m_objOverlayHTML = new MFOverlayHTMLClass();
                }

                buttonHTMLProps.Enabled = true;

                if (!String.IsNullOrEmpty(textBoxHTMLURL.Text))
                {
                    string strURL = textBoxHTMLURL.Text;

                    if (String.Equals(textBoxHTMLURL.Text, "demo") &&
                        File.Exists(m_CEFResourcesPath + "\\CEF\\demo.html"))
                    {
                        strURL = m_strDemoURL;
                    }

                    m_objOverlayHTML.BrowserPageLoad(strURL);
                }
            }
            else
            {
                lock (m_objHTMLLock)
                {
                    m_objOverlayHTML.BrowserClose();
                    Marshal.ReleaseComObject(m_objOverlayHTML);
                    m_objOverlayHTML = null;
                    buttonHTMLProps.Enabled = false;
                }                
            }
        }

        private void textBoxHTMLURL_KeyDown(object sender, KeyEventArgs e)
        {
            if (checkBoxHTML.Checked && e.KeyCode == Keys.Enter && !String.IsNullOrEmpty(textBoxHTMLURL.Text))
            {
                string strURL = textBoxHTMLURL.Text;

                if (String.Equals(textBoxHTMLURL.Text, "demo") &&
                    File.Exists(m_CEFResourcesPath + "\\CEF\\demo.html"))
                {
                    strURL = m_strDemoURL;
                }

                m_objOverlayHTML.BrowserPageLoad(strURL);
            }
        }
        OverlayHTMLWindow overlayHTMLWind = null;
        private void buttonHTMLProps_Click(object sender, EventArgs e)
        {
            overlayHTMLWind = new OverlayHTMLWindow(m_objOverlayHTML, m_objMFReader);

            if (overlayHTMLWind.m_bStateOk)
            {
                overlayHTMLWind.ShowDialog();
                overlayHTMLWind.Dispose();                
            }

            string strCurrURL;
            m_objOverlayHTML.PropsGet("current_url", out strCurrURL);
            if (strCurrURL != null)
                textBoxHTMLURL.Text = strCurrURL.Contains("demo.html") ? "demo" : strCurrURL;
        }

        private void textBoxTC_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Return || e.KeyCode == Keys.Tab)
                m_objMFReader.PropsSet("tc_pos", textBoxTC.Text);
        }
    }
}