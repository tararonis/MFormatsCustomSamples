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
    public partial class OpenVideoEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IPath m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }
        public OpenVideoEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.TextBoxText, "Path to Video");
            toolTip1.SetToolTip(this.button1, "Browse for Video");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
        }

        //public static readonly DependencyProperty ValueProperty = DependencyProperty.Register("Value", typeof(string), typeof(OpenImageEditor),
        //                                                                              new FrameworkPropertyMetadata(null, FrameworkPropertyMetadataOptions.BindsTwoWayByDefault));
        private string path;
        public string Value
        {
            get { return path; }
            set
            {
                path = value;
                if (m_Item != null && m_bChangedByUI)
                {
                    TextBoxText.Text = path;
                    UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ((CGBaseItem)m_Item).ID));
                    m_Item.Path = TextBoxText.Text;
                }
            }
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IPath)
                {
                    m_Item = (IPath)item;
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
                TextBoxText.Text = m_Item.Path;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            TextBoxText.Text = "";
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "path" || e.PropertyName == "xml")
                    FillControls();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Multiselect = false;
            openFile.Filter = "All Videos|*.MP4;*.MOV;*.AVI;*.MXF;*.TS";
            if (openFile.ShowDialog() == DialogResult.OK)
            {
                Value = openFile.FileName;
            }
        }
    }
}
