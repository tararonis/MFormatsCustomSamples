namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class TextOutlineColorEditor
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
            this.SpinnerOutlineWidth = new System.Windows.Forms.NumericUpDown();
            this.ComboGradientType = new System.Windows.Forms.ComboBox();
            this.buttonAddGradient = new System.Windows.Forms.Button();
            this.StackColors = new System.Windows.Forms.FlowLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerOutlineWidth)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.Outline;
            this.pictureBox3.Location = new System.Drawing.Point(258, 44);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 22;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.Gradient;
            this.pictureBox1.Location = new System.Drawing.Point(2, 44);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 21;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Color;
            this.pictureBox2.Location = new System.Drawing.Point(2, 3);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 20;
            this.pictureBox2.TabStop = false;
            // 
            // SpinnerOutlineWidth
            // 
            this.SpinnerOutlineWidth.Location = new System.Drawing.Point(294, 44);
            this.SpinnerOutlineWidth.Name = "SpinnerOutlineWidth";
            this.SpinnerOutlineWidth.Size = new System.Drawing.Size(79, 26);
            this.SpinnerOutlineWidth.TabIndex = 19;
            this.SpinnerOutlineWidth.ValueChanged += new System.EventHandler(this.SpinnerOutlineWidth_ValueChanged);
            // 
            // ComboGradientType
            // 
            this.ComboGradientType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboGradientType.FormattingEnabled = true;
            this.ComboGradientType.Location = new System.Drawing.Point(38, 44);
            this.ComboGradientType.Name = "ComboGradientType";
            this.ComboGradientType.Size = new System.Drawing.Size(214, 28);
            this.ComboGradientType.TabIndex = 18;
            this.ComboGradientType.SelectedIndexChanged += new System.EventHandler(this.ComboGradientType_SelectedIndexChanged);
            // 
            // buttonAddGradient
            // 
            this.buttonAddGradient.BackColor = System.Drawing.Color.White;
            this.buttonAddGradient.Location = new System.Drawing.Point(258, 3);
            this.buttonAddGradient.Name = "buttonAddGradient";
            this.buttonAddGradient.Size = new System.Drawing.Size(115, 35);
            this.buttonAddGradient.TabIndex = 16;
            this.buttonAddGradient.Text = "Add color";
            this.buttonAddGradient.UseVisualStyleBackColor = false;
            this.buttonAddGradient.Click += new System.EventHandler(this.buttonAddGradient_Click);
            // 
            // StackColors
            // 
            this.StackColors.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.StackColors.Location = new System.Drawing.Point(38, 3);
            this.StackColors.Name = "StackColors";
            this.StackColors.Size = new System.Drawing.Size(214, 35);
            this.StackColors.TabIndex = 17;
            // 
            // TextOutlineColorEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.SpinnerOutlineWidth);
            this.Controls.Add(this.ComboGradientType);
            this.Controls.Add(this.buttonAddGradient);
            this.Controls.Add(this.StackColors);
            this.Name = "TextOutlineColorEditor";
            this.Size = new System.Drawing.Size(378, 77);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerOutlineWidth)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.NumericUpDown SpinnerOutlineWidth;
        private System.Windows.Forms.ComboBox ComboGradientType;
        private System.Windows.Forms.Button buttonAddGradient;
        private System.Windows.Forms.FlowLayoutPanel StackColors;
    }
}
