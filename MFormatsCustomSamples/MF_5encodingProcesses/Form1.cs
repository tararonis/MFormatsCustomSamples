using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using MFORMATSLib;

namespace MF_5encodingProcesses
{
    public partial class Form1 : Form
    {
        string[] listOfEncodingConfigurations = {
            " format='mp4' video::codec='n264' audio::codec='aac'",
        };

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        MFReaderClass m_objReader1;
        MFReaderClass m_objReader2;
        MFReaderClass m_objReader3;
        MFReaderClass m_objReader4;
        MFReaderClass m_objReader5;

        MFPreviewClass m_objPreview1;
        MFPreviewClass m_objPreview2;
        MFPreviewClass m_objPreview3;
        MFPreviewClass m_objPreview4;
        MFPreviewClass m_objPreview5;

        MFWriterClass m_objWriter1;
        MFWriterClass m_objWriter2;
        MFWriterClass m_objWriter3;
        MFWriterClass m_objWriter4;
        MFWriterClass m_objWriter5;

        bool start1 = false;
        bool start2 = false;
        bool start3 = false;
        bool start4 = false;
        bool start5 = false;

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

            m_objPreview1 = new MFPreviewClass();
            m_objPreview1.PreviewWindowSet("",panel1.Handle.ToInt32());
            m_objPreview1.PreviewEnable("", 0, 1);

            m_objPreview2 = new MFPreviewClass();
            m_objPreview2.PreviewWindowSet("", panel2.Handle.ToInt32());
            m_objPreview2.PreviewEnable("", 0, 1);

            m_objPreview3 = new MFPreviewClass();
            m_objPreview3.PreviewWindowSet("", panel3.Handle.ToInt32());
            m_objPreview3.PreviewEnable("", 0, 1);

            m_objPreview4 = new MFPreviewClass();
            m_objPreview4.PreviewWindowSet("", panel4.Handle.ToInt32());
            m_objPreview4.PreviewEnable("", 0, 1);

            m_objPreview5 = new MFPreviewClass();
            m_objPreview5.PreviewWindowSet("", panel5.Handle.ToInt32());
            m_objPreview5.PreviewEnable("", 0, 1);

