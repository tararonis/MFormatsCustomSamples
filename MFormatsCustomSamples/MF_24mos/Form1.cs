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

namespace MF_24mos
{
    public partial class Form1 : Form
    {
        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        string path = "";
        bool start = false;
        bool startF = false;
        bool startS = false;
        bool startT = false;

        MFReaderClass m_objReader1;
        MFReaderClass m_objReader2;
        MFReaderClass m_objReader3;
        MFReaderClass m_objReader4;
        MFReaderClass m_objReader5;
        MFReaderClass m_objReader6;
        MFReaderClass m_objReader7;
        MFReaderClass m_objReader8;

        MFReaderClass m_objReader9;
        MFReaderClass m_objReader10;
        MFReaderClass m_objReader11;
        MFReaderClass m_objReader12;
        MFReaderClass m_objReader13;
        MFReaderClass m_objReader14;
        MFReaderClass m_objReader15;
        MFReaderClass m_objReader16;

        MFReaderClass m_objReader17;
        MFReaderClass m_objReader18;
        MFReaderClass m_objReader19;
        MFReaderClass m_objReader20;
        MFReaderClass m_objReader21;
        MFReaderClass m_objReader22;
        MFReaderClass m_objReader23;
        MFReaderClass m_objReader24;

        MFPreviewClass m_objPrev1;
        MFPreviewClass m_objPrev2;
        MFPreviewClass m_objPrev3;
        MFPreviewClass m_objPrev4;
        MFPreviewClass m_objPrev5;
        MFPreviewClass m_objPrev6;
        MFPreviewClass m_objPrev7;
        MFPreviewClass m_objPrev8;

        MFPreviewClass m_objPrev9;
        MFPreviewClass m_objPrev10;
        MFPreviewClass m_objPrev11;
        MFPreviewClass m_objPrev12;
        MFPreviewClass m_objPrev13;
        MFPreviewClass m_objPrev14;
        MFPreviewClass m_objPrev15;
        MFPreviewClass m_objPrev16;

        MFPreviewClass m_objPrev17;
        MFPreviewClass m_objPrev18;
        MFPreviewClass m_objPrev19;
        MFPreviewClass m_objPrev20;
        MFPreviewClass m_objPrev21;
        MFPreviewClass m_objPrev22;
        MFPreviewClass m_objPrev23;
        MFPreviewClass m_objPrev24;


        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_objReader1 = new MFReaderClass();
            m_objReader2 = new MFReaderClass();
            m_objReader3 = new MFReaderClass();
            m_objReader4 = new MFReaderClass();
            m_objReader5 = new MFReaderClass();
            m_objReader6 = new MFReaderClass();
            m_objReader7 = new MFReaderClass();
            m_objReader8 = new MFReaderClass();

            m_objReader9 = new MFReaderClass();
            m_objReader10 = new MFReaderClass();
            m_objReader11 = new MFReaderClass();
            m_objReader12 = new MFReaderClass();
            m_objReader13 = new MFReaderClass();
            m_objReader14 = new MFReaderClass();
            m_objReader15 = new MFReaderClass();
            m_objReader16 = new MFReaderClass();

            m_objReader17 = new MFReaderClass();
            m_objReader18 = new MFReaderClass();
            m_objReader19 = new MFReaderClass();
            m_objReader20 = new MFReaderClass();
            m_objReader21 = new MFReaderClass();
            m_objReader22 = new MFReaderClass();
            m_objReader23 = new MFReaderClass();
            m_objReader24 = new MFReaderClass();

            m_objPrev1 = new MFPreviewClass();
            m_objPrev2 = new MFPreviewClass();
            m_objPrev3 = new MFPreviewClass();
            m_objPrev4 = new MFPreviewClass();
            m_objPrev5 = new MFPreviewClass();
            m_objPrev6 = new MFPreviewClass();
            m_objPrev7 = new MFPreviewClass();
            m_objPrev8 = new MFPreviewClass();

            m_objPrev9 = new MFPreviewClass();
            m_objPrev10 = new MFPreviewClass();
            m_objPrev11 = new MFPreviewClass();
            m_objPrev12 = new MFPreviewClass();
            m_objPrev13 = new MFPreviewClass();
            m_objPrev14 = new MFPreviewClass();
            m_objPrev15 = new MFPreviewClass();
            m_objPrev16 = new MFPreviewClass();

