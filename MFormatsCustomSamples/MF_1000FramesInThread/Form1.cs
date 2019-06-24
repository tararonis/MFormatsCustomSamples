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
namespace MF_1000FramesInThread
{
    public partial class Form1 : Form
    {
        MFPreviewClass m_objPreview;
        MFReaderClass m_objReader;
        MFWriterClass m_objWriter;

        string openPath = @"M:\TEST_VIDEOS\Moneyball.2011.720p.BluRay.x264-HiDt.mkv";
        string savePath = @"M:\TempVideo\1000frames.mp4";

        bool start;

        public delegate void InvokeDelegate();

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
            m_objWriter = new MFWriterClass();

            m_objPreview.PreviewWindowSet("", panelPrev.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 1, 1);

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        DateTime endTime;
        int count = 0;
        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (start)
                {
                    count++;
                    if (count >= 1000 && count <= 2000)
                    {
                        MFFrame frame;
                        m_objReader.SourceFrameGet(-1, out frame, "");                       
                        m_objPreview.ReceiverFramePut(frame,0, "");
                        m_objWriter.ReceiverFramePut(frame, -1, "");

                        Marshal.ReleaseComObject(frame);
                        
                    }
                    if (count > 2000)
                    {
                        endTime = DateTime.Now;
                        m_objWriter.WriterClose(0);
                        m_objReader.ReaderClose();                       
                       
                        BeginInvoke(new InvokeDelegate(UpdateButton));
                        start = false;
                    }
                    BeginInvoke(new InvokeDelegate(UpdateLabel));
                }
            }
            }

        private void UpdateLabel()
        {
            frameN.Text = count.ToString();
        }

        private void UpdateButton()
        {
            int dif = (endTime.Millisecond + endTime.Second * 1000 + endTime.Minute * 60 * 1000) - (startTime.Millisecond + startTime.Second * 1000 + startTime.Minute * 60 * 1000);
            start_btn.Text = dif.ToString() + " ms";
        }

        DateTime startTime;
        private void Start_btn_Click(object sender, EventArgs e)
        {
            saveFileDialog1 = new SaveFileDialog();
            if (saveFileDialog1.ShowDialog() == DialogResult.OK && saveFileDialog1.FileName != string.Empty)
            {
                savePath = saveFileDialog1.FileName;
                count = 0;
                startTime = DateTime.Now;
                //m_objReader.PropsSet("experimental.mfcodecs", "true");
                //m_objReader.PropsSet("experimental.out_video_packets", "true");
                //m_objReader.PropsSet("external_process", "false");
                m_objReader.ReaderOpen(openPath, "");

                //m_objWriter.PropsSet("external_process", "false");
                m_objWriter.WriterSet(savePath, 0, "format='mp4' video::codec='n264' audio::codec='aac'"); // video::codec='packets' audio::codec='audio_packets'"

                start = true;
            }
        }

        private void OpenFile_btn_Click(object sender, EventArgs e)
        {
            openFileDialog1 = new OpenFileDialog();
            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileName != string.Empty)
                openPath = openFileDialog1.FileName;
        }
    }
}
