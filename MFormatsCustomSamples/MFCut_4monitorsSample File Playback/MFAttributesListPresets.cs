using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Drawing;
using System.Windows.Forms;
using MFORMATSLib;
using System.Xml.Linq;
using System.IO;
using Microsoft.Win32;

namespace SampleFilePlayback
{
    public partial class MFAttributesListPresets : ListViewEx
    {
        public event EventHandler OnPropsUpdated;

        private MFOverlayHTMLClass m_pMFHTML;
        private string m_CEFResourcesPath = "";
        private string m_strPresetsPath = "";

        public MFAttributesListPresets()
        {
            InitializeComponent();

            Columns[0].Tag = new TextBox();
            Columns[1].Tag = new TextBox();
            Columns[2].Tag = new TextBox();

            this.ListSubItemChanged += new EventHandler(MFAttributesList_ListSubItemChanged);
            this.ListSubItemOnStartChange += new EventHandler(MFAttributesList_ListSubItemOnStartChange);
            this.ListSubItemOnDoubleClick += new EventHandler(MFAttributesListPresets_ListSubItemOnDoubleClick);
            this.ShowItemToolTips = true;
        }

        void MFAttributesListPresets_ListSubItemOnDoubleClick(object sender, EventArgs e)
        {

            UpdateHTML((ListViewEx_EventArgs)e, true);
        }

        public Object SetControlledObject(Object pObject)
        {
            Object pOld = (Object)m_pMFHTML;
            try
            {
                m_pMFHTML = (MFOverlayHTMLClass)pObject;

                m_CEFResourcesPath = (string)Registry.GetValue(@"HKEY_CURRENT_USER\SOFTWARE\Medialooks\MFormats", "resources.path", null);
                m_strPresetsPath = m_CEFResourcesPath + "\\CEF\\HTMLOverlayPresets.xml";                
                UpdateList();
            }
            catch (Exception) { }            

            return pOld;
        }

        public void UpdateList()
        {
            //Remember selection and scroll position
            EndEdit();

            int nTopItemIndex = -1;
            if (this.TopItem != null && this.TopItem.Index >= 0)
                nTopItemIndex = this.TopItem.Index;

            int nSelectedIndex = -1;
            if (this.SelectedIndices.Count > 0)
                nSelectedIndex = this.SelectedIndices[0];

            Items.Clear();

            // Load presets from a file
            try
            {
                XDocument xDocument = XDocument.Load(m_strPresetsPath);
                foreach (var preset in xDocument.Element("Settings").Elements("Preset"))
                {
                    ListViewItem lvItem = Items.Add(preset.Attribute("site").Value);
                    lvItem.SubItems.Add(preset.Attribute("props").Value);
                    lvItem.SubItems.Add(preset.Attribute("JS").Value);
                    lvItem.Tag = "";
                }
            }
            catch (Exception ex)
            {
                //MessageBox.Show("HTMLOverlayPresets.xml not found.\n\n");
            }
            
            Columns[0].Width = 180;
            Columns[1].Width = 520;
            Columns[2].Width = 310;

            //Restore selection and scroll
            if (nSelectedIndex > 0 && this.Items.Count - 1 >= nSelectedIndex)
                this.Items[nSelectedIndex].Selected = true;
            if (nTopItemIndex > 0 && this.Items.Count - 1 >= nTopItemIndex)
                this.TopItem = this.Items[nTopItemIndex];
        }

        public void AddPreset(string _strJS)
        {
            string strCurrURL;
            string strProps;
            m_pMFHTML.PropsGet("current_url", out strCurrURL);
            m_pMFHTML.PropsGet("", out strProps);

            ListViewItem lvItem = Items.Add(strCurrURL);
            lvItem.SubItems.Add(strProps);
            lvItem.SubItems.Add(_strJS);
            lvItem.Tag = "";

            XDocument xDocument = null;
            try
            {
                xDocument = XDocument.Load(m_strPresetsPath);
            }
            catch { }

            if (xDocument == null)
            {
                xDocument = new XDocument(new XElement("Settings"));
            }
            xDocument.Element("Settings").Add(new XElement("Preset", new XAttribute("site", strCurrURL), new XAttribute("props", strProps), new XAttribute("JS", _strJS)));
            xDocument.Save(m_strPresetsPath);
        }

        public void UpdatePreset(string _strJS)
        {
            if (SelectedIndices.Count == 0)
                return;

            string strCurrURL;
            string strProps;
            m_pMFHTML.PropsGet("current_url", out strCurrURL);
            m_pMFHTML.PropsGet("", out strProps);

            XDocument xDocument = null;
            try
            {
                xDocument = XDocument.Load(m_strPresetsPath);
            }
            catch { }

            if (xDocument == null)
            {
                xDocument = new XDocument(new XElement("Settings"));
            }

            int i = 0;
            foreach (var preset in xDocument.Element("Settings").Elements("Preset"))
            {
                if (this.SelectedIndices[0] == i)
                {
                    preset.Attribute("site").Value = strCurrURL;
                    preset.Attribute("props").Value = strProps;
                    preset.Attribute("JS").Value = _strJS;

                    xDocument.Save(m_strPresetsPath);
                    break;
                }
                i++;
            }

            Items[SelectedIndices[0]].Text = strCurrURL;
            Items[SelectedIndices[0]].SubItems[1].Text = strProps;
            Items[SelectedIndices[0]].SubItems[2].Text = _strJS;
        }

