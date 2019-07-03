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
    public partial class SimpleColorEditor : UserControl
    {
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private ISimpleColor m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public SimpleColorEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.CPBackground, "Item's Background Color");
            toolTip1.SetToolTip(this.SliderAlpha, "Item's transparency");
            m_bChangedByUI = false;
            CPBackground.BackColor = Color.Transparent;
            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;
            this.Enabled = false;
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is ISimpleColor)
                {
                    m_Item = (ISimpleColor)item;
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
                SliderAlpha.Value = m_Item.Alpha;
                CPBackground.BackColor = WrapperHelpers.ParseCGColor(m_Item.BGColor);
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            SliderAlpha.Value = 255;
            CPBackground.BackColor = Color.Transparent;
            m_bChangedByUI = true;
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "bgcolor" || e.PropertyName == "alpha" || e.PropertyName == "xml")
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

        private void SliderAlpha_Scroll(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.Alpha = (int)SliderAlpha.Value;
            });
        }

        private void CPBackground_Click(object sender, EventArgs e)
        {
            ColorChooser chooser = new ColorChooser { Color = CPBackground.BackColor };
            if (chooser.ShowDialog() == DialogResult.OK)
            {
                CPBackground.BackColor = chooser.Color;

                string colorCode = ColorToHexString(chooser.Color); 
                SetProperty(delegate
                {
                    m_Item.BGColor = colorCode;
                });
            }
        }

        public static string ColorToHexString(Color color)
        {
            byte[] bytes = new byte[3];
            bytes[0] = color.R;
            bytes[1] = color.G;
            bytes[2] = color.B;
            char[] chars = new char[bytes.Length * 2];
            for (int i = 0; i < bytes.Length; i++)
            {
                int b = bytes[i];
                chars[i * 2] = hexDigits[b >> 4];
                chars[i * 2 + 1] = hexDigits[b & 0xF];
            }
            return new string(chars);
        }

        static char[] hexDigits = {
         '0', '1', '2', '3', '4', '5', '6', '7',
         '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    }
}
