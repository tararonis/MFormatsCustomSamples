namespace MF_DVB
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.OpenFile_btn = new System.Windows.Forms.Button();
            this.config_txb = new System.Windows.Forms.TextBox();
            this.url_txb = new System.Windows.Forms.TextBox();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel1.Location = new System.Drawing.Point(142, 23);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(246, 230);
            this.panel1.TabIndex = 0;
            // 
            // OpenFile_btn
            // 
            this.OpenFile_btn.Location = new System.Drawing.Point(142, 275);
            this.OpenFile_btn.Name = "OpenFile_btn";
            this.OpenFile_btn.Size = new System.Drawing.Size(246, 23);
            this.OpenFile_btn.TabIndex = 1;
            this.OpenFile_btn.Text = "Open";
            this.OpenFile_btn.UseVisualStyleBackColor = true;
            this.OpenFile_btn.Click += new System.EventHandler(this.OpenFile_btn_Click);
            // 
            // config_txb
            // 
            this.config_txb.Location = new System.Drawing.Point(12, 349);
            this.config_txb.Name = "config_txb";
            this.config_txb.Size = new System.Drawing.Size(518, 20);
            this.config_txb.TabIndex = 2;
            this.config_txb.Text = "format=\'dvb\' protocol=\'udp://\' video::codec=\'libopenh264\' audio::codec=\'aac\' vide" +
    "o::b=\'3000000\'";
            // 
            // url_txb
            // 
            this.url_txb.Location = new System.Drawing.Point(142, 304);
            this.url_txb.Name = "url_txb";
            this.url_txb.Size = new System.Drawing.Size(246, 20);
            this.url_txb.TabIndex = 3;
            this.url_txb.Text = "udp://239.1.1.7:1239?pkt_size=1316";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(142, 376);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(246, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "Add CG";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(541, 424);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.url_txb);
            this.Controls.Add(this.config_txb);
            this.Controls.Add(this.OpenFile_btn);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button OpenFile_btn;
        private System.Windows.Forms.TextBox config_txb;
        private System.Windows.Forms.TextBox url_txb;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button button1;
    }
}

