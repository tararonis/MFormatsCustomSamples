namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class ShadowEditor
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
            this.ToggleShadow = new System.Windows.Forms.CheckBox();
            this.SliderShadowAlpha = new System.Windows.Forms.TrackBar();
            this.SpinnerOffsetX = new System.Windows.Forms.NumericUpDown();
            this.SpinnerShadowBlur = new System.Windows.Forms.NumericUpDown();
            this.CPShadowColor = new System.Windows.Forms.Button();
            this.SpinnerOffsetY = new System.Windows.Forms.NumericUpDown();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox5 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SliderShadowAlpha)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerOffsetX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerShadowBlur)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerOffsetY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).BeginInit();
            this.SuspendLayout();
            // 
            // ToggleShadow
            // 
            this.ToggleShadow.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleShadow.AutoSize = true;
            this.ToggleShadow.Location = new System.Drawing.Point(3, 3);
            this.ToggleShadow.MinimumSize = new System.Drawing.Size(100, 40);
            this.ToggleShadow.Name = "ToggleShadow";
            this.ToggleShadow.Size = new System.Drawing.Size(100, 40);
            this.ToggleShadow.TabIndex = 0;
            this.ToggleShadow.Text = "Shadow";
            this.ToggleShadow.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.ToggleShadow.UseVisualStyleBackColor = true;
            this.ToggleShadow.CheckedChanged += new System.EventHandler(this.ToggleShadow_CheckedChanged);
            // 
            // SliderShadowAlpha
            // 
            this.SliderShadowAlpha.Location = new System.Drawing.Point(145, 12);
            this.SliderShadowAlpha.Maximum = 255;
            this.SliderShadowAlpha.Name = "SliderShadowAlpha";
            this.SliderShadowAlpha.Size = new System.Drawing.Size(154, 69);
            this.SliderShadowAlpha.TabIndex = 1;
            this.SliderShadowAlpha.TickStyle = System.Windows.Forms.TickStyle.None;
            this.SliderShadowAlpha.Scroll += new System.EventHandler(this.SliderShadowAlpha_Scroll);
            // 
            // SpinnerOffsetX
            // 
            this.SpinnerOffsetX.Location = new System.Drawing.Point(341, 16);
            this.SpinnerOffsetX.Name = "SpinnerOffsetX";
            this.SpinnerOffsetX.Size = new System.Drawing.Size(67, 26);
            this.SpinnerOffsetX.TabIndex = 4;
            this.SpinnerOffsetX.ValueChanged += new System.EventHandler(this.SpinnerOffsetX_ValueChanged);
            // 
            // SpinnerShadowBlur
            // 
            this.SpinnerShadowBlur.DecimalPlaces = 1;
            this.SpinnerShadowBlur.Location = new System.Drawing.Point(39, 49);
            this.SpinnerShadowBlur.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.SpinnerShadowBlur.Name = "SpinnerShadowBlur";
            this.SpinnerShadowBlur.Size = new System.Drawing.Size(64, 26);
            this.SpinnerShadowBlur.TabIndex = 6;
            this.SpinnerShadowBlur.ValueChanged += new System.EventHandler(this.SpinnerShadowBlur_ValueChanged);
            // 
            // CPShadowColor
            // 
            this.CPShadowColor.Location = new System.Drawing.Point(181, 46);
            this.CPShadowColor.Name = "CPShadowColor";
            this.CPShadowColor.Size = new System.Drawing.Size(81, 30);
            this.CPShadowColor.TabIndex = 8;
            this.CPShadowColor.UseVisualStyleBackColor = true;
            this.CPShadowColor.Click += new System.EventHandler(this.CPShadowColor_Click);
            // 
            // SpinnerOffsetY
            // 
            this.SpinnerOffsetY.Location = new System.Drawing.Point(341, 48);
            this.SpinnerOffsetY.Name = "SpinnerOffsetY";
            this.SpinnerOffsetY.Size = new System.Drawing.Size(67, 26);
            this.SpinnerOffsetY.TabIndex = 10;
            this.SpinnerOffsetY.ValueChanged += new System.EventHandler(this.SpinnerOffsetY_ValueChanged);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.ShadowBlur;
            this.pictureBox2.Location = new System.Drawing.Point(3, 49);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 12;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Alpha;
            this.pictureBox1.Location = new System.Drawing.Point(109, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 11;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.Color;
            this.pictureBox3.Location = new System.Drawing.Point(109, 48);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 13;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = global::CGEditor_WinForms.Properties.Resources.XOffset;
            this.pictureBox4.Location = new System.Drawing.Point(305, 12);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(30, 30);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 14;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox5
            // 
            this.pictureBox5.Image = global::CGEditor_WinForms.Properties.Resources.YOffset;
            this.pictureBox5.Location = new System.Drawing.Point(305, 48);
            this.pictureBox5.Name = "pictureBox5";
            this.pictureBox5.Size = new System.Drawing.Size(30, 30);
            this.pictureBox5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox5.TabIndex = 15;
            this.pictureBox5.TabStop = false;
            // 
            // ShadowEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox5);
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.SpinnerOffsetY);
            this.Controls.Add(this.CPShadowColor);
            this.Controls.Add(this.SpinnerShadowBlur);
            this.Controls.Add(this.SpinnerOffsetX);
            this.Controls.Add(this.SliderShadowAlpha);
            this.Controls.Add(this.ToggleShadow);
            this.Name = "ShadowEditor";
            this.Size = new System.Drawing.Size(423, 81);
            ((System.ComponentModel.ISupportInitialize)(this.SliderShadowAlpha)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerOffsetX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerShadowBlur)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerOffsetY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox ToggleShadow;
        private System.Windows.Forms.TrackBar SliderShadowAlpha;
        private System.Windows.Forms.NumericUpDown SpinnerOffsetX;
        private System.Windows.Forms.NumericUpDown SpinnerShadowBlur;
        private System.Windows.Forms.Button CPShadowColor;
        private System.Windows.Forms.NumericUpDown SpinnerOffsetY;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox5;
    }
}
