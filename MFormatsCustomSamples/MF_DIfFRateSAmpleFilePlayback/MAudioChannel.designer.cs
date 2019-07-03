namespace SampleFilePlayback
{
    partial class MAudioChannel
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
            this.SuspendLayout();
            // 
            // MAudioChannel
            // 
            this.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(100)))), ((int)(((byte)(180)))));
            this.Name = "MAudioChMeter";
            this.Size = new System.Drawing.Size(14, 300);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.MAudioCh_Paint);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.MAudioCh_MouseMove);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.MAudioCh_MouseDown);
            this.ResumeLayout(false);

        }

        #endregion



    }
}
