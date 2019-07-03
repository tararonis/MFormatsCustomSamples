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

namespace MF_WriterTest
{
    public partial class Form1 : Form
    {

        private bool start = false;

        public delegate void InvokeDelegate();

        private MFPreviewClass m_objPrev;
        private MFReaderClass m_objRed;
        private MFWriterClass m_objWriter;


        private M_AV_PROPS m_avProps;


        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {


            m_objPrev = new MFPreviewClass();
            m_objRed = new MFReaderClass();
            m_objRed.PropsSet("external_process", "false");
            m_objWriter = new MFWriterClass();
            m_objWriter.PropsSet("external_process", "false");

            m_objWriter.WriterSet("", 0, "");

            m_objRed.ReaderOpen(openFilePath_txb.Text, "");
            m_objWriter.WriterSet(savePath_txb.Text, 0, @"format='mp4' video::codec='n264' video::size='720x1080' audio::codec='aac'");

            m_objPrev.PreviewWindowSet("", prevPanel.Handle.ToInt32());
            m_objPrev.PreviewEnable("", 1, 1);


            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            //m_threadWorker.Start();  
        }

        void StartTest()
        {
            bool reading = true;
            while (reading)
            {
                // Read the next frame from our source video
                m_objRed.SourceFrameGet(-1, out MFORMATSLib.MFFrame sourceFrame, "");

                // Get the information about the frame and stop reading if we reached the last frame
                sourceFrame.MFAllGet(out MFORMATSLib.MF_FRAME_INFO frameInfo);
                if ((frameInfo.mTime.eFFlags & MFORMATSLib.eMFrameFlags.eMFF_Last) != 0)
                    reading = false;

                // Send the frame to the encoder
                m_objWriter.ReceiverFramePut(sourceFrame, -1, "");
                m_objPrev.ReceiverFramePut(sourceFrame, -1, "");

                // Release the source frame
                Marshal.ReleaseComObject(sourceFrame);
            }
            m_objRed.ReaderClose();
            m_objWriter.WriterClose(1);

        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (NextFrame() == false) return;
            }
        }
        
        bool NextFrame()
        {
            MFFrame pFrame = null;
            if (start)
            {
                m_objRed.SourceFrameGet(-1, out pFrame, "");
                m_objWriter.ReceiverFramePut(pFrame, -1, "");
                m_objPrev.ReceiverFramePut(pFrame, -1, "");

                Marshal.ReleaseComObject(pFrame);
            }
            return true;
        }


        private void start_btn_Click(object sender, EventArgs e)
        {
            start = true;
            StartTest();
        }

        private void Stop_btn_Click(object sender, EventArgs e)
        {
            m_objRed.ReaderClose();
            m_objWriter.WriterClose(1);
        }
    }
}
