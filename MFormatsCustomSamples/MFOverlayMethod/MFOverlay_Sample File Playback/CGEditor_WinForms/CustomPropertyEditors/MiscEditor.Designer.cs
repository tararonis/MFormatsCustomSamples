namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class MiscEditor
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
            this.SpinnerEdgeSmooth = new System.Windows.Forms.NumericUpDown();
            this.SpinnerPixelAR = new System.Windows.Forms.NumericUpDown();
            this.ComboScale = new System.Windows.Forms.ComboBox();
            this.ComboAlign = new System.Windows.Forms.ComboBox();
            this.ComboInterlace = new System.Windows.Forms.ComboBox();
            this.ComboPlayMode = new System.Windows.Forms.ComboBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox5 = new System.Windows.Forms.PictureBox();
            this.pictureBox6 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerEdgeSmooth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerPixelAR)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).BeginInit();
            this.SuspendLayout();
            // 
            // SpinnerEdgeSmooth
            // 
            this.SpinnerEdgeSmooth.Location = new System.Drawing.Point(39, 3);
            this.SpinnerEdgeSmooth.Name = "SpinnerEdgeSmooth";
            this.SpinnerEdgeSmooth.Size = new System.Drawing.Size(71, 26);
            this.SpinnerEdgeSmooth.TabIndex = 0;
            this.SpinnerEdgeSmooth.ValueChanged += new System.EventHandler(this.SpinnerEdgeSmooth_ValueChanged);
            // 
            // SpinnerPixelAR
            // 
            this.SpinnerPixelAR.DecimalPlaces = 1;
            this.SpinnerPixelAR.Location = new System.Drawing.Point(39, 35);
            this.SpinnerPixelAR.Maximum = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.SpinnerPixelAR.Name = "SpinnerPixelAR";
            this.SpinnerPixelAR.Size = new System.Drawing.Size(71, 26);
            this.SpinnerPixelAR.TabIndex = 1;
            this.SpinnerPixelAR.ValueChanged += new System.EventHandler(this.SpinnerPixelAR_ValueChanged);
            // 
            // ComboScale
            // 
            this.ComboScale.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboScale.FormattingEnabled = true;
            this.ComboScale.Location = new System.Drawing.Point(152, 3);
            this.ComboScale.Name = "ComboScale";
            this.ComboScale.Size = new System.Drawing.Size(148, 28);
            this.ComboScale.TabIndex = 4;
            this.ComboScale.SelectedIndexChanged += new System.EventHandler(this.ComboScale_SelectedIndexChanged);
            // 
            // ComboAlign
            // 
            this.ComboAlign.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboAlign.FormattingEnabled = true;
            this.ComboAlign.Location = new System.Drawing.Point(152, 37);
            this.ComboAlign.Name = "ComboAlign";
            this.ComboAlign.Size = new System.Drawing.Size(148, 28);
            this.ComboAlign.TabIndex = 7;
            this.ComboAlign.SelectedIndexChanged += new System.EventHandler(this.ComboAlign_SelectedIndexChanged);
            // 
            // ComboInterlace
            // 
            this.ComboInterlace.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboInterlace.FormattingEnabled = true;
            this.ComboInterlace.Location = new System.Drawing.Point(342, 3);
            this.ComboInterlace.Name = "ComboInterlace";
            this.ComboInterlace.Size = new System.Drawing.Size(148, 28);
            this.ComboInterlace.TabIndex = 9;
            this.ComboInterlace.SelectedIndexChanged += new System.EventHandler(this.ComboInterlace_SelectedIndexChanged);
            // 
            // ComboPlayMode
            // 
            this.ComboPlayMode.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ComboPlayMode.FormattingEnabled = true;
            this.ComboPlayMode.Location = new System.Drawing.Point(344, 37);
            this.ComboPlayMode.Name = "ComboPlayMode";
            this.ComboPlayMode.Size = new System.Drawing.Size(148, 28);
            this.ComboPlayMode.TabIndex = 11;
            this.ComboPlayMode.SelectedIndexChanged += new System.EventHandler(this.ComboPlayMode_SelectedIndexChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.EdgeSmooth;
            this.pictureBox1.Location = new System.Drawing.Point(3, 2);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(30, 30);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 12;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Image = global::CGEditor_WinForms.Properties.Resources.PixelAR;
            this.pictureBox2.Location = new System.Drawing.Point(3, 35);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(30, 30);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 13;
            this.pictureBox2.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = global::CGEditor_WinForms.Properties.Resources.ScaleType;
            this.pictureBox3.Location = new System.Drawing.Point(116, 2);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(30, 30);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 14;
            this.pictureBox3.TabStop = false;
            // 
            // pictureBox4
            // 
            this.pictureBox4.Image = global::CGEditor_WinForms.Properties.Resources.Alignment;
            this.pictureBox4.Location = new System.Drawing.Point(116, 34);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(30, 30);
            this.pictureBox4.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox4.TabIndex = 15;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox5
            // 
            this.pictureBox5.Image = global::CGEditor_WinForms.Properties.Resources.Interlace;
            this.pictureBox5.Location = new System.Drawing.Point(306, 1);
            this.pictureBox5.Name = "pictureBox5";
            this.pictureBox5.Size = new System.Drawing.Size(30, 30);
            this.pictureBox5.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox5.TabIndex = 16;
            this.pictureBox5.TabStop = false;
            // 
            // pictureBox6
            // 
            this.pictureBox6.Image = global::CGEditor_WinForms.Properties.Resources.PlayMode;
            this.pictureBox6.Location = new System.Drawing.Point(306, 34);
            this.pictureBox6.Name = "pictureBox6";
            this.pictureBox6.Size = new System.Drawing.Size(30, 30);
            this.pictureBox6.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox6.TabIndex = 17;
            this.pictureBox6.TabStop = false;
            // 
            // MiscEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox6);
            this.Controls.Add(this.pictureBox5);
            this.Controls.Add(this.pictureBox4);
            this.Controls.Add(this.pictureBox3);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.ComboPlayMode);
            this.Controls.Add(this.ComboInterlace);
            this.Controls.Add(this.ComboAlign);
            this.Controls.Add(this.ComboScale);
            this.Controls.Add(this.SpinnerPixelAR);
            this.Controls.Add(this.SpinnerEdgeSmooth);
            this.Name = "MiscEditor";
            this.Size = new System.Drawing.Size(495, 81);
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerEdgeSmooth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.SpinnerPixelAR)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox6)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NumericUpDown SpinnerEdgeSmooth;
        private System.Windows.Forms.NumericUpDown SpinnerPixelAR;
        private System.Windows.Forms.ComboBox ComboScale;
        private System.Windows.Forms.ComboBox ComboAlign;
        private System.Windows.Forms.ComboBox ComboInterlace;
        private System.Windows.Forms.ComboBox ComboPlayMode;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.PictureBox pictureBox5;
        private System.Windows.Forms.PictureBox pictureBox6;
    }
}
