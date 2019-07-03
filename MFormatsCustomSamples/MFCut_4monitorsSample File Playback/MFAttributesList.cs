using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Drawing;
using System.Windows.Forms;
using MFORMATSLib;

namespace SampleFilePlayback
{
    public partial class MFAttributesList : ListViewEx
    {
        IMFProps m_pMFProps;        
        public MFAttributesList()
        {
            InitializeComponent();

            Columns[0].Tag = new TextBox();
            Columns[1].Tag = new TextBox();

            this.ListSubItemChanged += new EventHandler(MFAttributesList_ListSubItemChanged);
            this.ListSubItemOnStartChange += new EventHandler(MFAttributesList_ListSubItemOnStartChange);
            this.ShowItemToolTips = true;
        }

        public Object SetControlledObject(Object pObject)
        {
            Object pOld = (Object)m_pMFProps;
            try
            {
                m_pMFProps = (IMFProps)pObject;
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

            HybridDictionary mapTypesPos = new HybridDictionary();
            ListDictionary mapAttributes = new ListDictionary();

            mapTypesPos.Clear();
            mapTypesPos.Add("", AddDelimer(""));


            // Custom attributes
            var nCount = 0;
            m_pMFProps.PropsGetCount("", out nCount);
            for (int i = 0; i < nCount; i++)
            {
                string strName, strValue;
                int isNode;
                m_pMFProps.PropsGetByIndex("", i, out strName, out strValue, out isNode);
                if (!mapAttributes.Contains(strName))
                    mapAttributes.Add(strName, null);
            }

            // Insert names and attributes into list
            foreach (DictionaryEntry entry in mapAttributes)
            {
                string strFullName = (string)entry.Key;
                string strName = strFullName;
                string strPrefix = GetPrefix(ref strName);

                if (!mapTypesPos.Contains(strPrefix))
                    mapTypesPos.Add(strPrefix, AddDelimer(strPrefix));

                int nPos = Items.IndexOf((ListViewItem)mapTypesPos[strPrefix]);

                string strValue;
                m_pMFProps.PropsGet(strFullName, out strValue);

                string strHelp;
                m_pMFProps.PropsInfoGet(strName, eMInfoType.eMIT_Help, out strHelp);

                string strType;
                m_pMFProps.PropsInfoGet(strName, eMInfoType.eMIT_Type, out strType);

                string strDefault;
                m_pMFProps.PropsInfoGet(strName, eMInfoType.eMIT_Default, out strDefault);

                string strValues;
                m_pMFProps.PropsInfoGet(strName, eMInfoType.eMIT_Values, out strValues);

                if (string.IsNullOrEmpty(strValue))
                    strValue = strDefault;

                string strHelpString;
                m_pMFProps.PropsInfoGet(strName, eMInfoType.eMIT_Help, out strHelpString);

                ListViewItem lvItem = nPos >= 0 ? Items.Insert(nPos, strName) : Items.Add(strName);
                if (strHelpString != string.Empty && strHelpString != "null")
                    lvItem.ToolTipText = strHelpString;
                ListViewItem.ListViewSubItem lvSubItem = lvItem.SubItems.Add(strValue);
                if ((strType == "option" || strType == "option_fixed") && !string.IsNullOrEmpty(strValues))
                {   
                    strValues = strValues.TrimEnd('|');                    
                    // The option values may be divided by commas or by '|' if values contain commas
                    string[] arrValues = strValues.Contains("|") ? strValues.Split('|') : strValues.Split(',');
                    if (arrValues.Length > 1)
                    {
                        ComboBox pCombo = SubItem_SetCombo(lvSubItem, null, strType == "option_fixed" ? ComboBoxStyle.DropDownList : ComboBoxStyle.DropDown);
                        for (int k = 0; k < arrValues.Length; k++)
                        {
                            pCombo.Items.Add(arrValues[k].Trim());
                        }
                    }
                }
                else if (strType == "bool")
                {
                    ComboBox pCombo = SubItem_SetCombo(lvSubItem, null, ComboBoxStyle.DropDownList);
                    pCombo.Items.Add("true");
                    pCombo.Items.Add("false");
                }
                else if (strType == "path")
                {
                    SaveFileDialog pDialog = new SaveFileDialog { Title = strHelp };
                    TextBox textWithDialog = new TextBox { Tag = pDialog };
                    SubItem_SetControl(lvSubItem, textWithDialog);
                }
                else if (strType == "path_open")
                {
                    OpenFileDialog pDialog = new OpenFileDialog { Title = strHelp };
                    TextBox textWithDialog = new TextBox { Tag = pDialog };
                    SubItem_SetControl(lvSubItem, textWithDialog);
                }

                if (strPrefix != "")
                    strPrefix += "::";
                lvItem.Tag = strPrefix;
                lvItem.UseItemStyleForSubItems = false;
                lvItem.BackColor = Color.White;
                if (strValue != strDefault)
                {
                    // User modified values select by black
                    lvSubItem.ForeColor = Color.Black;
                }
                else
                {
                    lvSubItem.ForeColor = Color.Gray;
                }
            }
            Columns[0].Width = -2;
            //Columns[1].Width = -2;

            //Restore selection and scroll
            if (nSelectedIndex > 0 && this.Items.Count - 1 >= nSelectedIndex)
                this.Items[nSelectedIndex].Selected = true;
            if (nTopItemIndex > 0 && this.Items.Count - 1 >= nTopItemIndex)
                this.TopItem = this.Items[nTopItemIndex];
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

            if (lvArg.nSubItem == 0)
            {
                // Add subitem for value
                lvArg.lvItem.SubItems.Add("");
            }
            else
            {
                // Update attribute
                string sPrefix = (string)lvArg.lvItem.Tag;
                string sName = sPrefix + lvArg.lvItem.SubItems[0].Text;
                string sValue = lvArg.lvItem.SubItems[1].Text;


                // Check for new element
                int bHave = 0;
                string sOldValue;
                m_pMFProps.PropsGet(sName, out sOldValue);

                string sDefValue;
                m_pMFProps.PropsInfoGet(sName, eMInfoType.eMIT_Default, out sDefValue);

                // Check for remove attributes
                if (sValue == "" && bHave == 1)
                {
                    m_pMFProps.PropsRemove(sName);

                    lvArg.lvItem.SubItems[1].Text = sDefValue;
                    lvArg.lvItem.SubItems[1].ForeColor = Color.Gray;
                }
                else
                {
                    m_pMFProps.PropsSet(sName, sValue);

                    lvArg.lvItem.SubItems[1].ForeColor = Color.Black;

                    if (bHave == 0)
                    {
                        UpdateList();
                    }
                    else if (sValue == "" || sValue == sDefValue)
                    {
                        lvArg.lvItem.SubItems[1].Text = sDefValue;
                        lvArg.lvItem.SubItems[1].ForeColor = Color.Gray;
                    }
                }
            }
        }
    }
}
