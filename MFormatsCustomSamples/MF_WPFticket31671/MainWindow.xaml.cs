using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Interop;
using System.Windows.Resources;
using MFORMATSLib;
using System.IO;

namespace MF_WPFticket31671
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>

    public partial class MainWindow : Window
    {
        bool decoder = false;
        private CancellationTokenSource cancelSource;
        private Thread m_threadWorker;
        MFPreviewClass m_objPreview;
        MFLiveClass m_objLive;
        private D3DImage previewSource;
        MFReaderClass m_objReader;
        MFFrame noSignalFrame;

        public MainWindow()
        {
            InitializeComponent();
        }
        private void Grid_Loaded(object sender, RoutedEventArgs e)
        {
            m_objPreview = new MFPreviewClass();
            m_objLive = new MFLiveClass();
            m_objReader = new MFReaderClass();

            m_objPreview.PropsSet("wpf_preview", "true");
            previewSource = new D3DImage();

            m_objPreview.PreviewEnable("", 1, 1);
            m_objPreview.OnEventSafe += M_objPreview_OnEventSafe;
            GetNoSignalFrame();

            cancelSource = new CancellationTokenSource();
            m_threadWorker = new Thread(() => thread_DoWork(cancelSource.Token));
            m_threadWorker.Name = "thread_DoWork";
            m_threadWorker.Start();
        }

        private IntPtr pSavedSurfaceIUnk;
        private void M_objPreview_OnEventSafe(string bsChannelID, string bsEventName, string bsEventParam, object pEventObject)
        {
            // specific name for event is "wpf_nextframe"
            if (bsEventName == "wpf_nextframe")
            {
                // it is necessary to keep a pointer in memory cause in case of format or source changes the pointer can be changed too
                IntPtr pSurfaceIUnk = Marshal.GetIUnknownForObject(pEventObject);
                if (pSurfaceIUnk != IntPtr.Zero)
                {
                    if (pSurfaceIUnk != pSavedSurfaceIUnk)
                    {

                        // Release prev object
                        if (pSavedSurfaceIUnk != IntPtr.Zero)
                        {
                            Marshal.Release(pSavedSurfaceIUnk);
                            pSavedSurfaceIUnk = IntPtr.Zero;
                        }

                        // here we change back buffer of the surface (only in case of the pointer is changed)
                        pSavedSurfaceIUnk = pSurfaceIUnk;
                        Marshal.AddRef(pSavedSurfaceIUnk);

                        // Lock and Unlock are required for update of the surface
                        previewSource.Lock();
                        previewSource.SetBackBuffer(D3DResourceType.IDirect3DSurface9, IntPtr.Zero);
                        previewSource.SetBackBuffer(D3DResourceType.IDirect3DSurface9, pSavedSurfaceIUnk);
                        previewSource.Unlock();
                        // use this 3D surface as source for preview control
                        preview.Source = previewSource;
                        //GC.Collect();
                    }

                    Marshal.Release(pSurfaceIUnk);
                }


                Marshal.ReleaseComObject(pEventObject);

                // update of preview rectangle
                previewSource.Lock();
                try
                {
                    previewSource.AddDirtyRect(new Int32Rect(0, 0, previewSource.PixelWidth, previewSource.PixelHeight));
                }
                catch (Exception)
                {
                    previewSource.SetBackBuffer(D3DResourceType.IDirect3DSurface9, pSavedSurfaceIUnk);
                }
                previewSource.Unlock();
            }
        }

        
        private void thread_DoWork(CancellationToken token)
        {
            while (!token.IsCancellationRequested)
            {
                MFFrame frame = null;
                if (!decoder)
                {
                    noSignalFrame.MFClone(out frame, eMFrameClone.eMFC_Full, eMFCC.eMFCC_Default);
                    m_objPreview.ReceiverFramePut(frame, -1, "");
                }
                if (decoder)
                {                    
                    m_objLive.SourceFrameGet(-1, out frame, "");                   
                    m_objPreview.ReceiverFramePut(frame, -1, "");
                }
                if (frame != null)
                {
                    Marshal.ReleaseComObject(frame);
                }
            }
            }

        void GetNoSignalFrame()
        {
            Uri uri1 = new Uri("800m800.jpeg", UriKind.Relative);            
            StreamResourceInfo sri = Application.GetResourceStream(uri1);
            Bitmap bm = new Bitmap(sri.Stream);
            IntPtr hBitmap = bm.GetHbitmap();
            MFFactoryClass frameFactory = new MFFactoryClass();
            frameFactory.MFFrameCreateFromHBITMAP(hBitmap.ToInt64(), out noSignalFrame, "");
            noSignalFrame.MFResize(eMFCC.eMFCC_Default, 1920, 1080, -1, out noSignalFrame, "", "");
        }
       
        private void Button_Click(object sender, RoutedEventArgs e)
        {          
            m_objLive.DeviceSet(eMFDeviceType.eMFDT_Video, 1, ""); //On my side it's DUO2
            decoder = decoder == true ? false : true;
        }

       
    }
}
