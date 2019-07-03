using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using MFORMATSLib;

namespace MF_1000Frames
{
    public partial class Form1 : Form
    {
        MFReaderClass reader;
        MFWriterClass writer;
        MFPreviewClass prev;
        int count = 0;

        public delegate void InvokeDelegate();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            reader = new MFReaderClass();
            prev = new MFPreviewClass();
            writer = new MFWriterClass();           
        }

        private void Button1_Click(object sender, EventArgs e)
        {         
            reader.PropsSet("experimental.mfcodecs", "true");
            reader.PropsSet("experimental.out_video_packets", "true");
            reader.PropsSet("external_process", "false");
            reader.ReaderOpen(@"M:\TEST_VIDEOS\Moneyball.2011.720p.BluRay.x264-HiDt.mkv", "");

            writer.PropsSet("external_process", "false");
            writer.WriterSet(@"M:\TempVideo\mariovideo.mp4", 0, "format='mp4' video::codec='packets' audio::codec='audio_packets'");
            //writer.PropsSet("rate_control", "true");
            //writer.PropsSet("pull_mode", "false");

            
            var w = new Stopwatch();

            w.Start();

            DateTime start = DateTime.Now;
            while (count <= 2000)
            {
                count++;
                if (count >= 1000)
                {
                    MFFrame frame;
                    reader.SourceFrameGet(-1, out frame, "");
                    writer.ReceiverFramePut(frame, -1, "");
                    prev.ReceiverFramePut(frame, -1, "");
                    Marshal.ReleaseComObject(frame);

                    BeginInvoke(new InvokeDelegate(UpdateLabel));
                }    

            }
            DateTime end = DateTime.Now;
            w.Stop();
            writer.WriterClose(0);
            reader.ReaderClose();
            int dif = (end.Millisecond + end.Second * 1000 + end.Minute * 60 * 1000) - (start.Millisecond + start.Second * 1000 + start.Minute*60 *1000);
            button1.Text = "END. Dif ==" + dif.ToString();
        }

        private void UpdateLabel()
        {
            label1.Text = count.ToString();
        }
    }
}
