using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using MFORMATSLib;

namespace SampleLiveSyncPlayback
{
    public partial class LiveControl : Form
    {
        private MFLiveClass m_objLive;

        public LiveControl(ref MFLiveClass _objLive)
        {
            InitializeComponent();
            m_objLive = _objLive;
        }
        public string GetName()
        {
            string name;
            int idx;
            m_objLive.DeviceGet(eMFDeviceType.eMFDT_Video, out idx, out name);
            return name;
        }

        private void LiveControl_Load(object sender, EventArgs e)
        {
            //Fill live devices
            FillMLiveDevices();
        }

        private void FillMLiveDevices()
        {
            comboBoxVideo.Items.Clear();
            //Fill video devices
            int vCount;
            try
            {
                m_objLive.DeviceGetCount(eMFDeviceType.eMFDT_Video, out vCount);

                if (vCount > 0)
                {
                    for (int i = 0; i < vCount; i++)
                    {
                        string strName;
                        int _pbBusy;
                        m_objLive.DeviceGetByIndex(eMFDeviceType.eMFDT_Video, i, out strName, out _pbBusy);
                        comboBoxVideo.Items.Add(strName);

                    }
                    comboBoxVideo.SelectedIndex = 0;

                    FillCombo(eMFDeviceType.eMFDT_ExtAudio, comboBoxAudioEx);
                    FillCombo(eMFDeviceType.eMFDT_Audio, comboBoxAudio);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
        private void FillCombo(eMFDeviceType strType, ComboBox cbxType)
        {
            cbxType.Items.Clear();
            int nCount = 0;
            //Get device count / input line count
            m_objLive.DeviceGetCount(strType, out nCount);
            cbxType.Enabled = nCount > 0;
            if (nCount > 0)
            {
                for (int i = 0; i < nCount; i++)
                {
                    string strName;
                    int pbBusy;
                    //Get deveice / input line
                    m_objLive.DeviceGetByIndex(strType, i, out strName, out pbBusy);
                    cbxType.Items.Add(strName);
                }
                string name = "";
                int nIndex = 0;
                try
                {
                    //Check if there is already selected device / input line
                    m_objLive.DeviceGet(strType, out nIndex, out name);
                    cbxType.SelectedIndex = nIndex >= 0 && nIndex < cbxType.Items.Count - 1 ? nIndex : 0;
                }
                catch
                {
                    if (strType == eMFDeviceType.eMFDT_Audio)
                    {
                        cbxType.SelectedIndex = cbxType.Items.Count > 1 ? cbxType.Items.Count - 2 : 0;
                    }
                    else
                    {
                        cbxType.SelectedIndex = 0;
                    }
                }
            }
        }

        private void comboBoxVideo_SelectedIndexChanged(object sender, EventArgs e)
        {
            comboBoxFormat.Items.Clear();
            comboBoxVideoLine.Items.Clear();

            M_VID_PROPS vidProps;

            try
            {

                m_objLive.DeviceSet(eMFDeviceType.eMFDT_Video, comboBoxVideo.SelectedIndex, "");

                int fCount;
                string name;

                m_objLive.FormatVideoGetCount(eMFormatType.eMFT_Input, out fCount);
                if (fCount > 0)
                {
                    for (int i = 0; i < fCount; i++)
                    {
                        m_objLive.FormatVideoGetByIndex(eMFormatType.eMFT_Input, i, out vidProps, out name);
                        comboBoxFormat.Items.Add(name);
                    }

                    // Get current format
                    int nCurrent = 0;
                    try
                    {
                        string strName;
                        M_VID_PROPS _vidProps;
                        m_objLive.FormatVideoGet(eMFormatType.eMFT_Input, out _vidProps, out nCurrent, out strName);
                    }
                    catch (System.Exception)
                    {
                    }

                    // Select current
                    comboBoxFormat.SelectedIndex = nCurrent > 0 ? nCurrent : 0;
                }

                CheckLineIn();

                FillCombo(eMFDeviceType.eMFDT_Audio, comboBoxAudio);
                FillCombo(eMFDeviceType.eMFDT_ExtAudio, comboBoxAudioEx);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        void CheckLineIn()
        {
            comboBoxVideoLine.Items.Clear();

            string strCurrent;
            string _pOptions;
            string _pHelp;
            int nSel = -1;
            int lCount;
            m_objLive.PropsGet("line-in", out strCurrent);

            m_objLive.PropsOptionGetCount("line-in", out lCount);
            if (lCount > 0)
            {
                for (int i = 0; i < lCount; i++)
                {
                    m_objLive.PropsOptionGetByIndex("line-in", i, out _pOptions, out _pHelp);
                    comboBoxVideoLine.Items.Add(_pHelp);

                    if (strCurrent == _pOptions)
                        nSel = i;
                }

                if (nSel >= 0)
                    comboBoxVideoLine.SelectedIndex = nSel;
                else
                    comboBoxVideoLine.SelectedIndex = 0;
            }
        }

        private void comboBoxVideoLine_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_objLive.PropsOptionSetByIndex("line-in", comboBoxVideoLine.SelectedIndex);
        }

        private void buttonUpdateLineIn_Click(object sender, EventArgs e)
        {
            CheckLineIn();
        }

        private void comboBoxFormat_SelectedIndexChanged(object sender, EventArgs e)
        {
            M_VID_PROPS vidProps;
            string name;
            m_objLive.FormatVideoGetByIndex(eMFormatType.eMFT_Input, comboBoxFormat.SelectedIndex, out vidProps, out name);

            m_objLive.FormatVideoSet(eMFormatType.eMFT_Input, vidProps);
        }

        private void comboBoxAudio_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                m_objLive.DeviceSet(eMFDeviceType.eMFDT_Audio, comboBoxAudio.SelectedIndex, "");
            }
            catch { }
        }

        private void comboBoxAudioEx_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                m_objLive.DeviceSet(eMFDeviceType.eMFDT_ExtAudio, comboBoxAudioEx.SelectedIndex, "");
            }
            catch { }
        }
    }
}
