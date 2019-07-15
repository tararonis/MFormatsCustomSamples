namespace MF_SeekingMPEGfileProblem
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
            this.panelPr = new System.Windows.Forms.Panel();
            this.Open_btn = new System.Windows.Forms.Button();
            this.panelFrame = new System.Windows.Forms.Panel();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.Stop_btn = new System.Windows.Forms.Button();
            this.GetFrame_btn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // panelPr
            // 
            this.panelPr.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panelPr.Location = new System.Drawing.Point(30, 52);
            this.panelPr.Name = "panelPr";
            this.panelPr.Size = new System.Drawing.Size(269, 174);
            this.panelPr.TabIndex = 0;
            // 
            // Open_btn
            // 
            this.Open_btn.Location = new System.Drawing.Point(30, 260);
            this.Open_btn.Name = "Open_btn";
            this.Open_btn.Size = new System.Drawing.Size(269, 23);
            this.Open_btn.TabIndex = 1;
            this.Open_btn.Text = "OPen";
            this.Open_btn.UseVisualStyleBackColor = true;
            this.Open_btn.Click += new System.EventHandler(this.Open_btn_Click);
            // 
            // panelFrame
            // 
            this.panelFrame.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.panelFrame.Location = new System.Drawing.Point(350, 52);
            this.panelFrame.Name = "panelFrame";
            this.panelFrame.Size = new System.Drawing.Size(271, 183);
            this.panelFrame.TabIndex = 3;
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Stop_btn
            // 
            this.Stop_btn.Location = new System.Drawing.Point(30, 288);
            this.Stop_btn.Name = "Stop_btn";
            this.Stop_btn.Size = new System.Drawing.Size(269, 23);
            this.Stop_btn.TabIndex = 4;
            this.Stop_btn.Text = "Stop";
            this.Stop_btn.UseVisualStyleBackColor = true;
            this.Stop_btn.Click += new System.EventHandler(this.Stop_btn_Click_1);
            // 
            // GetFrame_btn
            // 
            this.GetFrame_btn.Location = new System.Drawing.Point(350, 260);
            this.GetFrame_btn.Name = "GetFrame_btn";
            this.GetFrame_btn.Size = new System.Drawing.Size(271, 23);
            this.GetFrame_btn.TabIndex = 5;
            this.GetFrame_btn.Text = "GetFrame";
            this.GetFrame_btn.UseVisualStyleBackColor = true;
            this.GetFrame_btn.Click += new System.EventHandler(this.GetFrame_btn_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.GetFrame_btn);
            this.Controls.Add(this.Stop_btn);
            this.Controls.Add(this.panelFrame);
            this.Controls.Add(this.Open_btn);
            this.Controls.Add(this.panelPr);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelPr;
        private System.Windows.Forms.Button Open_btn;
        private System.Windows.Forms.Panel panelFrame;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button Stop_btn;
        private System.Windows.Forms.Button GetFrame_btn;
    }
}

