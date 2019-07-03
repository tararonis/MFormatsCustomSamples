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
    public partial class TextOutlineColorEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private ITextOutline m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }

        Button CPColor0 = new Button();
        public int nMaxGradients = 4; //Max number of gradients in control

        public TextOutlineColorEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.ComboGradientType, "Outline Gradient Type");
            toolTip1.SetToolTip(this.SpinnerOutlineWidth, "Outline Angle");
            toolTip1.SetToolTip(this.buttonAddGradient, "Add Color to Outline Gradient");
            m_bChangedByUI = false;
            CPColor0.BackColor = Color.Transparent;
            CPColor0.Width = 28;
            CPColor0.Height = 16;
            CPColor0.Click += CPColor0_Click;
            StackColors.Controls.Add(CPColor0);
            m_bChangedByUI = true;

            m_bAceptExternalUpdate = true;

            this.Enabled = false;
            ComboGradientType.DataSource = ObservableCollections.GetGradientTypes();
        }

        void CPColor0_Click(object sender, EventArgs e)
        {
            ColorChooser chooser = new ColorChooser { Color = ((Button)sender).BackColor };
            if (chooser.ShowDialog() == DialogResult.OK)
            {
                ((Button)sender).BackColor = chooser.Color;
                SetColor();
            }
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IOutline)
                {
                    m_Item = (ITextOutline)item;
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
                GetColor();
                GetOutlineWidth();
                m_bChangedByUI = true;
            }
        }


        private void AddColorPicker(Color startColor)
        {
            Button cPicker = new Button();
            cPicker.Width = 28;
            cPicker.Height = 16;
            cPicker.BackColor = startColor;
            //cPicker.ColorPickerDisabled += new EventHandler(cp_ColorPickerDisabled);
            cPicker.Click += CPColor0_Click;
            StackColors.Controls.Add(cPicker);
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            ComboGradientType.SelectedItem = null;
            SpinnerOutlineWidth.Value = 0;
            CPColor0.BackColor = Color.Transparent;
            //Remove ColorPickers
            for (int i = StackColors.Controls.Count - 1; i >= 0; i--)
            {
                Control element = StackColors.Controls[i];
                if (element is Button && element != CPColor0)
                {
                    StackColors.Controls.Remove(element);
                }
            }
            m_bChangedByUI = true;
        }

        private void GetColor()
        {
            //Get gradient colors
            string strGradientType;
            string strColor = m_Item.TextOutlineColor;
            Color[] gradientColors = WrapperHelpers.parseCGradient(strColor, out strGradientType);
            ComboGradientType.SelectedItem = strGradientType;

            if (gradientColors != null && gradientColors.Length > 0)
            {
                for (int i = 0; i < gradientColors.Length; i++)
                {
                    if (i == 0)
                        CPColor0.BackColor = gradientColors[0];
                    else
                    {
                        AddColorPicker(gradientColors[i]);
                    }
                }
            }
        }

        private void GetOutlineWidth()
        {
            SpinnerOutlineWidth.Value = (decimal)m_Item.TextOutlineWidth;
        }

        private void SetColor()
        {
            if (m_Item != null && m_bChangedByUI)
            {
                Color[] colors = new Color[StackColors.Controls.Count];
                for (int i = 0; i < StackColors.Controls.Count; i++)
                {
                    if (StackColors.Controls[i] is Button)
                    {
                        Button cp = (Button)StackColors.Controls[i];
                        colors[i] = cp.BackColor;
                    }
                }
                string strGrType = ComboGradientType.SelectedItem.ToString();
                string strCGGradient = WrapperHelpers.ComposeCGGradient(colors, strGrType);

                UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ((CGBaseItem)m_Item).ID));

                m_bAceptExternalUpdate = false;
                m_Item.TextOutlineColor = strCGGradient;
                m_bAceptExternalUpdate = true;
            }
        }

        private void SetOutlineWidth()
        {
            if (m_Item != null && m_bChangedByUI)
            {
                UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(m_Editor.UndoAction, new HistoryItem(m_Editor.CurrentState, ((CGBaseItem)m_Item).ID));
                double dblWidth = (double)SpinnerOutlineWidth.Value;
                m_bAceptExternalUpdate = false;
                m_Item.TextOutlineWidth = dblWidth;
                m_bAceptExternalUpdate = true;
            }
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "textoutline-color")
                    GetColor();
                else if (e.PropertyName == "textoutline")
                    GetOutlineWidth();
                else if (e.PropertyName == "xml")
                    FillControls();
            }
        }

        private void buttonAddGradient_Click(object sender, EventArgs e)
        {
            if (StackColors.Controls.Count < nMaxGradients)
            {
                m_bAceptExternalUpdate = false;
                AddColorPicker(Color.Transparent);
                SetColor();
                m_bAceptExternalUpdate = true;
            }
        }

        private void ComboGradientType_SelectedIndexChanged(object sender, EventArgs e)
        {
            SetColor();
        }

        private void SpinnerOutlineWidth_ValueChanged(object sender, EventArgs e)
        {
            SetOutlineWidth();
        }
    }
}
