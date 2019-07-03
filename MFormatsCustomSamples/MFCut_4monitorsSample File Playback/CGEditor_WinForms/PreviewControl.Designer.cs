namespace CGEditor_WinForms
{
    partial class PreviewControl
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
            this.labelStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.imageCtrl = new System.Windows.Forms.PictureBox();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageCtrl)).BeginInit();
            this.SuspendLayout();
            // 
            // labelStatus
            // 
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(60, 25);
            this.labelStatus.Text = "Status";
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.labelStatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 0);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(0, 30);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 3;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // imageCtrl
            // 
            this.imageCtrl.BackgroundImage = global::CGEditor_WinForms.Properties.Resources.Tile2;
            this.imageCtrl.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imageCtrl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageCtrl.Location = new System.Drawing.Point(0, 0);
            this.imageCtrl.Name = "imageCtrl";
            this.imageCtrl.Size = new System.Drawing.Size(0, 30);
            this.imageCtrl.TabIndex = 2;
            this.imageCtrl.TabStop = false;
            this.imageCtrl.DragDrop += new System.Windows.Forms.DragEventHandler(this.imageCtrl_DragDrop);
            this.imageCtrl.DragOver += new System.Windows.Forms.DragEventHandler(this.imageCtrl_DragOver);
            this.imageCtrl.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.imageCtrl_MouseDoubleClick);
            this.imageCtrl.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseDown);
            this.imageCtrl.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseMove);
            this.imageCtrl.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pictureBox1_MouseUp);
            // 
            // PreviewControl
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.imageCtrl);
            this.Name = "PreviewControl";
            this.Size = new System.Drawing.Size(0, 30);
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.GeneralForm_DragDrop);
            this.DragOver += new System.Windows.Forms.DragEventHandler(this.GeneralForm_DragOver);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageCtrl)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ToolStripStatusLabel labelStatus;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.PictureBox imageCtrl;
    }
}
