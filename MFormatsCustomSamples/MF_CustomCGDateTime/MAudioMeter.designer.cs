namespace SampleFilePlayback
{
    partial class MAudioMeter
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.timerUpdate = new System.Windows.Forms.Timer(this.components);
            this.mAudioCh1 = new MAudioChannel();
            this.SuspendLayout();
            // 
            // mAudioCh1
            // 
            this.mAudioCh1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.mAudioCh1.BackColor = System.Drawing.SystemColors.Control;
            
            this.mAudioCh1.ColorGainSlider = System.Drawing.Color.Red;
            this.mAudioCh1.ColorLevelBack = System.Drawing.Color.DarkGray;
            this.mAudioCh1.ColorLevelHi = System.Drawing.Color.Red;
            this.mAudioCh1.ColorLevelLo = System.Drawing.Color.Green;
            this.mAudioCh1.ColorLevelMid = System.Drawing.Color.Yellow;
            this.mAudioCh1.ColorLevelOrg = System.Drawing.Color.Silver;
            this.mAudioCh1.ColorOutline = System.Drawing.Color.Black;
            this.mAudioCh1.Font = new System.Drawing.Font("Microsoft Sans Serif", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.mAudioCh1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(100)))), ((int)(((byte)(180)))));
            
            this.mAudioCh1.Level = -20D;
            this.mAudioCh1.LevelOrg = -10D;
            this.mAudioCh1.Location = new System.Drawing.Point(31, 3);
            this.mAudioCh1.Name = "mAudioCh1";
            this.mAudioCh1.Outline = 0F;
            this.mAudioCh1.Risk = 1F;
            this.mAudioCh1.Size = new System.Drawing.Size(13, 297);
            this.mAudioCh1.TabIndex = 0;
            this.mAudioCh1.TextLeft = null;
            this.mAudioCh1.TextRight = null;
            this.mAudioCh1.Visible = false;
            // 
            // MAudioMeter
            // 
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(237)))), ((int)(((byte)(242)))), ((int)(((byte)(255)))));
            this.Controls.Add(this.mAudioCh1);
            this.Name = "MAudioMeter";
            this.Size = new System.Drawing.Size(74, 300);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.MAudioMeter_Paint);
            this.Resize += new System.EventHandler(this.MAudioMeter_Resize);
            this.ResumeLayout(false);

        }

        #endregion

        private MAudioChannel mAudioCh1;
        private System.Windows.Forms.Timer timerUpdate;
    }
}
