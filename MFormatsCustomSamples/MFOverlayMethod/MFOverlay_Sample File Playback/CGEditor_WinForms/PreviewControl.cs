using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CGEditor.CGItemWrappers;
using MLCHARGENLib;

namespace CGEditor_WinForms
{
    public enum eMode
    {
        eM_Default = 0,
        eM_Add = 1,
        eM_Drag = 2
    }
    public partial class PreviewControl : UserControl
    {
       
        private Point m_ptTransformOrigin;
        private Point m_ptImgStartPoint;
        private Point m_ptRectSelStartPoint;
        private string m_strCurrentGroupForEdit;

        private bool isResizingFromTop;
        private bool isResizingFromBottom;
        private bool isResizingFromLeft;
        private bool isResizingFromRight;
        private bool isMooving;
        private bool isPanning;
        private bool isRectSelecting;
        private bool isDoubleClick;

        private eMode m_eMode;
        public eMode eMode
        {
            get { return m_eMode; }
            set { m_eMode = value; }
        }


        private double m_dblResizeAreaWidth = 0.08;

        public event EventHandler PreviewItemDoubleClick;
        public event EventHandler PreviewItemDragged;
        public event EventHandler PreviewLeftButtonDown;
        public event EventHandler PreviewLeftButtonUp;
        public event EventHandler PreviewDoubleClick;

