using System;
using System.Collections.Generic;
using System.Threading;
using CGEditor.CGItemWrappers;
using CGEditor_WinForms.CGItemWrappers;
using MLCHARGENLib;
using MFORMATSLib;
//using eMFCC = MLCHARGENLib.eMFCC;
//using IMFrame = MLCHARGENLib.IMFrame;
//using M_VID_PROPS = MLCHARGENLib.M_VID_PROPS;

namespace CGEditor_WinForms
{
    public class Editor
    {
        private object m_objSource;

        public event EventHandler FrameProcessed;
        public event EventHandler ItemSelected;
        public event EventHandler ItemsListUpdated;

        private List<CopyItem> m_CopyList;
        private int m_nPasteCount;

        public string CurrentState
        {
            get
            {
                string state = string.Empty;
                if (m_objCG != null)
                    ((IMLXMLPersist)m_objCG).SaveToXMLString(out state, 0);
                return state;
            }
        }

        private string m_strBGPath;
        public string BackgroundPath
        {
            set
            {
                m_strBGPath = value;
            }
        }

        public string m_strCurrGroupID;
        public string CurrGroupID
        {
            get { return m_strCurrGroupID; }
            set 
            { 
                m_strCurrGroupID = value;
                if (m_strCurrGroupID != null && m_strCurrGroupID != string.Empty)
                {
                    SelectGroup(m_strCurrGroupID);
                }
                if (m_strCurrGroupID == string.Empty)
                {
                    IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;
                    cgedit.EditSelectionRemove("");
                    UpdateItemsList();
                }
            }
        }

        private bool isThreadWorking;

        public void StopThread()
        {
            isThreadWorking = false;
        }

        public Object SetSourceObject(Object pSource, MLCHARGENLib.CoMLCharGen pCG)
        {
            Object pOld = (Object)m_objSource;
            try
            {
                m_objSource = pSource;
                if (m_objSource != null && pCG != null)
                {
                    CGObject = pCG;
                    UpdateItemsList();
                    Thread workingThread = new Thread(CgEditorBody);
                    isThreadWorking = true;
                    workingThread.Start();
                }

                m_CopyList = new List<CopyItem>();
                m_nPasteCount = 0;
            }
            catch (System.Exception) { }

            return pOld;
        }

        private long sourceCookies = 0;
        private int gcCounter = 0;
        private void CgEditorBody()
        {
            
        }

        private MFORMATSLib.M_AV_PROPS m_avProps;
        public MFORMATSLib.M_AV_PROPS AVProps
        {
            get
            {
                    return m_avProps;
            }
            set 
            {
                m_avProps = value;
            }
        }

        
        private CoMLCharGen m_objCG;  //Medialooks Character Generator object
        public CoMLCharGen CGObject
        {
            get
            {
                return m_objCG;
            }
            set
            {
                m_objCG = value;
                CGBaseItem.m_objCG = m_objCG; //Initialize CGItemWrappers
                m_objCG.OnCGEvent += new IMLCharGenEvents_OnCGEventEventHandler(m_objCG_OnCGEvent);
            }
        }

        void m_objCG_OnCGEvent(string bsItemOrCompositionID, string bsEventType, string bsEventParam)
        {

        }

        private List<CGBaseItem> m_CGItems;
        public List<CGBaseItem> CGItems
        {
            get
            {
                if (m_CGItems == null)
                    m_CGItems = new List<CGBaseItem>();
                return m_CGItems;
            }
            set { m_CGItems = value; }
        }

        public List<CGBaseItem> SelectedtItems
        {
            get
            {
                List<CGBaseItem> selected = new List<CGBaseItem>();
                IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;
                int nCount;
                cgedit.EditSelectionGetCount(out nCount);
                string strId, strColor;
                for (int i = 0; i < nCount; i++)
                {
                    cgedit.EditSelectionGetByIndex(i, out strId, out strColor);
                    foreach (CGBaseItem item in m_CGItems)
                    {
                        if (item.ID == strId)
                        {
                            item.isSelected = true;
                            selected.Add(item);
                            break;
                        }
                    }
                }
                return selected;
            }
        }

