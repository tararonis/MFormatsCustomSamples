namespace SampleFilePlayback
{
    partial class MFAttributesListPresets
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
            this.columnSite = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnProps = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnJS = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.SuspendLayout();
            // 
            // columnSite
            // 
            this.columnSite.Text = "Site";
            // 
            // columnProps
            // 
            this.columnProps.Text = "Props";
            this.columnProps.Width = 120;
            // 
            // columnJS
            // 
            this.columnJS.Text = "JS";
            // 
            // MFAttributesListPresets
            // 
            this.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnSite,
            this.columnProps,
            this.columnJS});
            this.Size = new System.Drawing.Size(210, 214);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ColumnHeader columnSite;
        private System.Windows.Forms.ColumnHeader columnProps;
        private System.Windows.Forms.ColumnHeader columnJS;
    }
}