        Rectangle rectangleSelect = new Rectangle();
        public double ResizeAreaRatio
        {
            get { return m_dblResizeAreaWidth; }
            set { m_dblResizeAreaWidth = value; }
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


        Image img;
        Point mouseDown;
        int startx = 0;                         // offset of image when mouse was pressed
        int starty = 0;
        int imgx = 0;                         // current offset of image
        int imgy = 0;

        //bool isPanning = false;  // true as long as left mousebutton is pressed
        float zoom = 1;

        public PreviewControl()
        {
            InitializeComponent();

            m_eMode = eMode.eM_Default;



            //string imagefilename = @"C:\Users\Alexander\Documents\CGEditor.png";
            //img = Image.FromFile(imagefilename);

            //Graphics g = this.CreateGraphics();
            // Fit width
            //zoom = 1;
            //pictureBox1.SizeMode = PictureBoxSizeMode.CenterImage;
            imageCtrl.Paint += new PaintEventHandler(imageBox_Paint);
        }

        public PreviewControl(Editor editor)
        {
            InitializeComponent();
            Editor = editor;
            Graphics g = this.CreateGraphics();
            // Fit width
            zoom = ((float)imageCtrl.Width / (float)img.Width) * (img.HorizontalResolution / g.DpiX);
            //pictureBox1.SizeMode = PictureBoxSizeMode.CenterImage;
            imageCtrl.Paint += new PaintEventHandler(imageBox_Paint);
        }

        public void SetSource(Image bmpSource)
        {
            if (bmpSource != null)
            {
                img = bmpSource;
                //try
                //{
                //    if (imageCtrl.InvokeRequired)
                //imageCtrl.Invoke(new Action(() => 
                imageCtrl.Refresh();
                //));
                //}
                //    catch (Exception)
                //    {
                //    }
            }
        }

        public void SetEditGroup(string groupId)
        {
            if (string.IsNullOrEmpty(groupId))
            {
                m_strCurrentGroupForEdit = string.Empty;
            }
            else
            {
                m_strCurrentGroupForEdit = groupId;
                ((IMLCharGenEdit)m_Editor.CGObject).EditSelectionAdd(m_strCurrentGroupForEdit, "lime", 1);
            }
        }


        private bool firstFrame = true;
        private void imageBox_Paint(object sender, PaintEventArgs e)
        {
            //if (img == null) return;

            if (firstFrame)
            {
                try
                {
                    Graphics g = this.CreateGraphics();
                    var tmp = ((float)imageCtrl.Width / (float)img.Width) * (img.HorizontalResolution / g.DpiX);
                    if (img.Height * tmp > imageCtrl.Height)
                        tmp = ((imageCtrl.Height - statusStrip1.Height) / (float)img.Height) * (img.VerticalResolution / g.DpiY);
                    zoom = tmp;//((float)imageCtrl.Width / (float)img.Width) * (img.HorizontalResolution / g.DpiX);
                    imgx = (int)((imageCtrl.Width / zoom - img.Width) / 2);
                    imgy = (int)(((imageCtrl.Height - statusStrip1.Height) / zoom - img.Height) / 2);
                }
                catch (Exception)
                {
                    
                }
                




            }
            firstFrame = false;
            e.Graphics.InterpolationMode = InterpolationMode.Default;
            e.Graphics.ScaleTransform(zoom, zoom);
            try
            {
                if (img.Height < int.MaxValue)
                {
                    e.Graphics.DrawImage(img, imgx, imgy);
                    using (Pen pen = new Pen(Color.Lime, 1))
                    {
                        e.Graphics.DrawRectangle(pen, rectangleSelect);
                    }
                }
            }
            catch { }
        }

        protected override void OnMouseWheel(MouseEventArgs e)
        {
            imageCtrl.Paint -= new PaintEventHandler(imageBox_Paint);
            float oldzoom = zoom;

            if (e.Delta > 0)
            {
                zoom *= 1.1F;
            }

            else if (e.Delta < 0)
            {
                zoom = Math.Max(zoom / 1.1F, 0.01F);
            }

            MouseEventArgs mouse = e as MouseEventArgs;
            Point mousePosNow = mouse.Location;

            int x = mousePosNow.X - imageCtrl.Location.X;    // Where location of the mouse in the pictureframe
            int y = mousePosNow.Y - imageCtrl.Location.Y;

            int oldimagex = (int)(x / oldzoom);  // Where in the IMAGE is it now
            int oldimagey = (int)(y / oldzoom);

            int newimagex = (int)(x / zoom);     // Where in the IMAGE will it be when the new zoom i made
            int newimagey = (int)(y / zoom);

            imgx = newimagex - oldimagex + imgx;  // Where to move image to keep focus on one point
            imgy = newimagey - oldimagey + imgy;

            imageCtrl.Refresh();  // calls imageBox_Paint
            imageCtrl.Paint += new PaintEventHandler(imageBox_Paint);
        }

        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            MouseEventArgs mouse = e as MouseEventArgs;
            if (mouse.Button == MouseButtons.Left)
            {
                if (PreviewLeftButtonDown != null)
                {
                    //Point previewPt = e.Location;
                    Point previewPt = new Point(e.X - imgx, e.Y - imgy);
                    Point cgPt = GetCGPoint(previewPt);
                    PreviewLeftButtonDown(this, new PreviewClickArgs(cgPt));
                }
                if (m_eMode == eMode.eM_Default)
                {
                    if (imageCtrl.Focused) return;
                    imageCtrl.Focus();

                    isMooving = false;
                    isRectSelecting = false;
                    isResizingFromTop = false;
                    isResizingFromBottom = false;
                    isResizingFromLeft = false;
                    isResizingFromRight = false;
                    isDoubleClick = false;


                    double previewScaleFactor = GetPreviewScaleFactor();
                    double transformScaleFactor = GetTransformScaleFactor();

                    double selStartX = e.X / zoom - imgx;
                    double selStartY = e.Y / zoom - imgy;
                    m_ptRectSelStartPoint = new Point((int)selStartX, (int)selStartY);
                    Point pt = new Point((int)selStartX, (int)selStartY);
                    m_ptImgStartPoint = m_ptRectSelStartPoint;
                    //for (int i = m_Editor.CGItems.Count ; i > 0; i-- )
                    for (int i = 0; i < m_Editor.CGItems.Count; i++)
                    {
                        //CGBaseItem item = m_Editor.CGItems[i-1];
                        CGBaseItem item = m_Editor.CGItems[i];

                        double recalcX = item.PosX;// * previewScaleFactor;
                        double recalcY = item.PosY;// * previewScaleFactor;

                        double recalcWidth = item.Width;// * previewScaleFactor;
                        double recalcHeight = item.Height;// * previewScaleFactor;

                        double resizeWidth = 8;//recalcWidth * m_dblResizeAreaWidth;
                        double resizeHeight = 8;// recalcHeight * m_dblResizeAreaWidth;

                        Rectangle rectInner = new Rectangle();
                        rectInner.X = (int)(recalcX + resizeWidth);
                        rectInner.Y = (int)(recalcY + resizeHeight);
                        rectInner.Width = (int)(recalcWidth - 2 * resizeWidth);
                        rectInner.Height = (int)(recalcHeight - 2 * resizeHeight);

                        item.StartLocation = new Point((int)item.PosX, (int)item.PosY);
                        item.StartWidth = item.Width;
                        item.StartHeight = item.Height;

                        if (rectInner.Contains(pt))
                        {
                            if (ModifierKeys.HasFlag(Keys.Control))
                                m_Editor.SelectItem(item.ID, false);
                            else if (item.isSelected == false)
                                m_Editor.SelectItem(item.ID, true);


                            if (PreviewItemDoubleClick != null && e.Clicks == 2)
                            {
                                PreviewItemDoubleClick(item, new EventArgs());
                                isDoubleClick = true;
                                return;
                            }
                            isMooving = true;
                        }
                    }
                    if (isMooving)
                    {
                        UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ""));
                        return;
                    }
                    if (m_Editor.SelectedtItems.Count == 1)
                    {
                        CGBaseItem item = m_Editor.SelectedtItems[0];
                        double recalcX = item.PosX;// * previewScaleFactor;// *transformScaleFactor;
                        double recalcY = item.PosY;// * previewScaleFactor;// *transformScaleFactor;

                        double recalcWidth = item.Width;// * previewScaleFactor;// *transformScaleFactor;
                        double recalcHeight = item.Height;// * previewScaleFactor;// *transformScaleFactor;

                        double resizeWidth = 8;//recalcWidth * m_dblResizeAreaWidth;
                        double resizeHeight = 8;//recalcHeight * m_dblResizeAreaWidth;

                        Rectangle rectOuterTop = new Rectangle();
                        rectOuterTop.X = (int)(recalcX + resizeWidth);
                        rectOuterTop.Y = (int)(recalcY - resizeHeight);
                        rectOuterTop.Width = (int)(recalcWidth - 2 * resizeWidth);
                        rectOuterTop.Height = (int)(2 * resizeHeight);

                        Rectangle rectOuterBottom = new Rectangle();
                        rectOuterBottom.X = (int)(recalcX + resizeWidth);
                        rectOuterBottom.Y = (int)(recalcY + recalcHeight - resizeHeight);
                        rectOuterBottom.Width = (int)(recalcWidth - 2 * resizeWidth);
                        rectOuterBottom.Height = (int)(2 * resizeHeight);

                        Rectangle rectOuterLeft = new Rectangle();
                        rectOuterLeft.X = (int)(recalcX - resizeWidth);
                        rectOuterLeft.Y = (int)(recalcY + resizeHeight);
                        rectOuterLeft.Width = (int)(2 * resizeWidth);
                        rectOuterLeft.Height = (int)(recalcHeight - 2 * resizeHeight);

                        Rectangle rectOuterRight = new Rectangle();
                        rectOuterRight.X = (int)(recalcX + recalcWidth - resizeWidth);
                        rectOuterRight.Y = (int)(recalcY + resizeHeight);
                        rectOuterRight.Width = (int)(2 * resizeWidth);
                        rectOuterRight.Height = (int)(recalcHeight - 2 * resizeHeight);


                        item.StartLocation = new Point((int)item.PosX, (int)item.PosY);
                        item.StartWidth = item.Width;
                        item.StartHeight = item.Height;

                        if (rectOuterTop.Contains(pt))
                        {
                            isResizingFromTop = true;
                            UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ""));
                            return;
                        }
                        if (rectOuterBottom.Contains(pt))
                        {
                            isResizingFromBottom = true;
                            UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ""));
                            return;
                        }
                        if (rectOuterLeft.Contains(pt))
                        {
                            isResizingFromLeft = true;
                            UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ""));
                            return;
                        }
                        if (rectOuterRight.Contains(pt))
                        {
                            isResizingFromRight = true;
                            UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ""));
                            return;
                        }
                    }

                    if (PreviewDoubleClick != null && e.Clicks == 2)
                    {
                        PreviewDoubleClick(this, new EventArgs());
                        return;
                    }

                    //if (!isMooving && !isDoubleClick)
                    isRectSelecting = true;
                }
            }
            if (mouse.Button == MouseButtons.Right)
            {
                if (!isPanning)
                {
                    isPanning = true;
                    mouseDown = mouse.Location;
                    m_ptImgStartPoint = mouse.Location; ;
                    startx = imgx;
                    starty = imgy;
                    isPanning = true;
                }
            }
        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            MouseEventArgs mouse = e as MouseEventArgs;

            //if (mouse.Button == MouseButtons.Right)
            //{
            //    Point mousePosNow = mouse.Location;

            //    int deltaX = mousePosNow.X - mouseDown.X; // the distance the mouse has been moved since mouse was pressed
            //    int deltaY = mousePosNow.Y - mouseDown.Y;

            //    imgx = (int)(startx + (deltaX / zoom));  // calculate new offset of image based on the current zoom factor
            //    imgy = (int)(starty + (deltaY / zoom));

            //    imageCtrl.Refresh();
            //}

            if (m_eMode == eMode.eM_Default)
            {
                double transformScaleFactor = GetTransformScaleFactor();
                double previewScaleFactor = GetPreviewScaleFactor();


                //=========================================================================
                if (!isMooving && !isRectSelecting && !isPanning && !isResizingFromTop
                    && !isResizingFromBottom && !isResizingFromLeft && !isResizingFromRight && m_Editor.SelectedtItems.Count == 1)
                {
                    double selStartX = e.X / zoom - imgx;// *transformScaleFactor;
                    double selStartY = e.Y / zoom - imgy;// *transformScaleFactor;
                    Point pt = new Point((int)selStartX, (int)selStartY);
                    foreach (CGBaseItem item in m_Editor.SelectedtItems)
                    {
                        //CGBaseItem item = m_Editor.SelectedtItems[0];
                        double recalcX = item.PosX;// * previewScaleFactor;// *transformScaleFactor;
                        double recalcY = item.PosY;// * previewScaleFactor;// *transformScaleFactor;

                        double recalcWidth = item.Width;// * previewScaleFactor;// *transformScaleFactor;
                        double recalcHeight = item.Height;// * previewScaleFactor;// *transformScaleFactor;

                        double resizeWidth = 8;//recalcWidth * m_dblResizeAreaWidth;
                        double resizeHeight = 8;//recalcHeight * m_dblResizeAreaWidth;

                        Rectangle rectOuterTop = new Rectangle();
                        rectOuterTop.X = (int)(recalcX + resizeWidth);
                        rectOuterTop.Y = (int)(recalcY - resizeHeight);
                        rectOuterTop.Width = (int)(recalcWidth - 2 * resizeWidth);
                        rectOuterTop.Height = (int)(2 * resizeHeight);

                        Rectangle rectOuterBottom = new Rectangle();
                        rectOuterBottom.X = (int)(recalcX + resizeWidth);
                        rectOuterBottom.Y = (int)(recalcY + recalcHeight - resizeHeight);
                        rectOuterBottom.Width = (int)(recalcWidth - 2 * resizeWidth);
                        rectOuterBottom.Height = (int)(2 * resizeHeight);

                        Rectangle rectOuterLeft = new Rectangle();
                        rectOuterLeft.X = (int)(recalcX - resizeWidth);
                        rectOuterLeft.Y = (int)(recalcY + resizeHeight);
                        rectOuterLeft.Width = (int)(2 * resizeWidth);
                        rectOuterLeft.Height = (int)(recalcHeight - 2 * resizeHeight);

                        Rectangle rectOuterRight = new Rectangle();
                        rectOuterRight.X = (int)(recalcX + recalcWidth - resizeWidth);
                        rectOuterRight.Y = (int)(recalcY + resizeHeight);
                        rectOuterRight.Width = (int)(2 * resizeWidth);
                        rectOuterRight.Height = (int)(recalcHeight - 2 * resizeHeight);


                        item.StartLocation = new Point((int)item.PosX, (int)item.PosY);
                        item.StartWidth = item.Width;
                        item.StartHeight = item.Height;

                        if (rectOuterTop.Contains(pt))
                        {
                            this.Cursor = Cursors.SizeNS;
                            break;
                        }
                        else if (rectOuterBottom.Contains(pt))
                        {
                            this.Cursor = Cursors.SizeNS;
                            break;
                        }
                        else if (rectOuterLeft.Contains(pt))
                        {
                            this.Cursor = Cursors.SizeWE;
                            break;
                        }
                        else if (rectOuterRight.Contains(pt))
                        {
                            this.Cursor = Cursors.SizeWE;
                            break;
                        }
                        else this.Cursor = Cursors.Arrow;
                    }
                }
                //=========================================================================


                if (isPanning)
                {
                    this.Cursor = Cursors.Hand;
                    Point mousePosNow = mouse.Location;

                    int deltaX = mousePosNow.X - mouseDown.X; // the distance the mouse has been moved since mouse was pressed
                    int deltaY = mousePosNow.Y - mouseDown.Y;

                    imgx = (int)(startx + (deltaX / zoom));  // calculate new offset of image based on the current zoom factor
                    imgy = (int)(starty + (deltaY / zoom));

                    //imageCtrl.Refresh();
                    labelStatus.Text = String.Format("X Offset = {0:0.00} , Y Offset = {1:0.00}", imgx, imgy);
                }
                else if (isMooving)
                {
                    this.Cursor = Cursors.SizeAll;
                    //Point p = e.Location;
                    Point p = new Point((int)(e.X / zoom - imgx), (int)(e.Y / zoom - imgy));
                    foreach (CGBaseItem item in m_Editor.SelectedtItems)
                    {
                        double xPos = item.StartLocation.X + (p.X - m_ptImgStartPoint.X);// / previewScaleFactor;
                        double yPos = item.StartLocation.Y + (p.Y - m_ptImgStartPoint.Y);// / previewScaleFactor;
                        item.PosX = xPos;
                        item.PosY = yPos;
                    }
                    //labelStatus.Content = String.Format("X Pos = {0:0.00} , Y Offset = {1:0.00}", xPos, yPos);
                }
                else if (isResizingFromRight)
                {
                    this.Cursor = Cursors.SizeWE;
                    //Point p = e.Location;
                    Point p = new Point((int)(e.X / zoom - imgx), (int)(e.Y / zoom - imgy));
                    foreach (CGBaseItem item in m_Editor.SelectedtItems)
                    {
                        item.Width = item.StartWidth + (p.X - m_ptImgStartPoint.X);// / previewScaleFactor;
                    }
                }
                else if (isResizingFromLeft)
                {
                    this.Cursor = Cursors.SizeWE;
                    //Point p = e.Location;
                    Point p = new Point((int)(e.X / zoom - imgx), (int)(e.Y / zoom - imgy));
                    foreach (CGBaseItem item in m_Editor.SelectedtItems)
                    {
                        item.Width = item.StartWidth - (p.X - m_ptImgStartPoint.X);// / previewScaleFactor;
                        item.PosX = p.X;// / previewScaleFactor;
                    }
                }
                else if (isResizingFromTop)
                {
                    this.Cursor = Cursors.SizeNS;
                    //Point p = e.Location;
                    Point p = new Point((int)(e.X / zoom - imgx), (int)(e.Y / zoom - imgy));
                    foreach (CGBaseItem item in m_Editor.SelectedtItems)
                    {
                        item.Height = item.StartHeight - (p.Y - m_ptImgStartPoint.Y);// / previewScaleFactor;
                        item.PosY = p.Y;// / previewScaleFactor;
                    }
                }

                else if (isResizingFromBottom)
                {
                    this.Cursor = Cursors.SizeNS;
                    //Point p = e.Location;
                    Point p = new Point((int)(e.X / zoom - imgx), (int)(e.Y / zoom - imgy));
                    foreach (CGBaseItem item in m_Editor.SelectedtItems)
                    {
                        item.Height = item.StartHeight + (p.Y - m_ptImgStartPoint.Y);// / previewScaleFactor;
                    }
                }

                else if (isRectSelecting)
                {

                    double x = (e.X / zoom - imgx);// * transformScaleFactor;
                    double y = (e.Y / zoom - imgy);// * transformScaleFactor;

                    double xStart = m_ptRectSelStartPoint.X;// * transformScaleFactor;
                    double yStart = m_ptRectSelStartPoint.Y;// * transformScaleFactor;

                    //UIElement container = VisualTreeHelper.GetParent(imageCtrl) as UIElement;
                    //Point relativeLocation = imageCtrl.TranslatePoint(new Point(0, 0), container);

                    //rectangleSelect.Margin = new System.Windows.Thickness(
                    //    Math.Min(x + relativeLocation.X, xStart + relativeLocation.X),
                    //    Math.Min(y + relativeLocation.Y, yStart + relativeLocation.Y),
                    //    0, 0);
                    //rectangleSelect.Visibility = Visibility.Visible;
                    rectangleSelect.X = (int)Math.Min(x, xStart);
                    rectangleSelect.Y = (int)Math.Min(y, yStart);
                    rectangleSelect.Width = (int)Math.Abs(x - xStart);
                    rectangleSelect.Height = (int)Math.Abs(y - yStart);
                }
            }
        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                isPanning = false;
                this.Cursor = Cursors.Arrow;
            }
            if (e.Button == MouseButtons.Left)
            {
                if (PreviewLeftButtonUp != null)
                {
                    //Point previewPt = e.Location;
                    Point previewPt = new Point(e.X - imgx, e.Y - imgy);
                    Point cgPt = GetCGPoint(previewPt);
                    PreviewLeftButtonUp(this, new PreviewClickArgs(cgPt));
                }
                if (m_eMode == eMode.eM_Default)
                {
                    //rectangleSelect.Visibility = Visibility.Hidden;
                    if (isRectSelecting)
                    {

                        double transformScaleFactor = GetTransformScaleFactor();
                        double previewScaleFactor = GetPreviewScaleFactor();

                        double x = e.X / zoom - imgx;
                        double y = e.Y / zoom - imgy;

                        double xx = Math.Min(x, m_ptRectSelStartPoint.X);
                        double yy = Math.Min(y, m_ptRectSelStartPoint.Y);

                        double cgX = xx;// / previewScaleFactor;
                        double cgY = yy;// / previewScaleFactor;

                        double width = rectangleSelect.Width / transformScaleFactor / previewScaleFactor;
                        double height = rectangleSelect.Width / transformScaleFactor / previewScaleFactor;

                        int left = (int)cgX;
                        int top = (int)cgY;
                        int right = (int)(cgX + width);
                        int bottom = (int)(cgY + height);

                        if (width <= 10 || height <= 10)
                        {
                            m_Editor.SelectByPoint(left, top);
                        }
                        else
                            m_Editor.SelectByRectangle(left, top, right, bottom);

                    }

                    statusStrip1.Focus();

                    isMooving = false;
                    isRectSelecting = false;
                    isResizingFromTop = false;
                    isResizingFromBottom = false;
                    isResizingFromLeft = false;
                    isResizingFromRight = false;
                    isDoubleClick = false;

                    //rectangleSelect.Margin = new Thickness(0, 0, 0, 0);
                    rectangleSelect.Width = 0;
                    rectangleSelect.Height = 0;

                    m_ptRectSelStartPoint = new Point(0, 0);

                    this.Cursor = Cursors.Arrow;
                }

                eMode = eMode.eM_Default;
                SetCurcsor(Cursors.Arrow);
            }
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            const int WM_KEYDOWN = 0x100;
            const int WM_SYSKEYDOWN = 0x104;

            if ((msg.Msg == WM_KEYDOWN) || (msg.Msg == WM_SYSKEYDOWN))
            {
                switch (keyData)
                {
                    case Keys.Right:
                        imgx -= (int)(imageCtrl.Width * 0.1F / zoom);
                        imageCtrl.Refresh();
                        break;

                    case Keys.Left:
                        imgx += (int)(imageCtrl.Width * 0.1F / zoom);
                        imageCtrl.Refresh();
                        break;

                    case Keys.Down:
                        imgy -= (int)(imageCtrl.Height * 0.1F / zoom);
                        imageCtrl.Refresh();
                        break;

                    case Keys.Up:
                        imgy += (int)(imageCtrl.Height * 0.1F / zoom);
                        imageCtrl.Refresh();
                        break;

                    case Keys.PageDown:
                        imgy -= (int)(imageCtrl.Height * 0.90F / zoom);
                        imageCtrl.Refresh();
                        break;

                    case Keys.PageUp:
                        imgy += (int)(imageCtrl.Height * 0.90F / zoom);
                        imageCtrl.Refresh();
                        break;
                }
            }

            return base.ProcessCmdKey(ref msg, keyData);
        }

        private double GetPreviewScaleFactor()
        {
            double scaleRatio = 0;
            if (m_Editor != null)
            {
                //if (m_Editor.AVProps.vidProps.nWidth >= Math.Abs(m_Editor.AVProps.vidProps.nHeight))
                //    scaleRatio = imageCtrl.Height / Math.Abs((double)m_Editor.AVProps.vidProps.nHeight);
                //else
                scaleRatio = 1 / (((double)m_Editor.AVProps.vidProps.nWidth) / imageCtrl.Width);
            }
            return scaleRatio;
        }

        private double GetTransformScaleFactor()
        {
            return zoom; //imageCtrl.RenderTransform.Value.M11;
        }

        private Point GetCGPoint(Point previewPoint)
        {
            Point res = new Point();
            if (previewPoint != null)
            {
                double previewScaleFactor = GetPreviewScaleFactor();
                res.X = (int)(Math.Abs(previewPoint.X / previewScaleFactor));
                res.Y = (int)(Math.Abs(previewPoint.Y / previewScaleFactor));
            }
            return res;
        }

        public void SetCurcsor(Cursor cursor)
        {
            this.Cursor = cursor;
        }

        private void GeneralForm_MouseEnter(object sender, EventArgs e)
        {
            Focus();
        }

        private void imageCtrl_DragDrop(object sender, DragEventArgs e)
        {

        }

        private void GeneralForm_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Delete && m_Editor != null)
            {
                UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ""));
                for (int i = 0; i < m_Editor.SelectedtItems.Count; i++)
                {
                    string strId = ((CGBaseItem)m_Editor.SelectedtItems[i]).ID;
                    m_Editor.CGObject.RemoveItemWithDelay(strId, 0, 0);
                }
                m_Editor.UpdateItemsList();
            }
        }

        private void imageCtrl_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (PreviewDoubleClick != null)
            {
                PreviewDoubleClick(this, new EventArgs());
                return;
            }
            if (PreviewItemDoubleClick != null)
            {
                CGBaseItem item = m_Editor.SelectedtItems[0];
                PreviewItemDoubleClick(item, new EventArgs());
                isDoubleClick = true;
                return;
            }
        }

        private void imageCtrl_DragOver(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(typeof(String)))
                e.Effect = DragDropEffects.Move;
            else e.Effect = DragDropEffects.None;
        }

        private void GeneralForm_DragOver(object sender, DragEventArgs e)
        {
            var tmp = this.PointToClient(new Point(e.X, e.Y));
            Point point = new Point((int)(tmp.X - imgx * zoom), (int)(tmp.Y - imgy * zoom));
            Rectangle imageRectangle = new Rectangle();
            imageRectangle.X = 0;
            imageRectangle.Y = 0;
            imageRectangle.Width = (int)(img.Width * zoom);
            imageRectangle.Height = (int)(img.Height * zoom);
            labelStatus.Text = "Control Size: " + imageCtrl.Width + "  " + imageCtrl.Height + "Zoom=" + zoom + "  Point: " + point.X + " ; " + point.Y + " current point: " + tmp.X + " ; " + tmp.Y + " Image Location:" + imgx + " ; " + imgy + " Image Width: " + imageRectangle.Width;

            if (imageRectangle.Contains(point))
            {
                if (e.Data.GetDataPresent(typeof(String)))
                    e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        private void GeneralForm_DragDrop(object sender, DragEventArgs e)
        {
            if (PreviewItemDragged != null)
            {
                var tmp = this.PointToClient(new Point(e.X, e.Y));
                Point point = new Point((int)(tmp.X - imgx * zoom), (int)(tmp.Y - imgy * zoom));
                Rectangle imageRectangle = new Rectangle();
                imageRectangle.X = 0;
                imageRectangle.Y = 0;
                imageRectangle.Width = (int)(img.Width * zoom);
                imageRectangle.Height = (int)(img.Height * zoom);


                if (imageRectangle.Contains(point))
                {
                    object data = e.Data.GetData(typeof(String));
                    PreviewItemDragged(this, new ItemDragEventArgs(data.ToString(), new Point((int)(point.X / zoom), (int)(point.Y / zoom))));
                }

            }
        }

        private void GeneralForm_Resize(object sender, EventArgs e)
        {
            if (img == null) return;
            Graphics g = this.CreateGraphics();
            var tmp = ((float)imageCtrl.Width / (float)img.Width) * (img.HorizontalResolution / g.DpiX);
            if (img.Height * tmp > imageCtrl.Height)
                tmp = ((imageCtrl.Height - statusStrip1.Height) / (float)img.Height) * (img.VerticalResolution / g.DpiY);
            zoom = tmp;
            imgx = (int)((imageCtrl.Width / zoom - img.Width) / 2);
            imgy = (int)(((imageCtrl.Height - statusStrip1.Height) / zoom - img.Height) / 2);
            imageCtrl.Refresh();
        }

        public void DisableRepaint()
        {
            imageCtrl.Paint -= new PaintEventHandler(imageBox_Paint);
        }

        public void EnableRepaint()
        {
            imageCtrl.Paint += new PaintEventHandler(imageBox_Paint);
        }
    }
    public class PreviewClickArgs : EventArgs
    {
        private Point m_cgPoint;
        public Point CGPoint
        {
            get { return m_cgPoint; }
            set { m_cgPoint = value; }
        }

        public PreviewClickArgs(Point cgPoint)
        {
            m_cgPoint = cgPoint;
        }
    }

    public class ItemDragEventArgs : EventArgs
    {
        public ItemDragEventArgs(string type, Point location)
        {
            m_strItemType = type;
            m_ptLocation = location;

        }
        private string m_strItemType;
        public string ItemType
        {
            get { return m_strItemType; }
            set { m_strItemType = value; }
        }
        private Point m_ptLocation;
        public Point Location
        {
            get { return m_ptLocation; }
            set { m_ptLocation = value; }
        }
    }
}

