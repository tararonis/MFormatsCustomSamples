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

namespace MF_SeekingMPEGfileProblem
{
    public partial class Form1 : Form
    {
        bool decode = false;
        bool start = false;
        MFPreviewClass m_objPreview;
        MFPreviewClass m_objPreviewFrame;
        MFReaderClass m_objReaderFrame;
        MFReaderClass m_objReader;

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_objPreview = new MFPreviewClass();
            m_objPreviewFrame = new MFPreviewClass();

            m_objReader = new MFReaderClass();
            m_objReaderFrame = new MFReaderClass();            

            m_objPreview.PreviewWindowSet("", panelPr.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 1, 1);

            m_objPreviewFrame.PreviewWindowSet("", panelFrame.Handle.ToInt32());
            m_objPreviewFrame.PreviewEnable("", 1, 1);

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (decode)
                {
                    MFFrame frame = null;

                    M_AV_PROPS props = new M_AV_PROPS();
                    props.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_60p;

                    m_objReader.SourceFrameConvertedGet(ref props, -1, out frame, "");

                    m_objPreview.ReceiverFramePut(frame, -1, "");

                    Marshal.FinalReleaseComObject(frame);
                }
            }
        }

        string path;
        void OpenFile()
        {
            openFileDialog1 = new OpenFileDialog();
            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileName != string.Empty)
                path = openFileDialog1.FileName;

            m_objReader.ReaderOpen(path, "");
            

            decode = true;

        }

        private void Open_btn_Click(object sender, EventArgs e)
        {
            start = start == false ? true : false;
            if (decode)
            {
                decode = false;
            }
            if (start)
                OpenFile();
        }
      
        private void Stop_btn_Click_1(object sender, EventArgs e)
        {
            decode = false;
            start = false;
        }

        private void GetFrame_btn_Click(object sender, EventArgs e)
        {
            m_objReaderFrame.ReaderOpen(path, "");
            m_objReaderFrame.SourceAVPropsGet(out M_AV_PROPS props);
            m_objReaderFrame.ReaderDurationGet(out double duration);

            m_objReaderFrame.SourceFrameConvertedGetByTime(ref props, duration / 2, -1, out MFFrame frameTumbnail, "");

            m_objPreviewFrame.ReceiverFramePut(frameTumbnail, -1, "");

            Marshal.ReleaseComObject(frameTumbnail);
        }
    }
}
