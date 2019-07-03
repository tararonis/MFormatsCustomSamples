using System;
using System.Drawing;
using System.Windows.Forms;
using MFORMATSLib;

namespace SampleFilePlayback
{
    public partial class MAudioMeter : UserControl
    {
        int m_nCurrentTrack = 0;
        // The indent for first VU meter (calc from text)
        float fLeftIndent = 22.0f;
        float fRightIndent = 22.0f;

        public MAudioMeter()
        {
            InitializeComponent();
        }

        public MFFrame pFrame;
       
        Color colorLevelBack = Color.DarkGray;
        public Color ColorLevelBack
        {
            get { return colorLevelBack; }
            set { colorLevelBack = value; }
        }

        Color colorLevelOrg = Color.LightGray;
        public Color ColorLevelOrg
        {
            get { return colorLevelOrg; }
            set { colorLevelOrg = value; }
        }

        Color colorLevelHi = Color.Red;
        public Color ColorLevelHi
        {
            get { return colorLevelHi; }
            set { colorLevelHi = value; }
        }

        Color colorLevelMid = Color.Blue;
        public Color ColorLevelMid
        {
            get { return colorLevelMid; }
            set { colorLevelMid = value; }
        }

        Color colorLevelLo = Color.DarkBlue;
        public Color ColorLevelLo
        {
            get { return colorLevelLo; }
            set { colorLevelLo = value; }
        }

        Color colorOutline = Color.Black;
        public Color ColorOutline
        {
            get { return colorOutline; }
            set { colorOutline = value; }
        }

        Color colorGainSlider = Color.Red;
        public Color ColorGainSlider
        {
            get { return colorGainSlider; }
            set { colorGainSlider = value; }
        }

        Color backColorHi = Color.FromArgb(222, 232, 254);
        public Color BackColorHi
        {
            get { return backColorHi; }
            set { backColorHi = value; }
        }

        MAudioChannel[] arrChannels;
        public void UpdateControl()
        {
            if (pFrame != null)
            {
                try
                {
                    M_AV_PROPS avProps;
                    int samples;
                    pFrame.MFAVPropsGet(out avProps, out samples);
                    if (arrChannels != null)
                    {
                        for (int i = 0; i < arrChannels.Length; i++)
                        {
                            if (arrChannels[i] != null )
                                arrChannels[i].Dispose();
                        }
                    }

                    arrChannels = new MAudioChannel[avProps.audProps.nChannels];
                    for (int i = 0; i < avProps.audProps.nChannels; i++)
                    {
                        arrChannels[i] = new MAudioChannel();
                        arrChannels[i].Parent = this;
                        arrChannels[i].Visible = true;
                        arrChannels[i].ColorLevelBack = ColorLevelBack;
                        arrChannels[i].ColorLevelOrg = ColorLevelOrg;
                        arrChannels[i].ColorLevelHi = ColorLevelHi;
                        arrChannels[i].ColorLevelMid = ColorLevelMid;
                        arrChannels[i].ColorLevelLo = ColorLevelLo;
                        arrChannels[i].ColorOutline = ColorOutline;
                        arrChannels[i].ColorGainSlider = ColorGainSlider;
                        arrChannels[i].Risk = 6;

                        int nTop = 10;
                        

                        arrChannels[i].Location = new Point((int)(arrChannels[i].Bounds.Width * i + fLeftIndent), 
                            (int)(Font.Height / 2) + nTop );
                        arrChannels[i].Height = (int)(this.Height - Font.Height / 2.0 - nTop);
                        arrChannels[i].Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Bottom;
                    }

                    float fWidth = avProps.audProps.nChannels > 0 ? (int)(arrChannels[0].Bounds.Width * avProps.audProps.nChannels + fLeftIndent + fRightIndent)
                        : (int)(fLeftIndent + fRightIndent);

                    this.Width = (int)Math.Max(fWidth,0);


                    if (ChannelsUpdated != null)
                        ChannelsUpdated(null, null);
                    Invalidate();
                }
                catch (System.Exception ex)
                {

                }
            }
        }

        public event EventHandler ChannelsUpdated;
        public void SetValues()
        {
            if (pFrame != null)
            {
                try
                {
                    M_AV_PROPS avProps;
                    int samples;
                    pFrame.MFAVPropsGet(out avProps, out samples);
                    
                    if (avProps.audProps.nChannels != arrChannels.Length && avProps.audProps.nChannels>0)
                    {
                        UpdateControl();
                    }

                    for (int i = 0; i < arrChannels.Length; i++)
                    {
                        pFrame.MFAudioGain(i.ToString(), arrChannels[i].Gain, arrChannels[i].Gain);
                    }
                    pFrame.MFAVPropsGet(out avProps, out samples);

                    for (int i = 0; i < arrChannels.Length; i++)
                    {
                        arrChannels[i].Level = avProps.ancData.audOutput.arrVUMeter[i];
                        arrChannels[i].LevelOrg = avProps.ancData.audOriginal.arrVUMeter[i];
                        pFrame.MFAudioGain(i.ToString(), arrChannels[i].Gain, arrChannels[i].Gain);
                        arrChannels[i].Invalidate();
                    }
                }
                catch (System.Exception ex)
                {

                }
            }
        }
        
        private void MAudioMeter_Paint(object sender, PaintEventArgs e)
        {
            int nIndent = 0;

            float fMax = this.Bounds.Height - mAudioCh1.Width - Font.Height / 2.0f;

            string[] pLevel = new string[] { "0dB", "-10", "-20", "-30", "-60" };
            string[] pGain = new string[] { "+20", "+10", "0dB", "-10", "-20" };

            float fStep = fMax / (pLevel.Length - 1);

            RectangleF rcBG_Block = new RectangleF(0, Font.Height / 2.0f, Bounds.Width, fStep);
            Brush brBG_1 = new SolidBrush(this.BackColorHi);
            Brush brBG_2 = new SolidBrush(this.BackColor);


            Brush brText = new SolidBrush(this.ForeColor);

            StringFormat strFormat = new StringFormat();
            strFormat.Alignment = StringAlignment.Far;

            fLeftIndent = 0;
            fRightIndent = 0;
            RectangleF rcString = new RectangleF(0, 0, Bounds.Width, Font.Height);
            for (int i = 0; i < pLevel.Length; i++)
            {
                rcString.Y = nIndent + fStep * i;

                rcBG_Block.Y = nIndent + fStep * i + Font.Height / 2.0f;
                e.Graphics.FillRectangle(i % 2 == 0 ? brBG_1 : brBG_2, rcBG_Block);

                // For VU indent
                float fWidth = e.Graphics.MeasureString(pLevel[i], Font).Width;
                fLeftIndent = fLeftIndent < fWidth ? fWidth : fLeftIndent;

                fWidth = e.Graphics.MeasureString(pGain[i], Font).Width;
                fRightIndent = fRightIndent < fWidth ? fWidth : fRightIndent;

                e.Graphics.DrawString(pLevel[i], this.Font, brText, rcString);
                e.Graphics.DrawString(pGain[i], this.Font, brText, rcString, strFormat);
            }


        }

        private void MAudioMeter_Resize(object sender, EventArgs e)
        {
            this.Refresh();
        }

       
    }
}
