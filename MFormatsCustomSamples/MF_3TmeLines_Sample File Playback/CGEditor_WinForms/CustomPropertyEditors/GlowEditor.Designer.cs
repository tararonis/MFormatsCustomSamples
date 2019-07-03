namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class GlowEditor
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
            this.SpinnerGlowSizeY = new System.Windows.Forms.NumericUpDown();
            this.SpinnerGlowSizeX = new System.Windows.Forms.NumericUpDown();
            this.ToggleGlow = new System.Windows.Forms.CheckBox();
            this.SpinnerGlowVlue = new System.Windows.Forms.NumericUpDown();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGlowSizeY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGlowSizeX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGlowVlue)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            this.SuspendLayout();
            // 
            // SpinnerGlowSizeY
            // 
            this.SpinnerGlowSizeY.Location = new System.Drawing.Point(145, 40);
            this.SpinnerGlowSizeY.Name = "SpinnerGlowSizeY";
            this.SpinnerGlowSizeY.Size = new System.Drawing.Size(72, 26);
            this.SpinnerGlowSizeY.TabIndex = 13;
            this.SpinnerGlowSizeY.ValueChanged += new System.EventHandler(this.SpinnerGlowSizeY_ValueChanged);
            // 
            // SpinnerGlowSizeX
            // 
            this.SpinnerGlowSizeX.Location = new System.Drawing.Point(145, 6);
            this.SpinnerGlowSizeX.Name = "SpinnerGlowSizeX";
            this.SpinnerGlowSizeX.Size = new System.Drawing.Size(71, 26);
            this.SpinnerGlowSizeX.TabIndex = 12;
            this.SpinnerGlowSizeX.ValueChanged += new System.EventHandler(this.SpinnerGlowSizeX_ValueChanged);
            // 
            // ToggleGlow
            // 
            this.ToggleGlow.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleGlow.AutoSize = true;
            this.ToggleGlow.Location = new System.Drawing.Point(3, 3);
            this.ToggleGlow.MinimumSize = new System.Drawing.Size(100, 0);
            this.ToggleGlow.Name = "ToggleGlow";
            this.ToggleGlow.Size = new System.Drawing.Size(100, 30);
            this.ToggleGlow.TabIndex = 7;
            this.ToggleGlow.Text = "Glow";
            this.ToggleGlow.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.ToggleGlow.UseVisualStyleBackColor = true;
            this.ToggleGlow.CheckedChanged += new System.EventHandler(this.ToggleGlow_CheckedChanged);
            // 
            // SpinnerGlowVlue
            // 
            this.SpinnerGlowVlue.Location = new System.Drawing.Point(39, 39);
            this.SpinnerGlowVlue.Name = "SpinnerGlowVlue";
            this.SpinnerGlowVlue.Size = new System.Drawing.Size(64, 26);
            this.SpinnerGlowVlue.TabIndex = 14;
            this.SpinnerGlowVlue.ValueChanged += new System.EventHandler(this.SpinnerGlowVlue_ValueChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Width;
            this.pictureBox1.Location = new System.Drawing.Point(109, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 15;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Height;
            this.pictureBox2.Location = new System.Drawing.Point(109, 39);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 16;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.Glow;
            this.pictureBox3.Location = new System.Drawing.Point(3, 39);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 17;
            this.pictureBox3.TabStop = false;
            // 
            // GlowEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.SpinnerGlowVlue);
            this.Controls.Add(this.SpinnerGlowSizeY);
            this.Controls.Add(this.SpinnerGlowSizeX);
            this.Controls.Add(this.ToggleGlow);
            this.Name = "GlowEditor";
            this.Size = new System.Drawing.Size(223, 72);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGlowSizeY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGlowSizeX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGlowVlue)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown SpinnerGlowSizeY;
        private System.Windows.Forms.NumericUpDown SpinnerGlowSizeX;
        private System.Windows.Forms.CheckBox ToggleGlow;
        private System.Windows.Forms.NumericUpDown SpinnerGlowVlue;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
    }
}
