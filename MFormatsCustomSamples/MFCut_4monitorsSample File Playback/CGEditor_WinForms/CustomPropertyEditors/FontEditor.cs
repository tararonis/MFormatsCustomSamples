using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Drawing.Text;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CGEditor.CGItemWrappers;

namespace CGEditor_WinForms.CustomPropertyEditors
{
    public partial class FontEditor : UserControl
    {

        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private IFont m_Item;
        private Editor m_Editor;

        public void SetEditor(Editor editor)
        {
            m_Editor = editor;
        }


        public FontEditor()
        {
            InitializeComponent();
            ToolTip toolTip1 = new ToolTip();

            // Set up the delays for the ToolTip.
            toolTip1.AutoPopDelay = 5000;
            toolTip1.InitialDelay = 1000;
            toolTip1.ReshowDelay = 500;
            // Force the ToolTip text to be displayed whether or not the form is active.
            toolTip1.ShowAlways = true;
            toolTip1.SetToolTip(this.ComboFlip, "Text Flip Type");
            toolTip1.SetToolTip(this.ComboFontFamily, "Font Family");
            toolTip1.SetToolTip(this.ComboFontTypeface, "Font Type");
            toolTip1.SetToolTip(this.SpinnerFontSize, "Font Size");
            toolTip1.SetToolTip(this.ToggleNoTabs, "No Tabs");
            toolTip1.SetToolTip(this.ToggleRightToLeft, "Right To Left");
            toolTip1.SetToolTip(this.ToggleStrikeout, "Strikeout");
            toolTip1.SetToolTip(this.ToggleUnderline, "Underline");
            toolTip1.SetToolTip(this.ToggleVertical, "Vertical");
            toolTip1.SetToolTip(this.ToggleWordWrap, "Word Wrap");
            m_bChangedByUI = false;
            FillFontFamilies();

            m_bChangedByUI = true;
            m_bAceptExternalUpdate = true;

            this.Enabled = false;
            //Set collections
            ComboFlip.DataSource = ObservableCollections.GetFlipModes();
        }

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is IGradient)
                {
                    m_Item = (IFont)item;
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
                ComboFontFamily.SelectedItem = m_Item.FontFamily;

                FillFontTypeFaces();
                ComboFontTypeface.SelectedItem = m_Item.FontTypeface;

                SpinnerFontSize.Value = (decimal)m_Item.FontSize;

                ToggleUnderline.Checked = m_Item.Underline;
                ToggleStrikeout.Checked = m_Item.StrikeOut;
                ToggleWordWrap.Checked = m_Item.WordWrap;
                ToggleVertical.Checked = m_Item.Vertical;
                ToggleRightToLeft.Checked = m_Item.RightToLeft;
                ToggleNoTabs.Checked = m_Item.NoTabs;

                ComboFlip.SelectedItem = m_Item.Flip;

                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            ComboFontFamily.SelectedItem = null;
            ComboFontTypeface.SelectedItem = null;
            SpinnerFontSize.Value = 0;
            ToggleUnderline.Checked = false;
            ToggleStrikeout.Checked = false;
            ToggleWordWrap.Checked = false;
            ToggleVertical.Checked = false;
            ToggleRightToLeft.Checked = false;
            ToggleNoTabs.Checked = false;
            m_bChangedByUI = true;
        }

        private void FillFontFamilies()
        {
            InstalledFontCollection fontCollection = new InstalledFontCollection();
            List<string> fontFamilies = fontCollection.Families.Select(ff => ff.Name).ToList();
            fontFamilies = new List<string>(fontFamilies.OrderBy(i => i));
            ComboFontFamily.DataSource = fontFamilies;
        }

        private void FillFontTypeFaces()
        {
            if (ComboFontFamily.SelectedItem != null && ComboFontFamily.SelectedItem.ToString() != string.Empty)
            {
                ComboFontTypeface.SelectedItem = null;
                FontFamily ff = new FontFamily(ComboFontFamily.SelectedItem.ToString());
                List<string> fontTypefaces = new List<string>();
		fontTypefaces.Add("");
                if (ff.IsStyleAvailable(FontStyle.Regular))
                {
                    fontTypefaces.Add("Regular");
                }
                if (ff.IsStyleAvailable(FontStyle.Italic))
                {
                    fontTypefaces.Add("Italic");
                }
                if (ff.IsStyleAvailable(FontStyle.Bold))
                {
                    fontTypefaces.Add("Bold");
                }
                if (ff.IsStyleAvailable(FontStyle.Italic | FontStyle.Bold))
                {
                    fontTypefaces.Add("Bold Italic");
                }

                //fontTypefaces.Add("");
                //foreach (FamilyTypeface ft in ff.FamilyTypefaces)
                //{
                //    if (ft.AdjustedFaceNames.ContainsKey(lang))
                //    {
                //        if (!ft.AdjustedFaceNames[lang].ToLower().Contains("regular") &&
                //            !ft.AdjustedFaceNames[lang].ToLower().Contains("oblique"))
                //        {
                //            fontTypefaces.Add(ft.AdjustedFaceNames[lang]);
                //        }
                //    }
                //}
                if (fontTypefaces.Count > 0)
                {
                    ComboFontTypeface.DataSource = fontTypefaces;
                }
            }
        }

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                if (e.PropertyName == "font" || e.PropertyName == "font-size" || e.PropertyName == "word-break"
                    || e.PropertyName == "textformat" || e.PropertyName == "textflip" || e.PropertyName == "xml")
                {
                    FillControls();
                }
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

        private void ComboFontFamily_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ComboFontFamily.SelectedItem != null)
            {
                SetProperty(delegate
                {
                    m_Item.FontFamily = ComboFontFamily.SelectedItem.ToString();
                    FillFontTypeFaces();
                });

            }
        }

        private void SpinnerFontSize_ValueChanged(object sender, EventArgs e)
        {
            SetProperty(delegate
            {
                m_Item.FontSize = (double)SpinnerFontSize.Value;
            });
        }

        private void ComboFontTypeface_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ComboFontTypeface.SelectedItem != null)
            {
                SetProperty(delegate
                {
                    m_Item.FontTypeface = ComboFontTypeface.SelectedItem.ToString();
                });
            }
        }

        private void ToggleUnderline_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleUnderline.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.Underline = true;
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.Underline = false;
                });
            }
        }

        private void ToggleStrikeout_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleStrikeout.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.StrikeOut = true;
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.StrikeOut = false;
                });
            }
        }

        private void ToggleWordWrap_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleWordWrap.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.WordWrap = true;
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.WordWrap = false;
                });
            }
        }

        private void ToggleVertical_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleVertical.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.Vertical = true;
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.Vertical = false;
                });
            }
        }

        private void ToggleRightToLeft_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleRightToLeft.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.RightToLeft = true;
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.RightToLeft = false;
                });
            }
        }

        private void ToggleNoTabs_CheckedChanged(object sender, EventArgs e)
        {
            if (ToggleNoTabs.Checked)
            {
                SetProperty(delegate
                {
                    m_Item.NoTabs = true;
                });
            }
            else
            {
                SetProperty(delegate
                {
                    m_Item.NoTabs = false;
                });
            }
        }

        private void ComboFlip_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (ComboFlip.SelectedItem != null)
            {
                SetProperty(delegate
                {
                    m_Item.Flip = ComboFlip.SelectedItem.ToString();
                });
            }
        }
    }
}
