namespace MF_5encodingProcesses
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
            this.components = new System.ComponentModel.Container();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.panel4 = new System.Windows.Forms.Panel();
            this.panel5 = new System.Windows.Forms.Panel();
            this.Start1_btn = new System.Windows.Forms.Button();
            this.Start2_btn = new System.Windows.Forms.Button();
            this.Start3_btn = new System.Windows.Forms.Button();
            this.Start4_btn = new System.Windows.Forms.Button();
            this.Start5_btn = new System.Windows.Forms.Button();
            this.EncodingConfigs_lsb = new System.Windows.Forms.ListBox();
            this.SavePath_txb = new System.Windows.Forms.TextBox();
            this.SelectSaveFolder_btn = new System.Windows.Forms.Button();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBoxLiveName1 = new System.Windows.Forms.TextBox();
            this.textBoxLiveName2 = new System.Windows.Forms.TextBox();
            this.textBoxLiveName3 = new System.Windows.Forms.TextBox();
            this.textBoxLiveName4 = new System.Windows.Forms.TextBox();
            this.textBoxLiveName5 = new System.Windows.Forms.TextBox();
            this.progressBarCPU = new System.Windows.Forms.ProgressBar();
            this.label6 = new System.Windows.Forms.Label();
            this.CPUload_lbl = new System.Windows.Forms.Label();
            this.CPU = new System.Diagnostics.PerformanceCounter();
            this.CheckCPU_lsb = new System.Windows.Forms.ListBox();
            ((System.ComponentModel.ISupportInitialize)(this.CPU)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel1.Location = new System.Drawing.Point(12, 12);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(248, 203);
            this.panel1.TabIndex = 0;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel2.Location = new System.Drawing.Point(374, 12);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(248, 203);
            this.panel2.TabIndex = 1;
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel3.Location = new System.Drawing.Point(750, 12);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(248, 203);
            this.panel3.TabIndex = 1;
            // 
            // panel4
            // 
            this.panel4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.panel4.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel4.Location = new System.Drawing.Point(11, 388);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(248, 203);
            this.panel4.TabIndex = 1;
            // 
            // panel5
            // 
            this.panel5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.panel5.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panel5.Location = new System.Drawing.Point(373, 388);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(248, 203);
            this.panel5.TabIndex = 1;
            // 
            // Start1_btn
            // 
            this.Start1_btn.Enabled = false;
            this.Start1_btn.Location = new System.Drawing.Point(12, 233);
            this.Start1_btn.Name = "Start1_btn";
            this.Start1_btn.Size = new System.Drawing.Size(248, 23);
            this.Start1_btn.TabIndex = 2;
            this.Start1_btn.Tag = "0";
            this.Start1_btn.Text = "Open Live And Start Record";
            this.Start1_btn.UseVisualStyleBackColor = true;
            this.Start1_btn.Click += new System.EventHandler(this.Start1_btn_Click);
            // 
            // Start2_btn
            // 
            this.Start2_btn.Enabled = false;
            this.Start2_btn.Location = new System.Drawing.Point(374, 233);
            this.Start2_btn.Name = "Start2_btn";
            this.Start2_btn.Size = new System.Drawing.Size(247, 23);
            this.Start2_btn.TabIndex = 3;
            this.Start2_btn.Tag = "1";
            this.Start2_btn.Text = "Open Live And Start Record";
            this.Start2_btn.UseVisualStyleBackColor = true;
            this.Start2_btn.Click += new System.EventHandler(this.Start1_btn_Click);
            // 
            // Start3_btn
            // 
            this.Start3_btn.Enabled = false;
            this.Start3_btn.Location = new System.Drawing.Point(750, 233);
            this.Start3_btn.Name = "Start3_btn";
            this.Start3_btn.Size = new System.Drawing.Size(247, 23);
            this.Start3_btn.TabIndex = 4;
            this.Start3_btn.Tag = "2";
            this.Start3_btn.Text = "Open Live And Start Record";
            this.Start3_btn.UseVisualStyleBackColor = true;
            this.Start3_btn.Click += new System.EventHandler(this.Start1_btn_Click);
            // 
            // Start4_btn
            // 
            this.Start4_btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Start4_btn.Enabled = false;
            this.Start4_btn.Location = new System.Drawing.Point(11, 598);
            this.Start4_btn.Name = "Start4_btn";
            this.Start4_btn.Size = new System.Drawing.Size(247, 23);
            this.Start4_btn.TabIndex = 5;
            this.Start4_btn.Tag = "3";
            this.Start4_btn.Text = "Open Live And Start Record";
            this.Start4_btn.UseVisualStyleBackColor = true;
            this.Start4_btn.Click += new System.EventHandler(this.Start1_btn_Click);
            // 
            // Start5_btn
            // 
            this.Start5_btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.Start5_btn.Enabled = false;
            this.Start5_btn.Location = new System.Drawing.Point(373, 598);
            this.Start5_btn.Name = "Start5_btn";
            this.Start5_btn.Size = new System.Drawing.Size(247, 23);
            this.Start5_btn.TabIndex = 6;
            this.Start5_btn.Tag = "4";
            this.Start5_btn.Text = "Open Live And Start Record";
            this.Start5_btn.UseVisualStyleBackColor = true;
            this.Start5_btn.Click += new System.EventHandler(this.Start1_btn_Click);
            // 
            // EncodingConfigs_lsb
            // 
            this.EncodingConfigs_lsb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.EncodingConfigs_lsb.FormattingEnabled = true;
            this.EncodingConfigs_lsb.HorizontalScrollbar = true;
            this.EncodingConfigs_lsb.Location = new System.Drawing.Point(660, 388);
            this.EncodingConfigs_lsb.Name = "EncodingConfigs_lsb";
            this.EncodingConfigs_lsb.Size = new System.Drawing.Size(422, 69);
            this.EncodingConfigs_lsb.TabIndex = 7;
            // 
            // SavePath_txb
            // 
            this.SavePath_txb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.SavePath_txb.Location = new System.Drawing.Point(660, 482);
            this.SavePath_txb.Name = "SavePath_txb";
            this.SavePath_txb.Size = new System.Drawing.Size(422, 20);
            this.SavePath_txb.TabIndex = 13;
            // 
            // SelectSaveFolder_btn
            // 
            this.SelectSaveFolder_btn.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.SelectSaveFolder_btn.Location = new System.Drawing.Point(660, 509);
            this.SelectSaveFolder_btn.Name = "SelectSaveFolder_btn";
            this.SelectSaveFolder_btn.Size = new System.Drawing.Size(422, 23);
            this.SelectSaveFolder_btn.TabIndex = 14;
            this.SelectSaveFolder_btn.Text = "Save Folder";
            this.SelectSaveFolder_btn.UseVisualStyleBackColor = true;
            this.SelectSaveFolder_btn.Click += new System.EventHandler(this.SelectSaveFolder_btn_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label1.Location = new System.Drawing.Point(12, 318);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 15;
            this.label1.Text = "label1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label2.Location = new System.Drawing.Point(371, 318);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 13);
            this.label2.TabIndex = 16;
            this.label2.Text = "label2";
            // 
            // timer1
            // 
            this.timer1.Enabled = true;
            this.timer1.Interval = 400;
            this.timer1.Tick += new System.EventHandler(this.Timer1_Tick);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label3.Location = new System.Drawing.Point(748, 318);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 13);
            this.label3.TabIndex = 17;
            this.label3.Text = "label3";
            // 
            // label4
            // 
            this.label4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label4.AutoSize = true;
            this.label4.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label4.Location = new System.Drawing.Point(9, 669);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "label4";
            // 
            // label5
            // 
            this.label5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label5.AutoSize = true;
            this.label5.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.label5.Location = new System.Drawing.Point(371, 669);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(35, 13);
            this.label5.TabIndex = 19;
            this.label5.Text = "label5";
            // 
            // textBoxLiveName1
            // 
            this.textBoxLiveName1.Location = new System.Drawing.Point(11, 263);
            this.textBoxLiveName1.Name = "textBoxLiveName1";
            this.textBoxLiveName1.Size = new System.Drawing.Size(249, 20);
            this.textBoxLiveName1.TabIndex = 20;
            // 
            // textBoxLiveName2
            // 
            this.textBoxLiveName2.Location = new System.Drawing.Point(374, 263);
            this.textBoxLiveName2.Name = "textBoxLiveName2";
            this.textBoxLiveName2.Size = new System.Drawing.Size(249, 20);
            this.textBoxLiveName2.TabIndex = 21;
            // 
            // textBoxLiveName3
            // 
            this.textBoxLiveName3.Location = new System.Drawing.Point(748, 263);
            this.textBoxLiveName3.Name = "textBoxLiveName3";
            this.textBoxLiveName3.Size = new System.Drawing.Size(249, 20);
            this.textBoxLiveName3.TabIndex = 22;
            // 
            // textBoxLiveName4
            // 
            this.textBoxLiveName4.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.textBoxLiveName4.Location = new System.Drawing.Point(9, 627);
            this.textBoxLiveName4.Name = "textBoxLiveName4";
            this.textBoxLiveName4.Size = new System.Drawing.Size(249, 20);
            this.textBoxLiveName4.TabIndex = 23;
            // 
            // textBoxLiveName5
            // 
            this.textBoxLiveName5.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.textBoxLiveName5.Location = new System.Drawing.Point(372, 627);
            this.textBoxLiveName5.Name = "textBoxLiveName5";
            this.textBoxLiveName5.Size = new System.Drawing.Size(249, 20);
            this.textBoxLiveName5.TabIndex = 24;
            // 
            // progressBarCPU
            // 
            this.progressBarCPU.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.progressBarCPU.Location = new System.Drawing.Point(660, 693);
            this.progressBarCPU.Name = "progressBarCPU";
            this.progressBarCPU.Size = new System.Drawing.Size(422, 23);
            this.progressBarCPU.TabIndex = 25;
            // 
            // label6
            // 
            this.label6.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(657, 736);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(64, 13);
            this.label6.TabIndex = 26;
            this.label6.Text = "CPU_LOAD";
            // 
            // CPUload_lbl
            // 
            this.CPUload_lbl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CPUload_lbl.AutoSize = true;
            this.CPUload_lbl.Location = new System.Drawing.Point(727, 736);
            this.CPUload_lbl.Name = "CPUload_lbl";
            this.CPUload_lbl.Size = new System.Drawing.Size(39, 13);
            this.CPUload_lbl.TabIndex = 27;
            this.CPUload_lbl.Text = "0.00 %";
            // 
            // CPU
            // 
            this.CPU.CategoryName = "Processor";
            this.CPU.CounterName = "% Processor Time";
            this.CPU.InstanceName = "_Total";
            // 
            // CheckCPU_lsb
            // 
            this.CheckCPU_lsb.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.CheckCPU_lsb.FormattingEnabled = true;
            this.CheckCPU_lsb.Location = new System.Drawing.Point(660, 540);
            this.CheckCPU_lsb.Name = "CheckCPU_lsb";
            this.CheckCPU_lsb.Size = new System.Drawing.Size(422, 147);
            this.CheckCPU_lsb.TabIndex = 28;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1094, 754);
            this.Controls.Add(this.CheckCPU_lsb);
            this.Controls.Add(this.CPUload_lbl);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.progressBarCPU);
            this.Controls.Add(this.textBoxLiveName5);
            this.Controls.Add(this.textBoxLiveName4);
            this.Controls.Add(this.textBoxLiveName3);
            this.Controls.Add(this.textBoxLiveName2);
            this.Controls.Add(this.textBoxLiveName1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.SelectSaveFolder_btn);
            this.Controls.Add(this.SavePath_txb);
            this.Controls.Add(this.EncodingConfigs_lsb);
            this.Controls.Add(this.Start5_btn);
            this.Controls.Add(this.Start4_btn);
            this.Controls.Add(this.Start3_btn);
            this.Controls.Add(this.Start2_btn);
            this.Controls.Add(this.Start1_btn);
            this.Controls.Add(this.panel5);
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.CPU)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Button Start1_btn;
        private System.Windows.Forms.Button Start2_btn;
        private System.Windows.Forms.Button Start3_btn;
        private System.Windows.Forms.Button Start4_btn;
        private System.Windows.Forms.Button Start5_btn;
        private System.Windows.Forms.ListBox EncodingConfigs_lsb;
        private System.Windows.Forms.TextBox SavePath_txb;
        private System.Windows.Forms.Button SelectSaveFolder_btn;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBoxLiveName1;
        private System.Windows.Forms.TextBox textBoxLiveName2;
        private System.Windows.Forms.TextBox textBoxLiveName3;
        private System.Windows.Forms.TextBox textBoxLiveName4;
        private System.Windows.Forms.TextBox textBoxLiveName5;
        private System.Windows.Forms.ProgressBar progressBarCPU;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label CPUload_lbl;
        private System.Diagnostics.PerformanceCounter CPU;
        private System.Windows.Forms.ListBox CheckCPU_lsb;
    }
}

