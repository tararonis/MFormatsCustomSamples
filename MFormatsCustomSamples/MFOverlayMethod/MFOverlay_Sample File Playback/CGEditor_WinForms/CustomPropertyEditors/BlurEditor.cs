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
    public partial class BlurEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IBlur m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }

        public BlurEditor()
        {
            InitializeComponent();

            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.ComboBlurAlign, "Blur Align");
            toolTip1.SetToolTip(this.SpinnerBlurSizeX, "Blur's Width");
            toolTip1.SetToolTip(this.SpinnerBlurSizeY, "Blur's Height");
            toolTip1.SetToolTip(this.ToggleBlur, "Enable/Disable Blur");
            m_bChangedByUI = false;
            ComboBlurAlign.DataSource = ObservableCollections.GetAligns();
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IBlur)
                {
                    m_Item = (IBlur)item;
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
                ToggleBlur.Checked = m_Item.BlurEnable;
                if ((bool)ToggleBlur.Checked)
                    EnableControls();
                else
                    DisableControls();

                ComboBlurAlign.SelectedItem = m_Item.BlurAlign;
                SpinnerBlurSizeX.Value = (decimal)m_Item.BlurSizeX;
                SpinnerBlurSizeY.Value = (decimal)m_Item.BlurSizeY;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            ComboBlurAlign.SelectedItem = null;
            SpinnerBlurSizeX.Value = 0;
            SpinnerBlurSizeY.Value = 0;
            m_bChangedByUI = true;
        }

        private void DisableControls()
        {
            ComboBlurAlign.Enabled = false;
            SpinnerBlurSizeX.Enabled = false;
            SpinnerBlurSizeY.Enabled = false;
        }
        private void EnableControls()
        {
            ComboBlurAlign.Enabled = true;
            SpinnerBlurSizeX.Enabled = true;
            SpinnerBlurSizeY.Enabled = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "blur-enabled" || e.PropertyName == "blur-size-x" || e.PropertyName == "blur-size-y" ||
                    e.PropertyName == "blur-align" || e.PropertyName == "xml")
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

        private void ToggleBlur_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleBlur.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.BlurEnable = true;
                    EnableControls();
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.BlurEnable = false;
                    DisableControls();
                });
            }
        }

        private void ComboBlurAlign_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.BlurAlign = ComboBlurAlign.SelectedItem.ToString();
            });
        }

        private void SpinnerBlurSizeX_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.BlurSizeX = (double)SpinnerBlurSizeX.Value;
            });
        }

        private void SpinnerBlurSizeY_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.BlurSizeY = (double)SpinnerBlurSizeY.Value;
            });
        }
    }
}
