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

namespace MF_testFistFrameIssue
{
    public partial class Form1 : Form
    {
        MFReaderClass m_objReader;
        MFPreviewClass m_objPreview;

        bool decode = false;

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        public delegate void InvokeDelegate();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_objReader = new MFReaderClass();

            m_objPreview = new MFPreviewClass();

            m_objPreview.PreviewWindowSet("", panel1.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 1, 1);


            string[] files = {@"\\MLDiskStation\MLFiles\MediaTest\MP4\!audi_a7_borussia_vfl.mp4" };
            foreach(var i in files)
            {
                comboBox1.Items.Add(i);
            }
            comboBox1.SelectedIndex = 0;

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();



        }
        string mes = string.Empty;
        bool firstFrame = true;
        DateTime prevFrameTime;
        List<double> framesTimes = new List<double> {0};
        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (decode)
                {
                    if (firstFrame)
                    {
                        prevFrameTime = DateTime.Now;
                        firstFrame = false;                        
                    }
                    MFFrame frame = null;                  

                    m_objReader.SourceFrameGet(-1, out frame, "");
                    m_objPreview.ReceiverFramePut(frame, -1, "");
                    TimeSpan tick = DateTime.Now - prevFrameTime;
                    framesTimes.Add(tick.TotalMilliseconds);

                    frame.MFTimeGet(out M_TIME time);
                    mes += $"start_time ={time.rtStartTime}, end_time = {time.rtEndTime}, span ={tick.TotalMilliseconds}, dif = {framesTimes[framesTimes.Count-1] - framesTimes[framesTimes.Count-2]}";

                    BeginInvoke(new InvokeDelegate(UpdateListBox));                   
                    

                    Marshal.FinalReleaseComObject(frame);
                }
            }
        }

        private void UpdateListBox()
        {
            listBox1.Items.Add(mes);
            mes = string.Empty;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            m_objReader.ReaderOpen(comboBox1.SelectedItem.ToString(), "");
            decode = !decode;
            firstFrame = true;
        }
    }
}