            m_objPrev17 = new MFPreviewClass();
            m_objPrev18 = new MFPreviewClass();
            m_objPrev19 = new MFPreviewClass();
            m_objPrev20 = new MFPreviewClass();
            m_objPrev21 = new MFPreviewClass();
            m_objPrev22 = new MFPreviewClass();
            m_objPrev23 = new MFPreviewClass();
            m_objPrev24 = new MFPreviewClass();

            m_objPrev1.PreviewWindowSet("", panel1.Handle.ToInt32());
            m_objPrev1.PreviewEnable("", 0, 1);

            m_objPrev2.PreviewWindowSet("", panel2.Handle.ToInt32());
            m_objPrev2.PreviewEnable("", 0, 1);

            m_objPrev3.PreviewWindowSet("", panel3.Handle.ToInt32());
            m_objPrev3.PreviewEnable("", 0, 1);

            m_objPrev4.PreviewWindowSet("", panel4.Handle.ToInt32());
            m_objPrev4.PreviewEnable("", 0, 1);

            m_objPrev5.PreviewWindowSet("", panel5.Handle.ToInt32());
            m_objPrev5.PreviewEnable("", 0, 1);

            m_objPrev6.PreviewWindowSet("", panel6.Handle.ToInt32());
            m_objPrev6.PreviewEnable("", 0, 1);

            m_objPrev7.PreviewWindowSet("", panel7.Handle.ToInt32());
            m_objPrev7.PreviewEnable("", 0, 1);

            m_objPrev8.PreviewWindowSet("", panel8.Handle.ToInt32());
            m_objPrev8.PreviewEnable("", 0, 1);

            m_objPrev9.PreviewWindowSet("", panel9.Handle.ToInt32());
            m_objPrev9.PreviewEnable("", 0, 1);

            m_objPrev10.PreviewWindowSet("", panel10.Handle.ToInt32());
            m_objPrev10.PreviewEnable("", 0, 1);

            m_objPrev11.PreviewWindowSet("", panel11.Handle.ToInt32());
            m_objPrev11.PreviewEnable("", 0, 1);

            m_objPrev12.PreviewWindowSet("", panel12.Handle.ToInt32());
            m_objPrev12.PreviewEnable("", 0, 1);

            m_objPrev13.PreviewWindowSet("", panel13.Handle.ToInt32());
            m_objPrev13.PreviewEnable("", 0, 1);

            m_objPrev14.PreviewWindowSet("", panel14.Handle.ToInt32());
            m_objPrev14.PreviewEnable("", 0, 1);

            m_objPrev15.PreviewWindowSet("", panel15.Handle.ToInt32());
            m_objPrev15.PreviewEnable("", 0, 1);

            m_objPrev16.PreviewWindowSet("", panel16.Handle.ToInt32());
            m_objPrev16.PreviewEnable("", 0, 1);

            m_objPrev17.PreviewWindowSet("", panel17.Handle.ToInt32());
            m_objPrev17.PreviewEnable("", 0, 1);

            m_objPrev18.PreviewWindowSet("", panel18.Handle.ToInt32());
            m_objPrev18.PreviewEnable("", 0, 1);

            m_objPrev19.PreviewWindowSet("", panel19.Handle.ToInt32());
            m_objPrev19.PreviewEnable("", 0, 1);

            m_objPrev20.PreviewWindowSet("", panel20.Handle.ToInt32());
            m_objPrev20.PreviewEnable("", 0, 1);

            m_objPrev21.PreviewWindowSet("", panel21.Handle.ToInt32());
            m_objPrev21.PreviewEnable("", 0, 1);

            m_objPrev22.PreviewWindowSet("", panel22.Handle.ToInt32());
            m_objPrev22.PreviewEnable("", 0, 1);

            m_objPrev23.PreviewWindowSet("", panel23.Handle.ToInt32());
            m_objPrev23.PreviewEnable("", 0, 1);

            m_objPrev24.PreviewWindowSet("", panel24.Handle.ToInt32());
            m_objPrev24.PreviewEnable("", 0, 1);

                        


            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();

