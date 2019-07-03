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
    public partial class ShapeEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IShape m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }

        public ShapeEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.ComboShapeType, "Shape Type");
            toolTip1.SetToolTip(this.SpinnerNSides, "Number of Sides");
            toolTip1.SetToolTip(this.SpinnerRotate, "Rotate Angle");
            toolTip1.SetToolTip(this.SpinnerRoundCorners, "Round Corners");
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
            ComboShapeType.DataSource = ObservableCollections.GetShapeTypes();
        }
        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IShape)
                {
                    m_Item = (IShape)item;
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
                ComboShapeType.SelectedItem = m_Item.Type;
                if (m_Item.Type == "polygon")
                {
                    SpinnerNSides.Enabled = true;
                    SpinnerNSides.Value = (decimal)m_Item.Sides;
                }
                else
                {
                    SpinnerNSides.Enabled = false;
                    SpinnerNSides.Value = 0;
                }

                SpinnerRoundCorners.Value = (decimal)m_Item.RoundCorners * 100;
                SpinnerRotate.Value = (decimal)m_Item.Rotate;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            ComboShapeType.SelectedItem = null;
            SpinnerNSides.Value = 0;
            SpinnerRoundCorners.Value = 0;
            SpinnerRotate.Value = 0;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "shapetype" || e.PropertyName == "nsides" || e.PropertyName == "round-corners" ||
                    e.PropertyName == "rotate" || e.PropertyName == "xml")
                    FillControls();
            }
        }

        delegate void Setter();
        private void SetProperty(Setter s)
        {
            try
            {
                UndoRedoManager.UndoRedoManager.Instance()
                    .Push<HistoryItem>(m_Editor.UndoAction,
                        new HistoryItem(m_Editor.CurrentState, ((CGBaseItem) m_Item).ID));
            }
            catch
            {
            }
            if (m_Item != null && m_bChangedByUI)
            {
                m_bAceptExternalUpdate = false;
                s();
                m_bAceptExternalUpdate = true;
            }
        }

        private void ComboShapeType_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Type = ComboShapeType.SelectedItem.ToString();
                if (m_Item.Type == "polygon")
                {
                    SpinnerNSides.Enabled = true;
                    SpinnerNSides.Value = (decimal)m_Item.Sides;
                }
                else
                {
                    SpinnerNSides.Enabled = false;
                    SpinnerNSides.Value = 0;
                }
            });
        }

        private void SpinnerNSides_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Sides = (int)SpinnerNSides.Value;
            });
        }

        private void SpinnerRoundCorners_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.RoundCorners = (double)SpinnerRoundCorners.Value / 100;
            });
        }

        private void SpinnerRotate_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Rotate = (double)SpinnerRotate.Value;
            });
        }
    }
}
