namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class ShapeEditor
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
            this.ComboShapeType = new System.Windows.Forms.ComboBox();
            this.SpinnerRoundCorners = new System.Windows.Forms.NumericUpDown();
            this.SpinnerNSides = new System.Windows.Forms.NumericUpDown();
            this.SpinnerRotate = new System.Windows.Forms.NumericUpDown();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerRoundCorners)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerNSides)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerRotate)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            this.SuspendLayout();
            // 
            // ComboShapeType
            // 
            this.ComboShapeType.FormattingEnabled = true;
            this.ComboShapeType.Location = new System.Drawing.Point(39, 3);
            this.ComboShapeType.Name = "ComboShapeType";
            this.ComboShapeType.Size = new System.Drawing.Size(120, 28);
            this.ComboShapeType.TabIndex = 1;
            this.ComboShapeType.SelectedIndexChanged += new System.EventHandler(this.ComboShapeType_SelectedIndexChanged);
            // 
            // SpinnerRoundCorners
            // 
            this.SpinnerRoundCorners.Location = new System.Drawing.Point(201, 4);
            this.SpinnerRoundCorners.Name = "SpinnerRoundCorners";
            this.SpinnerRoundCorners.Size = new System.Drawing.Size(71, 26);
            this.SpinnerRoundCorners.TabIndex = 3;
            this.SpinnerRoundCorners.ValueChanged += new System.EventHandler(this.SpinnerRoundCorners_ValueChanged);
            // 
            // SpinnerNSides
            // 
            this.SpinnerNSides.Location = new System.Drawing.Point(39, 39);
            this.SpinnerNSides.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.SpinnerNSides.Name = "SpinnerNSides";
            this.SpinnerNSides.Size = new System.Drawing.Size(120, 26);
            this.SpinnerNSides.TabIndex = 5;
            this.SpinnerNSides.ValueChanged += new System.EventHandler(this.SpinnerNSides_ValueChanged);
            // 
            // SpinnerRotate
            // 
            this.SpinnerRotate.Location = new System.Drawing.Point(201, 39);
            this.SpinnerRotate.Maximum = new decimal(new int[] {
            360,
            0,
            0,
            0});
            this.SpinnerRotate.Name = "SpinnerRotate";
            this.SpinnerRotate.Size = new System.Drawing.Size(71, 26);
            this.SpinnerRotate.TabIndex = 7;
            this.SpinnerRotate.ValueChanged += new System.EventHandler(this.SpinnerRotate_ValueChanged);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.Shape;
            this.pictureBox2.Location = new System.Drawing.Point(3, 3);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 17;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.NumberSides;
            this.pictureBox1.Location = new System.Drawing.Point(3, 39);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 18;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.GradType;
            this.pictureBox3.Location = new System.Drawing.Point(165, 3);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 19;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = global::CGEditor_WinForms.Properties.Resources.Angle;
            this.pictureBox4.Location = new System.Drawing.Point(165, 39);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(30, 30);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 20;
            this.pictureBox4.TabStop = false;
            // 
            // ShapeEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.SpinnerRotate);
            this.Controls.Add(this.SpinnerNSides);
            this.Controls.Add(this.SpinnerRoundCorners);
            this.Controls.Add(this.ComboShapeType);
            this.Name = "ShapeEditor";
            this.Size = new System.Drawing.Size(279, 72);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerRoundCorners)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerNSides)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerRotate)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox ComboShapeType;
        private System.Windows.Forms.NumericUpDown SpinnerRoundCorners;
        private System.Windows.Forms.NumericUpDown SpinnerNSides;
        private System.Windows.Forms.NumericUpDown SpinnerRotate;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
    }
}
