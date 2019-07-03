using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using CGEditor.CGItemWrappers;
using CGEditor_WinForms.CGItemWrappers;
using MLCHARGENLib;
using MFORMATSLib;
using eMFCC = MFORMATSLib.eMFCC;
using eMFrameClone = MFORMATSLib.eMFrameClone;
using M_AV_PROPS = MFORMATSLib.M_AV_PROPS;

namespace CGEditor_WinForms
{
    public partial class MainWindow : Form
    {
        //ItemListForm itemListForm1 = new ItemListForm();
        //PropertiesForm addedListForm = new PropertiesForm();
        //GeneralForm ctrlPreview = new GeneralForm();
        //XMLDescription xmlDescriptionForm = new XMLDescription();
        //ListBox ListBoxTools = new ListBox();
        //ListBox listBoxItems = new ListBox();
        private CGBaseItem m_BaseItem;

        //MPlaylistClass testPlaylist = new MPlaylistClass();

        //  CoMLCharGenClass  charGen = new CoMLCharGenClass();

        public MainWindow()
        {

            m_objEditor = new Editor();
            //testPlaylist.ObjectStart(null);
            //testPlaylist.FilePlayStart();
            //testPlaylist.PluginsAdd(charGen,0);
            //string testID = "";
            //charGen.AddNewItem("Test Text Item", 0.05, 0.05, 1, 1, ref testID);
            //testID = "";
            //charGen.AddNewItem("2nd text", 0.15, 0.15, 1, 1, ref testID);
            //SetSourceObject(testPlaylist, charGen);
            InitializeComponent();
            m_objEditor.ItemSelected += new EventHandler(m_objEditor_ItemSelected);
            m_objEditor.ItemsListUpdated += new EventHandler(m_objEditor_ItemsListUpdated);
            m_objEditor.FrameProcessed += new EventHandler(m_objEditor_FrameProcessed);
            MainPreview.PreviewItemDoubleClick += new EventHandler(ctrlPreview_ItemDoubleClick);
            MainPreview.PreviewItemDragged += new EventHandler(ctrlPreview_ItemDragged);
            MainPreview.PreviewDoubleClick += new EventHandler(ctrlPreview_DoubleClick);
            MainPreview.PreviewLeftButtonDown += new EventHandler(ctrlLeftButtonDown_Click);
            MainPreview.PreviewLeftButtonUp += ctrlPreview_PreviewLeftButtonUp;
            MainPreview.Editor = m_objEditor;
            this.Editor = m_objEditor;
            ListBoxTools.Items.Add("Text");
            ListBoxTools.Items.Add("Graphics");
            ListBoxTools.Items.Add("Image");
            ListBoxTools.Items.Add("Image Sequence");
            ListBoxTools.Items.Add("Video");
            ListBoxTools.Items.Add("Flash");
            ListBoxTools.Items.Add("Crawling Ticker");
            ListBoxTools.Items.Add("Rolling Ticker");
            ListBoxTools.Items.Add("Ticker");
            ListBoxTools.ItemHeight = 50;
            ListBoxTools.Font = new Font("Segoe UI", 15);
            ListBoxTools.SelectionMode = SelectionMode.One;
            ListBoxTools.MouseDown += ListBoxTools_MouseDown;

            listBoxItems.Font = new Font("Segoe UI", 12);
            listBoxItems.SelectionMode = SelectionMode.MultiExtended;
            listBoxItems.DisplayMember = "ID";
            listBoxItems.MouseDown += listBoxItems_MouseDown;
            listBoxItems.KeyUp += listBoxItems_KeyUp;
            listBoxItems.SelectedIndexChanged += listBoxItems_SelectionChanged;
            //ListBoxTools.SetControlledObject(ListBoxTools);
            //addedListForm.SetControlledObject(listBoxItems);
            m_nFrameCounter = 0;
            HideTabs();
        }

