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
    public partial class VisibilityEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IVisible m_Item;
        private Editor m_Editor;

        public VisibilityEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;

            toolTip1.SetToolTip(this.ToggleVisible, "Enable/Disable Visibility");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IVisible)
                {
                    m_Item = (IVisible)item;
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

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }

        private void FillControls()
        {
            if (m_Item != null)
            {
                ClearControls();
                m_bChangedByUI = false;
                ToggleVisible.Checked = m_Item.Show;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            ToggleVisible.Checked = true;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "show" || e.PropertyName == "xml")
                    FillControls();
            }
        }

        delegate void Setter();
        private void SetProperty(Setter s)
        {
            if (m_Item != null && m_bChangedByUI)
            {
                UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ((CGBaseItem)m_Item).ID));
                m_bAceptExternalUpdate = false;
                s();
                m_bAceptExternalUpdate = true;
            }
        }

        private void ToggleVisible_CheckedChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Show = ToggleVisible.Checked;

            });
        }
    }
}
