namespace MF_FastAudioExtract
{
    partial class Form1
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Open_btn = new System.Windows.Forms.Button();
            this.panelPrev = new System.Windows.Forms.Panel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.Extract_btn = new System.Windows.Forms.Button();
            this.Stop_btn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // Open_btn
            // 
            this.Open_btn.Location = new System.Drawing.Point(45, 248);
            this.Open_btn.Name = "Open_btn";
            this.Open_btn.Size = new System.Drawing.Size(114, 23);
            this.Open_btn.TabIndex = 0;
            this.Open_btn.Text = "Open";
            this.Open_btn.UseVisualStyleBackColor = true;
            this.Open_btn.Click += new System.EventHandler(this.Open_btn_Click);
            // 
            // panelPrev
            // 
            this.panelPrev.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panelPrev.Location = new System.Drawing.Point(45, 42);
            this.panelPrev.Name = "panelPrev";
            this.panelPrev.Size = new System.Drawing.Size(396, 186);
            this.panelPrev.TabIndex = 1;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Extract_btn
            // 
            this.Extract_btn.Location = new System.Drawing.Point(45, 288);
            this.Extract_btn.Name = "Extract_btn";
            this.Extract_btn.Size = new System.Drawing.Size(114, 23);
            this.Extract_btn.TabIndex = 2;
            this.Extract_btn.Text = "Extract";
            this.Extract_btn.UseVisualStyleBackColor = true;
            this.Extract_btn.Click += new System.EventHandler(this.Extract_btn_Click);
            // 
            // Stop_btn
            // 
            this.Stop_btn.Location = new System.Drawing.Point(45, 318);
            this.Stop_btn.Name = "Stop_btn";
            this.Stop_btn.Size = new System.Drawing.Size(114, 23);
            this.Stop_btn.TabIndex = 3;
            this.Stop_btn.Text = "Stop";
            this.Stop_btn.UseVisualStyleBackColor = true;
            this.Stop_btn.Click += new System.EventHandler(this.Stop_btn_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 450);
            this.Controls.Add(this.Stop_btn);
            this.Controls.Add(this.Extract_btn);
            this.Controls.Add(this.panelPrev);
            this.Controls.Add(this.Open_btn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button Open_btn;
        private System.Windows.Forms.Panel panelPrev;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button Extract_btn;
        private System.Windows.Forms.Button Stop_btn;
    }
}

