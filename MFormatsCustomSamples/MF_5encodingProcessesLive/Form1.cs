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
using System.Xml;
using MFORMATSLib;
using SampleLiveSyncPlayback;

namespace MF_5encodingProcesses
{
    public partial class Form1 : Form
    {
        
        private object m_objLock = new object();

        //Maximum count of MFLives
        static int COUNT = 5;

        private MFLiveClass[] m_arrLives = new MFLiveClass[COUNT];     // MFLive objects for sync seeking
        private MFPreviewClass[] m_arrPreviews = new MFPreviewClass[COUNT];
        private MFWriterClass[] m_arrWriters = new MFWriterClass[COUNT];

        private bool[] m_arrLiveState = new bool[COUNT];
        private bool[] m_arrLiveStateRestart = new bool[COUNT];
        private bool[] m_arrWritersState = new bool[COUNT];

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        public Form1()
        {    
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Panel[] panels = { panel1, panel2, panel3, panel4, panel5 };
            for (int i = 0; i < COUNT; i++)
            {
                m_arrPreviews[i] = new MFPreviewClass();
                m_arrPreviews[i].PreviewWindowSet("", panels[i].Handle.ToInt32());
                m_arrPreviews[i].PreviewEnable("", 0, 1);

                m_arrWriters[i] = new MFWriterClass();

                m_arrLiveState[i] = false;
                m_arrWritersState[i] = false;
                m_arrLiveStateRestart[i] = false;

            }

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
            MFFrame[] arrFrames = new MFFrame[COUNT];
            while (!token.IsCancellationRequested)
            {
                M_AV_PROPS props = new M_AV_PROPS();
                props.vidProps.eInterlace = eMInterlace.eMI_Progressive;

                for (int i = 0; i < COUNT; i++)
                {
                    lock (m_objLock)
                    {
                        if (m_arrLives[i] != null && m_arrLiveState[i])
                        {
                            // Get next frame
                            try
                            {
                                m_arrLives[i].SourceFrameConvertedGetByTime(ref props, -1, -1,
                                    out arrFrames[i], " scaling_quality=2 async_id=ch" + i.ToString());
                                m_arrPreviews[i].ReceiverFramePut(arrFrames[i], -1, "");
                                if(m_arrWritersState[i])
                                    m_arrWriters[i].ReceiverFramePut(arrFrames[i], -1, "");

                                Marshal.FinalReleaseComObject(arrFrames[i]);
                            }
                            catch
                            {
                            }
                        }
                    }
                }
            }
        }

