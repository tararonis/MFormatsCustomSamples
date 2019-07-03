namespace _MF_Constructor
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
            this.panelPrev = new System.Windows.Forms.Panel();
            this.OpenFile_btn = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.Stop_btn = new System.Windows.Forms.Button();
            this.LiveList_lsb = new System.Windows.Forms.ListBox();
            this.LiveAList_lsb = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // panelPrev
            // 
            this.panelPrev.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelPrev.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panelPrev.Location = new System.Drawing.Point(13, 13);
            this.panelPrev.Name = "panelPrev";
            this.panelPrev.Size = new System.Drawing.Size(313, 234);
            this.panelPrev.TabIndex = 0;
            // 
            // OpenFile_btn
            // 
            this.OpenFile_btn.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.OpenFile_btn.Location = new System.Drawing.Point(13, 253);
            this.OpenFile_btn.Name = "OpenFile_btn";
            this.OpenFile_btn.Size = new System.Drawing.Size(313, 23);
            this.OpenFile_btn.TabIndex = 1;
            this.OpenFile_btn.Text = "OpenFile";
            this.OpenFile_btn.UseVisualStyleBackColor = true;
            this.OpenFile_btn.Click += new System.EventHandler(this.OpenFile_btn_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // Stop_btn
            // 
            this.Stop_btn.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.Stop_btn.Location = new System.Drawing.Point(13, 282);
            this.Stop_btn.Name = "Stop_btn";
            this.Stop_btn.Size = new System.Drawing.Size(313, 23);
            this.Stop_btn.TabIndex = 3;
            this.Stop_btn.Text = "Stop";
            this.Stop_btn.UseVisualStyleBackColor = true;
            this.Stop_btn.Click += new System.EventHandler(this.Stop_btn_Click);
            // 
            // LiveList_lsb
            // 
            this.LiveList_lsb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LiveList_lsb.FormattingEnabled = true;
            this.LiveList_lsb.Location = new System.Drawing.Point(345, 13);
            this.LiveList_lsb.Name = "LiveList_lsb";
            this.LiveList_lsb.Size = new System.Drawing.Size(191, 199);
            this.LiveList_lsb.TabIndex = 4;
            this.LiveList_lsb.SelectedIndexChanged += new System.EventHandler(this.LiveList_lsb_SelectedIndexChanged);
            // 
            // LiveAList_lsb
            // 
            this.LiveAList_lsb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.LiveAList_lsb.FormattingEnabled = true;
            this.LiveAList_lsb.Location = new System.Drawing.Point(553, 12);
            this.LiveAList_lsb.Name = "LiveAList_lsb";
            this.LiveAList_lsb.Size = new System.Drawing.Size(186, 199);
            this.LiveAList_lsb.TabIndex = 5;
            this.LiveAList_lsb.SelectedIndexChanged += new System.EventHandler(this.LiveAList_lsb_SelectedIndexChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 345);
            this.Controls.Add(this.LiveAList_lsb);
            this.Controls.Add(this.LiveList_lsb);
            this.Controls.Add(this.Stop_btn);
            this.Controls.Add(this.OpenFile_btn);
            this.Controls.Add(this.panelPrev);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelPrev;
        private System.Windows.Forms.Button OpenFile_btn;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button Stop_btn;
        private System.Windows.Forms.ListBox LiveList_lsb;
        private System.Windows.Forms.ListBox LiveAList_lsb;
    }
}

