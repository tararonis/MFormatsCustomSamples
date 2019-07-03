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
using MLCHARGENLib;

namespace MF_DVB
{
    public partial class Form1 : Form
    {
        MFWriterClass m_objWriter;
        MFReaderClass m_objReader;
        MFPreviewClass m_objPreview;

        CoMLCharGenClass m_objCharGen;

        bool start = false;
        bool encode = false;
        bool cg = false;

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

            m_objCharGen = new CoMLCharGenClass();

            m_objPreview.PreviewWindowSet("", panel1.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 1, 1);

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (encode)
                {
                    MFFrame frame = null;

                    M_AV_PROPS props = new M_AV_PROPS();
                    props.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_25p;

                    //m_objReader.IMFSource_SourceFrameGet(-1, out frame, "");
                    m_objReader.SourceFrameConvertedGet(ref props, -1, out frame, "");

                    if (cg)
                    { ShowPluggin(m_objCharGen, ref frame); }

                    m_objWriter.ReceiverFramePut(frame, -1, "");                  

                    m_objPreview.ReceiverFramePut(frame, -1, "");

                    Marshal.FinalReleaseComObject(frame);
                }
            }
        }

        private void ShowPluggin(object _obj, ref MFFrame pFrame)
        {
            int nFramesRes;
            MFFrame pFrameOut = null;
            ((MFORMATSLib.IMFProcess)_obj).ProcessFrame(pFrame, out pFrameOut, out nFramesRes, "");
            if (pFrameOut != null)
            {
                Marshal.FinalReleaseComObject(pFrame);
                pFrame = pFrameOut;
            }
        }

        private void OpenFile_btn_Click(object sender, EventArgs e)
        {
            start = start == false ? true : false;
            if (encode)
            {
                encode = false;
                cg = false;
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
            string url = "udp://127.0.0.1:5000?pkt_size=1316";
            m_objWriter.WriterSet(url_txb.Text, 0, config_txb.Text);

            encode = true;

        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (m_objCharGen != null)
            {
                m_objCharGen.ShowPropertiesPage(0);
                cg = true;
            }
        }
    }
}
