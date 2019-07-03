using System;
using System.Windows.Forms;
using CGEditor.CGItemWrappers;

namespace CGEditor_WinForms.CustomPropertyEditors
{
    public partial class GlowEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IGlow m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public GlowEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.SpinnerGlowSizeX, "Glow Width");
            toolTip1.SetToolTip(this.SpinnerGlowSizeY, "Glow Height");
            toolTip1.SetToolTip(this.SpinnerGlowVlue, "Glow Value");
            toolTip1.SetToolTip(this.ToggleGlow, "Enable/Disable Glow");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IGlow)
                {
                    m_Item = (IGlow)item;
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
                ToggleGlow.Checked = m_Item.GlowEnable;
                if ((bool)ToggleGlow.Checked)
                    EnableControls();
                else
                    DisableControls();

                SpinnerGlowVlue.Value = (decimal)m_Item.GlowValue;
                SpinnerGlowSizeX.Value = (decimal)m_Item.GlowSizeX;
                SpinnerGlowSizeY.Value = (decimal)m_Item.GlowSizeY;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            SpinnerGlowVlue.Value = 0;
            SpinnerGlowSizeX.Value = 0;
            SpinnerGlowSizeY.Value = 0;
            m_bChangedByUI = true;
        }

        private void DisableControls()
        {
            SpinnerGlowVlue.Enabled = false;
            SpinnerGlowSizeX.Enabled = false;
            SpinnerGlowSizeY.Enabled = false;
        }
        private void EnableControls()
        {
            SpinnerGlowVlue.Enabled = true;
            SpinnerGlowSizeX.Enabled = true;
            SpinnerGlowSizeY.Enabled = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "glow-enabled" || e.PropertyName == "glow-size-x" || e.PropertyName == "glow-size-y" ||
                    e.PropertyName == "glow-value" || e.PropertyName == "xml")
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

        private void ToggleGlow_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleGlow.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.GlowEnable = true;
                    EnableControls();
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.GlowEnable = false;
                    DisableControls();
                });
            }
        }

        private void SpinnerGlowSizeX_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.GlowSizeX = (double)SpinnerGlowSizeX.Value;
            });
        }

        private void SpinnerGlowSizeY_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.GlowSizeY = (double)SpinnerGlowSizeY.Value;
            });
        }

        private void SpinnerGlowVlue_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.GlowValue = (double)SpinnerGlowVlue.Value;
            });
        }
    }
}
