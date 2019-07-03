using MFORMATSLib;
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

namespace MF_ConvertIssue32631
{
    public partial class Form1 : Form
    {
        bool decode = false;
        bool start = false;
        MFPreviewClass m_objPreview;
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
            m_objReader = new MFReaderClass();

            m_objPreview.PreviewWindowSet("", panelPrev.Handle.ToInt32());
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
                if (decode)
                {
                    MFFrame frame = null;

                    M_AV_PROPS properties = new M_AV_PROPS { vidProps = new M_VID_PROPS { eVideoFormat = eMVideoFormat.eMVF_Custom, nWidth = 800, nHeight = 600, fccType = eMFCC.eMFCC_ARGB32 } };
                    //M_AV_PROPS properties = new M_AV_PROPS { vidProps = new M_VID_PROPS { eVideoFormat = eMVideoFormat.eMVF_HD720_50p, fccType = eMFCC.eMFCC_ARGB32 } };
                    m_objReader.SourceFrameConvertedGet(ref properties, -1, out frame, "");

                    frame.MFVideoGetBytes(out var size, out var buffer);
                    Console.Out.WriteLine($"Frame size: {size}");

                    frame.MFAVPropsGet(out var frameProperties, out var sampleCount);
                    Console.Out.WriteLine($"Expected: {properties.vidProps.nWidth}, {properties.vidProps.nHeight}, {properties.vidProps.fccType}");
                    Console.Out.WriteLine($"Found: {frameProperties.vidProps.nWidth}, {frameProperties.vidProps.nHeight}, {frameProperties.vidProps.fccType}");

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

        private void Stop_btn_Click(object sender, EventArgs e)
        {
            decode = false;
            start = false;
        }

        private void OpenFile_btn_Click_1(object sender, EventArgs e)
        {
            start = start == false ? true : false;
            if (decode)
            {
                decode = false;
            }
            if (start)
                OpenFile();
        }
    }
}