        public void ResetProps()
        {
            //Default properties
            string strProps = 
            "<m-props cef.log_file='' cef.log_severity='0' transparency='auto' cef.default_css='' cef.web_security='false' interlacing='false' " +
            "cef.mute_audio='true' width_min='320' height_min='16' scrollbars.enabled='true' scroll.speed_v='0.0' scroll.speed_h='0.0' crop='' zoom='100%' " +
            "external_process='true' dst.alpha='100%' dst.border_width='0' dst.border_color='white' dst.x='0.0%' dst.y='0.0%' dst.w='0.0%' dst.h='0.0%'/>";

            m_pMFHTML.PropsSet("", strProps);
        }

        public void RemovePreset()
        {
            if (SelectedIndices.Count == 0)
                return;

            XDocument xDocument = null;
            try
            {
                xDocument = XDocument.Load(m_strPresetsPath);
            }
            catch { }

            if (xDocument != null)
            {
                int i = 0;
                foreach (var preset in xDocument.Element("Settings").Elements("Preset"))
                {
                    if (this.SelectedIndices[0] == i)
                    {
                        preset.Remove();

                        xDocument.Save(m_strPresetsPath);
                        break;
                    }
                    i++;
                }

                Items.Clear();

                foreach (var preset in xDocument.Element("Settings").Elements("Preset"))
                {
                    ListViewItem lvItem = Items.Add(preset.Attribute("site").Value);
                    lvItem.SubItems.Add(preset.Attribute("props").Value);
                    lvItem.SubItems.Add(preset.Attribute("JS").Value);
                    lvItem.Tag = "";
                }
            }

        }
        private ListViewItem AddDelimer(string strName)
        {
            if (strName == "") return null;
            // Add delimer
            var lvItem = Items.Add(strName);
            lvItem.UseItemStyleForSubItems = true;
            lvItem.BackColor = Color.FromArgb(222, 232, 254);
            lvItem.SubItems.Add("");
            return lvItem;
        }

        string GetPrefix(ref string strName)
        {
            string strPrefix = "";
            int nDelimer = strName.IndexOf("::", StringComparison.Ordinal);
            if (nDelimer >= 0)
            {
                strPrefix = strName.Substring(0, nDelimer);
                strName = strName.Substring(nDelimer + 2);
            }
            return strPrefix;
        }


        void MFAttributesList_ListSubItemOnStartChange(object sender, EventArgs e)
        {
            ListViewEx_EventArgs lvArg = (ListViewEx_EventArgs)e;

            // Check for separator
            if (lvArg.lvItem.Tag == null)
                lvArg.Cancel = true;

            // Check for property name (edit only if <new name> - one subitem)
            if (lvArg.nSubItem == 0 && lvArg.lvItem.SubItems.Count > 1)
                lvArg.Cancel = true;
        }

        void MFAttributesList_ListSubItemChanged(object sender, EventArgs e)
        {
            ListViewEx_EventArgs lvArg = (ListViewEx_EventArgs)e;

            XDocument xDocument = null;
            try
            {
                xDocument = XDocument.Load(m_strPresetsPath);
            }
            catch { }

            if (lvArg.nSubItem == 0)
            {
                // Add subitem for value
                string strProps;
                m_pMFHTML.PropsGet("", out strProps);
                lvArg.lvItem.SubItems.Add(strProps); // Props
                lvArg.lvItem.SubItems.Add(""); // JS

                if (xDocument == null)
                {
                    xDocument = new XDocument(new XElement("Settings"));
                }

                xDocument.Element("Settings").Add(new XElement("Preset", new XAttribute("site", lvArg.lvItem.Text), new XAttribute("props", strProps), new XAttribute("JS", "")));
                xDocument.Save(m_strPresetsPath);

//                 ListViewItem lvItem = Items.Add("<New preset>");
//                 lvItem.Tag = "";
            }
            else
            {
                UpdateHTML(lvArg);

                if (xDocument != null)
                {
                    foreach (var preset in xDocument.Element("Settings").Elements("Preset"))
                    {
                        if (preset.Attribute("site").Value == lvArg.lvItem.Text)
                        {
                            preset.Attribute("props").Value = lvArg.lvItem.SubItems[1].Text;
                            preset.Attribute("JS").Value = lvArg.lvItem.SubItems[2].Text;

                            xDocument.Save(m_strPresetsPath);
                            break;
                        }
                    }
                }
            }
        }

        private void UpdateHTML(ListViewEx_EventArgs lvArg, bool _bPageLoad = false)
        {
            string strSite = lvArg.lvItem.SubItems[0].Text;
            string strXML = lvArg.lvItem.SubItems[1].Text;
            string strJS = lvArg.lvItem.SubItems[2].Text;

            string strCurrURL;
            m_pMFHTML.PropsGet("current_url", out strCurrURL);

            bool bTheSameURL = false;
            
            if (strCurrURL != null)
                bTheSameURL = strCurrURL.Contains(strSite);

            if (bTheSameURL || _bPageLoad)
            {
                if (_bPageLoad)
                    m_pMFHTML.BrowserPageLoad(strSite);

                m_pMFHTML.PropsSet("", strXML);

                if (!String.IsNullOrEmpty(strJS))
                    m_pMFHTML.BrowserJavascriptExecute(strJS);

                EventHandler handler = this.OnPropsUpdated;
                if (handler != null)
                {
                    handler(this, null);
                }
            }
        }
    }
}
