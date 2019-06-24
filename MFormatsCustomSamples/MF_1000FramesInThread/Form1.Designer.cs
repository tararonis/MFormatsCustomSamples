namespace MF_1000FramesInThread
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
            this.start_btn = new System.Windows.Forms.Button();
            this.panelPrev = new System.Windows.Forms.Panel();
            this.frameN = new System.Windows.Forms.Label();
            this.openFile_btn = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // start_btn
            // 
            this.start_btn.Location = new System.Drawing.Point(77, 342);
            this.start_btn.Name = "start_btn";
            this.start_btn.Size = new System.Drawing.Size(340, 23);
            this.start_btn.TabIndex = 0;
            this.start_btn.Text = "Start";
            this.start_btn.UseVisualStyleBackColor = true;
            this.start_btn.Click += new System.EventHandler(this.Start_btn_Click);
            // 
            // panelPrev
            // 
            this.panelPrev.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panelPrev.Location = new System.Drawing.Point(77, 22);
            this.panelPrev.Name = "panelPrev";
            this.panelPrev.Size = new System.Drawing.Size(340, 294);
            this.panelPrev.TabIndex = 1;
            // 
            // frameN
            // 
            this.frameN.AutoSize = true;
            this.frameN.Location = new System.Drawing.Point(434, 117);
            this.frameN.Name = "frameN";
            this.frameN.Size = new System.Drawing.Size(31, 13);
            this.frameN.TabIndex = 2;
            this.frameN.Text = "0000";
            // 
            // openFile_btn
            // 
            this.openFile_btn.Location = new System.Drawing.Point(77, 372);
            this.openFile_btn.Name = "openFile_btn";
            this.openFile_btn.Size = new System.Drawing.Size(340, 23);
            this.openFile_btn.TabIndex = 3;
            this.openFile_btn.Text = "OpenFile";
            this.openFile_btn.UseVisualStyleBackColor = true;
            this.openFile_btn.Click += new System.EventHandler(this.OpenFile_btn_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(556, 399);
            this.Controls.Add(this.openFile_btn);
            this.Controls.Add(this.frameN);
            this.Controls.Add(this.panelPrev);
            this.Controls.Add(this.start_btn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button start_btn;
        private System.Windows.Forms.Panel panelPrev;
        private System.Windows.Forms.Label frameN;
        private System.Windows.Forms.Button openFile_btn;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
    }
}

