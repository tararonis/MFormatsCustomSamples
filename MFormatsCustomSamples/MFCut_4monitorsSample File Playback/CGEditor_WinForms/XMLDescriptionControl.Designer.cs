namespace CGEditor_WinForms
{
    partial class XMLDescriptionControl
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
            this.ApplyXMLButton = new System.Windows.Forms.Button();
            this.XMLBody = new XmlEditor();
            this.SuspendLayout();
            // 
            // ApplyXMLButton
            // 
            this.ApplyXMLButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.ApplyXMLButton.Location = new System.Drawing.Point(627, 468);
            this.ApplyXMLButton.Name = "ApplyXMLButton";
            this.ApplyXMLButton.Size = new System.Drawing.Size(170, 33);
            this.ApplyXMLButton.TabIndex = 3;
            this.ApplyXMLButton.Text = "APPLY";
            this.ApplyXMLButton.UseVisualStyleBackColor = true;
            this.ApplyXMLButton.Click += new System.EventHandler(this.ApplyXMLButton_Click);
            // 
            // XMLBody
            // 
            this.XMLBody.AllowXmlFormatting = true;
            this.XMLBody.AutoSize = true;
            this.XMLBody.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.XMLBody.Dock = System.Windows.Forms.DockStyle.Fill;
            this.XMLBody.Location = new System.Drawing.Point(0, 0);
            this.XMLBody.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.XMLBody.Name = "XMLBody";
            this.XMLBody.ReadOnly = false;
            this.XMLBody.Size = new System.Drawing.Size(800, 504);
            this.XMLBody.TabIndex = 2;
            // 
            // XMLDescriptionControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.ApplyXMLButton);
            this.Controls.Add(this.XMLBody);
            this.Name = "XMLDescriptionControl";
            this.Size = new System.Drawing.Size(800, 504);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button ApplyXMLButton;
        private XmlEditor XMLBody;
    }
}