        private void Start1_btn_Click(object sender, EventArgs e)
        {
            int idx = Convert.ToInt32(((Button)sender).Tag);
            lock (m_objLock)
            {
                if (!m_arrLiveState[idx] || !m_arrLiveStateRestart[idx])
                {
                    if (m_arrLives[idx] == null)
                        m_arrLives[idx] = new MFLiveClass();
                    LiveControl Lc = new LiveControl(ref m_arrLives[idx]);
                    Lc.ShowDialog();

                    switch (idx)
                    {
                        case 0:
                            textBoxLiveName1.Text = Lc.GetName();
                            Start1_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 1:
                            textBoxLiveName2.Text = Lc.GetName();
                            Start2_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 2:
                            textBoxLiveName3.Text = Lc.GetName();
                            Start3_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 3:
                            textBoxLiveName4.Text = Lc.GetName();
                            Start4_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 4:
                            textBoxLiveName5.Text = Lc.GetName();
                            Start5_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                    }


                    m_arrWriters[idx].WriterSet(Path.Combine(SavePath_txb.Text, idx + ".mp4"), 0, EncodingConfigs_lsb.SelectedItem.ToString());
                    m_arrWritersState[idx] = true;
                    m_arrLiveState[idx] = true;
                    m_arrLiveStateRestart[idx] = true;
                }
                else if (m_arrWritersState[idx])
                {
                    switch (idx)
                    {
                        case 0:
                            m_arrWriters[idx].WriterClose(0);
                            Start1_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 1:
                            m_arrWriters[idx].WriterClose(0);
                            Start2_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 2:
                            m_arrWriters[idx].WriterClose(0);
                            Start3_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 3:
                            m_arrWriters[idx].WriterClose(0);
                            Start4_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                        case 4:
                            m_arrWriters[idx].WriterClose(0);
                            Start5_btn.BackColor = m_arrLiveStateRestart[idx] ? Color.Red : Color.Green;
                            break;
                    }
                    m_arrWritersState[idx] = false;
                    m_arrLiveStateRestart[idx] = false;
                }
            }
        }
        private void FillListBox()
        {
            XmlDocument configs = new XmlDocument();
            configs.Load("EncodingConfigurations.xml");

            foreach (XmlNode node in configs.DocumentElement)
            {
                EncodingConfigs_lsb.Items.Add(node["String"].InnerText);
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

        private void Timer1_Tick(object sender, EventArgs e)
        {
            Label[] labels = { label1, label2, label3, label4, label5 };
            for (int i = 0; i < COUNT; i++)
            {
                try
                {
                    if (m_arrLiveState[i])
                        UpdateStatistic(labels[i], m_arrWriters[i]);
                }
                catch { }
                }


            float fcpu =CPU.NextValue();
            string curCPUvalue = string.Format("{0:0.00}%", fcpu);

            CPUload_lbl.Text = curCPUvalue;
            if (fcpu > 90)
            {
                string now = DateTime.Now.ToString("HH:mm:ss");
                CheckCPU_lsb.Items.Insert(0,now + "--" + curCPUvalue);
            }
            progressBarCPU.Value = (int)fcpu;

        }
        private void UpdateStatistic(Label label, MFWriterClass m_objWriter)
        {
            label.BackColor = Color.OrangeRed;

            string srtOrPath, optionsList;
            string fIn, bIn, fDrops, buff;

            m_objWriter.WriterGet(out srtOrPath, out optionsList);

            int propsCount;
            m_objWriter.PropsGetCount("stat", out propsCount);
            StringBuilder propsBuilder = new StringBuilder();
            for (int propsIndex = 0; propsIndex < propsCount; propsIndex++)
            {
                string propsName;
                string propsValue;
                int propsIsNode;
                m_objWriter.PropsGetByIndex("stat", propsIndex, out propsName, out propsValue, out propsIsNode);
                propsBuilder.AppendLine(propsName + " : " + propsValue);
            }

            string propsStatList = propsBuilder.ToString();
            m_objWriter.PropsGet("stat::frames_in", out fIn);
            m_objWriter.PropsGet("stat::breaks_in", out bIn);

            m_objWriter.PropsGet("stat::buffers_overflow", out fDrops);
            m_objWriter.PropsGet("stat::buffered", out buff);


            label.Text = "Capturing file: " + srtOrPath + "\n\n" +
                                   "Frames In: " + fIn + "; " +
                                   "Break In: " + bIn + "; " +
                                   "Frame drops: " + fDrops + ";\n" +
                                   "Buffer: " + buff;

        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            for (int i = 0; i < COUNT; i++)
            {
                if (m_arrWriters[i] != null)
                {
                    m_arrWriters[i].WriterClose(0);
                    Thread.Sleep(60);
                    Marshal.ReleaseComObject(m_arrWriters[i]);
                }
                if (m_arrPreviews[i] != null)
                {
                    m_arrPreviews[i].MFClose();
                    Thread.Sleep(60);
                    Marshal.ReleaseComObject(m_arrPreviews[i]);
                }

                if (m_arrLives[i] != null)
                {
                    m_arrLives[i].MFClose();
                    Marshal.ReleaseComObject(m_arrPreviews[i]);
                }


            }
            GC.Collect();
        }
    }
}
