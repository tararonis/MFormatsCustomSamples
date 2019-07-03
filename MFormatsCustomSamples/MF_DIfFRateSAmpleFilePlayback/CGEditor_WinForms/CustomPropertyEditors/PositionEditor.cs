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
    public partial class PositionEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IPosition m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public PositionEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;

            toolTip1.SetToolTip(this.SpinnerHeight, "Item's Height");
            toolTip1.SetToolTip(this.SpinnerWidth, "Item's Width");
            toolTip1.SetToolTip(this.SpinnerXPos, "Item's Horizontal Position");
            toolTip1.SetToolTip(this.SpinnerYPos, "Item's Vertical Position");
            m_bChangedByUI = true;

            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IPosition)
                {
                    m_Item = (IPosition)item;
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
                    m_Item = null;
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
                SpinnerXPos.Value = (decimal)m_Item.PosX;
                SpinnerYPos.Value = (decimal)m_Item.PosY;
                SpinnerWidth.Value = (decimal)m_Item.Width;
                SpinnerHeight.Value = (decimal)m_Item.Height;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            SpinnerXPos.Value = 0;
            SpinnerYPos.Value = 0;
            SpinnerWidth.Value = 0;
            SpinnerHeight.Value = 0;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "posx" || e.PropertyName == "posx" ||
                    e.PropertyName == "width" || e.PropertyName == "height" || e.PropertyName == "xml")
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

        private void SpinnerXPos_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.PosX = (double)SpinnerXPos.Value;
            });
        }

        private void SpinnerYPos_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.PosY = (double)SpinnerYPos.Value;
            });
        }

        private void SpinnerWidth_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Width = (double)SpinnerWidth.Value;
            });
        }

        private void SpinnerHeight_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Height = (double)SpinnerHeight.Value;
            });
        }
    }
}
