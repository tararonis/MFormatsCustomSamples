namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class TextGradientEditor
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
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.SpinnerGradientAngle = new System.Windows.Forms.NumericUpDown();
            this.ComboGradientType = new System.Windows.Forms.ComboBox();
            this.buttonAddGradient = new System.Windows.Forms.Button();
            this.StackColors = new System.Windows.Forms.FlowLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGradientAngle)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.Angle;
            this.pictureBox3.Location = new System.Drawing.Point(257, 39);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 16;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Gradient;
            this.pictureBox1.Location = new System.Drawing.Point(1, 45);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 15;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Color;
            this.pictureBox2.Location = new System.Drawing.Point(1, 3);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 14;
            this.pictureBox2.TabStop = false;
            // 
            // SpinnerGradientAngle
            // 
            this.SpinnerGradientAngle.Location = new System.Drawing.Point(293, 40);
            this.SpinnerGradientAngle.Maximum = new decimal(new int[] {
            360,
            0,
            0,
            0});
            this.SpinnerGradientAngle.Name = "SpinnerGradientAngle";
            this.SpinnerGradientAngle.Size = new System.Drawing.Size(63, 26);
            this.SpinnerGradientAngle.TabIndex = 13;
            this.SpinnerGradientAngle.ValueChanged += new System.EventHandler(this.SpinnerGradientAngle_ValueChanged);
            // 
            // ComboGradientType
            // 
            this.ComboGradientType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboGradientType.FormattingEnabled = true;
            this.ComboGradientType.Location = new System.Drawing.Point(37, 43);
            this.ComboGradientType.Name = "ComboGradientType";
            this.ComboGradientType.Size = new System.Drawing.Size(214, 28);
            this.ComboGradientType.TabIndex = 12;
            this.ComboGradientType.SelectedIndexChanged += new System.EventHandler(this.ComboGradientType_SelectedIndexChanged);
            // 
            // buttonAddGradient
            // 
            this.buttonAddGradient.BackColor = System.Drawing.Color.White;
            this.buttonAddGradient.Location = new System.Drawing.Point(257, 3);
            this.buttonAddGradient.Name = "buttonAddGradient";
            this.buttonAddGradient.Size = new System.Drawing.Size(99, 31);
            this.buttonAddGradient.TabIndex = 10;
            this.buttonAddGradient.Text = "Add color";
            this.buttonAddGradient.UseVisualStyleBackColor = false;
            this.buttonAddGradient.Click += new System.EventHandler(this.buttonAddGradient_Click);
            // 
            // StackColors
            // 
            this.StackColors.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.StackColors.Location = new System.Drawing.Point(37, 3);
            this.StackColors.Name = "StackColors";
            this.StackColors.Size = new System.Drawing.Size(214, 35);
            this.StackColors.TabIndex = 11;
            // 
            // TextGradientEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.SpinnerGradientAngle);
            this.Controls.Add(this.ComboGradientType);
            this.Controls.Add(this.buttonAddGradient);
            this.Controls.Add(this.StackColors);
            this.Name = "TextGradientEditor";
            this.Size = new System.Drawing.Size(362, 76);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerGradientAngle)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.NumericUpDown SpinnerGradientAngle;
        private System.Windows.Forms.ComboBox ComboGradientType;
        private System.Windows.Forms.Button buttonAddGradient;
        private System.Windows.Forms.FlowLayoutPanel StackColors;
    }
}
