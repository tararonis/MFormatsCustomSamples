using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MFORMATSLib;
using System.Threading;
using System.Runtime.InteropServices;
using System.IO;
using Microsoft.Win32;

namespace SampleFilePlayback
{
    struct MousePos
    {
        public double dblX;
        public double dblY;
    }

    public partial class OverlayHTMLWindow : Form
    {
        private MFOverlayHTMLClass m_objOverlayHTML;
        private Object m_object;
        private MFPreviewClass m_objPreview;

        private M_AV_PROPS m_avProps;

        public bool m_bStateOk;

        public OverlayHTMLWindow(Object _objOverlayHTML, Object _source)
        {
            m_bStateOk = false;
            InitializeComponent();

            try
            {
                m_objOverlayHTML = (MFOverlayHTMLClass)_objOverlayHTML;
                m_object = _source;
            }
            catch (Exception ex)
            {
                return;
            }

            m_objPreview = new MFPreviewClass();
            m_objPreview.PreviewWindowSet("", panelPreview.Handle.ToInt32());
            m_objPreview.PreviewEnable("", 0, 1);
            m_objPreview.PropsSet("preview.downscale", "0");
            m_objPreview.PropsSet("deinterlace", "false");

            panelPreview.MouseWheel += new MouseEventHandler(panelPreview_MouseWheel);
            panelPreview.KeyDown += new KeyEventHandler(panelPreview_KeyDown);
            panelPreview.KeyPress += new KeyPressEventHandler(panelPreview_KeyPress);
            panelPreview.KeyUp += new KeyEventHandler(panelPreview_KeyUp);

            mfAttributesList.SetControlledObject((IMProps)m_objOverlayHTML);
            mfAttributesListPresets1.SetControlledObject(m_objOverlayHTML);
            mfAttributesListPresets1.OnPropsUpdated += new EventHandler(mfAttributesListPresets1_OnPropsUpdated);

            m_objOverlayHTML.OnEventSafe += new IMEventsEvent_OnEventSafeEventHandler(m_objOverlayHTML_OnEventSafe);
            string strCurrURL;
            m_objOverlayHTML.PropsGet("current_url", out strCurrURL);

            textBoxPathOrURL.Text = strCurrURL;

            m_bStateOk = true;
        }

        public void UpdatePreview(MFFrame _pFrame)
        {
            int nASamples = 0;       
            _pFrame.MFAVPropsGet(out m_avProps, out nASamples);

            m_objPreview.ReceiverFramePut(_pFrame, -1, "");
        }

        void m_objOverlayHTML_OnEventSafe(string bsChannelID, string bsEventName, string bsEventParam, object pEventObject)
        {
            if (String.Equals(bsChannelID, "CEF") && String.Equals(bsEventName, "on_load_end"))
            {
                string strCurrURL;
                m_objOverlayHTML.PropsGet("current_url", out strCurrURL);
                textBoxPathOrURL.Text = strCurrURL;
            }
        }

        void mfAttributesListPresets1_OnPropsUpdated(object sender, EventArgs e)
        {
            // TODO: !!! remove this when will be work m_objOverlayHTML_OnEventSafe !!!
            textBoxPathOrURL.Text = ((MFAttributesListPresets)sender).Items[((MFAttributesListPresets)sender).SelectedIndices[0]].Text;

            mfAttributesList.UpdateList();
        }

        private void buttonInit_Click(object sender, EventArgs e)
        {
            if (!String.IsNullOrEmpty(textBoxPathOrURL.Text))
            {
                m_objOverlayHTML.BrowserPageLoad(textBoxPathOrURL.Text);
            }
        }

        private void buttonOpenFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "All supported Files (*.html, *.htm, *.css, *.js)|*.html; *.htm; *.css; *.js|" +
                                     "HTML files (*.html, *.htm)|*.html; *.htm|" +
                                     "CSS files (*.css)|*.css|" +
                                     "JavaScript files (*.js)|*.js|" +
                                     "All Files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK && openFileDialog.FileName != string.Empty)
            {
                textBoxPathOrURL.Text = openFileDialog.FileName;
                m_objOverlayHTML.BrowserPageLoad(openFileDialog.FileName);
            }
        }

