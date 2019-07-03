namespace MF_WriterChange
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
            this.Prewiew_panel = new System.Windows.Forms.Panel();
            this.start_btn = new System.Windows.Forms.Button();
            this.encodingCon_txb = new System.Windows.Forms.TextBox();
            this.filePath_txb = new System.Windows.Forms.TextBox();
            this.savePath_txb = new System.Windows.Forms.TextBox();
            this.successList_lsv = new System.Windows.Forms.ListView();
            this.SuspendLayout();
            // 
            // Prewiew_panel
            // 
            this.Prewiew_panel.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.Prewiew_panel.Location = new System.Drawing.Point(66, 46);
            this.Prewiew_panel.Name = "Prewiew_panel";
            this.Prewiew_panel.Size = new System.Drawing.Size(379, 211);
            this.Prewiew_panel.TabIndex = 0;
            // 
            // start_btn
            // 
            this.start_btn.Location = new System.Drawing.Point(66, 320);
            this.start_btn.Name = "start_btn";
            this.start_btn.Size = new System.Drawing.Size(379, 23);
            this.start_btn.TabIndex = 1;
            this.start_btn.Text = "Start";
            this.start_btn.UseVisualStyleBackColor = true;
            this.start_btn.Click += new System.EventHandler(this.start_btn_Click);
            // 
            // encodingCon_txb
            // 
            this.encodingCon_txb.Location = new System.Drawing.Point(451, 46);
            this.encodingCon_txb.Name = "encodingCon_txb";
            this.encodingCon_txb.Size = new System.Drawing.Size(337, 20);
            this.encodingCon_txb.TabIndex = 3;
            this.encodingCon_txb.Text = " format=\'mp4\' video::codec=\'n264\' video::size=\'1280x720\' audio::codec=\'aac\'";
            // 
            // filePath_txb
            // 
            this.filePath_txb.Location = new System.Drawing.Point(66, 284);
            this.filePath_txb.Name = "filePath_txb";
            this.filePath_txb.Size = new System.Drawing.Size(379, 20);
            this.filePath_txb.TabIndex = 4;
            this.filePath_txb.Text = "M:\\MEDIA_TEST\\!1080p 60fpsOri and the Will of the Wisps.mp4";
            // 
            // savePath_txb
            // 
            this.savePath_txb.Location = new System.Drawing.Point(451, 73);
            this.savePath_txb.Name = "savePath_txb";
            this.savePath_txb.Size = new System.Drawing.Size(337, 20);
            this.savePath_txb.TabIndex = 5;
            this.savePath_txb.Text = "M:\\TempVideo\\1";
            // 
            // successList_lsv
            // 
            this.successList_lsv.Cursor = System.Windows.Forms.Cursors.Default;
            this.successList_lsv.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.15F, System.Drawing.FontStyle.Bold);
            this.successList_lsv.Location = new System.Drawing.Point(451, 108);
            this.successList_lsv.Name = "successList_lsv";
            this.successList_lsv.Size = new System.Drawing.Size(337, 330);
            this.successList_lsv.TabIndex = 6;
            this.successList_lsv.TileSize = new System.Drawing.Size(30, 20);
            this.successList_lsv.UseCompatibleStateImageBehavior = false;
            this.successList_lsv.View = System.Windows.Forms.View.Tile;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.successList_lsv);
            this.Controls.Add(this.savePath_txb);
            this.Controls.Add(this.filePath_txb);
            this.Controls.Add(this.encodingCon_txb);
            this.Controls.Add(this.start_btn);
            this.Controls.Add(this.Prewiew_panel);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel Prewiew_panel;
        private System.Windows.Forms.Button start_btn;
        private System.Windows.Forms.TextBox encodingCon_txb;
        private System.Windows.Forms.TextBox filePath_txb;
        private System.Windows.Forms.TextBox savePath_txb;
        private System.Windows.Forms.ListView successList_lsv;
    }
}

