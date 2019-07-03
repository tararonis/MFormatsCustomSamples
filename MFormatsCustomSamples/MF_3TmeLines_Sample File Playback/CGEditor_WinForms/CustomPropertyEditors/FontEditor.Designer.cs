namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class FontEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FontEditor));
            this.label1 = new System.Windows.Forms.Label();
            this.ComboFontFamily = new System.Windows.Forms.ComboBox();
            this.SpinnerFontSize = new System.Windows.Forms.NumericUpDown();
            this.ComboFontTypeface = new System.Windows.Forms.ComboBox();
            this.ToggleUnderline = new System.Windows.Forms.CheckBox();
            this.ToggleStrikeout = new System.Windows.Forms.CheckBox();
            this.ToggleWordWrap = new System.Windows.Forms.CheckBox();
            this.ToggleVertical = new System.Windows.Forms.CheckBox();
            this.ToggleRightToLeft = new System.Windows.Forms.CheckBox();
            this.ToggleNoTabs = new System.Windows.Forms.CheckBox();
            this.ComboFlip = new System.Windows.Forms.ComboBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerFontSize)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 6);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(42, 20);
            this.label1.TabIndex = 0;
            this.label1.Text = "Font";
            // 
            // ComboFontFamily
            // 
            this.ComboFontFamily.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboFontFamily.FormattingEnabled = true;
            this.ComboFontFamily.Location = new System.Drawing.Point(51, 3);
            this.ComboFontFamily.Name = "ComboFontFamily";
            this.ComboFontFamily.Size = new System.Drawing.Size(190, 28);
            this.ComboFontFamily.TabIndex = 1;
            this.ComboFontFamily.SelectedIndexChanged += new System.EventHandler(this.ComboFontFamily_SelectedIndexChanged);
            // 
            // SpinnerFontSize
            // 
            this.SpinnerFontSize.Location = new System.Drawing.Point(247, 4);
            this.SpinnerFontSize.Maximum = new decimal(new int[] {
            999,
            0,
            0,
            0});
            this.SpinnerFontSize.Name = "SpinnerFontSize";
            this.SpinnerFontSize.Size = new System.Drawing.Size(65, 26);
            this.SpinnerFontSize.TabIndex = 2;
            this.SpinnerFontSize.ValueChanged += new System.EventHandler(this.SpinnerFontSize_ValueChanged);
            // 
            // ComboFontTypeface
            // 
            this.ComboFontTypeface.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboFontTypeface.FormattingEnabled = true;
            this.ComboFontTypeface.Location = new System.Drawing.Point(318, 3);
            this.ComboFontTypeface.Name = "ComboFontTypeface";
            this.ComboFontTypeface.Size = new System.Drawing.Size(138, 28);
            this.ComboFontTypeface.TabIndex = 3;
            this.ComboFontTypeface.SelectedIndexChanged += new System.EventHandler(this.ComboFontTypeface_SelectedIndexChanged);
            // 
            // ToggleUnderline
            // 
            this.ToggleUnderline.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleUnderline.AutoSize = true;
            this.ToggleUnderline.Image = ((System.Drawing.Image)(resources.GetObject("ToggleUnderline.Image")));
            this.ToggleUnderline.Location = new System.Drawing.Point(7, 37);
            this.ToggleUnderline.MinimumSize = new System.Drawing.Size(34, 34);
            this.ToggleUnderline.Name = "ToggleUnderline";
            this.ToggleUnderline.Size = new System.Drawing.Size(34, 34);
            this.ToggleUnderline.TabIndex = 4;
            this.ToggleUnderline.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.ToggleUnderline.UseVisualStyleBackColor = true;
            this.ToggleUnderline.CheckedChanged += new System.EventHandler(this.ToggleUnderline_CheckedChanged);
            // 
            // ToggleStrikeout
            // 
            this.ToggleStrikeout.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleStrikeout.AutoSize = true;
            this.ToggleStrikeout.Image = ((System.Drawing.Image)(resources.GetObject("ToggleStrikeout.Image")));
            this.ToggleStrikeout.Location = new System.Drawing.Point(47, 37);
            this.ToggleStrikeout.MinimumSize = new System.Drawing.Size(34, 34);
            this.ToggleStrikeout.Name = "ToggleStrikeout";
            this.ToggleStrikeout.Size = new System.Drawing.Size(34, 34);
            this.ToggleStrikeout.TabIndex = 5;
            this.ToggleStrikeout.UseVisualStyleBackColor = true;
            this.ToggleStrikeout.CheckedChanged += new System.EventHandler(this.ToggleStrikeout_CheckedChanged);
            // 
            // ToggleWordWrap
            // 
            this.ToggleWordWrap.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleWordWrap.AutoSize = true;
            this.ToggleWordWrap.Image = ((System.Drawing.Image)(resources.GetObject("ToggleWordWrap.Image")));
            this.ToggleWordWrap.Location = new System.Drawing.Point(87, 37);
            this.ToggleWordWrap.MinimumSize = new System.Drawing.Size(34, 34);
            this.ToggleWordWrap.Name = "ToggleWordWrap";
            this.ToggleWordWrap.Size = new System.Drawing.Size(34, 34);
            this.ToggleWordWrap.TabIndex = 6;
            this.ToggleWordWrap.UseVisualStyleBackColor = true;
            this.ToggleWordWrap.CheckedChanged += new System.EventHandler(this.ToggleWordWrap_CheckedChanged);
            // 
            // ToggleVertical
            // 
            this.ToggleVertical.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleVertical.AutoSize = true;
            this.ToggleVertical.Image = ((System.Drawing.Image)(resources.GetObject("ToggleVertical.Image")));
            this.ToggleVertical.Location = new System.Drawing.Point(127, 37);
            this.ToggleVertical.MinimumSize = new System.Drawing.Size(34, 34);
            this.ToggleVertical.Name = "ToggleVertical";
            this.ToggleVertical.Size = new System.Drawing.Size(34, 34);
            this.ToggleVertical.TabIndex = 7;
            this.ToggleVertical.UseVisualStyleBackColor = true;
            this.ToggleVertical.CheckedChanged += new System.EventHandler(this.ToggleVertical_CheckedChanged);
            // 
            // ToggleRightToLeft
            // 
            this.ToggleRightToLeft.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleRightToLeft.AutoSize = true;
            this.ToggleRightToLeft.Image = ((System.Drawing.Image)(resources.GetObject("ToggleRightToLeft.Image")));
            this.ToggleRightToLeft.Location = new System.Drawing.Point(167, 37);
            this.ToggleRightToLeft.MinimumSize = new System.Drawing.Size(34, 34);
            this.ToggleRightToLeft.Name = "ToggleRightToLeft";
            this.ToggleRightToLeft.Size = new System.Drawing.Size(34, 34);
            this.ToggleRightToLeft.TabIndex = 8;
            this.ToggleRightToLeft.UseVisualStyleBackColor = true;
            this.ToggleRightToLeft.CheckedChanged += new System.EventHandler(this.ToggleRightToLeft_CheckedChanged);
            // 
            // ToggleNoTabs
            // 
            this.ToggleNoTabs.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleNoTabs.AutoSize = true;
            this.ToggleNoTabs.Image = ((System.Drawing.Image)(resources.GetObject("ToggleNoTabs.Image")));
            this.ToggleNoTabs.Location = new System.Drawing.Point(207, 37);
            this.ToggleNoTabs.MinimumSize = new System.Drawing.Size(34, 34);
            this.ToggleNoTabs.Name = "ToggleNoTabs";
            this.ToggleNoTabs.Size = new System.Drawing.Size(34, 34);
            this.ToggleNoTabs.TabIndex = 9;
            this.ToggleNoTabs.UseVisualStyleBackColor = true;
            this.ToggleNoTabs.CheckedChanged += new System.EventHandler(this.ToggleNoTabs_CheckedChanged);
            // 
            // ComboFlip
            // 
            this.ComboFlip.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboFlip.FormattingEnabled = true;
            this.ComboFlip.Location = new System.Drawing.Point(318, 37);
            this.ComboFlip.Name = "ComboFlip";
            this.ComboFlip.Size = new System.Drawing.Size(138, 28);
            this.ComboFlip.TabIndex = 11;
            this.ComboFlip.SelectedIndexChanged += new System.EventHandler(this.ComboFlip_SelectedIndexChanged);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Flip;
            this.pictureBox2.Location = new System.Drawing.Point(272, 37);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(40, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 14;
            this.pictureBox2.TabStop = false;
            // 
            // FontEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.ComboFlip);
            this.Controls.Add(this.ToggleNoTabs);
            this.Controls.Add(this.ToggleRightToLeft);
            this.Controls.Add(this.ToggleVertical);
            this.Controls.Add(this.ToggleWordWrap);
            this.Controls.Add(this.ToggleStrikeout);
            this.Controls.Add(this.ToggleUnderline);
            this.Controls.Add(this.ComboFontTypeface);
            this.Controls.Add(this.SpinnerFontSize);
            this.Controls.Add(this.ComboFontFamily);
            this.Controls.Add(this.label1);
            this.Name = "FontEditor";
            this.Size = new System.Drawing.Size(463, 75);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerFontSize)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox ComboFontFamily;
        private System.Windows.Forms.NumericUpDown SpinnerFontSize;
        private System.Windows.Forms.ComboBox ComboFontTypeface;
        private System.Windows.Forms.CheckBox ToggleUnderline;
        private System.Windows.Forms.CheckBox ToggleStrikeout;
        private System.Windows.Forms.CheckBox ToggleWordWrap;
        private System.Windows.Forms.CheckBox ToggleVertical;
        private System.Windows.Forms.CheckBox ToggleRightToLeft;
        private System.Windows.Forms.CheckBox ToggleNoTabs;
        private System.Windows.Forms.ComboBox ComboFlip;
        private System.Windows.Forms.PictureBox pictureBox2;
    }
}
