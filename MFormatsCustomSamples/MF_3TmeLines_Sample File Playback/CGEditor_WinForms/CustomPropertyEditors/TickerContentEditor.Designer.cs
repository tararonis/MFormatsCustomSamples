namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class TickerContentEditor
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
            this.TextBoxNewContent = new System.Windows.Forms.TextBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // TextBoxNewContent
            // 
            this.TextBoxNewContent.Location = new System.Drawing.Point(51, 3);
            this.TextBoxNewContent.Multiline = true;
            this.TextBoxNewContent.Name = "TextBoxNewContent";
            this.TextBoxNewContent.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.TextBoxNewContent.Size = new System.Drawing.Size(483, 74);
            this.TextBoxNewContent.TabIndex = 2;
            this.TextBoxNewContent.TextChanged += new System.EventHandler(this.TextBoxNewContent_TextChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = global::CGEditor_WinForms.Properties.Resources.TickerText;
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(42, 39);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            // 
            // TickerContentEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.TextBoxNewContent);
            this.Name = "TickerContentEditor";
            this.Size = new System.Drawing.Size(539, 82);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TextBoxNewContent;
        private System.Windows.Forms.PictureBox pictureBox1;
    }
}
