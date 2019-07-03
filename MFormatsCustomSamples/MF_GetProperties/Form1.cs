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
using Microsoft.Win32;

namespace MF_GetProperties
{
    public partial class Form1 : Form
    {
        MFReaderClass m_objReader;
        MFPreviewClass m_objPrev;
        MFPreviewClass m_objPrevResize;

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;

        string path;
        bool start = false;
        bool TC = true;
        int countProps = 0;

        public delegate void InvokeDel();

        string TC_data;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_objReader = new MFReaderClass();
            m_objPrev = new MFPreviewClass();
            m_objPrevResize = new MFPreviewClass();

            m_objPrevResize.PreviewWindowSet("", panelThubNail.Handle.ToInt32());
            m_objPrevResize.PreviewEnable("", 1, 1);

            m_objPrev.PreviewWindowSet("", panel1.Handle.ToInt32());
            m_objPrev.PreviewEnable("", 1, 1);

            string a = " - MFormats SDK " + CheckVersionClass.GetVersion();

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }
        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                if (start && path!=null)
                {
                    MFFrame pFrame = null;
                    MFFrame pFrameResize = null;


                    m_objReader.SourceFrameGet(-1, out pFrame, "");

                    if(Resize_chb.Checked)
                    {
                        pFrame.MFResize(eMFCC.eMFCC_RGB32, 256, 144, 0, out pFrameResize, "", "");

                        m_objPrevResize.ReceiverFramePut(pFrameResize, -1, "");
                    }

                    M_TIME timeF;
                    pFrame.MFTimeGet(out timeF);
                    

                    if (TC)
                    {
                        GetTC(timeF);
                    }

                    m_objPrev.ReceiverFramePut(pFrame, -1, "");

                    Marshal.ReleaseComObject(pFrame);

                    if (pFrameResize != null)
                        Marshal.ReleaseComObject(pFrameResize);


                }
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {

            start = start == false ? true : false;
            if (start)
                OpenFile();
        }
       
        void OpenFile()
        {
            openFileDialog1 = new OpenFileDialog();
            if (openFileDialog1.ShowDialog() == DialogResult.OK && openFileDialog1.FileName != string.Empty)
                path = openFileDialog1.FileName;

            m_objReader.ReaderOpen(path, "");
           
            m_objReader.PropsGetCount("stat", out countProps);

            GetProps();


        }

        private void button2_Click(object sender, EventArgs e)
        {
            TC = true; 
        }

        void GetTC(M_TIME time)
        {
           m_objReader.PropsGetCount("stat", out countProps);
           TC_data = string.Format("{0:D2}: {1:D2}: {2:D2}. {3:D3}", time.tcFrame.nHours, time.tcFrame.nMinutes, time.tcFrame.nSeconds, time.tcFrame.nFrames);
           TC = false;
           BeginInvoke(new InvokeDel(Update_lbl));
            GetProps();
        }
        void Update_lbl()
        {
            label2.Text = TC_data;
            label3.Text = countProps.ToString();
        }
        void GetProps()
        {
            string name;
            string value;
            int node;
            List<string> props = new List<string>();
            for (int i = 0; i < countProps; i++)
            {
                m_objReader.PropsGetByIndex("stat", i, out name, out value, out node);
                string add = name + " -- " + value + " -- " + node.ToString();
                props.Add(add);
            }
            int a = 42;
        }

        class CheckVersionClass
        {
            public static string GetVersion()
            {
                string sVersion = SearchIstalledVersion(32);

                return string.IsNullOrEmpty(sVersion) ? SearchIstalledVersion(64) : sVersion;
            }

            private static string SearchIstalledVersion(int bit)
            {
                string subKey = string.Empty;
                RegistryKey m__ReadKey;
                RegistryKey m_ReadSubKey;

                switch (bit)
                {
                    case 64:
                        subKey = @"SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\";
                        break;
                    case 32:
                        subKey = @"SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\";
                        break;
                }

                m__ReadKey = Registry.LocalMachine.OpenSubKey(subKey);
                string[] subKeyNames = m__ReadKey.GetSubKeyNames();

                foreach (string subKeyName in subKeyNames)
                {
                    m_ReadSubKey = Registry.LocalMachine.OpenSubKey(subKey + subKeyName);

                    string[] subValuesNames = m_ReadSubKey.GetValueNames();
                    foreach (string subname in subValuesNames)
                    {
                        if (subname == "DisplayName")
                        {
                            try
                            {
                                string displayNameValue = (string)m_ReadSubKey.GetValue(subname);
                                if (displayNameValue.Contains("MFormats"))
                                {
                                    return (string)m_ReadSubKey.GetValue("DisplayVersion");
                                }
                            }
                            catch { }
                        }
                    }
                }

                return string.Empty;
            }
        }

    }
}
