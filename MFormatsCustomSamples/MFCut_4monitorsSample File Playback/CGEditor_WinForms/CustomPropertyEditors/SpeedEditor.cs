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
    public partial class SpeedEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private ISpeed m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }

        public SpeedEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.SliderSpeedX, "Horizontal Speed");
            toolTip1.SetToolTip(this.SliderSpeedY, "Vertical Speed");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is ISpeed)
                {
                    m_Item = (ISpeed)item;
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
                SliderSpeedX.Value = (int)m_Item.SpeedX;
                SliderSpeedY.Value = (int)m_Item.SpeedY;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            SliderSpeedX.Value = 0;
            SliderSpeedY.Value = 0;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "speed-x" || e.PropertyName == "speed-y" || e.PropertyName == "xml")
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

        private void SliderSpeedX_Scroll(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.SpeedX = (int)SliderSpeedX.Value;
            });
        }

        private void SliderSpeedY_Scroll(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.SpeedY = (int)SliderSpeedY.Value;
            });
        }
    }
}
