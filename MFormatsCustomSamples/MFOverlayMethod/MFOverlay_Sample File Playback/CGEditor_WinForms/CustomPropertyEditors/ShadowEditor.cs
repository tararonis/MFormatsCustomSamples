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
    public partial class ShadowEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IShadow m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public ShadowEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.CPShadowColor, "Shadow Color");
            toolTip1.SetToolTip(this.SliderShadowAlpha, "Shadow's transparency");
            toolTip1.SetToolTip(this.SpinnerOffsetX, "Shadow's Horizontal Offset");
            toolTip1.SetToolTip(this.SpinnerOffsetY, "Shadow's Vertical Offset");
            toolTip1.SetToolTip(this.SpinnerShadowBlur, "Shadow's Blur");
            toolTip1.SetToolTip(this.ToggleShadow, "Enable/Disable Shadow");
            m_bChangedByUI = false;
            CPShadowColor.BackColor = Color.Transparent;
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IShadow)
                {
                    m_Item = (IShadow)item;
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
                ToggleShadow.Checked = m_Item.ShadowEnable;
                if ((bool)ToggleShadow.Checked)
                    EnableControls();
                else
                    DisableControls();
                SliderShadowAlpha.Value = (int)m_Item.ShadowAlpha;
                SpinnerShadowBlur.Value = (decimal)m_Item.ShadowBlur;
                SpinnerOffsetX.Value = (decimal)m_Item.ShadowXOffset;
                SpinnerOffsetY.Value = (decimal)m_Item.ShadowYOffset;
                CPShadowColor.BackColor = WrapperHelpers.ParseCGColor(m_Item.ShadowColor);
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            ToggleShadow.Checked = false;
            SliderShadowAlpha.Value = 255;
            SpinnerShadowBlur.Value = 0;
            SpinnerOffsetX.Value = 0;
            SpinnerOffsetY.Value = 0;
            CPShadowColor.BackColor = Color.Transparent;
            m_bChangedByUI = true;
        }

        private void DisableControls()
        {
            SpinnerShadowBlur.Enabled = false;
            SpinnerOffsetX.Enabled = false;
            SpinnerOffsetY.Enabled = false;
            SliderShadowAlpha.Enabled = false;
            CPShadowColor.Enabled = false;
        }
        private void EnableControls()
        {
            SpinnerShadowBlur.Enabled = true;
            SpinnerOffsetX.Enabled = true;
            SpinnerOffsetY.Enabled = true;
            SliderShadowAlpha.Enabled = true;
            CPShadowColor.Enabled = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "shadow-enabled" || e.PropertyName == "shadow-blur" || e.PropertyName == "shadow-alpha" ||
                    e.PropertyName == "shadow-offset-x" || e.PropertyName == "shadow-offset-y" || e.PropertyName == "shadow-color" || e.PropertyName == "xml")
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

        private void ToggleShadow_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleShadow.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.ShadowEnable = true;
                    EnableControls();
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.ShadowEnable = false;
                    DisableControls();
                });
            }
        }

        private void SpinnerShadowBlur_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.ShadowBlur = (double)SpinnerShadowBlur.Value;
            });
        }

        private void CPShadowColor_Click(object sender, EventArgs e)
        {
            ColorChooser chooser = new ColorChooser { Color = CPShadowColor.BackColor };
            if (chooser.ShowDialog() == DialogResult.OK)
            {
                CPShadowColor.BackColor = chooser.Color;

                string colorCode = SimpleColorEditor.ColorToHexString(chooser.Color);
                SetProperty(delegate
                {
                    m_Item.ShadowColor = colorCode;
                });
            }
        }

        private void SpinnerOffsetX_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.ShadowXOffset = (double)SpinnerOffsetX.Value;
            });
        }

        private void SpinnerOffsetY_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.ShadowYOffset = (double)SpinnerOffsetY.Value;
            });
        }

        private void SliderShadowAlpha_Scroll(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.ShadowAlpha = (int)SliderShadowAlpha.Value;
            });
        }
    }
}
