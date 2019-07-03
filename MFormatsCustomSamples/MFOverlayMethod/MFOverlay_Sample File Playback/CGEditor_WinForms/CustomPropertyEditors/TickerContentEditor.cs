using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CGEditor.CGItemWrappers;

namespace CGEditor_WinForms.CustomPropertyEditors
{
    public partial class TickerContentEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private ITickerNewContent m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public TickerContentEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.TextBoxNewContent, "Ticker's Text");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is ITickerNewContent)
                {
                    m_Item = (ITickerNewContent)item;
                    FillControls();
                    if (item is IChangeNotify)
                    {
                        ((IChangeNotify)item).ItemChanged += External_ItemChanged;
                    }
                    this.Enabled = true;
                }
                else
                {
                    ClearControls();
                    this.Enabled = false;
                }
            }
            catch (System.Exception) { }
        }

        private void FillControls()
        {
            if (m_Item != null)
            {
                ClearControls();
                m_bChangedByUI = false;
                //string strParsedText = parseFromCGFormat(m_Item.Text);
                TextBoxNewContent.Text = m_Item.NewContent;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            TextBoxNewContent.Text = string.Empty;
            m_bChangedByUI = true;
        }

        private string parseToCGFormat(string text)
        {
            string strRes = text;
            if (strRes != null && strRes != string.Empty)
            {
                strRes = strRes.Replace("\r\n", "<br/>");
            }
            return strRes;
        }

        private string parseFromCGFormat(string text)
        {
            string strRes = text;
            if (strRes != null && strRes != string.Empty)
            {
                strRes = strRes.Replace("\t", ""); //remove tabs
                //Remove starting \r\n's
                while (true)
                {
                    if (strRes.Length > 0 && strRes.StartsWith("\r\n"))
                        strRes = strRes.Substring(2);
                    else
                        break;
                }
                //Remove trailing \r\n's
                while (true)
                {
                    if (strRes.Length > 0 && strRes.EndsWith("\r\n"))
                        strRes = strRes.Substring(0, strRes.Length - 2);
                    else
                        break;
                }
                //Replace <br/> tags
                strRes = strRes.Replace(@"<br/>", "");
                strRes = strRes.Replace(@"<br />", "");
            }
            return strRes;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "tickercontent" || e.PropertyName == "xml")
                    FillControls();
            }
        }

        private void TextBoxNewContent_TextChanged(object sender, EventArgs e)
        {
            if (m_Item != null && TextBoxNewContent.Text != string.Empty && m_bChangedByUI)
            {
                UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ((CGBaseItem)m_Item).ID));
                m_bAceptExternalUpdate = false;
                //string strIn = parseToCGFormat(TextBoxNewContent.Text);
                m_Item.NewContent = TextBoxNewContent.Text;
                m_bAceptExternalUpdate = true;
            }
        }
    }
}
