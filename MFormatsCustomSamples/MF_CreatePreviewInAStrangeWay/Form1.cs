using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MF_CreatePreviewInAStrangeWay
{
    public partial class Form1 : Form
    {
        
        public Form1()
        {
            InitializeComponent();
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            TestVar();

            TestObj();

        }

        private void TestObj()
        {
            DateTime before = DateTime.Now;
            MFORMATSLib.MFPreviewClass a = new MFORMATSLib.MFPreviewClass();
            DateTime after = DateTime.Now;

            TimeSpan dif = after - before;
            listBox1.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFPreviewClass b = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;

            dif = after - before;
            listBox1.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            MFORMATSLib.MFPreviewClass c = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;

            dif = after - before;
            listBox1.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }

        private void TestVar()
        {
            DateTime before = DateTime.Now;
            var test = new MFORMATSLib.MFPreviewClass();
            DateTime after = DateTime.Now;

            TimeSpan dif = after - before;
            listBox1.Items.Add("First time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            var test2 = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;

            dif = after - before;
            listBox1.Items.Add("Second time we create: " + dif.TotalMilliseconds.ToString());

            before = DateTime.Now;
            var test3 = new MFORMATSLib.MFPreviewClass();
            after = DateTime.Now;

            dif = after - before;
            listBox1.Items.Add("Third time we create: " + dif.TotalMilliseconds.ToString());
        }
    }
}
