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

namespace MFOverlayRect
{
    public partial class Form1 : Form
    {
        MF_RECT overlayRect;

        MFPreviewClass m_objPrev;
        MFReaderClass m_objReader;
        MFReaderClass m_objReaderOver;
        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        bool start = false;

        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            m_objPrev = new MFPreviewClass();



            MF_RECT overlayRect = new MF_RECT();

            m_objPrev.PreviewWindowSet("", panel1.Handle.ToInt32());
            m_objPrev.PreviewEnable("", 1, 1);


            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (NextFrame() == false) return;
            }
        }

        private bool NextFrame()
        {
            if (m_objReader != null)
            {
                MFFrame frame = null;
                MFFrame frameOver = null;
                MFFrame frameClone = null;

                M_AV_PROPS props;
                m_objReader.SourceAVPropsGet(out props);
                props.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_50i;


                m_objReader.SourceFrameConvertedGet(ref props, -1, out frame, "");

                if (start)
                {
                   

                    m_objReader.SourceFrameConvertedGet(ref props, 0, out frameOver, "");
                    //frame.MFOverlayRect(frameOver, null, ref overlayRect, eMFOverlayFlags.eMFOF_Crop, 1, "", "");                    
                    //frame.MFOverlay(frameOver, null, 10, 10, 1, "", "ch1");
                }
                frame.MFClone(out frameClone, eMFrameClone.eMFC_Reference, eMFCC.eMFCC_RGB24);
                m_objPrev.ReceiverFramePut(frameClone, -1, "");

                Marshal.ReleaseComObject(frame);

                if (frameOver != null)
                    Marshal.ReleaseComObject(frameOver);
                if (frameClone != null)
                    Marshal.ReleaseComObject(frameClone);


            }
            return true;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileNames.Length != 0)
            {
                m_objReader = new MFReaderClass();
                string name = openFileDialog1.FileNames[0].ToString();

                m_objReader.ReaderOpen(name, "");


            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileNames.Length != 0)
            {
                m_objReaderOver = new MFReaderClass();
                m_objReaderOver.ReaderOpen(openFileDialog1.FileNames[0].ToString(), "");
                CreateRect();
                start = true;
            }
        }

        private void CreateRect()
        {
            overlayRect.dblHeight = Convert.ToDouble(textBox2.Text);
            overlayRect.dblWidth = Convert.ToDouble(textBox1.Text);
            overlayRect.dblPosX = 30;
            overlayRect.dblPosY = 20;
            overlayRect.eRectType = eMFRectType.eMFRT_Absolute;
        }
    }
}
