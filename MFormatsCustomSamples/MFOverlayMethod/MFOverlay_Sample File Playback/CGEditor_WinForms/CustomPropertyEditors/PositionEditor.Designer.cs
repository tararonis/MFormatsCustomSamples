namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class PositionEditor
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
            this.SpinnerXPos = new System.Windows.Forms.NumericUpDown();
            this.SpinnerHeight = new System.Windows.Forms.NumericUpDown();
            this.SpinnerYPos = new System.Windows.Forms.NumericUpDown();
            this.SpinnerWidth = new System.Windows.Forms.NumericUpDown();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerXPos)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerYPos)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.SuspendLayout();
            // 
            // SpinnerXPos
            // 
            this.SpinnerXPos.Location = new System.Drawing.Point(39, 3);
            this.SpinnerXPos.Maximum = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            this.SpinnerXPos.Minimum = new decimal(new int[] {
            20000,
            0,
            0,
            -2147483648});
            this.SpinnerXPos.Name = "SpinnerXPos";
            this.SpinnerXPos.Size = new System.Drawing.Size(70, 26);
            this.SpinnerXPos.TabIndex = 0;
            this.SpinnerXPos.ValueChanged += new System.EventHandler(this.SpinnerXPos_ValueChanged);
            // 
            // SpinnerHeight
            // 
            this.SpinnerHeight.Location = new System.Drawing.Point(151, 37);
            this.SpinnerHeight.Maximum = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            this.SpinnerHeight.Minimum = new decimal(new int[] {
            20000,
            0,
            0,
            -2147483648});
            this.SpinnerHeight.Name = "SpinnerHeight";
            this.SpinnerHeight.Size = new System.Drawing.Size(70, 26);
            this.SpinnerHeight.TabIndex = 1;
            this.SpinnerHeight.ValueChanged += new System.EventHandler(this.SpinnerHeight_ValueChanged);
            // 
            // SpinnerYPos
            // 
            this.SpinnerYPos.Location = new System.Drawing.Point(39, 37);
            this.SpinnerYPos.Maximum = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            this.SpinnerYPos.Minimum = new decimal(new int[] {
            20000,
            0,
            0,
            -2147483648});
            this.SpinnerYPos.Name = "SpinnerYPos";
            this.SpinnerYPos.Size = new System.Drawing.Size(70, 26);
            this.SpinnerYPos.TabIndex = 3;
            this.SpinnerYPos.ValueChanged += new System.EventHandler(this.SpinnerYPos_ValueChanged);
            // 
            // SpinnerWidth
            // 
            this.SpinnerWidth.Location = new System.Drawing.Point(151, 3);
            this.SpinnerWidth.Maximum = new decimal(new int[] {
            20000,
            0,
            0,
            0});
            this.SpinnerWidth.Minimum = new decimal(new int[] {
            20000,
            0,
            0,
            -2147483648});
            this.SpinnerWidth.Name = "SpinnerWidth";
            this.SpinnerWidth.Size = new System.Drawing.Size(70, 26);
            this.SpinnerWidth.TabIndex = 7;
            this.SpinnerWidth.ValueChanged += new System.EventHandler(this.SpinnerWidth_ValueChanged);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Height;
            this.pictureBox2.Location = new System.Drawing.Point(115, 38);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 9;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Width;
            this.pictureBox1.Location = new System.Drawing.Point(115, 4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.X;
            this.pictureBox3.Location = new System.Drawing.Point(3, 4);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 10;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = global::CGEditor_WinForms.Properties.Resources.Y;
            this.pictureBox4.Location = new System.Drawing.Point(3, 37);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(30, 30);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 11;
            this.pictureBox4.TabStop = false;
            // 
            // PositionEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.SpinnerWidth);
            this.Controls.Add(this.SpinnerYPos);
            this.Controls.Add(this.SpinnerHeight);
            this.Controls.Add(this.SpinnerXPos);
            this.Name = "PositionEditor";
            this.Size = new System.Drawing.Size(228, 75);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerXPos)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerYPos)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NumericUpDown SpinnerXPos;
        private System.Windows.Forms.NumericUpDown SpinnerHeight;
        private System.Windows.Forms.NumericUpDown SpinnerYPos;
        private System.Windows.Forms.NumericUpDown SpinnerWidth;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
    }
}