            m_objWriter1 = new MFWriterClass();
            m_objWriter2 = new MFWriterClass();
            m_objWriter3 = new MFWriterClass();
            m_objWriter4 = new MFWriterClass();
            m_objWriter5 = new MFWriterClass();

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token))
            {
                Name = "thread_DoWork"
            };
            m_threadWorker.Start();

            FillListBox();
        }

        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                MFFrame frame1 = null;
                MFFrame frame2 = null;
                MFFrame frame3 = null;
                MFFrame frame4 = null;
                MFFrame frame5 = null;

                M_AV_PROPS props = new M_AV_PROPS();

                props.vidProps.eInterlace = eMInterlace.eMI_Progressive;
                if (start1)
                {
                    m_objReader1.SourceFrameConvertedGet(ref props, -1, out frame1, "");
                    m_objWriter1.ReceiverFramePut(frame1, -1, "");
                    m_objPreview1.ReceiverFramePut(frame1, -1, "");
                }
                if (start2)
                {
                    m_objReader2.SourceFrameConvertedGet(ref props, -1, out frame2, "");
                    m_objWriter2.ReceiverFramePut(frame2, -1, "");
                    m_objPreview2.ReceiverFramePut(frame2, -1, "");
                }
                if (start3)
                {
                    m_objReader3.SourceFrameConvertedGet(ref props, -1, out frame3, "");
                    m_objWriter3.ReceiverFramePut(frame3, -1, "");
                    m_objPreview3.ReceiverFramePut(frame3, -1, "");
                }
                if (start4)
                {
                    m_objReader4.SourceFrameConvertedGet(ref props, -1, out frame4, "");
                    m_objWriter4.ReceiverFramePut(frame4, -1, "");
                    m_objPreview4.ReceiverFramePut(frame4, -1, "");
                }
                if (start5)
                {
                    m_objReader5.SourceFrameConvertedGet(ref props, -1, out frame5, "");
                    m_objWriter5.ReceiverFramePut(frame5,-1, "");
                    m_objPreview5.ReceiverFramePut(frame5, -1, "");
                }

                if (frame1 != null)
                    Marshal.ReleaseComObject(frame1);
                if (frame2 != null)
                    Marshal.ReleaseComObject(frame2);
                if (frame3 != null)
                    Marshal.ReleaseComObject(frame3);
                if (frame4 != null)
                    Marshal.ReleaseComObject(frame4);
                if (frame5 != null)
                    Marshal.ReleaseComObject(frame5);
            }
        }

        private void Start1_btn_Click(object sender, EventArgs e)
        {            
            if (!start1)
            {
                OpenFileDialog a = new OpenFileDialog();

                if (a.ShowDialog() == DialogResult.OK)
                {
                    m_objReader1.ReaderOpen(a.FileName, "loop=true");

                    m_objWriter1.WriterSet(SavePath_txb.Text + "\\1.mp4", 0, EncodingConfigs_lsb.SelectedItem.ToString());


                    Path1_txb.Text = a.FileName;
                    start1 = !start1;
                    Start1_btn.BackColor = start1 ? Color.Green : Color.Red;                    
                }
            }
            else if (start1)
            {                
                start1 = !start1;
                Thread.Sleep(10);
                m_objWriter1.WriterClose(0);
                Start1_btn.BackColor = start1 ? Color.Green : Color.Red;                
            }

        }

        private void Start2_btn_Click(object sender, EventArgs e)
        {
            if (!start2)
            {
                OpenFileDialog a = new OpenFileDialog();

                if (a.ShowDialog() == DialogResult.OK)
                {
                    m_objReader2.ReaderOpen(a.FileName, "loop=true");

                    m_objWriter2.WriterSet(SavePath_txb.Text + "\\2.mp4", 0, EncodingConfigs_lsb.SelectedItem.ToString());


                    Path2_txb.Text = a.FileName;
                    start2 = !start2;
                    Start2_btn.BackColor = start2 ? Color.Green : Color.Red;
                }
            }
            else if (start2)
            {
                start2 = !start2;
                Thread.Sleep(10);
                m_objWriter2.WriterClose(0);
                Start2_btn.BackColor = start2 ? Color.Green : Color.Red;
            }
        }

        private void Start3_btn_Click(object sender, EventArgs e)
        {
            if (!start3)
            {
                OpenFileDialog a = new OpenFileDialog();

                if (a.ShowDialog() == DialogResult.OK)
                {
                    m_objReader3.ReaderOpen(a.FileName, "loop=true");

                    m_objWriter3.WriterSet(SavePath_txb.Text + "\\3.mp4", 0, EncodingConfigs_lsb.SelectedItem.ToString());


                    Path3_txb.Text = a.FileName;
                    start3 = !start3;
                    Start3_btn.BackColor = start3 ? Color.Green : Color.Red;
                }
            }
            else if (start3)
            {
                start3 = !start3;
                Thread.Sleep(10);
                m_objWriter3.WriterClose(0);
                Start3_btn.BackColor = start3 ? Color.Green : Color.Red;
            }
        }

        private void Start4_btn_Click(object sender, EventArgs e)
        {
            if (!start4)
            {
                OpenFileDialog a = new OpenFileDialog();

                if (a.ShowDialog() == DialogResult.OK)
                {
                    m_objReader4.ReaderOpen(a.FileName, "loop=true");

                    m_objWriter4.WriterSet(SavePath_txb.Text + "\\4.mp4", 0, EncodingConfigs_lsb.SelectedItem.ToString());


                    Path4_txb.Text = a.FileName;
                    start4 = !start4;
                    Start4_btn.BackColor = start4 ? Color.Green : Color.Red;
                }
            }
            else if (start4)
            {
                start4 = !start4;
                Thread.Sleep(10);
                m_objWriter4.WriterClose(0);
                Start4_btn.BackColor = start4 ? Color.Green : Color.Red;
            }
        }

        private void Start5_btn_Click(object sender, EventArgs e)
        {
            if (!start5)
            {
                OpenFileDialog a = new OpenFileDialog();

                if (a.ShowDialog() == DialogResult.OK)
                {
                    m_objReader5.ReaderOpen(a.FileName, "loop=true");

                    m_objWriter5.WriterSet(SavePath_txb.Text + "\\5.mp4", 0, EncodingConfigs_lsb.SelectedItem.ToString());


                    Path5_txb.Text = a.FileName;
                    start5 = !start5;
                    Start5_btn.BackColor = start5 ? Color.Green : Color.Red;
                }
            }
            else if (start5)
            {
                start5 = !start5;
                Thread.Sleep(10);
                m_objWriter5.WriterClose(0);
                Start5_btn.BackColor = start5 ? Color.Green : Color.Red;
            }
        }

        private void FillListBox()
        {
            foreach (var c in listOfEncodingConfigurations)
            {
                EncodingConfigs_lsb.Items.Add(c);
            }
            EncodingConfigs_lsb.SelectedIndex = 0;
        }

        private void SelectSaveFolder_btn_Click(object sender, EventArgs e)
        {  
            DialogResult result = folderBrowserDialog1.ShowDialog();
            if (result == DialogResult.OK)
            {
                SavePath_txb.Text = folderBrowserDialog1.SelectedPath;

                Start1_btn.Enabled = true;
                Start2_btn.Enabled = true;
                Start3_btn.Enabled = true;
                Start4_btn.Enabled = true;
                Start5_btn.Enabled = true;
            }
        }
    }
}
