namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class SpeedEditor
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
            this.SliderSpeedX = new System.Windows.Forms.TrackBar();
            this.SliderSpeedY = new System.Windows.Forms.TrackBar();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SliderSpeedX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SliderSpeedY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.SuspendLayout();
            // 
            // SliderSpeedX
            // 
            this.SliderSpeedX.Location = new System.Drawing.Point(57, 3);
            this.SliderSpeedX.Minimum = -10;
            this.SliderSpeedX.Name = "SliderSpeedX";
            this.SliderSpeedX.Size = new System.Drawing.Size(165, 69);
            this.SliderSpeedX.TabIndex = 3;
            this.SliderSpeedX.TickStyle = System.Windows.Forms.TickStyle.None;
            this.SliderSpeedX.Scroll += new System.EventHandler(this.SliderSpeedX_Scroll);
            // 
            // SliderSpeedY
            // 
            this.SliderSpeedY.Location = new System.Drawing.Point(57, 38);
            this.SliderSpeedY.Minimum = -10;
            this.SliderSpeedY.Name = "SliderSpeedY";
            this.SliderSpeedY.Size = new System.Drawing.Size(165, 69);
            this.SliderSpeedY.TabIndex = 4;
            this.SliderSpeedY.TickStyle = System.Windows.Forms.TickStyle.None;
            this.SliderSpeedY.Scroll += new System.EventHandler(this.SliderSpeedY_Scroll);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.SpeedX;
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(48, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 6;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.SpeedY;
            this.pictureBox2.Location = new System.Drawing.Point(3, 42);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(48, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 7;
            this.pictureBox2.TabStop = false;
            // 
            // SpeedEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.SliderSpeedY);
            this.Controls.Add(this.SliderSpeedX);
            this.Name = "SpeedEditor";
            this.Size = new System.Drawing.Size(221, 77);
            ((System.ComponentModel.ISupportInitialize)(this.SliderSpeedX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SliderSpeedY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TrackBar SliderSpeedX;
        private System.Windows.Forms.TrackBar SliderSpeedY;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
    }
}
