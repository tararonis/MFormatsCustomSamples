using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CGEditor.CGItemWrappers;

namespace CGEditor_WinForms
{
    public partial class XMLDescriptionControl : UserControl
    {
        public XMLDescriptionControl()
        {
            InitializeComponent();
        }
        private bool m_bChangedByUI; //Indicates that value was changed by control UI
        private bool m_bAceptExternalUpdate; //Enables or disables exteranal update of the control

        private CGBaseItem m_Item;

        public void SetControlledObject(object item)
        {
            try
            {
                if (item != null && item is CGBaseItem)
                {
                    m_Item = (CGBaseItem)item;
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
                XMLBody.Text = m_Item.XML;
                m_bChangedByUI = true;
            }
        }

        private void ClearControls()
        {
            m_bChangedByUI = false;
            XMLBody.Text = "";
            m_bChangedByUI = true;
        }

        //public void SetXMLDescription(string xml)
        //{
        //    XmlDescriptionControl.Text = xml;
        //}

        void External_ItemChanged(object sender, ItemChangedArgs e)
        {
            if (m_bAceptExternalUpdate)
            {
                FillControls();
            }
        }



        private void ApplyXMLButton_Click(object sender, EventArgs e)
        {
            if (XMLBody.Text != string.Empty)
            {
                m_bAceptExternalUpdate = false;
                m_Item.XML = XMLBody.Text;
                m_bAceptExternalUpdate = true;
            }
        }
    }
}