            //MFReaderClass[] Readers = new MFReaderClass[24];
            //for (int i = 0; i < Readers.Length; i++)
            //{
            //    Readers[i] = new MFReaderClass();
            //}
            //MFPreviewClass[] Prev = new MFPreviewClass[24];
            //for (int i = 0; i < Prev.Length; i++)
            //{
            //    Prev[i] = new MFPreviewClass();
            //}

            //for (int i = 0; i < Prev.Length; i++)
            //{
            //    panel1.Handle.ToInt32();
            //    Prev[i].PreviewWindowSet("", (Panel)("panel" + (i + 1))

            //}


        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (start)
                {
                    MFFrame frame1 = null;
                    MFFrame frame2 = null;
                    MFFrame frame3 = null;
                    MFFrame frame4 = null;
                    MFFrame frame5 = null;
                    MFFrame frame6 = null;
                    MFFrame frame7 = null;
                    MFFrame frame8 = null;

                    MFFrame frame9 = null;
                    MFFrame frame10 = null;
                    MFFrame frame11 = null;
                    MFFrame frame12 = null;
                    MFFrame frame13 = null;
                    MFFrame frame14 = null;
                    MFFrame frame15 = null;
                    MFFrame frame16 = null;

                    MFFrame frame17 = null;
                    MFFrame frame18 = null;
                    MFFrame frame19 = null;
                    MFFrame frame20 = null;
                    MFFrame frame21 = null;
                    MFFrame frame22 = null;
                    MFFrame frame23 = null;
                    MFFrame frame24 = null;
                    if (first_chb.Checked)
                    {
                        m_objReader1.SourceFrameGet(-1, out frame1, "");
                        m_objReader2.SourceFrameGet(-1, out frame2, "");
                        m_objReader3.SourceFrameGet(-1, out frame3, "");
                        m_objReader4.SourceFrameGet(-1, out frame4, "");
                        m_objReader5.SourceFrameGet(-1, out frame5, "");
                        m_objReader6.SourceFrameGet(-1, out frame6, "");
                        m_objReader7.SourceFrameGet(-1, out frame7, "");
                        m_objReader8.SourceFrameGet(-1, out frame8, "");

                        m_objPrev1.ReceiverFramePut(frame1, -1, "");
                        m_objPrev2.ReceiverFramePut(frame2, -1, "");
                        m_objPrev3.ReceiverFramePut(frame3, -1, "");
                        m_objPrev4.ReceiverFramePut(frame4, -1, "");
                        m_objPrev5.ReceiverFramePut(frame5, -1, "");
                        m_objPrev6.ReceiverFramePut(frame6, -1, "");
                        m_objPrev7.ReceiverFramePut(frame7, -1, "");
                        m_objPrev8.ReceiverFramePut(frame8, -1, "");

                    }

                    if (second_chb.Checked)
                    {

                        m_objReader9.SourceFrameGet(-1, out frame9, "");
                        m_objReader10.SourceFrameGet(-1, out frame10, "");
                        m_objReader11.SourceFrameGet(-1, out frame11, "");
                        m_objReader12.SourceFrameGet(-1, out frame12, "");
                        m_objReader13.SourceFrameGet(-1, out frame13, "");
                        m_objReader14.SourceFrameGet(-1, out frame14, "");
                        m_objReader15.SourceFrameGet(-1, out frame15, "");
                        m_objReader16.SourceFrameGet(-1, out frame16, "");

                        m_objPrev9.ReceiverFramePut(frame9, -1, "");
                        m_objPrev10.ReceiverFramePut(frame10, -1, "");
                        m_objPrev11.ReceiverFramePut(frame11, -1, "");
                        m_objPrev12.ReceiverFramePut(frame12, -1, "");
                        m_objPrev13.ReceiverFramePut(frame13, -1, "");
                        m_objPrev14.ReceiverFramePut(frame14, -1, "");
                        m_objPrev15.ReceiverFramePut(frame15, -1, "");
                        m_objPrev16.ReceiverFramePut(frame16, -1, "");
                    }
                    if (third_chb.Checked)
                    {
                        m_objReader17.SourceFrameGet(-1, out frame17, "");
                        m_objReader18.SourceFrameGet(-1, out frame18, "");
                        m_objReader19.SourceFrameGet(-1, out frame19, "");
                        m_objReader20.SourceFrameGet(-1, out frame20, "");
                        m_objReader21.SourceFrameGet(-1, out frame21, "");
                        m_objReader22.SourceFrameGet(-1, out frame22, "");
                        m_objReader23.SourceFrameGet(-1, out frame23, "");
                        m_objReader24.SourceFrameGet(-1, out frame24, "");


                        m_objPrev17.ReceiverFramePut(frame17, -1, "");
                        m_objPrev18.ReceiverFramePut(frame18, -1, "");
                        m_objPrev19.ReceiverFramePut(frame19, -1, "");
                        m_objPrev20.ReceiverFramePut(frame20, -1, "");
                        m_objPrev21.ReceiverFramePut(frame21, -1, "");
                        m_objPrev22.ReceiverFramePut(frame22, -1, "");
                        m_objPrev23.ReceiverFramePut(frame23, -1, "");
                        m_objPrev24.ReceiverFramePut(frame24, -1, "");
                    }
                    if (frame1 != null)
                    {
                        Marshal.ReleaseComObject(frame1);
                        Marshal.ReleaseComObject(frame2);
                        Marshal.ReleaseComObject(frame3);
                        Marshal.ReleaseComObject(frame4);
                        Marshal.ReleaseComObject(frame5);
                        Marshal.ReleaseComObject(frame6);
                        Marshal.ReleaseComObject(frame7);
                        Marshal.ReleaseComObject(frame8);
                    }

                    if (frame9 != null)
                    {
                        Marshal.ReleaseComObject(frame9);
                        Marshal.ReleaseComObject(frame10);
                        Marshal.ReleaseComObject(frame11);
                        Marshal.ReleaseComObject(frame12);
                        Marshal.ReleaseComObject(frame13);
                        Marshal.ReleaseComObject(frame14);
                        Marshal.ReleaseComObject(frame15);
                        Marshal.ReleaseComObject(frame16);
                    }
                    if (frame17 != null)
                    {
                        Marshal.ReleaseComObject(frame17);
                        Marshal.ReleaseComObject(frame18);
                        Marshal.ReleaseComObject(frame19);
                        Marshal.ReleaseComObject(frame20);
                        Marshal.ReleaseComObject(frame21);
                        Marshal.ReleaseComObject(frame22);
                        Marshal.ReleaseComObject(frame23);
                        Marshal.ReleaseComObject(frame24);
                    }
                }

            }
        }

        private void Play_btn_Click(object sender, EventArgs e)
        {
            path = path_tb.Text;
            
            start = start == false ? true : false;
        }

        private void first_chb_CheckedChanged(object sender, EventArgs e)
        {
            path = path_tb.Text;
            m_objReader1.ReaderOpen(path, "loop=true");
            m_objReader2.ReaderOpen(path, "loop=true");
            m_objReader3.ReaderOpen(path, "loop=true");
            m_objReader4.ReaderOpen(path, "loop=true");
            m_objReader5.ReaderOpen(path, "loop=true");
            m_objReader6.ReaderOpen(path, "loop=true");
            m_objReader7.ReaderOpen(path, "loop=true");
            m_objReader8.ReaderOpen(path, "loop=true");
        }

        private void second_chb_CheckedChanged(object sender, EventArgs e)
        {
            path = path_tb.Text;
            m_objReader9.ReaderOpen(path, "loop=true");
            m_objReader10.ReaderOpen(path, "loop=true");
            m_objReader11.ReaderOpen(path, "loop=true");
            m_objReader12.ReaderOpen(path, "loop=true");
            m_objReader13.ReaderOpen(path, "loop=true");
            m_objReader14.ReaderOpen(path, "loop=true");
            m_objReader15.ReaderOpen(path, "loop=true");
            m_objReader16.ReaderOpen(path, "loop=true");
        }

       

        private void third_chb_CheckedChanged(object sender, EventArgs e)
        {
            path = path_tb.Text;
            m_objReader17.ReaderOpen(path, "loop=true");
            m_objReader18.ReaderOpen(path, "loop=true");
            m_objReader19.ReaderOpen(path, "loop=true");
            m_objReader20.ReaderOpen(path, "loop=true");
            m_objReader21.ReaderOpen(path, "loop=true");
            m_objReader22.ReaderOpen(path, "loop=true");
            m_objReader23.ReaderOpen(path, "loop=true");
            m_objReader24.ReaderOpen(path, "loop=true");
        }
    }
}
