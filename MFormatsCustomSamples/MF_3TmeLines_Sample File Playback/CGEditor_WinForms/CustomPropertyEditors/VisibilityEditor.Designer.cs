namespace CGEditor_WinForms.CustomPropertyEditors
{
    partial class VisibilityEditor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(VisibilityEditor));
            this.ToggleVisible = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // ToggleVisible
            // 
            this.ToggleVisible.Appearance = System.Windows.Forms.Appearance.Button;
            this.ToggleVisible.AutoSize = true;
            this.ToggleVisible.BackColor = System.Drawing.Color.White;
            this.ToggleVisible.Image = ((System.Drawing.Image)(resources.GetObject("ToggleVisible.Image")));
            this.ToggleVisible.Location = new System.Drawing.Point(3, 3);
            this.ToggleVisible.MinimumSize = new System.Drawing.Size(100, 67);
            this.ToggleVisible.Name = "ToggleVisible";
            this.ToggleVisible.Size = new System.Drawing.Size(100, 67);
            this.ToggleVisible.TabIndex = 0;
            this.ToggleVisible.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.ToggleVisible.UseVisualStyleBackColor = false;
            this.ToggleVisible.CheckedChanged += new System.EventHandler(this.ToggleVisible_CheckedChanged);
            // 
            // VisibilityEditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ToggleVisible);
            this.Name = "VisibilityEditor";
            this.Size = new System.Drawing.Size(108, 74);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox ToggleVisible;
    }
}