        void ctrlPreview_PreviewLeftButtonUp(object sender, EventArgs e)
        {
            XMLDescription.SetControlledObject(null);
            if (m_objEditor.SelectedtItems.Count == 1)
            {
                XMLDescription.SetControlledObject(m_objEditor.SelectedtItems[0]);
            }
        }

        void listBoxItems_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                m_objEditor.SelectItem("", true);
            }
            if (e.KeyCode == Keys.Delete)
            {
                UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_objEditor.UndoAction, new HistoryItem(m_objEditor.CurrentState, ""));
                for (int i = 0; i < listBoxItems.SelectedItems.Count; i++)
                {
                    string strId = ((CGBaseItem)listBoxItems.SelectedItems[i]).ID;
                    m_objEditor.CGObject.RemoveItemWithDelay(strId, 0, 0);
                }
                m_objEditor.UpdateItemsList();
            }
        }

        void listBoxItems_MouseDown(object sender, MouseEventArgs e)
        {
            string[] strIds = new string[listBoxItems.SelectedItems.Count];
            for (int i = 0; i < listBoxItems.SelectedItems.Count; i++)
            {
                strIds[i] = ((CGBaseItem)listBoxItems.SelectedItems[i]).ID;
            }
            if (strIds.Length > 0)
                m_objEditor.SelectItems(strIds, true);
            listBoxItems.SelectedIndexChanged -= listBoxItems_SelectionChanged;
            if (listBoxItems.IndexFromPoint(new Point(e.X, e.Y)) == ListBox.NoMatches)
            {
                //listBoxItems.ClearSelected();
                //listBoxItems.SelectedIndex = -1;
                m_objEditor.SelectItem("", true);
            }
            listBoxItems.SelectedIndexChanged += listBoxItems_SelectionChanged;
        }

        void ListBoxTools_MouseDown(object sender, MouseEventArgs e)
        {
            int indexOfItem = ListBoxTools.IndexFromPoint(e.X, e.Y);
            if (indexOfItem >= 0 && indexOfItem < ListBoxTools.Items.Count)  // check that an string is selected
            {
                ListBoxTools.DoDragDrop(ListBoxTools.Items[indexOfItem], DragDropEffects.Move);
            }
        }

        Editor m_Editor;
        public Editor Editor
        {
            get { return m_Editor; }
            set
            {
                m_Editor = value;
            }
        }

        private void ctrlLeftButtonDown_Click(object sender, EventArgs e)
        {
            if (ListBoxTools.SelectedItem != null && MainPreview.eMode == eMode.eM_Add)
            {
                try
                {
                    PreviewClickArgs args = (PreviewClickArgs)e;
                    var lbi = ListBoxTools.SelectedItem;
                    string tbCurrent = lbi.ToString();
                    string strType = tbCurrent;
                    AddDefaultItem(strType, args.CGPoint);
                    MainPreview.eMode = eMode.eM_Default;
                    MainPreview.SetCurcsor(Cursors.Arrow);
                }
                catch
                {
                }
            }
        }

        private void AddDefaultItem(string type, Point cgCoords)
        {
            if (type != null && type != string.Empty)
            {
                try
                {
                    UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_objEditor.UndoAction, new HistoryItem(m_objEditor.CurrentState, ""));
                    string strID = "";
                    if (type == "Text")
                    {
                        m_objEditor.CGObject.AddNewItem("Sample Text", cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                        m_objEditor.UpdateItemsList();
                    }
                    if (type == "Graphics")
                    {
                        m_objEditor.CGObject.AddNewItem(@"<graphics type='rect' round-corners='0.30' outline='3.0' color='ML(180)' outline-color='white'/>", cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                        m_objEditor.UpdateItemsList();
                    }
                    if (type == "Image")
                    {
                        if (m_strImagePath == null || m_strImagePath == string.Empty)
                        {

                            OpenFileDialog openFile = new OpenFileDialog();
                            openFile.Multiselect = false;
                            openFile.Title = "Select image";
                            openFile.Filter = "All Images|*.BMP;*.JPG;*.JPEG;*.GIF;*.TIF;*.TIFF;*.PNG";
                            if (openFile.ShowDialog() == DialogResult.OK)
                            {
                                m_objEditor.CGObject.AddNewItem(openFile.FileName, cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                                m_objEditor.UpdateItemsList();
                                m_strImagePath = openFile.FileName;
                            }
                        }
                        else
                        {
                            m_objEditor.CGObject.AddNewItem(m_strImagePath, cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                            m_objEditor.UpdateItemsList();
                        }
                    }
                    if (type == "Video")
                    {
                            OpenFileDialog openFile = new OpenFileDialog();
                            openFile.Multiselect = false;
                            openFile.Title = "Select video";
                            openFile.Filter = "All Videos|*.MP4;*.MOV;*.AVI;*.MXF;*.TS";
                            if (openFile.ShowDialog() == DialogResult.OK)
                            {
                                m_objEditor.CGObject.AddNewItem(openFile.FileName, cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                                m_objEditor.UpdateItemsList();
                                m_strImagePath = openFile.FileName;
                            }
                    }
                    if (type == "Image Sequence")
                    {
                        System.Windows.Forms.FolderBrowserDialog openFolder = new System.Windows.Forms.FolderBrowserDialog();
                        //System.Windows.Forms.DialogResult reopenFoldersult = openFolder.ShowDialog();
                        if (openFolder.ShowDialog() == System.Windows.Forms.DialogResult.OK)
                        {
                            m_objEditor.CGObject.AddNewItem(openFolder.SelectedPath, cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                            m_objEditor.UpdateItemsList();
                        }
                    }
                    if (type == "Flash")
                    {
                        if (m_strFlashPath == null || m_strFlashPath == string.Empty)
                        {
                            OpenFileDialog openFile = new OpenFileDialog();
                            openFile.Multiselect = false;
                            openFile.Title = "Select flash file";
                            openFile.Filter = "JPEG Files (*.swf)|*.swf";
                            if (openFile.ShowDialog() == DialogResult.OK)
                            {
                                m_objEditor.CGObject.AddNewItem(openFile.FileName, cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                                m_objEditor.UpdateItemsList();
                                m_strFlashPath = openFile.FileName;
                            }
                        }
                        else
                        {
                            m_objEditor.CGObject.AddNewItem(m_strFlashPath, cgCoords.X, cgCoords.Y, 0, 1, ref strID);
                            m_objEditor.UpdateItemsList();
                        }
                    }
                    if (type == "Ticker")
                    {
                        string strTickerDescr = @"<ticker type='ticker'>
<background color='ML(180)-White(180)'/>
<default-text bold=true size='20' style='uppercase'/>
</ticker>";
                        m_objEditor.CGObject.TickerAddNew(strTickerDescr, cgCoords.X, cgCoords.Y, 500, 100, 0, 1, ref strID);
                        m_objEditor.CGObject.TickerAddContent(strID, "Ticker content1\r\nTickercontent2\r\nTicker content3", "remove-all");
                        m_objEditor.UpdateItemsList();
                    }
                    if (type == "Crawling Ticker")
                    {
                        string strTickerDescr = @"<ticker type='crawl'>
<background color='ML(180)-White(180)'/>
<default-text bold=true size='20' style='uppercase'/>
</ticker>";
                        m_objEditor.CGObject.TickerAddNew(strTickerDescr, cgCoords.X, cgCoords.Y, 700, 50, 0, 1, ref strID);
                        m_objEditor.CGObject.TickerAddContent(strID, "Scrolling ticker content", "multiline, remove-all");
                        m_objEditor.UpdateItemsList();
                    }
                    if (type == "Rolling Ticker")
                    {
                        string strTickerDescr = @"<ticker type='roll'>
<background color='ML(180)*White(180)'/>
<default-text bold=true size='20' style='uppercase'/>
</ticker>";
                        m_objEditor.CGObject.TickerAddNew(strTickerDescr, cgCoords.X, cgCoords.Y, 500, 300, 0, 1, ref strID);
                        m_objEditor.CGObject.TickerAddContent(strID, "Rolling ticker content", "multiline, remove-all");
                        m_objEditor.UpdateItemsList();
                    }
                }
                catch
                { }
            }
        }


        [DllImport("gdi32.dll")]
        private static extern bool DeleteObject(IntPtr hObject);

        public static Bitmap CreateBitmapSourceFromBitmap(System.Drawing.Bitmap bitmap)
        {
            if (bitmap == null)
                throw new ArgumentNullException("bitmap");

            lock (bitmap)
            {
                IntPtr hBitmap = new IntPtr();

                try
                {
                    hBitmap = bitmap.GetHbitmap();

                    return Image.FromHbitmap(
                        hBitmap,
                        IntPtr.Zero);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }

                finally
                {
                    if (hBitmap != null)
                        DeleteObject(hBitmap);

                    GC.Collect();
                }
            }

            return null;
        }

        void ctrlPreview_ItemDoubleClick(object sender, EventArgs e)
        {
            if (sender is CGTextItem)
            {
                //propertyGridProps.SelectedProperty = "Text";
                //ContentManagers.TextContentManager contentWindow = new ContentManagers.TextContentManager();

                //bool? result = contentWindow.ShowDialog();
                //if (result == true && contentWindow.NewContent != string.Empty)
                //{
                //    ((CGTextItem)sender).Text = contentWindow.NewContent;
                //}
            }
            else if (sender is CGTickerItem)
            {
                m_objEditor.CurrGroupID = ((CGTickerItem)sender).ID;
                m_objEditor.UpdateItemsList();
            }
            else if (sender is CGImageItem)
            {
                OpenFileDialog openFile = new OpenFileDialog();
                openFile.Multiselect = false;
                openFile.Title = "Select image";
                openFile.Filter = "All Images|*.BMP;*.JPG;*.JPEG;*.GIF;*.TIF;*.TIFF;*.PNG";
                if (openFile.ShowDialog() == DialogResult.OK)
                {
                    ((CGImageItem)sender).Path = openFile.FileName;
                }
            }
            else if (sender is CGVideoItem)
            {
                OpenFileDialog openFile = new OpenFileDialog();
                openFile.Multiselect = false;
                openFile.Title = "Select video";
                openFile.Filter = "All Videos|*.MP4;*.MOV;*.AVI;*.MXF;*.TS";
                if (openFile.ShowDialog() == DialogResult.OK)
                {
                    ((CGVideoItem)sender).Path = openFile.FileName;
                }
            }
            else if (sender is CGGroupItem)
            {
                m_objEditor.CurrGroupID = ((CGGroupItem)sender).ID;
                m_objEditor.UpdateItemsList();
            }
            else if (sender is CGFlashItem)
            {
                OpenFileDialog openFile = new OpenFileDialog();
                openFile.Multiselect = false;
                openFile.Title = "Select flash file";
                openFile.Filter = "Flash Files (*.swf)|*.swf";
                if (openFile.ShowDialog() == DialogResult.OK)
                {
                    ((CGFlashItem)sender).Path = openFile.FileName;
                }
            }
        }

        void m_objEditor_ItemsListUpdated(object sender, EventArgs e)
        {
            this.listBoxItems.SelectedIndexChanged -= listBoxItems_SelectionChanged;
            this.listBoxItems.Items.Clear();// = null;
            this.listBoxItems.Items.AddRange(m_objEditor.CGItems.ToArray());
            //listBoxItems.DisplayMember = "ID";
            //listBoxItems.ClearSelected();
            listBoxItems.SelectedIndex = -1;
            for (int i = 0; i < m_objEditor.CGItems.Count; i++)
            {
                if (m_objEditor.CGItems[i].isSelected)
                {
                    listBoxItems.SelectedItems.Add(m_objEditor.CGItems[i]);
                }
            }

            if (m_objEditor.SelectedtItems.Count == 1)
            {
                this.RibbonSetControlledObject(m_objEditor.SelectedtItems[0]);
                XMLDescription.SetControlledObject(m_objEditor.SelectedtItems[0]);

            }

            this.listBoxItems.SelectedIndexChanged += listBoxItems_SelectionChanged;
        }

        void m_objEditor_ItemSelected(object sender, EventArgs e)
        {
            RibbonSetControlledObject(null);
            XMLDescription.SetControlledObject(null);

            listBoxItems.SelectedIndexChanged -= listBoxItems_SelectionChanged;

            //listBoxItems.ClearSelected();
            listBoxItems.SelectedIndex = -1;
            foreach (CGBaseItem item in m_objEditor.SelectedtItems)
            {
                if (item.isSelected)
                {
                    listBoxItems.SelectedItems.Add(item);
                }
            }

            if (m_objEditor.SelectedtItems.Count == 1)
            {
                RibbonSetControlledObject(m_objEditor.SelectedtItems[0]);
                XMLDescription.SetControlledObject(m_objEditor.SelectedtItems[0]);

            }

            listBoxItems.SelectedIndexChanged += listBoxItems_SelectionChanged;
        }

        private bool flagIsOver = true;
        public void m_objEditor_FrameProcessed(object sender, EventArgs e)
        {
            if (flagIsOver)
                ThreadPool.QueueUserWorkItem(delegate { BodyAction(sender); });
        }

        private void BodyAction(object sender)
        {
            try
            {
                flagIsOver = false;
                MFFrame pFrame = (MFFrame)sender;
                MFFrame clonedFrame;
                pFrame.MFClone(out clonedFrame, eMFrameClone.eMFC_Video, eMFCC.eMFCC_ARGB32);
                M_AV_PROPS avProps;
                int audiosamples;
                clonedFrame.MFAVPropsGet(out avProps, out audiosamples);

                int cbPicture;
                long pbPicture;
                clonedFrame.MFVideoGetBytes(out cbPicture, out pbPicture);
                int nRowBytes = avProps.vidProps.nRowBytes;
                IntPtr ptr = new IntPtr(pbPicture);
                Image bSource = new Bitmap(avProps.vidProps.nWidth, Math.Abs(avProps.vidProps.nHeight), nRowBytes,
                    PixelFormat.Format32bppRgb, ptr);
                this.Invoke(new Action(() => MainPreview.SetSource(bSource)));
                Marshal.ReleaseComObject(clonedFrame);

                m_nFrameCounter++;
                if (m_nFrameCounter == 10)
                {
                    GC.Collect();
                    m_nFrameCounter = 0;
                }
                flagIsOver = true;
            }
            catch
            {
                flagIsOver = true;
            }
        }

        private void listBoxItems_SelectionChanged(object sender, EventArgs e)
        {
        }

        void ctrlPreview_DoubleClick(object sender, EventArgs e)
        {
            m_objEditor.CurrGroupID = "";
        }

        void ctrlPreview_ItemDragged(object sender, EventArgs e)
        {
            MainPreview.eMode = eMode.eM_Default;
            MainPreview.SetCurcsor(Cursors.Arrow);
            try
            {
                ItemDragEventArgs args = (ItemDragEventArgs)e;
                AddDefaultItem(args.ItemType, new Point(args.Location.X, args.Location.Y));
            }
            catch
            {
            }
        }

        public void RibbonSetControlledObject(CGBaseItem cgItem)
        {
            try
            {
                m_BaseItem = cgItem;
                if (m_BaseItem != null)
                {
                    BasicVisibilityEditor.SetControlledObject(cgItem);
                    BasicVisibilityEditor.SetEditor(m_Editor);

                    BasicPositionEditor.SetControlledObject(cgItem);
                    BasicPositionEditor.SetEditor(m_Editor);

                    BasicColorEditor.SetControlledObject(cgItem);
                    BasicColorEditor.SetEditor(m_Editor);

                    BasicMiscEditor.SetControlledObject(cgItem);
                    BasicMiscEditor.SetEditor(m_Editor);

                    EffectsSpeedEditor.SetControlledObject(cgItem);
                    EffectsSpeedEditor.SetEditor(m_Editor);

                    EffectsShadowEditor.SetControlledObject(cgItem);
                    EffectsShadowEditor.SetEditor(m_Editor);

                    EffectsBlurEditor.SetControlledObject(cgItem);
                    EffectsBlurEditor.SetEditor(m_Editor);

                    EffectsGlowEditor.SetControlledObject(cgItem);
                    EffectsGlowEditor.SetEditor(m_Editor);

                    if (m_BaseItem is CGTextItem || m_BaseItem is CGTickerLine)
                    {
                        TextContEditor.SetControlledObject(cgItem);
                        TextContEditor.SetEditor(m_Editor);

                        TextColorEditor.SetControlledObject(cgItem);
                        TextColorEditor.SetEditor(m_Editor);

                        TextOutlineEditor.SetControlledObject(cgItem);
                        TextOutlineEditor.SetEditor(m_Editor);

                        TextFontEditor.SetControlledObject(cgItem);
                        TextFontEditor.SetEditor(m_Editor);

                        ShowTab(PageText);
                    }
                    if (m_BaseItem is CGGraphicsItem)
                    {
                        GraphicsShapeEditor.SetControlledObject(cgItem);
                        GraphicsShapeEditor.SetEditor(m_Editor);

                        GraphicsColorEditor.SetControlledObject(cgItem);
                        GraphicsColorEditor.SetEditor(m_Editor);

                        GraphicsOutlineEditor.SetControlledObject(cgItem);
                        GraphicsOutlineEditor.SetEditor(m_Editor);

                        ShowTab(PageGraphics);
                    }
                    if (m_BaseItem is CGTickerItem)
                    {
                        TickerNewContentEditor.SetControlledObject(cgItem);
                        TickerNewContentEditor.SetEditor(m_Editor);

                        TickerPropsEditor.SetControlledObject(cgItem);
                        TickerPropsEditor.SetEditor(m_Editor);

                        TickerShapeEditor.SetControlledObject(cgItem);
                        TickerShapeEditor.SetEditor(m_Editor);

                        TickerColorEditor.SetControlledObject(cgItem);
                        TickerColorEditor.SetEditor(m_Editor);

                        TickerOutlineEditor.SetControlledObject(cgItem);
                        TickerOutlineEditor.SetEditor(m_Editor);

                        TickerTextColorEditor.SetControlledObject(cgItem);
                        TickerTextColorEditor.SetEditor(m_Editor);

                        TickerTextOutlineEditor.SetControlledObject(cgItem);
                        TickerTextOutlineEditor.SetEditor(m_Editor);

                        TickerTextFontEditor.SetControlledObject(cgItem);
                        TickerTextFontEditor.SetEditor(m_Editor);

                        ShowTab(PageTickerContent);
                    }
                    if (m_BaseItem is CGImageItem)
                    {
                        ImageEditor.SetControlledObject(cgItem);
                        ImageEditor.SetEditor(m_Editor);
                        ShowTab(PageImage);
                    }
                    if (m_BaseItem is CGVideoItem)
                    {
                        openVideoEditor1.SetControlledObject(cgItem);
                        openVideoEditor1.SetEditor(m_Editor);
                        ShowTab(PageVideo);
                    }
                    if (m_BaseItem is CGFlashItem)
                    {
                        FlashEditor.SetControlledObject(cgItem);
                        FlashEditor.SetEditor(m_Editor);
                        ShowTab(PageFlash);
                    }
                }
                else
                {
                    ClearControls();
                    HideTabs();
                }
            }
            catch (System.Exception) { }
        }

        private void ShowTab(TabPage ribbonTabItem)
        {
            HideTabs();
            tabControl1.TabPages.Insert(2, ribbonTabItem);
            tabControl1.SelectedTab = ribbonTabItem;
            if (ribbonTabItem == PageTickerContent)
            {
                tabControl1.TabPages.Insert(3, PageTickerProps);
            }
        }

        private void HideTabs()
        {
            tabControl1.TabPages.Remove(PageText);
            tabControl1.TabPages.Remove(PageImage);
            tabControl1.TabPages.Remove(PageFlash);
            tabControl1.TabPages.Remove(PageVideo);
            tabControl1.TabPages.Remove(PageGraphics);
            tabControl1.TabPages.Remove(PageTickerContent);
            tabControl1.TabPages.Remove(PageTickerProps);
            
            //PageText.Visible = false;
            //PageImage.Visible = false;
            //PageFlash.Visible = false;
            //PageVideo.Visible = false;
            //PageGraphics.Visible = false;
            //PageTickerContent.Visible = false;
            //PageTickerProps.Visible = false;

            tabControl1.SelectedTab = PageCommon;
        }

        private void ClearControls()
        {
            ////================Clear basic props======================
            BasicVisibilityEditor.SetControlledObject(null);
            BasicPositionEditor.SetControlledObject(null);
            BasicColorEditor.SetControlledObject(null);
            BasicMiscEditor.SetControlledObject(null);
            ////================Clear effects======================
            EffectsSpeedEditor.SetControlledObject(null);
            EffectsShadowEditor.SetControlledObject(null);
            EffectsBlurEditor.SetControlledObject(null);
            EffectsGlowEditor.SetControlledObject(null);
            //================Clear text======================
            TextContEditor.SetControlledObject(null);
            TextColorEditor.SetControlledObject(null);
            TextOutlineEditor.SetControlledObject(null);
            TextFontEditor.SetControlledObject(null);
            //================Clear graphics======================
            GraphicsShapeEditor.SetControlledObject(null);
            GraphicsColorEditor.SetControlledObject(null);
            GraphicsOutlineEditor.SetControlledObject(null);
            // //================Clear ticker======================
            TickerNewContentEditor.SetControlledObject(null);
            TickerShapeEditor.SetControlledObject(null);
            TickerColorEditor.SetControlledObject(null);
            TickerOutlineEditor.SetControlledObject(null);
            TickerPropsEditor.SetControlledObject(null);
            TickerTextColorEditor.SetControlledObject(null);
            TickerTextOutlineEditor.SetControlledObject(null);
            TickerTextFontEditor.SetControlledObject(null);
            ////================Clear Image======================
            ImageEditor.SetControlledObject(null);
            ////================Clear Falsh======================
            FlashEditor.SetControlledObject(null);
            ////=================================================
            /// 
           // ribbon1.ResumeUpdating(true);
        }

        private void ribbonOrbOptionButton1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void SaveConfig_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveDlg = new SaveFileDialog();
            saveDlg.DefaultExt = ".ml-cgc"; // Default file extension
            saveDlg.Filter = "CG config files (.ml-cgc)|*.ml-cgc"; // Filter files by extension
            if (saveDlg.ShowDialog() == DialogResult.OK)
            {
                IMLXMLPersist pXMLPersist = (IMLXMLPersist)m_Editor.CGObject;
                pXMLPersist.SaveToXMLFile(saveDlg.FileName, 1);
            }
        }

        private void LoadConfig_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Multiselect = false;
            openFile.Filter = "CG config files (.ml-cgc)|*.ml-cgc"; // Filter files by extension
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                IMLXMLPersist pXMLPersist = (IMLXMLPersist)m_Editor.CGObject;
                pXMLPersist.LoadFromXML(openFile.FileName, -1);

                m_Editor.UpdateItemsList();
                List<CGBaseItem> items = m_Editor.CGItems;
                m_Editor.UpdateItemsList();
            }
        }

        private void BtnGroup_Click(object sender, EventArgs e)
        {
            if (m_Editor != null)
            {
                m_Editor.GroupSelected();
            }
        }

        private void BtnUnGroup_Click(object sender, EventArgs e)
        {
            if (m_Editor != null)
            {
                m_Editor.UnGroupSelected();
            }
        }

        private void BtnUp_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                for (int i = 0; i < m_Editor.SelectedtItems.Count; i++)
                {
                    m_Editor.CGObject.ChangeItemZOrder(m_Editor.SelectedtItems[i].ID, 1);
                    m_Editor.UpdateItemsList();
                }
            }
        }

        private void BtnDown_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                for (int i = 0; i < m_Editor.SelectedtItems.Count; i++)
                {
                    m_Editor.CGObject.ChangeItemZOrder(m_Editor.SelectedtItems[i].ID, -1);
                    m_Editor.UpdateItemsList();

                }
            }
        }

        private void BtnToBack_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                for (int i = 0; i < m_Editor.SelectedtItems.Count; i++)
                {
                    m_Editor.CGObject.ChangeItemZOrder(m_Editor.SelectedtItems[i].ID, -10000);
                    m_Editor.UpdateItemsList();
                }
            }
        }

        private void BtnToFront_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                for (int i = 0; i < m_Editor.SelectedtItems.Count; i++)
                {
                    m_Editor.CGObject.ChangeItemZOrder(m_Editor.SelectedtItems[i].ID, 10000);
                    m_Editor.UpdateItemsList();
                }
            }
        }

        private void AlignLefts_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                m_Editor.AlignLefts();
            }
        }

        private void AlignCenters_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                m_Editor.AlignCenters();
            }
        }

        private void AlignRights_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                m_Editor.AlignRights();
            }
        }

        private void AlignTops_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                m_Editor.AlignTops();
            }
        }

        private void AlignMiddles_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                m_Editor.AlignMiddles();
            }
        }

        private void AlignBottoms_Click(object sender, EventArgs e)
        {
            if (m_Editor != null && m_Editor.SelectedtItems.Count > 0)
            {
                m_Editor.AlignBottoms();
            }
        }


        Editor m_objEditor;
        //BackgroundWorker m_bgWorker;

        //MPLATFORMLib.MPlaylistClass m_tempPlaylist;

        string m_strImagePath;
        string m_strFlashPath;

        private int m_nFrameCounter;
        private Point m_ptStartPos;

        private object sourceObj;
        public void SetSourceObject(Object pSource, MLCHARGENLib.CoMLCharGen pCG)
        {
            //try
            //{
            sourceObj = pSource;
            m_objEditor.SetSourceObject(pSource, pCG);
            m_objEditor.UpdateItemsList();
            List<CGBaseItem> items = m_objEditor.CGItems;
            this.listBoxItems.Items.Clear();
            this.listBoxItems.Items.AddRange(items.ToArray());

            //this.listBoxItems.DataSource = items;
            //}
            //catch (System.Exception) { }
        }

        private long sourceCookies = 0;
        

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            if (keyData == (Keys.Control | Keys.C))
            {
                CopyEventHandler();
                return true;
            }
            if (keyData == (Keys.Control | Keys.V))
            {
                PasteEventHandler();
                return true;
            }
            if (keyData == (Keys.Control | Keys.Z))
            {
                UndoEventHandler();
                return true;
            }
            if (keyData == (Keys.Control | Keys.Z))
            {
                RedoEventHandler();
                return true;
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void CopyEventHandler()
        {
            //handler code goes here.
            m_objEditor.Copy();
            //MessageBox.Show("CTRL+C key pressed");
        }
        private void PasteEventHandler()
        {
            m_objEditor.Paste();
            //handler code goes here.
            //MessageBox.Show("CTRL+V key pressed");
        }

        private void UndoEventHandler()
        {
            if (m_objEditor != null)
                m_objEditor.Undo();
        }

        private void RedoEventHandler()
        {
            if (m_objEditor != null)
                m_objEditor.Redo();
        }

        private void MainWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            Editor.StopThread();
        }

        private void MainForm_ResizeBegin(object sender, EventArgs e)
        {
            MainPreview.DisableRepaint();
        }

        private void MainForm_ResizeEnd(object sender, EventArgs e)
        {
            MainPreview.EnableRepaint();
        }
    }
}
