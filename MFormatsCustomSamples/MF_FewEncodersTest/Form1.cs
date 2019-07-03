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

namespace MF_FewEncodersTest
{
    public partial class Form1 : Form
    {
        #region bools

        bool Encoder1 = false;
        bool Encoder2 = false;
        bool Encoder3 = false;
        bool Encoder4 = false;
        bool Encoder5 = false;
        #endregion

        public delegate void InvokeDelegate();
        string currentEncodingConf;
        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        MFLiveClass m_objLive1;
        MFLiveClass m_objLive2;
        MFLiveClass m_objLive3;
        MFLiveClass m_objLive4;
        MFLiveClass m_objLive5;

        MFPreviewClass m_objPrev1;
        MFPreviewClass m_objPrev2;
        MFPreviewClass m_objPrev3;
        MFPreviewClass m_objPrev4;
        MFPreviewClass m_objPrev5;

        MFWriterClass m_objWriter1;
        MFWriterClass m_objWriter2;
        MFWriterClass m_objWriter3;
        MFWriterClass m_objWriter4;
        MFWriterClass m_objWriter5;

        M_AV_PROPS avProps;
        M_AUD_PROPS auProps;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {            
            EncodingConf_lsb.Items.Add(" format='mp4' video::codec='n264' video::b='7M' video::profile='High' video::preset='high_quality' audio::codec='aac'");
            EncodingConf_lsb.Items.Add(" format='mp4' video::codec='n264' video::b='7M' video::profile='High' video::preset='high_quality' video::interlace='Progressive' audio::codec='aac'");
            EncodingConf_lsb.Items.Add(" format='mp4' video::codec='n264' video::b='6M' video::profile='High' video::preset='high_quality' video::interlace='Progressive' audio::codec='aac'");
            EncodingConf_lsb.Items.Add(" format='mp4' video::codec='n264' video::b='6M' video::profile='High' video::preset='high_quality' video::interlace='Progressive' audio::codec='aac'");
            EncodingConf_lsb.Items.Add(" format='mp4' video::codec='n264' video::b='5M' video::interlace='Progressive' audio::codec='aac'");


            currentEncodingConf = EncodingConf_lsb.Items[0].ToString();

            m_objLive1 = new MFLiveClass();
            m_objLive2 = new MFLiveClass();
            m_objLive3 = new MFLiveClass();
            m_objLive4 = new MFLiveClass();
            m_objLive5 = new MFLiveClass();

            m_objPrev1 = new MFPreviewClass();
            m_objPrev2 = new MFPreviewClass();
            m_objPrev3 = new MFPreviewClass();
            m_objPrev4 = new MFPreviewClass();
            m_objPrev5 = new MFPreviewClass();

            m_objPrev1.PreviewWindowSet("", panelPr1.Handle.ToInt32());
            m_objPrev1.PreviewEnable("", 0, 1);
            m_objPrev2.PreviewWindowSet("", panelPr2.Handle.ToInt32());
            m_objPrev2.PreviewEnable("", 0, 1);
            m_objPrev3.PreviewWindowSet("", panelPr3.Handle.ToInt32());
            m_objPrev3.PreviewEnable("", 0, 1);
            m_objPrev4.PreviewWindowSet("", panelPr4.Handle.ToInt32());
            m_objPrev4.PreviewEnable("", 0, 1);
            m_objPrev5.PreviewWindowSet("", panelPr5.Handle.ToInt32());
            m_objPrev5.PreviewEnable("", 0, 1);

            m_objWriter1 = new MFWriterClass();
            m_objWriter2 = new MFWriterClass();
            m_objWriter3 = new MFWriterClass();
            m_objWriter4 = new MFWriterClass();
            m_objWriter5 = new MFWriterClass();

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();

            avProps.vidProps.eVideoFormat = eMVideoFormat.eMVF_HD1080_25p;               

            UpdateTheListOfDevices();            
        }
        string bIn;
        string fDrops;
        string fIn;
        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                MFFrame frame1 = null;
                MFFrame frame2  = null;
                MFFrame frame3 = null;
                MFFrame frame4 = null;
                MFFrame frame5 = null;

                
                if (Encoder1)
                {           
                    m_objLive1.SourceFrameConvertedGet(ref avProps, -1, out frame1, "");

                    m_objWriter1.ReceiverFramePut(frame1, -1, "");
                    m_objPrev1.ReceiverFramePut(frame1, -1, "");

                    m_objWriter1.PropsGet("stat::frames_in", out fIn);
                    m_objWriter1.PropsGet("stat::buffers_overflow", out fDrops);
                    m_objWriter1.PropsGet("stat::breaks_in", out bIn);

                    BeginInvoke(new InvokeDelegate(UpdateLabel1));                   

                }
                if (Encoder2)
                {                   

                    m_objLive2.SourceFrameConvertedGet(ref avProps, -1, out frame2, "");

                    m_objWriter2.ReceiverFramePut(frame2, -1, "");
                    m_objPrev2.ReceiverFramePut(frame2, -1, "");

                    m_objWriter2.PropsGet("stat::frames_in", out fIn);
                    m_objWriter2.PropsGet("stat::buffers_overflow", out fDrops);
                    m_objWriter2.PropsGet("stat::breaks_in", out bIn);

                    BeginInvoke(new InvokeDelegate(UpdateLabel2));
                }
                if (Encoder3)
                {                    

                    m_objLive3.SourceFrameConvertedGet(ref avProps, -1, out frame3, "");

                    m_objWriter3.ReceiverFramePut(frame3, -1, "");
                    m_objPrev3.ReceiverFramePut(frame3, -1, "");

                    m_objWriter3.PropsGet("stat::frames_in", out fIn);
                    m_objWriter3.PropsGet("stat::buffers_overflow", out fDrops);
                    m_objWriter3.PropsGet("stat::breaks_in", out bIn);

                    BeginInvoke(new InvokeDelegate(UpdateLabel3));
                }
                if (Encoder4)
                {                    

                    m_objLive4.SourceFrameConvertedGet(ref avProps, -1, out frame4, "");

                    m_objWriter4.ReceiverFramePut(frame4, -1, "");
                    m_objPrev4.ReceiverFramePut(frame4, -1, "");

                    m_objWriter4.PropsGet("stat::frames_in", out fIn);
                    m_objWriter4.PropsGet("stat::buffers_overflow", out fDrops);
                    m_objWriter4.PropsGet("stat::breaks_in", out bIn);

                    BeginInvoke(new InvokeDelegate(UpdateLabel4));
                }
                if (Encoder5)
                {
                    
                    m_objLive5.SourceFrameConvertedGet(ref avProps, -1, out frame5, "");

                    m_objWriter5.ReceiverFramePut(frame5, -1, "");
                    m_objPrev5.ReceiverFramePut(frame5, -1, "");

                    m_objWriter5.PropsGet("stat::frames_in", out fIn);
                    m_objWriter5.PropsGet("stat::buffers_overflow", out fDrops);
                    m_objWriter5.PropsGet("stat::breaks_in", out bIn);

                    BeginInvoke(new InvokeDelegate(UpdateLabel5));
                }