        public void SelectItem(string id, bool clearSelection)
        {
            if (CGObject != null)
            {
                IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;
                if (clearSelection)
                {
                    cgedit.EditSelectionRemove("");
                    foreach (CGBaseItem item in CGItems)
                        item.isSelected = false;
                }

                if (CurrGroupID != null && CurrGroupID != string.Empty)
                {
                    cgedit.EditSelectionAdd(CurrGroupID, "lime", 0);
                }

                cgedit.EditSelectionAdd(id, "red", 0);
                foreach (CGBaseItem item in CGItems)
                {
                    if (item.ID == id)
                    {
                        item.isSelected = true;
                        break;
                    }
                }
                if (ItemSelected != null) ItemSelected(this, new EventArgs());
            }
        }

        public void SelectGroup(string groupId)
        {
            IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;

            cgedit.EditSelectionAdd(groupId, "lime", 0);
            foreach (CGBaseItem item in CGItems)
            {
                if (item.ID == groupId)
                {
                    item.isSelected = true;
                    break;
                }
            }
            if (ItemSelected != null) ItemSelected(this, new EventArgs());
        }

        public void Copy()
        {
        }

        public void Paste()
        {
        }

        public void Undo()
        {
        }

        public void Redo()
        {
        }

        public void UndoAction(HistoryItem state)
        {
            string currState = string.Empty;
            ((IMLXMLPersist)m_objCG).SaveToXMLString(out currState, 0);

            UndoRedoManager.UndoRedoManager.Instance().Push<HistoryItem>(UndoAction, new HistoryItem(currState, state.ItemID));

            ((IMLXMLPersist)m_objCG).LoadFromXML(state.XMLState, -1);

            UpdateItemsList();
            foreach (CGBaseItem item in CGItems)
            {
                if (state.ItemID == string.Empty)
                    item.ForceUpdateNotification();
                else if (state.ItemID == item.ID)
                    item.ForceUpdateNotification();
            }
            
        }

        public void SelectItems(string[] ids, bool clearSelection)
        {
            IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;
            if (clearSelection)
            {
                cgedit.EditSelectionRemove("");
                foreach (CGBaseItem item in CGItems)
                    item.isSelected = false;
            }

            if (CurrGroupID != null && CurrGroupID != string.Empty)
            {
                cgedit.EditSelectionAdd(CurrGroupID, "lime", 0);
            }

            foreach (string id in ids)
            {
                cgedit.EditSelectionAdd(id, "red", 0);
                foreach (CGBaseItem item in CGItems)
                {
                    if (item.ID == id)
                    {
                        item.isSelected = true;
                    }
                }
            }
            if (ItemSelected != null) ItemSelected(this, new EventArgs());
        }

        public void SelectByPoint(int left, int top)
        {
            IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;
            string strItemID;
            cgedit.EditSelectionRemove("");
            foreach (CGBaseItem item in CGItems)
                item.isSelected = false;

            if (CurrGroupID != null && CurrGroupID != string.Empty)
            {
                cgedit.EditSelectionAdd(CurrGroupID, "lime", 0);
            }

            cgedit.EditItemGetByPoint(m_strCurrGroupID, left, top, out strItemID);
            cgedit.EditSelectionAdd(strItemID, "red", 0);
            foreach (CGBaseItem item in CGItems)
            {
                if (item.ID == strItemID)
                {
                    item.isSelected = true;
                }
            }
            if (ItemSelected != null) ItemSelected(this, new EventArgs());
        }

        public void SelectByRectangle(int left, int top, int right, int bottom)
        {
            IMLCharGenEdit cgedit = (IMLCharGenEdit)CGObject;
            string strItemIDs;
            cgedit.EditSelectionRemove("");
            foreach (CGBaseItem item in CGItems)
                item.isSelected = false;

            if (!string.IsNullOrEmpty(CurrGroupID))
            {
                cgedit.EditSelectionAdd(CurrGroupID, "lime", 0);
            }

            cgedit.EditItemsGetByRect(CurrGroupID, left, top, right, bottom, out strItemIDs);

            if (!string.IsNullOrEmpty(strItemIDs))
            {
                string[] strSplit = strItemIDs.Split(',');

                foreach (string s in strSplit)
                {
                    string strTrimmed = s.Trim();
                    cgedit.EditSelectionAdd(strTrimmed, "red", 0);
                    foreach (CGBaseItem item in CGItems)
                    {
                        if (item.ID == strTrimmed)
                        {
                            item.isSelected = true;
                            //break;
                        }
                    }
                }
            }
            if (ItemSelected != null) ItemSelected(this, new EventArgs());
        }

