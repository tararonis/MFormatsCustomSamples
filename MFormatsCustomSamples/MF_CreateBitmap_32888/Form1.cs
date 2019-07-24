using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MFORMATSLib;

namespace MF_CreateBitmap_32888
{
    public partial class Form1 : Form
    {
        bool decode = false;
        bool stopWr = true;
        MFPreviewClass m_objPreview;
        MFReaderClass m_objReader;
        MFWriterClass m_objWriter;

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

            m_objWriter.WriterSet(textBox1.Text, 0, ""); 

            m_objPreview.PreviewWindowSet("", panelPrev.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 1, 1);

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        int index = 0;
        private void thread_DoWork(CancellationToken token)
        {
            MFFactoryClass _factory = new MFFactoryClass();

            while (!token.IsCancellationRequested)
            {
                if (decode)
                {
                    MFFrame frame = null;

                    string path = String.Format(@"\\MLDiskStation\MLFiles\Trash\Roman\NetTestFiles\LG_jazz\LG_jazz\LG_jazz{0:d7}.jpg", index);

                    Debug.WriteLine("Picture:" + index.ToString());
                    index = ++index == 291 ? 0 : index; //291 is count of pictures in image sequences

                    {
                        Bitmap bmp = (Bitmap)System.Drawing.Image.FromFile(path);

                        Rectangle rect = new Rectangle(0, 0, bmp.Width, bmp.Height);
                        System.Drawing.Imaging.BitmapData bmpData = bmp.LockBits(rect, System.Drawing.Imaging.ImageLockMode.ReadOnly, PixelFormat.Format32bppPArgb);

                        M_AV_PROPS avProps = new M_AV_PROPS();
                        avProps.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_30p;
                        //avProps.vidProps.eInterlace = eMInterlace.eMI_Field1First;
                        avProps.vidProps.fccType = eMFCC.eMFCC_RGB32;
                        avProps.vidProps.nWidth = bmp.Width;
                        avProps.vidProps.nHeight = -1 * bmp.Height;    // Negative height for top-bottom RGB images (if positive->bottom-top)
                        avProps.vidProps.nRowBytes = bmpData.Stride;   // distance between lines

                        _factory.MFFrameCreateFromMem(ref avProps, bmpData.Scan0.ToInt64(), 0, 0, out frame, "");

                        bmp.UnlockBits(bmpData);

                        bmp.Dispose();
                    }

                    M_AV_PROPS props;
                    int samples;
                    frame.MFAVPropsGet(out props, out samples);
                    props.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_30p;
                    props.vidProps.dblRate = 30.0;
                    props.vidProps.nAspectX = 16;
                    props.vidProps.nAspectY = 9;

                    MFFrame frameConvert;
                    int nRest = 0;
                    frame.MFConvert(ref props, out frameConvert, out nRest, "", "");

                    m_objPreview.ReceiverFramePut(frameConvert, 0, "");
                    m_objWriter.ReceiverFramePut(frameConvert, -1, "");
                    if (stopWr)
                    {
                        m_objWriter.WriterClose(0);
                    }

                    Marshal.ReleaseComObject(frameConvert);
                    Marshal.ReleaseComObject(frame);

                    GC.Collect();
                }
            }
        }
        private void Start_btn_Click(object sender, EventArgs e)
        {
            stopWr = !stopWr;
            decode = !decode;
            Start_btn.BackColor = decode == true ? Color.Green : Color.Red;            
        }
    }
}
