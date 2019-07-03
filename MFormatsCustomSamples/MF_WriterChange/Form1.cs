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

namespace MF_WriterChange
{
    public partial class Form1 : Form
    {
        private bool start = false;

        public delegate void InvokeDelegate();

        private MFPreviewClass m_objPrev;
        private MFReaderClass m_objRed;
        private MFWriterClass m_objWr1;     
        

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
            m_objWr1 = new MFWriterClass();
            WriterSet();


            m_objPrev.PreviewWindowSet("", Prewiew_panel.Handle.ToInt32());
            m_objPrev.PreviewEnable("", 1, 1);


            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();

            //System.Windows.Forms.Control.CheckForIllegalCrossThreadCalls = false;



        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (NextFrame() == false) return;
            }
        }

        int count = 0;
        int number = 0;
        bool NextFrame()
        {
            MFFrame pFrame = null;
            if (start)
            {
                count++;
                if (count > 150)
                {
                    WriterSet();
                    count = 0;
                }        
                

                m_objRed.SourceFrameGet(-1, out pFrame, "");
                m_objWr1.ReceiverFramePut(pFrame, -1, "");
                m_objPrev.ReceiverFramePut(pFrame, -1, "");

                Marshal.ReleaseComObject(pFrame);
            }           
            return true;
        }
        void WriterSet()
        {
            m_objWr1.WriterClose(-1);
            Marshal.ReleaseComObject(m_objWr1);
            m_objWr1 = new MFWriterClass();
            string savePath = savePath_txb.Text + number.ToString() + ".mp4";
            m_objWr1.WriterSet(savePath, 0, encodingCon_txb.Text);


            string a = Environment.CurrentDirectory + "\\output\\mformats_test.flv";
            string b = @"format='flv' " +
                "video::codec='q264hw' video::b='3.7M' video::size='1280x720' video::g='30' " +
                "audio::codec='aac' audio::b='64K' audio::ar='44100'";
            


            BeginInvoke(new InvokeDelegate(UpdateList));           
            number++;
        }
        void UpdateList()
        {
            successList_lsv.Items.Add(number.ToString());
        }
        private void start_btn_Click(object sender, EventArgs e)
        {
            m_objRed.ReaderOpen(filePath_txb.Text, "");            
            start = true;
        }
    }
}
