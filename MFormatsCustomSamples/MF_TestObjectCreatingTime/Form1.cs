using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MFORMATSLib;

namespace MF_TestObjectCreatingTime
{
    public partial class Form1 : Form
    {

        DateTime before;
        DateTime after;
        TimeSpan dif;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Objects_lsb.Items.Add("Preview");
            Objects_lsb.Items.Add("Writer");
            Objects_lsb.Items.Add("Reader");
            Objects_lsb.Items.Add("Live");
            Objects_lsb.Items.Add("WebRTC");
        }
        

        private void Objects_lsb_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListBox comboBox = (ListBox)sender;
            int selected = (int)comboBox.SelectedIndex;
            if (selected == 0)
            {
                CreatePreviews();
            }
            else if (selected == 1)
            {
                CreateWriters();
            }
            else if (selected == 2)
            {
                CreateReaders();
            }
            else if (selected == 3)
            {
                CreateLives();
            }
            else if (selected == 4)
            {
                CreateWebRTCs();
            }
        }

        private void CreateWebRTCs()
        {
            before = DateTime.Now;
            MFORMATSLib.MWebRTCClass a = new MFORMATSLib.MWebRTCClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MWebRTCClass b = new MFORMATSLib.MWebRTCClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MWebRTCClass c = new MFORMATSLib.MWebRTCClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }

        private void CreateLives()
        {
            before = DateTime.Now;
            MFORMATSLib.MFLiveClass a = new MFORMATSLib.MFLiveClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFLiveClass b = new MFORMATSLib.MFLiveClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFLiveClass c = new MFORMATSLib.MFLiveClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }


        private void CreateReaders()
        {
            before = DateTime.Now;
            MFORMATSLib.MFReaderClass a = new MFORMATSLib.MFReaderClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFReaderClass b = new MFORMATSLib.MFReaderClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFReaderClass c = new MFORMATSLib.MFReaderClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }

        private void CreateWriters()
        {
            before = DateTime.Now;
            MFORMATSLib.MFWriterClass a = new MFORMATSLib.MFWriterClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFWriterClass b = new MFORMATSLib.MFWriterClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFWriterClass c = new MFORMATSLib.MFWriterClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }



        private void CreatePreviews()
        {
            before = DateTime.Now;
            MFORMATSLib.MFPreviewClass a = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFPreviewClass b = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFPreviewClass c = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;
            dif = after - before;
            Stat_lsb.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }
    }
}

