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
    public partial class TickerEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private ITicker m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public TickerEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.SpinnerDistance, "Distance between ticker's rows");
            toolTip1.SetToolTip(this.SpinnerLineHeight, "Line's Height");
            toolTip1.SetToolTip(this.SpinnerSpeed, "Ticker Speed");
            toolTip1.SetToolTip(this.ToggleGapless, "Enable/Disable Gapless Mode");
            toolTip1.SetToolTip(this.ToggleRewind, "Rewind Ticker");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }


        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is ITicker)
                {
                    m_Item = (ITicker)item;
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
                SpinnerSpeed.Value = (decimal)m_Item.Speed;
                SpinnerLineHeight.Value = (decimal)m_Item.LineHeight;
                SpinnerDistance.Value = (decimal)m_Item.Distance;
                ToggleGapless.Checked = m_Item.Gapless;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            SpinnerSpeed.Value = 0;
            SpinnerLineHeight.Value = 0;
            SpinnerDistance.Value = 0;
            ToggleGapless.Checked = false;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "ticker-speed" || e.PropertyName == "gaplesscrawl" || e.PropertyName == "ticker-line-height" ||
                    e.PropertyName == "ticker-distance" || e.PropertyName == "xml")
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

        private void SpinnerSpeed_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Speed = (double)SpinnerSpeed.Value;
            });
        }

        private void SpinnerLineHeight_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.LineHeight = (double)SpinnerLineHeight.Value;
            });
        }

        private void SpinnerDistance_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Distance = (double)SpinnerDistance.Value;
            });
        }

        private void ToggleGapless_CheckedChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Gapless = ToggleGapless.Checked;
            });
        }

        private void ToggleRewind_Click(object sender, EventArgs e)
        {
            MLCHARGENLib.CoMLCharGen cg = CGBaseItem.m_objCG;
            CGBaseItem baseItem = (CGBaseItem)m_Item;
            cg.RewindItem(baseItem.ID);
        }
    }
}
