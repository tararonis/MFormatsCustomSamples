namespace SampleLiveSyncPlayback
{
    partial class LiveControl
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonUpdateLineIn = new System.Windows.Forms.Button();
            this.comboBoxAudioEx = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.comboBoxAudio = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxFormat = new System.Windows.Forms.ComboBox();
            this.comboBoxVideo = new System.Windows.Forms.ComboBox();
            this.comboBoxVideoLine = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonUpdateLineIn
            // 
            this.buttonUpdateLineIn.Location = new System.Drawing.Point(235, 41);
            this.buttonUpdateLineIn.Name = "buttonUpdateLineIn";
            this.buttonUpdateLineIn.Size = new System.Drawing.Size(20, 23);
            this.buttonUpdateLineIn.TabIndex = 233;
            this.buttonUpdateLineIn.Text = "R";
            this.buttonUpdateLineIn.UseVisualStyleBackColor = true;
            this.buttonUpdateLineIn.Click += new System.EventHandler(this.buttonUpdateLineIn_Click);
            // 
            // comboBoxAudioEx
            // 
            this.comboBoxAudioEx.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAudioEx.FormattingEnabled = true;
            this.comboBoxAudioEx.Location = new System.Drawing.Point(71, 164);
            this.comboBoxAudioEx.Name = "comboBoxAudioEx";
            this.comboBoxAudioEx.Size = new System.Drawing.Size(184, 21);
            this.comboBoxAudioEx.TabIndex = 232;
            this.comboBoxAudioEx.SelectedIndexChanged += new System.EventHandler(this.comboBoxAudioEx_SelectedIndexChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(11, 168);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(45, 13);
            this.label9.TabIndex = 231;
            this.label9.Text = "External";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(11, 109);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(69, 13);
            this.label8.TabIndex = 230;
            this.label8.Text = "Audio device";
            // 
            // comboBoxAudio
            // 
            this.comboBoxAudio.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAudio.FormattingEnabled = true;
            this.comboBoxAudio.Location = new System.Drawing.Point(71, 133);
            this.comboBoxAudio.Name = "comboBoxAudio";
            this.comboBoxAudio.Size = new System.Drawing.Size(184, 21);
            this.comboBoxAudio.TabIndex = 229;
            this.comboBoxAudio.SelectedIndexChanged += new System.EventHandler(this.comboBoxAudio_SelectedIndexChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(11, 137);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 13);
            this.label2.TabIndex = 228;
            this.label2.Text = "Internal";
            // 
            // comboBoxFormat
            // 
            this.comboBoxFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxFormat.FormattingEnabled = true;
            this.comboBoxFormat.Location = new System.Drawing.Point(71, 72);
            this.comboBoxFormat.Name = "comboBoxFormat";
            this.comboBoxFormat.Size = new System.Drawing.Size(184, 21);
            this.comboBoxFormat.TabIndex = 225;
            this.comboBoxFormat.SelectedIndexChanged += new System.EventHandler(this.comboBoxFormat_SelectedIndexChanged);
            // 
            // comboBoxVideo
            // 
            this.comboBoxVideo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxVideo.FormattingEnabled = true;
            this.comboBoxVideo.Location = new System.Drawing.Point(71, 12);
            this.comboBoxVideo.Name = "comboBoxVideo";
            this.comboBoxVideo.Size = new System.Drawing.Size(184, 21);
            this.comboBoxVideo.TabIndex = 223;
            this.comboBoxVideo.SelectedIndexChanged += new System.EventHandler(this.comboBoxVideo_SelectedIndexChanged);
            // 
            // comboBoxVideoLine
            // 
            this.comboBoxVideoLine.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxVideoLine.FormattingEnabled = true;
            this.comboBoxVideoLine.Location = new System.Drawing.Point(71, 42);
            this.comboBoxVideoLine.Name = "comboBoxVideoLine";
            this.comboBoxVideoLine.Size = new System.Drawing.Size(158, 21);
            this.comboBoxVideoLine.TabIndex = 224;
            this.comboBoxVideoLine.SelectedIndexChanged += new System.EventHandler(this.comboBoxVideoLine_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(10, 17);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 220;
            this.label4.Text = "Video:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(10, 42);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(30, 13);
            this.label5.TabIndex = 221;
            this.label5.Text = "Line:";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(10, 72);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(42, 13);
            this.label6.TabIndex = 222;
            this.label6.Text = "Format:";
            // 
            // LiveControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(266, 197);
            this.Controls.Add(this.buttonUpdateLineIn);
            this.Controls.Add(this.comboBoxAudioEx);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.comboBoxAudio);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.comboBoxFormat);
            this.Controls.Add(this.comboBoxVideo);
            this.Controls.Add(this.comboBoxVideoLine);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label6);
            this.Name = "LiveControl";
            this.Text = "LiveControl";
            this.Load += new System.EventHandler(this.LiveControl_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonUpdateLineIn;
        private System.Windows.Forms.ComboBox comboBoxAudioEx;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ComboBox comboBoxAudio;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox comboBoxFormat;
        private System.Windows.Forms.ComboBox comboBoxVideo;
        private System.Windows.Forms.ComboBox comboBoxVideoLine;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label6;
    }
}