                if (frame1 != null)
                {
                    Marshal.ReleaseComObject(frame1);
                }
                if (frame2 != null)
                {
                    Marshal.ReleaseComObject(frame2);
                }
                if (frame3 != null)
                {
                    Marshal.ReleaseComObject(frame3);
                }
                if (frame4 != null)
                {
                    Marshal.ReleaseComObject(frame4);
                }
                if (frame5 != null)
                {
                    Marshal.ReleaseComObject(frame5);
                }
            }
        }

        private void UpdateLabel5()
        {
            LabelStat5.Text = "FramesIn:" + fIn + "#FrameDrops:" + fDrops + "#Breaks:" + bIn;
        }

        private void UpdateLabel4()
        {
            LabelStat4.Text = "FramesIn:" + fIn + "#FrameDrops:" + fDrops + "#Breaks:" + bIn;
        }
        private void UpdateLabel3()
        {
            LabelStat3.Text = "FramesIn:" + fIn + "#FrameDrops:" + fDrops + "#Breaks:" + bIn;
        }
        private void UpdateLabel2()
        {
            LabelStat2.Text = "FramesIn:" + fIn + "#FrameDrops:" + fDrops + "#Breaks:" + bIn;
        }

        private void UpdateLabel1()
        {
            LabelStat1.Text = "FramesIn:" + fIn + "#FrameDrops:" + fDrops + "#Breaks:" + bIn;
        }

        void UpdateAuidoList()
        {
            AudioProps.Items.Clear();
            int count;
            m_objLive1.FormatAudioGetCount(eMFormatType.eMFT_Input, out count);
            M_AUD_PROPS propsA;
            string name;
            for (int i = 0; i < count; i++)
            {
                m_objLive1.FormatAudioGetByIndex(eMFormatType.eMFT_Input, i, out propsA, out name);
                AudioProps.Items.Add(name);
            }
            AudioProps.SelectedIndex = 2;

        }
        void UpdateTheListOfDevices()
        {
            listBox1.Items.Clear();
            listBox2.Items.Clear();
            listBox3.Items.Clear();
            listBox4.Items.Clear();
            listBox5.Items.Clear();

            int count;
            m_objLive1.DeviceGetCount(eMFDeviceType.eMFDT_Video, out count);

            for (int i = 0; i < count; i++)
            {
                string name;
                int busy;
                m_objLive1.DeviceGetByIndex(eMFDeviceType.eMFDT_Video, i, out name, out busy);

                listBox1.Items.Add(name);
                listBox2.Items.Add(name);
                listBox3.Items.Add(name);
                listBox4.Items.Add(name);
                listBox5.Items.Add(name);
            }
        }

        #region ListBoxEvents
        private void ListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            string name;
            ListBox comboBox = (ListBox)sender;
            string selected = (string)comboBox.SelectedItem;

            m_objLive1.DeviceSet(eMFDeviceType.eMFDT_Video, comboBox.Items.IndexOf(selected), "");
            UpdateAuidoList();
            m_objLive1.FormatAudioGetByIndex(eMFormatType.eMFT_Input, AudioProps.SelectedIndex, out auProps, out name);
            m_objLive1.FormatAudioSet(eMFormatType.eMFT_Input, ref auProps);
            m_objWriter1.WriterSet(SavePath_txb.Text + "\\1.mp4", 1, currentEncodingConf);

            Encoder1 = true;
            button1.BackColor = Color.Green;            

        }
         
        private void ListBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox comboBox = (ListBox)sender;
            string selected = (string)comboBox.SelectedItem;

            m_objLive2.DeviceSet(eMFDeviceType.eMFDT_Video, comboBox.Items.IndexOf(selected), "");
            m_objLive2.FormatAudioSet(eMFormatType.eMFT_Input, ref auProps);
            m_objWriter2.WriterSet(SavePath_txb.Text + "\\2.mp4", 1, currentEncodingConf);

            Encoder2 = true;
            button2.BackColor = Color.Green;
        }

        private void ListBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox comboBox = (ListBox)sender;
            string selected = (string)comboBox.SelectedItem;

            m_objLive3.DeviceSet(eMFDeviceType.eMFDT_Video, comboBox.Items.IndexOf(selected), "");
            m_objLive3.FormatAudioSet(eMFormatType.eMFT_Input, ref auProps);
            m_objWriter3.WriterSet(SavePath_txb.Text + "\\3.mp4", 1, currentEncodingConf);

            Encoder3 = true;
            button3.BackColor = Color.Green;
        }

        private void ListBox4_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox comboBox = (ListBox)sender;
            string selected = (string)comboBox.SelectedItem;

            m_objLive4.DeviceSet(eMFDeviceType.eMFDT_Video, comboBox.Items.IndexOf(selected), "");
            m_objLive4.FormatAudioSet(eMFormatType.eMFT_Input, ref auProps);
            m_objWriter4.WriterSet(SavePath_txb.Text + "\\4.mp4", 1, currentEncodingConf);

            Encoder4 = true;
            button4.BackColor = Color.Green;
        }

        private void ListBox5_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox comboBox = (ListBox)sender;
            string selected = (string)comboBox.SelectedItem;

            m_objLive5.DeviceSet(eMFDeviceType.eMFDT_Video, comboBox.Items.IndexOf(selected), "");
            m_objLive5.FormatAudioSet(eMFormatType.eMFT_Input, ref auProps);
            m_objWriter5.WriterSet(SavePath_txb.Text + "\\5.mp4", 1, currentEncodingConf);

            Encoder5 = true;
            button5.BackColor = Color.Green;
        }

        #endregion

        private void Update_btn_Click(object sender, EventArgs e)
        {
            UpdateTheListOfDevices();
        }

        private void EncodingConf_lsb_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox comboBox = (ListBox)sender;
            currentEncodingConf = (string)comboBox.SelectedItem;             
        }


        #region StopEncoding
        private void Button1_Click(object sender, EventArgs e)
        {
            button1.BackColor = Color.Red;
            Encoder1 = false;
            m_objWriter1.WriterClose(0);
        }

        private void Button2_Click(object sender, EventArgs e)
        {
            button2.BackColor = Color.Red;
            Encoder2 = false;
            m_objWriter2.WriterClose(0);
        }

        private void Button3_Click(object sender, EventArgs e)
        {
            button3.BackColor = Color.Red;
            Encoder3 = false;
            m_objWriter3.WriterClose(0);
        }

        private void Button4_Click(object sender, EventArgs e)
        {
            button4.BackColor = Color.Red;
            Encoder4 = false;
            m_objWriter4.WriterClose(0);
        }

        private void Button5_Click(object sender, EventArgs e)
        {
            button5.BackColor = Color.Red;
            Encoder5 = false;
            m_objWriter5.WriterClose(0);
        }
        #endregion
    }
}
