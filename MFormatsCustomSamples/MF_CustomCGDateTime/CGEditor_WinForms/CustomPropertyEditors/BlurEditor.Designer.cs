namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class BlurEditor
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
            this.ToggleBlur = new System.Windows.Forms.CheckBox();
            this.ComboBlurAlign = new System.Windows.Forms.ComboBox();
            this.SpinnerBlurSizeX = new System.Windows.Forms.NumericUpDown();
            this.SpinnerBlurSizeY = new System.Windows.Forms.NumericUpDown();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerBlurSizeX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerBlurSizeY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            this.SuspendLayout();
            // 
            // ToggleBlur
            // 
            this.ToggleBlur.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleBlur.AutoSize = true;
            this.ToggleBlur.Location = new System.Drawing.Point(3, 3);
            this.ToggleBlur.MinimumSize = new System.Drawing.Size(100, 0);
            this.ToggleBlur.Name = "ToggleBlur";
            this.ToggleBlur.Size = new System.Drawing.Size(100, 30);
            this.ToggleBlur.TabIndex = 0;
            this.ToggleBlur.Text = "Blur";
            this.ToggleBlur.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.ToggleBlur.UseVisualStyleBackColor = true;
            this.ToggleBlur.CheckedChanged += new System.EventHandler(this.ToggleBlur_CheckedChanged);
            // 
            // ComboBlurAlign
            // 
            this.ComboBlurAlign.FormattingEnabled = true;
            this.ComboBlurAlign.Location = new System.Drawing.Point(39, 39);
            this.ComboBlurAlign.Name = "ComboBlurAlign";
            this.ComboBlurAlign.Size = new System.Drawing.Size(64, 28);
            this.ComboBlurAlign.TabIndex = 2;
            this.ComboBlurAlign.SelectedIndexChanged += new System.EventHandler(this.ComboBlurAlign_SelectedIndexChanged);
            // 
            // SpinnerBlurSizeX
            // 
            this.SpinnerBlurSizeX.Location = new System.Drawing.Point(145, 6);
            this.SpinnerBlurSizeX.Name = "SpinnerBlurSizeX";
            this.SpinnerBlurSizeX.Size = new System.Drawing.Size(71, 26);
            this.SpinnerBlurSizeX.TabIndex = 5;
            this.SpinnerBlurSizeX.ValueChanged += new System.EventHandler(this.SpinnerBlurSizeX_ValueChanged);
            // 
            // SpinnerBlurSizeY
            // 
            this.SpinnerBlurSizeY.Location = new System.Drawing.Point(144, 38);
            this.SpinnerBlurSizeY.Name = "SpinnerBlurSizeY";
            this.SpinnerBlurSizeY.Size = new System.Drawing.Size(72, 26);
            this.SpinnerBlurSizeY.TabIndex = 6;
            this.SpinnerBlurSizeY.ValueChanged += new System.EventHandler(this.SpinnerBlurSizeY_ValueChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Width;
            this.pictureBox1.Location = new System.Drawing.Point(109, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 16;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Height;
            this.pictureBox2.Location = new System.Drawing.Point(109, 39);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 17;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.Glow;
            this.pictureBox3.Location = new System.Drawing.Point(3, 39);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 18;
            this.pictureBox3.TabStop = false;
            // 
            // BlurEditor
            // 
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.SpinnerBlurSizeY);
            this.Controls.Add(this.SpinnerBlurSizeX);
            this.Controls.Add(this.ComboBlurAlign);
            this.Controls.Add(this.ToggleBlur);
            this.Name = "BlurEditor";
            this.Size = new System.Drawing.Size(222, 70);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerBlurSizeX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerBlurSizeY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox ToggleBlur;
        private System.Windows.Forms.ComboBox ComboBlurAlign;
        private System.Windows.Forms.NumericUpDown SpinnerBlurSizeX;
        private System.Windows.Forms.NumericUpDown SpinnerBlurSizeY;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;



        
    }
}