        private void textBoxPathOrURL_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && !String.IsNullOrEmpty(textBoxPathOrURL.Text))
            {
                m_objOverlayHTML.BrowserPageLoad(textBoxPathOrURL.Text);
            }
        }

        private void OverlayHTMLWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (m_objPreview != null)
                m_objPreview.PreviewEnable("", 0, 0);
        }

        private void OverlayHTMLWindow_SizeChanged(object sender, EventArgs e)
        {
            if (m_objPreview != null)
                m_objPreview.PreviewWindowSet("", panelPreview.Handle.ToInt32());
        }

        private void panelPreview_MouseDown(object sender, MouseEventArgs e)
        {
            panelPreview.Focus();

            MF_MOUSE_EVENT mouseEvent = new MF_MOUSE_EVENT();
            mouseEvent.dblPosX = GetPos((double)e.X, (double)e.Y).dblX;
            mouseEvent.dblPosY = GetPos((double)e.X, (double)e.Y).dblY;

            mouseEvent.eFlags = MouseButtons2eMFFlag(e.Button);

            m_objOverlayHTML.BrowserMouseClick(mouseEvent, MouseButtons2eMF(e.Button), 0, 1);
        }

        private void panelPreview_MouseUp(object sender, MouseEventArgs e)
        {
            MF_MOUSE_EVENT mouseEvent = new MF_MOUSE_EVENT();
            mouseEvent.dblPosX = GetPos((double)e.X, (double)e.Y).dblX;
            mouseEvent.dblPosY = GetPos((double)e.X, (double)e.Y).dblY;

            mouseEvent.eFlags = MouseButtons2eMFFlag(e.Button);

            m_objOverlayHTML.BrowserMouseClick(mouseEvent, MouseButtons2eMF(e.Button), 1, 1);
        }

        double dblPrevX = 0;
        double dblPrevY = 0;
        private void panelPreview_MouseMove(object sender, MouseEventArgs e)
        {
            MF_MOUSE_EVENT mouseEvent = new MF_MOUSE_EVENT();
            mouseEvent.dblPosX = GetPos((double)e.X, (double)e.Y).dblX;
            mouseEvent.dblPosY = GetPos((double)e.X, (double)e.Y).dblY;

            if (mouseEvent.dblPosX == dblPrevX && mouseEvent.dblPosY == dblPrevY)
                return;

            mouseEvent.eFlags = MouseButtons2eMFFlag(e.Button);

            m_objOverlayHTML.BrowserMouseMove(mouseEvent, 0);

            dblPrevX = mouseEvent.dblPosX;
            dblPrevY = mouseEvent.dblPosY;
        }

        private void buttonPrev_Click(object sender, EventArgs e)
        {
            m_objOverlayHTML.BrowserCommand(eMFBrowserCommand.eMFBC_GoBack, "");
        }

        private void buttonReload_Click(object sender, EventArgs e)
        {
            try
            {
                m_objOverlayHTML.BrowserCommand(eMFBrowserCommand.eMFBC_Reload, "");
            }
            catch { }
        }

        private void buttonNext_Click(object sender, EventArgs e)
        {
            m_objOverlayHTML.BrowserCommand(eMFBrowserCommand.eMFBC_GoForward, "");
        }

        private void panelPreview_MouseWheel(object sender, MouseEventArgs e)
        {
            MF_MOUSE_EVENT mouseEvent = new MF_MOUSE_EVENT();
            mouseEvent.dblPosX = GetPos((double)e.X, (double)e.Y).dblX;
            mouseEvent.dblPosY = GetPos((double)e.X, (double)e.Y).dblY;
            
            m_objOverlayHTML.BrowserMouseWheel(mouseEvent, 0, e.Delta);
        }

        void panelPreview_KeyPress(object sender, KeyPressEventArgs e)
        {
            m_objOverlayHTML.BrowserKeyEvent(GetKey(Convert.ToInt32(e.KeyChar), eMFKeyEventType.eMFKT_Char));
        }

        void panelPreview_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Back || e.KeyCode == Keys.ShiftKey || e.KeyCode == Keys.ControlKey || e.KeyCode == Keys.Alt ||
                e.KeyCode == Keys.Left || e.KeyCode == Keys.Right || e.KeyCode == Keys.Up || e.KeyCode == Keys.Down)
            {
                m_objOverlayHTML.BrowserKeyEvent(GetKey(Convert.ToInt32(e.KeyCode), eMFKeyEventType.eMFKT_KeyDown));
            }
        }

        void panelPreview_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Back || e.KeyCode == Keys.ShiftKey || e.KeyCode == Keys.ControlKey || e.KeyCode == Keys.Alt ||
                e.KeyCode == Keys.Left || e.KeyCode == Keys.Right || e.KeyCode == Keys.Up || e.KeyCode == Keys.Down)
            {
                m_objOverlayHTML.BrowserKeyEvent(GetKey(Convert.ToInt32(e.KeyCode), eMFKeyEventType.eMFKT_KeyUp));
            }
        }

        ///////////////////////////////////////////////////////////////////////////////////
        // Helpers

        private MF_KEY_EVENT GetKey(int _nKeyCode, eMFKeyEventType _keyEventType)
        {
            MF_KEY_EVENT keyEvent = new MF_KEY_EVENT();
            keyEvent.eType = _keyEventType;
            keyEvent.windows_key_code = _nKeyCode;

            return keyEvent;
        }

        private MousePos GetPos(double _x, double _y)
        {
            MousePos mousePosReturn = new MousePos();

            if (m_avProps.vidProps.nHeight == 0 || m_avProps.vidProps.nWidth == 0)
                return mousePosReturn;

            if (!checkBoxAR.Checked)
            {
                mousePosReturn.dblX = _x / panelPreview.Width;
                mousePosReturn.dblY = _y / panelPreview.Height;

                return mousePosReturn;
            }

            if ((double)m_avProps.vidProps.nWidth / m_avProps.vidProps.nHeight > (double)panelPreview.Width / panelPreview.Height)
            {
                mousePosReturn.dblX = _x / panelPreview.Width;

                double dblHeighInt = (double)panelPreview.Width * m_avProps.vidProps.nHeight / m_avProps.vidProps.nWidth;
                double nYInt = (panelPreview.Height - dblHeighInt) / 2;
                if (_y < nYInt || _y > panelPreview.Height - nYInt)
                {
                    mousePosReturn.dblY = 0;
                }
                else
                {
                    mousePosReturn.dblY = (_y - nYInt) / dblHeighInt;
                }
            }
            else
            {
                mousePosReturn.dblY = _y / panelPreview.Height;

                double dblWidthInt = (double)panelPreview.Height * m_avProps.vidProps.nWidth / m_avProps.vidProps.nHeight;
                double nXInt = (panelPreview.Width - dblWidthInt) / 2;
                if (_x < nXInt || _x > panelPreview.Width - nXInt)
                {
                    mousePosReturn.dblX = 0;
                }
                else
                {
                    mousePosReturn.dblX = (_x - nXInt) / dblWidthInt;
                }
            }

            return mousePosReturn;
        }

        private eMFMouseButtons MouseButtons2eMF(MouseButtons _mouseButton)
        {
            switch (_mouseButton)
            {
                case MouseButtons.None: return eMFMouseButtons.eMFMB_None;
                case MouseButtons.Left: return eMFMouseButtons.eMFMB_Left;
                case MouseButtons.Right: return eMFMouseButtons.eMFMB_Right;
                case MouseButtons.Middle: return eMFMouseButtons.eMFMB_Middle;

                default: return eMFMouseButtons.eMFMB_None;
            }
        }

        private eMFEventsFlags MouseButtons2eMFFlag(MouseButtons _mouseButton)
        {
            eMFEventsFlags eflag = 0;

            if (_mouseButton.HasFlag(MouseButtons.Left))
                eflag = eMFEventsFlags.eMFEF_LEFT_MOUSE_BUTTON;
            if (_mouseButton.HasFlag(MouseButtons.Right))
                eflag |= eMFEventsFlags.eMFEF_RIGHT_MOUSE_BUTTON;
            if (_mouseButton.HasFlag(MouseButtons.Middle))
                eflag |= eMFEventsFlags.eMFEF_MIDDLE_MOUSE_BUTTON;

            return eflag;
        }

        private void checkBoxAR_CheckedChanged(object sender, EventArgs e)
        {
            m_objPreview.PropsSet("maintain_ar", Convert.ToString(checkBoxAR.Checked));
        }

        private void buttonExecJS_Click(object sender, EventArgs e)
        {
            m_objOverlayHTML.BrowserJavascriptExecute(textBoxJS.Text);
        }

        private void checkBoxVideoPrev_CheckedChanged(object sender, EventArgs e)
        {
            m_objPreview.PreviewEnable("", Convert.ToInt32(checkBoxAudioPrev.Checked), Convert.ToInt32(checkBoxVideoPrev.Checked));
        }

        private void checkBoxAudioPrev_CheckedChanged(object sender, EventArgs e)
        {
            m_objPreview.PreviewEnable("", Convert.ToInt32(checkBoxAudioPrev.Checked), Convert.ToInt32(checkBoxVideoPrev.Checked));
        }

        private void checkBoxDeint_CheckedChanged(object sender, EventArgs e)
        {
            m_objPreview.PropsSet("deinterlace", Convert.ToString(checkBoxDeint.Checked));
        }

        private void buttonAddPreset_Click(object sender, EventArgs e)
        {
            mfAttributesListPresets1.AddPreset(textBoxJS.Text);
        }

        private void buttonUpdatePreset_Click(object sender, EventArgs e)
        {
            mfAttributesListPresets1.UpdatePreset(textBoxJS.Text);
        }

        private void buttonRemove_Click(object sender, EventArgs e)
        {
            mfAttributesListPresets1.RemovePreset();
        }

        private void buttonResetProps_Click(object sender, EventArgs e)
        {
            mfAttributesListPresets1.ResetProps();
            mfAttributesList.UpdateList();
        }

        private void buttonPic_Click(object sender, EventArgs e)
        {
            if (!textBoxPathOrURL.Text.Contains("demo.html"))
            {
                DialogResult dialogResult = MessageBox.Show("Do you want to load demo page?", "Load demo.", MessageBoxButtons.YesNo, MessageBoxIcon.Question);
                if (dialogResult == DialogResult.Yes)
                {
                    string strRegResources = (string)Registry.GetValue(@"HKEY_CURRENT_USER\SOFTWARE\Medialooks\MFormats", "resources.path", null);
                    string strDemoPath = strRegResources + "\\CEF\\demo.html";

                    if (File.Exists(strDemoPath))
                    {   
                        m_objOverlayHTML.BrowserPageLoad(strDemoPath);
                        textBoxPathOrURL.Text = strDemoPath;
                        mfAttributesList.UpdateList();
                    }
                    else
                        MessageBox.Show("Demo file not found.");
                }
            }
            
            MFFrame pFrame;
            ((MFReaderClass)m_object).SourceFrameGet(-1, out pFrame, "");
            ((IMCallback)m_objOverlayHTML).OnFrame(0, "", pFrame);            
        }

        private void buttonClosePage_Click(object sender, EventArgs e)
        {
            m_objOverlayHTML.BrowserClose();
        }
    }
}
