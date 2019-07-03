using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MFORMATSLib;

namespace _MF_Constructor
{
    public partial class Form1 : Form
    {
        bool encode = false;
        bool start = false;
        bool live = false;
        MFPreviewClass m_objPreview;
        MFReaderClass m_objReader;

        MFLiveClass m_objLive;

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_objPreview = new MFPreviewClass();
            m_objReader = new MFReaderClass();
            m_objLive = new MFLiveClass();

            m_objPreview.PreviewWindowSet("", panelPrev.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 1, 1);

            LiveVideoUpdate();
            

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (encode || live)
                {
                    MFFrame frameLive = null;
                    MFFrame frameFile = null;
                    M_AV_PROPS props = new M_AV_PROPS();
                    if (live)
                    {
                        M_VID_PROPS vidProps = new M_VID_PROPS();
                        vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_5994i;
                        m_objLive.FormatVideoSet(eMFormatType.eMFT_Input, ref vidProps);
                        props.vidProps.eVideoFormat = eMVideoFormat.eMVF_Custom;
                        m_objLive.SourceFrameConvertedGet(ref props, -1, out frameLive, "");
                    }
                    if (encode)
                    {
                        props.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_5994i;

                        m_objReader.SourceFrameConvertedGet(ref props, -1, out frameFile, "");
                        if (frameLive != null)
                            frameLive.MFOverlay(frameFile, null, 0, 0, 0, "audio_gain=0", "ch1");
                    }

                    if (frameLive != null)
                        m_objPreview.ReceiverFramePut(frameLive, -1, "");
                    else
                        m_objPreview.ReceiverFramePut(frameFile, -1, "");

                    if (frameLive != null)
                        Marshal.FinalReleaseComObject(frameLive);
                    if (frameFile != null)
                        Marshal.FinalReleaseComObject(frameFile);
                }
            }
        }

        private void OpenFile_btn_Click(object sender, EventArgs e)
        {
            start = start == false ? true : false;
            if (encode)
            {
                encode = false;
            }
            if (start)
                OpenFile();
        }

        string path;
        void OpenFile()
        {
            openFileDialog1 = new OpenFileDialog();
            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileName != string.Empty)
                path = openFileDialog1.FileName;

            m_objReader.ReaderOpen(path, "");

            encode = true;

        }

        private void Stop_btn_Click(object sender, EventArgs e)
        {
            encode = false;
            start = false;
            live = false;
        }
        void LiveVideoUpdate()
        {
            LiveList_lsb.Items.Clear();

            int count;
            m_objLive.DeviceGetCount(eMFDeviceType.eMFDT_Video, out count);

            string name;
            int busy;
            for (int i = 0; i < count; i++)
            {
                m_objLive.DeviceGetByIndex(eMFDeviceType.eMFDT_Video, i, out name, out busy);
                LiveList_lsb.Items.Add(name);
            }
        }
        void LiveAudioUpdate()
        {
            LiveAList_lsb.Items.Clear();

            int count;
            m_objLive.DeviceGetCount(eMFDeviceType.eMFDT_Audio, out count);

            string name;
            int busy;
            for (int i = 0; i < count; i++)
            {
                m_objLive.DeviceGetByIndex(eMFDeviceType.eMFDT_Audio, i, out name, out busy);
                LiveAList_lsb.Items.Add(name);
            }

        }
        private void LiveList_lsb_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_objLive.DeviceSet(eMFDeviceType.eMFDT_Video, LiveList_lsb.SelectedIndex, "");
            LiveAudioUpdate();
            live = true;
        }

        private void LiveAList_lsb_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_objLive.DeviceSet(eMFDeviceType.eMFDT_Audio, LiveAList_lsb.SelectedIndex, "");
        }
        
    }
}
