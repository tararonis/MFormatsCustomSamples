namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class SimpleColorEditor
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
            this.SliderAlpha = new System.Windows.Forms.TrackBar();
            this.CPBackground = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SliderAlpha)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // SliderAlpha
            // 
            this.SliderAlpha.Location = new System.Drawing.Point(39, 3);
            this.SliderAlpha.Maximum = 255;
            this.SliderAlpha.Name = "SliderAlpha";
            this.SliderAlpha.Size = new System.Drawing.Size(162, 69);
            this.SliderAlpha.TabIndex = 2;
            this.SliderAlpha.TickStyle = System.Windows.Forms.TickStyle.None;
            this.SliderAlpha.Scroll += new System.EventHandler(this.SliderAlpha_Scroll);
            // 
            // CPBackground
            // 
            this.CPBackground.BackColor = System.Drawing.Color.Black;
            this.CPBackground.Location = new System.Drawing.Point(80, 37);
            this.CPBackground.Name = "CPBackground";
            this.CPBackground.Size = new System.Drawing.Size(88, 32);
            this.CPBackground.TabIndex = 4;
            this.CPBackground.UseVisualStyleBackColor = false;
            this.CPBackground.Click += new System.EventHandler(this.CPBackground_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Alpha;
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 5;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Color;
            this.pictureBox2.Location = new System.Drawing.Point(3, 38);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 6;
            this.pictureBox2.TabStop = false;
            // 
            // SimpleColorEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.CPBackground);
            this.Controls.Add(this.SliderAlpha);
            this.Name = "SimpleColorEditor";
            this.Size = new System.Drawing.Size(202, 68);
            ((System.ComponentModel.ISupportInitialize)(this.SliderAlpha)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar SliderAlpha;
        private System.Windows.Forms.Button CPBackground;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
    }
}
