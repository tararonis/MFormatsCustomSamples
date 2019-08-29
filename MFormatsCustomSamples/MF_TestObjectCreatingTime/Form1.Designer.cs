namespace MF_TestObjectCreatingTime
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
            this.Stat_lsb = new System.Windows.Forms.ListBox();
            this.Objects_lsb = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // Stat_lsb
            // 
            this.Stat_lsb.FormattingEnabled = true;
            this.Stat_lsb.Location = new System.Drawing.Point(12, 35);
            this.Stat_lsb.Name = "Stat_lsb";
            this.Stat_lsb.Size = new System.Drawing.Size(269, 355);
            this.Stat_lsb.TabIndex = 0;
            // 
            // Objects_lsb
            // 
            this.Objects_lsb.FormattingEnabled = true;
            this.Objects_lsb.Location = new System.Drawing.Point(296, 35);
            this.Objects_lsb.Name = "Objects_lsb";
            this.Objects_lsb.Size = new System.Drawing.Size(120, 173);
            this.Objects_lsb.TabIndex = 1;
            this.Objects_lsb.SelectedIndexChanged += new System.EventHandler(this.Objects_lsb_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(30, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Time";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(293, 9);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "SelectObj";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(438, 402);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Objects_lsb);
            this.Controls.Add(this.Stat_lsb);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox Stat_lsb;
        private System.Windows.Forms.ListBox Objects_lsb;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}

