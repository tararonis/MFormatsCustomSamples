using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Resources;
using MFORMATSLib;

namespace MF_ticket31671
{
    public partial class Form1 : Form
    {
        MFLiveClass m_objLive;
        MFPreviewClass m_objPreview;

        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                StreamResourceInfo sri = Application.GetResourceStream(new Uri("/APP;component/Resources/noinputselected.png", UriKind.Relative));
                Bitmap bm = new Bitmap(sri.Stream);
                IntPtr hBitmap = bm.GetHbitmap();
                MFFactoryClass frameFactory = new MFFactoryClass();
                frameFactory.MFFrameCreateFromHBITMAP(hBitmap.ToInt64(), out noSignalFrame, "");
                noSignalFrame.MFResize(eMFCC.eMFCC_Default, 1920, 1080, -1, out noSignalFrame, "", "");

            }
        }
    }
}
