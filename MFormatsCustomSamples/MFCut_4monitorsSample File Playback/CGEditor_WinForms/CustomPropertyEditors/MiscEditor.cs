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
    public partial class MiscEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IMisc m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public MiscEditor()
        {
            InitializeComponent();

            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;

            toolTip1.SetToolTip(this.ComboAlign, "Item's Align");
            toolTip1.SetToolTip(this.ComboInterlace, "Interlace type");
            toolTip1.SetToolTip(this.ComboPlayMode, "Play Mode");
            toolTip1.SetToolTip(this.ComboScale, "Scale Type");
            toolTip1.SetToolTip(this.SpinnerEdgeSmooth, "Edge Smooth");
            toolTip1.SetToolTip(this.SpinnerPixelAR, "Pixel AR");

            m_bChangedByUI = false;
            ComboScale.DataSource = ObservableCollections.GetScaleTypes();
            ComboAlign.DataSource = ObservableCollections.GetAligns();
            ComboInterlace.DataSource = ObservableCollections.GetInterlaceTypes();
            ComboPlayMode.DataSource = ObservableCollections.GetPlayModes();
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }


        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IMisc)
                {
                    m_Item = (IMisc)item;
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
                SpinnerEdgeSmooth.Value = (decimal)m_Item.EdgesSmooth;
                SpinnerPixelAR.Value = (decimal)m_Item.PixelAR;
                ComboScale.SelectedItem = m_Item.Scale;
                ComboAlign.SelectedItem = m_Item.Align;
                ComboInterlace.SelectedItem = m_Item.Interlace;
                ComboPlayMode.SelectedItem = m_Item.PlayMode;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            SpinnerEdgeSmooth.Value = 0;
            SpinnerPixelAR.Value = 0;
            ComboScale.SelectedItem = null;
            ComboAlign.SelectedItem = null;
            ComboInterlace.SelectedItem = null;
            ComboPlayMode.SelectedItem = null;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "align" || e.PropertyName == "interlace" || e.PropertyName == "pixel-ar" ||
                    e.PropertyName == "scale" || e.PropertyName == "edges-smooth" || e.PropertyName == "play-mode" || e.PropertyName == "xml")
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

        private void SpinnerEdgeSmooth_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.EdgesSmooth = (double)SpinnerEdgeSmooth.Value;
            });
        }

        private void SpinnerPixelAR_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.PixelAR = (double)SpinnerPixelAR.Value;
            });
        }

        private void ComboScale_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Scale = ComboScale.SelectedItem.ToString();
            });
        }

        private void ComboAlign_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Align = ComboAlign.SelectedItem.ToString();
            });
        }

        private void ComboInterlace_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Interlace = ComboInterlace.SelectedItem.ToString();
            });
        }

        private void ComboPlayMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.PlayMode = ComboPlayMode.SelectedItem.ToString();
            });
        }
    }
}
