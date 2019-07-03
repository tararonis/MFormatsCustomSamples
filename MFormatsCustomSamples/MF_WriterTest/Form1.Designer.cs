namespace MF_WriterTest
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
            this.prevPanel = new System.Windows.Forms.Panel();
            this.openFilePath_txb = new System.Windows.Forms.TextBox();
            this.savePath_txb = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.start_btn = new System.Windows.Forms.Button();
            this.Stop_btn = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // prevPanel
            // 
            this.prevPanel.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.prevPanel.Location = new System.Drawing.Point(63, 13);
            this.prevPanel.Name = "prevPanel";
            this.prevPanel.Size = new System.Drawing.Size(372, 250);
            this.prevPanel.TabIndex = 0;
            // 
            // openFilePath_txb
            // 
            this.openFilePath_txb.Location = new System.Drawing.Point(60, 285);
            this.openFilePath_txb.Name = "openFilePath_txb";
            this.openFilePath_txb.Size = new System.Drawing.Size(375, 20);
            this.openFilePath_txb.TabIndex = 1;
            this.openFilePath_txb.Text = "\\\\192.168.0.100\\MLFiles\\Trash\\Roman\\WorkFiles\\FilesFromCostumer\\2019.03.24Михаил3" +
    "1880\\50.png";
            // 
            // savePath_txb
            // 
            this.savePath_txb.Location = new System.Drawing.Point(60, 335);
            this.savePath_txb.Name = "savePath_txb";
            this.savePath_txb.Size = new System.Drawing.Size(375, 20);
            this.savePath_txb.TabIndex = 2;
            this.savePath_txb.Text = "M:\\TempVideo\\1.mp4";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(60, 266);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "open";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(60, 319);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(30, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "save";
            // 
            // start_btn
            // 
            this.start_btn.Location = new System.Drawing.Point(60, 373);
            this.start_btn.Name = "start_btn";
            this.start_btn.Size = new System.Drawing.Size(375, 23);
            this.start_btn.TabIndex = 5;
            this.start_btn.Text = "Start";
            this.start_btn.UseVisualStyleBackColor = true;
            this.start_btn.Click += new System.EventHandler(this.start_btn_Click);
            // 
            // Stop_btn
            // 
            this.Stop_btn.Location = new System.Drawing.Point(60, 402);
            this.Stop_btn.Name = "Stop_btn";
            this.Stop_btn.Size = new System.Drawing.Size(375, 23);
            this.Stop_btn.TabIndex = 6;
            this.Stop_btn.Text = "Stop";
            this.Stop_btn.UseVisualStyleBackColor = true;
            this.Stop_btn.Click += new System.EventHandler(this.Stop_btn_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(495, 442);
            this.Controls.Add(this.Stop_btn);
            this.Controls.Add(this.start_btn);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.savePath_txb);
            this.Controls.Add(this.openFilePath_txb);
            this.Controls.Add(this.prevPanel);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel prevPanel;
        private System.Windows.Forms.TextBox openFilePath_txb;
        private System.Windows.Forms.TextBox savePath_txb;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button start_btn;
        private System.Windows.Forms.Button Stop_btn;
    }
}