        public void AlignLefts()
        {
            if (SelectedtItems.Count > 1)
            {
                double left = SelectedtItems[0].PosX;
                for (int i = 1; i < SelectedtItems.Count; i++)
                {
                    SelectedtItems[i].PosX = left;
                }
            }
        }
        
        public void AlignCenters()
        {
            if (SelectedtItems.Count > 1)
            {
                double left = SelectedtItems[0].PosX;
                double width = SelectedtItems[0].Width;
                for (int i = 1; i < SelectedtItems.Count; i++)
                {
                    SelectedtItems[i].PosX = left + width / 2 - SelectedtItems[i].Width / 2;
                }
            }
        }

        public void AlignRights()
        {
            if (SelectedtItems.Count > 1)
            {
                double left = SelectedtItems[0].PosX;
                double width = SelectedtItems[0].Width;
                for (int i = 1; i < SelectedtItems.Count; i++)
                {
                    SelectedtItems[i].PosX = left + width - SelectedtItems[i].Width;
                }
            }
        }

        public void AlignTops()
        {
            if (SelectedtItems.Count > 1)
            {
                double top = SelectedtItems[0].PosY;
                for (int i = 1; i < SelectedtItems.Count; i++)
                {
                    SelectedtItems[i].PosY = top;
                }
            }
        }

        public void AlignMiddles()
        {
            if (SelectedtItems.Count > 1)
            {
                double top = SelectedtItems[0].PosY;
                double height = SelectedtItems[0].Height;
                for (int i = 1; i < SelectedtItems.Count; i++)
                {
                    SelectedtItems[i].PosY = top + height / 2 - SelectedtItems[i].Height / 2;
                }
            }
        }

        public void AlignBottoms()
        {
            if (SelectedtItems.Count > 1)
            {
                double top = SelectedtItems[0].PosY;
                double height = SelectedtItems[0].Height;
                for (int i = 1; i < SelectedtItems.Count; i++)
                {
                    SelectedtItems[i].PosY = top + height - SelectedtItems[i].Height;
                }
            }
        }
        

        public void UpdateItemsList()
        {
            CGItems.Clear();
            int nItems = 0;

            if (!string.IsNullOrEmpty(CurrGroupID))
                CGObject.GroupItemsCount(CurrGroupID, out nItems);
            else
                CGObject.GetItemsCount(out nItems);

            for (int i = 0; i < nItems; i++)
            {
                string strId = "";
                if (!string.IsNullOrEmpty(CurrGroupID))
                    CGObject.GroupGetItem(CurrGroupID, i, out strId);
                else
                    CGObject.GetItem(i, out strId);


                string strNameDesc;
                CG_ITEM_PROPS itemProps;
                CGObject.GetItemBaseProps(strId, out strNameDesc, out itemProps);
                string strXMLDesc;
                CGObject.GetItemProperties(strId, "", out strXMLDesc);

                CGBaseItem itemToAdd = null;
                if (itemProps.eType == eCG_ItemType.eCGIT_Text)
                {
                    CGTextItem text = new CGTextItem(strId);
                    if (text.FontFamily == "")
                        text.FontFamily = "Arial";
                    itemToAdd = text;
                    CGItems.Add(text);
                }
                if (itemProps.eType == eCG_ItemType.eCGIT_Flash)
                {
                    CGFlashItem flash = new CGFlashItem(strId);
                    itemToAdd = flash;
                    CGItems.Add(flash);
                }
                if (itemProps.eType == eCG_ItemType.eCGIT_Image)
                {
                    CGImageItem img = new CGImageItem(strId);
                    itemToAdd = img;
                    CGItems.Add(img);
                }
                if (itemProps.eType == eCG_ItemType.eCGIT_MediaFile)
                {
                    CGVideoItem img = new CGVideoItem(strId);
                    itemToAdd = img;
                    CGItems.Add(img);
                }
                if (itemProps.eType == eCG_ItemType.eCGIT_Graphics)
                {
                    CGGraphicsItem gr = new CGGraphicsItem(strId);
                    itemToAdd = gr;
                    CGItems.Add(gr);
                }
                if (strXMLDesc.Contains("<ticker"))
                {
                    CGTickerItem tkr = new CGTickerItem(strId);
                    itemToAdd = tkr;
                    CGItems.Add(tkr);
                }
                if ((itemProps.eType & eCG_ItemType.eCGIT_Group) != 0 && strXMLDesc.Contains(@"group-type='line'") && strXMLDesc.Contains(@"<text"))
                {
                    CGTickerLine line = new CGTickerLine(strId);
                    itemToAdd = line;
                    CGItems.Add(line);
                }
                if ((itemProps.eType & eCG_ItemType.eCGIT_Group) != 0 && !strXMLDesc.Contains("<ticker") && !strXMLDesc.Contains(@"group-type='line'"))
                {
                    CGGroupItem group = new CGGroupItem(strId);
                    itemToAdd = group;
                    CGItems.Add(group);
                }

                if (itemToAdd != null)
                {
                    IMLCharGenEdit cgEdit = (IMLCharGenEdit)CGObject;
                    int nCount;
                    cgEdit.EditSelectionGetCount(out nCount);
                    for (int j = 0; j < nCount; j++)
                    {
                        string strID, strColor;
                        cgEdit.EditSelectionGetByIndex(j, out strID, out strColor);
                        {
                            if (strID == itemToAdd.ID)
                                itemToAdd.isSelected = true;
                        }
                    }
                }


            }
            if (ItemsListUpdated != null) ItemsListUpdated(this, new EventArgs());
        }

