namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class TickerEditor
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
            this.SpinnerSpeed = new System.Windows.Forms.NumericUpDown();
            this.SpinnerLineHeight = new System.Windows.Forms.NumericUpDown();
            this.SpinnerDistance = new System.Windows.Forms.NumericUpDown();
            this.ToggleRewind = new System.Windows.Forms.Button();
            this.ToggleGapless = new System.Windows.Forms.CheckBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerLineHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerDistance)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            this.SuspendLayout();
            // 
            // SpinnerSpeed
            // 
            this.SpinnerSpeed.Location = new System.Drawing.Point(50, 3);
            this.SpinnerSpeed.Minimum = new decimal(new int[] {
            100,
            0,
            0,
            -2147483648});
            this.SpinnerSpeed.Name = "SpinnerSpeed";
            this.SpinnerSpeed.Size = new System.Drawing.Size(74, 26);
            this.SpinnerSpeed.TabIndex = 1;
            this.SpinnerSpeed.ValueChanged += new System.EventHandler(this.SpinnerSpeed_ValueChanged);
            // 
            // SpinnerLineHeight
            // 
            this.SpinnerLineHeight.DecimalPlaces = 1;
            this.SpinnerLineHeight.Location = new System.Drawing.Point(50, 37);
            this.SpinnerLineHeight.Name = "SpinnerLineHeight";
            this.SpinnerLineHeight.Size = new System.Drawing.Size(74, 26);
            this.SpinnerLineHeight.TabIndex = 3;
            this.SpinnerLineHeight.ValueChanged += new System.EventHandler(this.SpinnerLineHeight_ValueChanged);
            // 
            // SpinnerDistance
            // 
            this.SpinnerDistance.DecimalPlaces = 1;
            this.SpinnerDistance.Location = new System.Drawing.Point(182, 39);
            this.SpinnerDistance.Name = "SpinnerDistance";
            this.SpinnerDistance.Size = new System.Drawing.Size(123, 26);
            this.SpinnerDistance.TabIndex = 5;
            this.SpinnerDistance.ValueChanged += new System.EventHandler(this.SpinnerDistance_ValueChanged);
            // 
            // ToggleRewind
            // 
            this.ToggleRewind.Location = new System.Drawing.Point(130, 1);
            this.ToggleRewind.Name = "ToggleRewind";
            this.ToggleRewind.Size = new System.Drawing.Size(82, 32);
            this.ToggleRewind.TabIndex = 6;
            this.ToggleRewind.Text = "Rewind";
            this.ToggleRewind.UseVisualStyleBackColor = true;
            this.ToggleRewind.Click += new System.EventHandler(this.ToggleRewind_Click);
            // 
            // ToggleGapless
            // 
            this.ToggleGapless.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleGapless.AutoSize = true;
            this.ToggleGapless.Location = new System.Drawing.Point(218, 1);
            this.ToggleGapless.MaximumSize = new System.Drawing.Size(0, 32);
            this.ToggleGapless.MinimumSize = new System.Drawing.Size(0, 32);
            this.ToggleGapless.Name = "ToggleGapless";
            this.ToggleGapless.Size = new System.Drawing.Size(87, 32);
            this.ToggleGapless.TabIndex = 7;
            this.ToggleGapless.Text = "Gapeless";
            this.ToggleGapless.UseVisualStyleBackColor = true;
            this.ToggleGapless.CheckedChanged += new System.EventHandler(this.ToggleGapless_CheckedChanged);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.TickerSpeed;
            this.pictureBox2.Location = new System.Drawing.Point(14, 4);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 18;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.LineHeight;
            this.pictureBox1.Location = new System.Drawing.Point(3, 39);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(41, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 19;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.Distance;
            this.pictureBox3.Location = new System.Drawing.Point(130, 39);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(46, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 20;
            this.pictureBox3.TabStop = false;
            // 
            // TickerEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.ToggleGapless);
            this.Controls.Add(this.ToggleRewind);
            this.Controls.Add(this.SpinnerDistance);
            this.Controls.Add(this.SpinnerLineHeight);
            this.Controls.Add(this.SpinnerSpeed);
            this.Name = "TickerEditor";
            this.Size = new System.Drawing.Size(316, 73);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerLineHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerDistance)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown SpinnerSpeed;
        private System.Windows.Forms.NumericUpDown SpinnerLineHeight;
        private System.Windows.Forms.NumericUpDown SpinnerDistance;
        private System.Windows.Forms.Button ToggleRewind;
        private System.Windows.Forms.CheckBox ToggleGapless;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox3;
    }
}
