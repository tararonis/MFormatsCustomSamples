namespace SampleFilePlayback
{
    partial class OverlayHTMLWindow
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
            this.buttonInit = new System.Windows.Forms.Button();
            this.textBoxPathOrURL = new System.Windows.Forms.TextBox();
            this.buttonOpenFile = new System.Windows.Forms.Button();
            this.panelPreview = new System.Windows.Forms.Panel();
            this.buttonPrev = new System.Windows.Forms.Button();
            this.buttonReload = new System.Windows.Forms.Button();
            this.buttonNext = new System.Windows.Forms.Button();
            this.checkBoxAR = new System.Windows.Forms.CheckBox();
            this.textBoxJS = new System.Windows.Forms.TextBox();
            this.buttonExecJS = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.checkBoxVideoPrev = new System.Windows.Forms.CheckBox();
            this.checkBoxAudioPrev = new System.Windows.Forms.CheckBox();
            this.checkBoxDeint = new System.Windows.Forms.CheckBox();
            this.buttonAddPreset = new System.Windows.Forms.Button();
            this.buttonUpdatePreset = new System.Windows.Forms.Button();
            this.buttonRemove = new System.Windows.Forms.Button();
            this.buttonPic = new System.Windows.Forms.Button();
            this.buttonResetProps = new System.Windows.Forms.Button();
            this.buttonClosePage = new System.Windows.Forms.Button();
            this.mfAttributesListPresets1 = new SampleFilePlayback.MFAttributesListPresets();
            this.mfAttributesList = new SampleFilePlayback.MFAttributesList();
            this.SuspendLayout();
            // 
            // buttonInit
            // 
            this.buttonInit.Location = new System.Drawing.Point(12, 12);
            this.buttonInit.Name = "buttonInit";
            this.buttonInit.Size = new System.Drawing.Size(75, 23);
            this.buttonInit.TabIndex = 0;
            this.buttonInit.Text = "InitSource";
            this.buttonInit.UseVisualStyleBackColor = true;
            this.buttonInit.Click += new System.EventHandler(this.buttonInit_Click);
            // 
            // textBoxPathOrURL
            // 
            this.textBoxPathOrURL.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxPathOrURL.Location = new System.Drawing.Point(93, 14);
            this.textBoxPathOrURL.Name = "textBoxPathOrURL";
            this.textBoxPathOrURL.Size = new System.Drawing.Size(552, 20);
            this.textBoxPathOrURL.TabIndex = 1;
            this.textBoxPathOrURL.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxPathOrURL_KeyDown);
            // 
            // buttonOpenFile
            // 
            this.buttonOpenFile.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonOpenFile.Location = new System.Drawing.Point(651, 12);
            this.buttonOpenFile.Name = "buttonOpenFile";
            this.buttonOpenFile.Size = new System.Drawing.Size(30, 23);
            this.buttonOpenFile.TabIndex = 2;
            this.buttonOpenFile.Text = "...";
            this.buttonOpenFile.UseVisualStyleBackColor = true;
            this.buttonOpenFile.Click += new System.EventHandler(this.buttonOpenFile_Click);
            // 
            // panelPreview
            // 
            this.panelPreview.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelPreview.Location = new System.Drawing.Point(278, 57);
            this.panelPreview.Name = "panelPreview";
            this.panelPreview.Size = new System.Drawing.Size(750, 399);
            this.panelPreview.TabIndex = 4;
            this.panelPreview.MouseDown += new System.Windows.Forms.MouseEventHandler(this.panelPreview_MouseDown);
            this.panelPreview.MouseMove += new System.Windows.Forms.MouseEventHandler(this.panelPreview_MouseMove);
            this.panelPreview.MouseUp += new System.Windows.Forms.MouseEventHandler(this.panelPreview_MouseUp);
            // 
            // buttonPrev
            // 
            this.buttonPrev.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPrev.Location = new System.Drawing.Point(906, 12);
            this.buttonPrev.Name = "buttonPrev";
            this.buttonPrev.Size = new System.Drawing.Size(29, 23);
            this.buttonPrev.TabIndex = 5;
            this.buttonPrev.Text = "<-";
            this.buttonPrev.UseVisualStyleBackColor = true;
            this.buttonPrev.Click += new System.EventHandler(this.buttonPrev_Click);
            // 
            // buttonReload
            // 
            this.buttonReload.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonReload.Location = new System.Drawing.Point(942, 12);
            this.buttonReload.Name = "buttonReload";
            this.buttonReload.Size = new System.Drawing.Size(51, 23);
            this.buttonReload.TabIndex = 6;
            this.buttonReload.Text = "R";
            this.buttonReload.UseVisualStyleBackColor = true;
            this.buttonReload.Click += new System.EventHandler(this.buttonReload_Click);
            // 
            // buttonNext
            // 
            this.buttonNext.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonNext.Location = new System.Drawing.Point(999, 12);
            this.buttonNext.Name = "buttonNext";
            this.buttonNext.Size = new System.Drawing.Size(29, 23);
            this.buttonNext.TabIndex = 7;
            this.buttonNext.Text = "->";
            this.buttonNext.UseVisualStyleBackColor = true;
            this.buttonNext.Click += new System.EventHandler(this.buttonNext_Click);
            // 
            // checkBoxAR
            // 
            this.checkBoxAR.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxAR.AutoSize = true;
            this.checkBoxAR.Checked = true;
            this.checkBoxAR.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAR.Location = new System.Drawing.Point(396, 463);
            this.checkBoxAR.Name = "checkBoxAR";
            this.checkBoxAR.Size = new System.Drawing.Size(82, 17);
            this.checkBoxAR.TabIndex = 8;
            this.checkBoxAR.Text = "Preview AR";
            this.checkBoxAR.UseVisualStyleBackColor = true;
            this.checkBoxAR.CheckedChanged += new System.EventHandler(this.checkBoxAR_CheckedChanged);
            // 
            // textBoxJS
            // 
            this.textBoxJS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxJS.Location = new System.Drawing.Point(733, 486);
            this.textBoxJS.Multiline = true;
            this.textBoxJS.Name = "textBoxJS";
            this.textBoxJS.Size = new System.Drawing.Size(295, 117);
            this.textBoxJS.TabIndex = 10;
            this.textBoxJS.Text = "document.getElementById(\'email\').value = \'YourEmail\';\r\ndocument.getElementById(\'p" +
    "ass\').value = \'YourPass\';\r\ndocument.getElementById(\'loginbutton\').click();\r\n";
            // 
            // buttonExecJS
            // 
            this.buttonExecJS.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonExecJS.Location = new System.Drawing.Point(834, 609);
            this.buttonExecJS.Name = "buttonExecJS";
            this.buttonExecJS.Size = new System.Drawing.Size(101, 23);
            this.buttonExecJS.TabIndex = 11;
            this.buttonExecJS.Text = "Run Java script";
            this.buttonExecJS.UseVisualStyleBackColor = true;
            this.buttonExecJS.Click += new System.EventHandler(this.buttonExecJS_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 13;
            this.label1.Text = "Properties:";
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 470);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(45, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "Presets:";
            // 
            // label3
            // 
            this.label3.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(733, 470);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(61, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Java script:";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(277, 41);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(48, 13);
            this.label4.TabIndex = 13;
            this.label4.Text = "Preview:";
            // 
            // checkBoxVideoPrev
            // 
            this.checkBoxVideoPrev.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxVideoPrev.AutoSize = true;
            this.checkBoxVideoPrev.Checked = true;
            this.checkBoxVideoPrev.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxVideoPrev.Location = new System.Drawing.Point(278, 463);
            this.checkBoxVideoPrev.Name = "checkBoxVideoPrev";
            this.checkBoxVideoPrev.Size = new System.Drawing.Size(53, 17);
            this.checkBoxVideoPrev.TabIndex = 15;
            this.checkBoxVideoPrev.Text = "Video";
            this.checkBoxVideoPrev.UseVisualStyleBackColor = true;
            this.checkBoxVideoPrev.CheckedChanged += new System.EventHandler(this.checkBoxVideoPrev_CheckedChanged);
            // 
            // checkBoxAudioPrev
            // 
            this.checkBoxAudioPrev.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxAudioPrev.AutoSize = true;
            this.checkBoxAudioPrev.Location = new System.Drawing.Point(337, 463);
            this.checkBoxAudioPrev.Name = "checkBoxAudioPrev";
            this.checkBoxAudioPrev.Size = new System.Drawing.Size(53, 17);
            this.checkBoxAudioPrev.TabIndex = 16;
            this.checkBoxAudioPrev.Text = "Audio";
            this.checkBoxAudioPrev.UseVisualStyleBackColor = true;
            this.checkBoxAudioPrev.CheckedChanged += new System.EventHandler(this.checkBoxAudioPrev_CheckedChanged);
            // 
            // checkBoxDeint
            // 
            this.checkBoxDeint.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.checkBoxDeint.AutoSize = true;
            this.checkBoxDeint.Location = new System.Drawing.Point(484, 462);
            this.checkBoxDeint.Name = "checkBoxDeint";
            this.checkBoxDeint.Size = new System.Drawing.Size(80, 17);
            this.checkBoxDeint.TabIndex = 17;
            this.checkBoxDeint.Text = "Deinterlace";
            this.checkBoxDeint.UseVisualStyleBackColor = true;
            this.checkBoxDeint.CheckedChanged += new System.EventHandler(this.checkBoxDeint_CheckedChanged);
            // 
            // buttonAddPreset
            // 
            this.buttonAddPreset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonAddPreset.Location = new System.Drawing.Point(25, 433);
            this.buttonAddPreset.Name = "buttonAddPreset";
            this.buttonAddPreset.Size = new System.Drawing.Size(75, 23);
            this.buttonAddPreset.TabIndex = 18;
            this.buttonAddPreset.Text = "Add preset";
            this.buttonAddPreset.UseVisualStyleBackColor = true;
            this.buttonAddPreset.Click += new System.EventHandler(this.buttonAddPreset_Click);
            // 
            // buttonUpdatePreset
            // 
            this.buttonUpdatePreset.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonUpdatePreset.Location = new System.Drawing.Point(106, 433);
            this.buttonUpdatePreset.Name = "buttonUpdatePreset";
            this.buttonUpdatePreset.Size = new System.Drawing.Size(75, 23);
            this.buttonUpdatePreset.TabIndex = 19;
            this.buttonUpdatePreset.Text = "Update";
            this.buttonUpdatePreset.UseVisualStyleBackColor = true;
            this.buttonUpdatePreset.Click += new System.EventHandler(this.buttonUpdatePreset_Click);
            // 
            // buttonRemove
            // 
            this.buttonRemove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonRemove.Location = new System.Drawing.Point(187, 433);
            this.buttonRemove.Name = "buttonRemove";
            this.buttonRemove.Size = new System.Drawing.Size(75, 23);
            this.buttonRemove.TabIndex = 20;
            this.buttonRemove.Text = "Remove";
            this.buttonRemove.UseVisualStyleBackColor = true;
            this.buttonRemove.Click += new System.EventHandler(this.buttonRemove_Click);
            // 
            // buttonPic
            // 
            this.buttonPic.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonPic.Location = new System.Drawing.Point(788, 12);
            this.buttonPic.Name = "buttonPic";
            this.buttonPic.Size = new System.Drawing.Size(103, 23);
            this.buttonPic.TabIndex = 21;
            this.buttonPic.Text = "Send screenshot";
            this.buttonPic.UseVisualStyleBackColor = true;
            this.buttonPic.Click += new System.EventHandler(this.buttonPic_Click);
            // 
            // buttonResetProps
            // 
            this.buttonResetProps.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonResetProps.Location = new System.Drawing.Point(106, 403);
            this.buttonResetProps.Name = "buttonResetProps";
            this.buttonResetProps.Size = new System.Drawing.Size(75, 23);
            this.buttonResetProps.TabIndex = 22;
            this.buttonResetProps.Text = "Reset props";
            this.buttonResetProps.UseVisualStyleBackColor = true;
            this.buttonResetProps.Click += new System.EventHandler(this.buttonResetProps_Click);
            // 
            // buttonClosePage
            // 
            this.buttonClosePage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonClosePage.Location = new System.Drawing.Point(697, 12);
            this.buttonClosePage.Name = "buttonClosePage";
            this.buttonClosePage.Size = new System.Drawing.Size(75, 23);
            this.buttonClosePage.TabIndex = 23;
            this.buttonClosePage.Text = "Close page";
            this.buttonClosePage.UseVisualStyleBackColor = true;
            this.buttonClosePage.Click += new System.EventHandler(this.buttonClosePage_Click);
            // 
            // mfAttributesListPresets1
            // 
            this.mfAttributesListPresets1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mfAttributesListPresets1.FullRowSelect = true;
            this.mfAttributesListPresets1.GridLines = true;
            this.mfAttributesListPresets1.Location = new System.Drawing.Point(12, 486);
            this.mfAttributesListPresets1.Name = "mfAttributesListPresets1";
            this.mfAttributesListPresets1.ShowItemToolTips = true;
            this.mfAttributesListPresets1.Size = new System.Drawing.Size(715, 146);
            this.mfAttributesListPresets1.TabIndex = 12;
            this.mfAttributesListPresets1.UseCompatibleStateImageBehavior = false;
            this.mfAttributesListPresets1.View = System.Windows.Forms.View.Details;
            // 
            // mfAttributesList
            // 
            this.mfAttributesList.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.mfAttributesList.FullRowSelect = true;
            this.mfAttributesList.GridLines = true;
            this.mfAttributesList.Location = new System.Drawing.Point(12, 57);
            this.mfAttributesList.Name = "mfAttributesList";
            this.mfAttributesList.ShowItemToolTips = true;
            this.mfAttributesList.Size = new System.Drawing.Size(260, 340);
            this.mfAttributesList.TabIndex = 3;
            this.mfAttributesList.UseCompatibleStateImageBehavior = false;
            this.mfAttributesList.View = System.Windows.Forms.View.Details;
            // 
            // OverlayHTMLWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1041, 644);
            this.Controls.Add(this.buttonClosePage);
            this.Controls.Add(this.buttonResetProps);
            this.Controls.Add(this.buttonPic);
            this.Controls.Add(this.buttonRemove);
            this.Controls.Add(this.buttonUpdatePreset);
            this.Controls.Add(this.buttonAddPreset);
            this.Controls.Add(this.checkBoxDeint);
            this.Controls.Add(this.checkBoxAudioPrev);
            this.Controls.Add(this.checkBoxVideoPrev);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.mfAttributesListPresets1);
            this.Controls.Add(this.buttonExecJS);
            this.Controls.Add(this.textBoxJS);
            this.Controls.Add(this.checkBoxAR);
            this.Controls.Add(this.buttonNext);
            this.Controls.Add(this.buttonReload);
            this.Controls.Add(this.buttonPrev);
            this.Controls.Add(this.panelPreview);
            this.Controls.Add(this.mfAttributesList);
            this.Controls.Add(this.buttonOpenFile);
            this.Controls.Add(this.textBoxPathOrURL);
            this.Controls.Add(this.buttonInit);
            this.Name = "OverlayHTMLWindow";
            this.Text = "OverlayHTMLWindow";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OverlayHTMLWindow_FormClosed);
            this.SizeChanged += new System.EventHandler(this.OverlayHTMLWindow_SizeChanged);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonInit;
        private System.Windows.Forms.TextBox textBoxPathOrURL;
        private System.Windows.Forms.Button buttonOpenFile;
        private MFAttributesList mfAttributesList;
        private System.Windows.Forms.Panel panelPreview;
        private System.Windows.Forms.Button buttonPrev;
        private System.Windows.Forms.Button buttonReload;
        private System.Windows.Forms.Button buttonNext;
        private System.Windows.Forms.CheckBox checkBoxAR;
        private System.Windows.Forms.TextBox textBoxJS;
        private System.Windows.Forms.Button buttonExecJS;
        private MFAttributesListPresets mfAttributesListPresets1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.CheckBox checkBoxVideoPrev;
        private System.Windows.Forms.CheckBox checkBoxAudioPrev;
        private System.Windows.Forms.CheckBox checkBoxDeint;
        private System.Windows.Forms.Button buttonAddPreset;
        private System.Windows.Forms.Button buttonUpdatePreset;
        private System.Windows.Forms.Button buttonRemove;
        private System.Windows.Forms.Button buttonPic;
        private System.Windows.Forms.Button buttonResetProps;
        private System.Windows.Forms.Button buttonClosePage;
    }
}