        public void GroupSelected()
        {
            string strGroupID = string.Empty;
            for (int i = 0; i < SelectedtItems.Count; i++)
            {
                m_objCG.GroupAddItem(SelectedtItems[i].ID, 0, null, ref strGroupID);
            }
            SelectItem(strGroupID, true);
            UpdateItemsList();
        }
        public void UnGroupSelected()
        {
            string strGroupID = string.Empty;
            for (int i = 0; i < SelectedtItems.Count; i++)
            {
                string strNameDesc;
                CG_ITEM_PROPS baseProps;
                m_objCG.GetItemBaseProps(SelectedtItems[i].ID, out strNameDesc, out baseProps);

                if ((baseProps.eType & eCG_ItemType.eCGIT_Group) != 0)
                {
                    m_objCG.GroupRemoveAll(SelectedtItems[i].ID, eCG_GroupItemsRemoveType.eCGRT_Ungroup);
                    m_objCG.RemoveItem(SelectedtItems[i].ID, 0);
                }
            }
	    SelectItem("", true);
            UpdateItemsList();
        }

        private bool isSameVideoProps(M_VID_PROPS vProps1, M_VID_PROPS vProps2)
        {
            if (vProps1.fccType == vProps2.fccType &&
                vProps1.nWidth == vProps2.nWidth &&
                vProps1.nHeight == vProps2.nHeight)
                return true;
            return false;
        }
    }

    public class CopyItem
    {
        public CopyItem(string id, string xml, double posX, double posY)
        {
            m_strID = id;
            m_strXML = xml;
            m_posX = posX;
            m_posY = posY;
        }

        private string m_strID;

        public string ID
        {
            get { return m_strID; }
            set { m_strID = value; }
        }

        private string m_strXML;
        public string XML
        {
            get { return m_strXML; }
            set { m_strXML = value; }
        }

        private double m_posX;
        public double PosX
        {
            get { return m_posX; }
            set { m_posX = value; }
        }

        private double m_posY;
        public double PosY
        {
            get { return m_posY; }
            set { m_posY = value; }
        }
    }

    public class HistoryItem
    {
        public HistoryItem(string state, string itemId)
        {
            m_strXMLState = state;
            m_strItemID = itemId;
        }

        private string m_strXMLState;

        public string XMLState
        {
            get { return m_strXMLState; }
            set { m_strXMLState = value; }
        }

        private string m_strItemID;

        public string ItemID
        {
            get { return m_strItemID; }
            set { m_strItemID = value; }
        }
    }

}
