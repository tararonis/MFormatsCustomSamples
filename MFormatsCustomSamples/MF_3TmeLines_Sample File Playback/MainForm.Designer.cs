namespace SampleFilePlayback
{
	partial class MainForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.trackBarSeek = new System.Windows.Forms.TrackBar();
            this.panelPreview = new System.Windows.Forms.Panel();
            this.buttonSetFile = new System.Windows.Forms.Button();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.labelStatus = new System.Windows.Forms.Label();
            this.labelDescr = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.buttonReverse = new System.Windows.Forms.Button();
            this.panelPos = new System.Windows.Forms.Panel();
            this.labelPos = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.comboBoxTSProg = new System.Windows.Forms.ComboBox();
            this.buttonFastForward = new System.Windows.Forms.Button();
            this.buttonFastBackward = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.comboBoxAudioTrack = new System.Windows.Forms.ComboBox();
            this.textBoxTC = new System.Windows.Forms.TextBox();
            this.textBoxToPos = new System.Windows.Forms.TextBox();
            this.checkBoxLoop = new System.Windows.Forms.CheckBox();
            this.buttonFrBack = new System.Windows.Forms.Button();
            this.buttonPlay = new System.Windows.Forms.Button();
            this.buttonFrFwd = new System.Windows.Forms.Button();
            this.checkBoxRenderer = new System.Windows.Forms.CheckBox();
            this.comboBoxRenrerers = new System.Windows.Forms.ComboBox();
            this.checkBoxAudio = new System.Windows.Forms.CheckBox();
            this.trackBarVolume = new System.Windows.Forms.TrackBar();
            this.buttonLogo = new System.Windows.Forms.Button();
            this.pictureLogo = new System.Windows.Forms.PictureBox();
            this.comboBoxVideoFormat = new System.Windows.Forms.ComboBox();
            this.checkBoxVideo = new System.Windows.Forms.CheckBox();
            this.checkBoxCG = new System.Windows.Forms.CheckBox();
            this.buttonCGProps = new System.Windows.Forms.Button();
            this.checkBoxCC = new System.Windows.Forms.CheckBox();
            this.timerUpdatePos = new System.Windows.Forms.Timer(this.components);
            this.comboBoxAudioRenderers = new System.Windows.Forms.ComboBox();
            this.CGEditorButton = new System.Windows.Forms.Button();
            this.checkBoxVirtDev = new System.Windows.Forms.CheckBox();
            this.buttonHTMLProps = new System.Windows.Forms.Button();
            this.textBoxHTMLURL = new System.Windows.Forms.TextBox();
            this.checkBoxHTML = new System.Windows.Forms.CheckBox();
            this.mAudioMeter1 = new SampleFilePlayback.MAudioMeter();
            this.Add3TL_btn = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarSeek)).BeginInit();
            this.panel2.SuspendLayout();
            this.panelPos.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVolume)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureLogo)).BeginInit();
            this.SuspendLayout();
            // 
            // trackBarSeek
            // 
            this.trackBarSeek.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBarSeek.Location = new System.Drawing.Point(3, 6);
            this.trackBarSeek.Maximum = 1000;
            this.trackBarSeek.Name = "trackBarSeek";
            this.trackBarSeek.Size = new System.Drawing.Size(701, 45);
            this.trackBarSeek.TabIndex = 4;
            this.trackBarSeek.TickFrequency = 10;
            this.trackBarSeek.ValueChanged += new System.EventHandler(this.trackBarSeek_ValueChanged);
            this.trackBarSeek.MouseDown += new System.Windows.Forms.MouseEventHandler(this.trackBarSeek_MouseDown);
            // 
            // panelPreview
            // 
            this.panelPreview.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelPreview.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(88)))), ((int)(((byte)(143)))));
            this.panelPreview.Location = new System.Drawing.Point(10, 145);
            this.panelPreview.Name = "panelPreview";
            this.panelPreview.Size = new System.Drawing.Size(838, 400);
            this.panelPreview.TabIndex = 5;
            this.panelPreview.SizeChanged += new System.EventHandler(this.panelPreview_SizeChanged);
            // 
            // buttonSetFile
            // 
            this.buttonSetFile.Location = new System.Drawing.Point(10, 78);
            this.buttonSetFile.Name = "buttonSetFile";
            this.buttonSetFile.Size = new System.Drawing.Size(75, 23);
            this.buttonSetFile.TabIndex = 6;
            this.buttonSetFile.Text = "Load File";
            this.buttonSetFile.UseVisualStyleBackColor = true;
            this.buttonSetFile.Click += new System.EventHandler(this.buttonLoadFile_Click);
            // 
            // labelStatus
            // 
            this.labelStatus.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.labelStatus.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(222)))), ((int)(((byte)(232)))), ((int)(((byte)(254)))));
            this.labelStatus.Font = new System.Drawing.Font("Tahoma", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.labelStatus.ForeColor = System.Drawing.Color.Black;
            this.labelStatus.Location = new System.Drawing.Point(10, 106);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Padding = new System.Windows.Forms.Padding(5);
            this.labelStatus.Size = new System.Drawing.Size(838, 36);
            this.labelStatus.TabIndex = 168;
            this.labelStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // labelDescr
            // 
            this.labelDescr.Anchor = System.Windows.Forms.AnchorStyles.Top;
            this.labelDescr.Location = new System.Drawing.Point(239, 12);
            this.labelDescr.Name = "labelDescr";
            this.labelDescr.Size = new System.Drawing.Size(387, 51);
            this.labelDescr.TabIndex = 171;
            this.labelDescr.Text = "Sample File Playback demonstrates the usage of the filter\'s own methods without d" +
    "irectshow environment.";
            this.labelDescr.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // panel2
            // 
            this.panel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel2.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panel2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(222)))), ((int)(((byte)(232)))), ((int)(((byte)(254)))));
            this.panel2.Controls.Add(this.label4);
            this.panel2.Controls.Add(this.label3);
            this.panel2.Controls.Add(this.buttonReverse);
            this.panel2.Controls.Add(this.panelPos);
            this.panel2.Controls.Add(this.label2);
            this.panel2.Controls.Add(this.comboBoxTSProg);
            this.panel2.Controls.Add(this.buttonFastForward);
            this.panel2.Controls.Add(this.buttonFastBackward);
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.comboBoxAudioTrack);
            this.panel2.Controls.Add(this.textBoxTC);
            this.panel2.Controls.Add(this.textBoxToPos);
            this.panel2.Controls.Add(this.checkBoxLoop);
            this.panel2.Controls.Add(this.buttonFrBack);
            this.panel2.Controls.Add(this.buttonPlay);
            this.panel2.Controls.Add(this.buttonFrFwd);
            this.panel2.Controls.Add(this.trackBarSeek);
            this.panel2.Location = new System.Drawing.Point(10, 551);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(707, 86);
            this.panel2.TabIndex = 173;
            // 
            // label4
            // 
            this.label4.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(254, 57);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(28, 13);
            this.label4.TabIndex = 175;
            this.label4.Text = "Pos:";
            // 
            // label3
            // 
            this.label3.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(391, 57);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 13);
            this.label3.TabIndex = 175;
            this.label3.Text = "TC:";
            // 
            // buttonReverse
            // 
            this.buttonReverse.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.buttonReverse.Image = global::SampleFilePlayback.Properties.Resources.reverse;
            this.buttonReverse.Location = new System.Drawing.Point(73, 51);
            this.buttonReverse.Name = "buttonReverse";
            this.buttonReverse.Size = new System.Drawing.Size(28, 25);
            this.buttonReverse.TabIndex = 154;
            this.buttonReverse.UseVisualStyleBackColor = true;
            this.buttonReverse.Click += new System.EventHandler(this.buttonReverse_Click);
            // 
            // panelPos
            // 
            this.panelPos.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelPos.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.panelPos.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(222)))), ((int)(((byte)(232)))), ((int)(((byte)(254)))));
            this.panelPos.Controls.Add(this.labelPos);
            this.panelPos.Location = new System.Drawing.Point(14, 38);
            this.panelPos.Name = "panelPos";
            this.panelPos.Size = new System.Drawing.Size(681, 7);
            this.panelPos.TabIndex = 174;
            // 
            // labelPos
            // 
            this.labelPos.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.labelPos.BackColor = System.Drawing.Color.Lime;
            this.labelPos.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.labelPos.ForeColor = System.Drawing.Color.Lime;
            this.labelPos.Location = new System.Drawing.Point(0, 0);
            this.labelPos.Name = "labelPos";
            this.labelPos.Size = new System.Drawing.Size(681, 7);
            this.labelPos.TabIndex = 151;
            // 
            // label2
            // 
            this.label2.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(617, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(46, 13);
            this.label2.TabIndex = 163;
            this.label2.Text = "TS Prog";
            // 
            // comboBoxTSProg
            // 
            this.comboBoxTSProg.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.comboBoxTSProg.CausesValidation = false;
            this.comboBoxTSProg.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTSProg.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.comboBoxTSProg.FormattingEnabled = true;
            this.comboBoxTSProg.Location = new System.Drawing.Point(663, 51);
            this.comboBoxTSProg.Name = "comboBoxTSProg";
            this.comboBoxTSProg.Size = new System.Drawing.Size(32, 24);
            this.comboBoxTSProg.TabIndex = 162;
            this.comboBoxTSProg.SelectedIndexChanged += new System.EventHandler(this.comboBoxTSProg_SelectedIndexChanged);
            // 
            // buttonFastForward
            // 
            this.buttonFastForward.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.buttonFastForward.Image = global::SampleFilePlayback.Properties.Resources.rewindfwd;
            this.buttonFastForward.Location = new System.Drawing.Point(187, 51);
            this.buttonFastForward.Name = "buttonFastForward";
            this.buttonFastForward.Size = new System.Drawing.Size(28, 25);
            this.buttonFastForward.TabIndex = 161;
            this.buttonFastForward.UseVisualStyleBackColor = true;
            this.buttonFastForward.Click += new System.EventHandler(this.buttonFastForward_Click);
            // 
            // buttonFastBackward
            // 
            this.buttonFastBackward.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.buttonFastBackward.Image = global::SampleFilePlayback.Properties.Resources.rewindback;
            this.buttonFastBackward.Location = new System.Drawing.Point(13, 51);
            this.buttonFastBackward.Name = "buttonFastBackward";
            this.buttonFastBackward.Size = new System.Drawing.Size(28, 25);
            this.buttonFastBackward.TabIndex = 160;
            this.buttonFastBackward.UseVisualStyleBackColor = true;
            this.buttonFastBackward.Click += new System.EventHandler(this.buttonFastBackward_Click);
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(525, 57);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(57, 13);
            this.label1.TabIndex = 159;
            this.label1.Text = "Aud Track";
            // 
            // comboBoxAudioTrack
            // 
            this.comboBoxAudioTrack.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.comboBoxAudioTrack.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAudioTrack.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.comboBoxAudioTrack.FormattingEnabled = true;
            this.comboBoxAudioTrack.Location = new System.Drawing.Point(583, 51);
            this.comboBoxAudioTrack.Name = "comboBoxAudioTrack";
            this.comboBoxAudioTrack.Size = new System.Drawing.Size(32, 24);
            this.comboBoxAudioTrack.TabIndex = 158;
            this.comboBoxAudioTrack.SelectedIndexChanged += new System.EventHandler(this.comboBoxAudioTrack_SelectedIndexChanged);
            // 
            // textBoxTC
            // 
            this.textBoxTC.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.textBoxTC.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBoxTC.Location = new System.Drawing.Point(417, 51);
            this.textBoxTC.Name = "textBoxTC";
            this.textBoxTC.Size = new System.Drawing.Size(104, 24);
            this.textBoxTC.TabIndex = 156;
            this.textBoxTC.Text = "00:00:00:00";
            this.textBoxTC.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxTC.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxTC_KeyDown);
            // 
            // textBoxToPos
            // 
            this.textBoxToPos.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.textBoxToPos.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.textBoxToPos.Location = new System.Drawing.Point(282, 51);
            this.textBoxToPos.Name = "textBoxToPos";
            this.textBoxToPos.Size = new System.Drawing.Size(104, 24);
            this.textBoxToPos.TabIndex = 156;
            this.textBoxToPos.Text = "00:00:00.000";
            this.textBoxToPos.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.textBoxToPos.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxToPos_KeyDown);
            // 
            // checkBoxLoop
            // 
            this.checkBoxLoop.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.checkBoxLoop.Appearance = System.Windows.Forms.Appearance.Button;
            this.checkBoxLoop.Checked = true;
            this.checkBoxLoop.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxLoop.Image = global::SampleFilePlayback.Properties.Resources.loop;
            this.checkBoxLoop.Location = new System.Drawing.Point(217, 51);
            this.checkBoxLoop.Name = "checkBoxLoop";
            this.checkBoxLoop.Size = new System.Drawing.Size(28, 25);
            this.checkBoxLoop.TabIndex = 152;
            this.checkBoxLoop.UseVisualStyleBackColor = true;
            this.checkBoxLoop.CheckedChanged += new System.EventHandler(this.checkBoxLoop_CheckedChanged);
            // 
            // buttonFrBack
            // 
            this.buttonFrBack.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.buttonFrBack.Image = global::SampleFilePlayback.Properties.Resources.frameback2;
            this.buttonFrBack.Location = new System.Drawing.Point(43, 51);
            this.buttonFrBack.Name = "buttonFrBack";
            this.buttonFrBack.Size = new System.Drawing.Size(28, 25);
            this.buttonFrBack.TabIndex = 1;
            this.buttonFrBack.UseVisualStyleBackColor = true;
            this.buttonFrBack.Click += new System.EventHandler(this.buttonFrBack_Click);
            // 
            // buttonPlay
            // 
            this.buttonPlay.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.buttonPlay.Image = global::SampleFilePlayback.Properties.Resources.play;
            this.buttonPlay.Location = new System.Drawing.Point(103, 51);
            this.buttonPlay.Name = "buttonPlay";
            this.buttonPlay.Size = new System.Drawing.Size(52, 25);
            this.buttonPlay.TabIndex = 0;
            this.buttonPlay.UseVisualStyleBackColor = true;
            this.buttonPlay.Click += new System.EventHandler(this.buttonPlay_Click);
            // 
            // buttonFrFwd
            // 
            this.buttonFrFwd.Anchor = System.Windows.Forms.AnchorStyles.Bottom;
            this.buttonFrFwd.Image = global::SampleFilePlayback.Properties.Resources.framefwd21;
            this.buttonFrFwd.Location = new System.Drawing.Point(157, 51);
            this.buttonFrFwd.Name = "buttonFrFwd";
            this.buttonFrFwd.Size = new System.Drawing.Size(28, 25);
            this.buttonFrFwd.TabIndex = 3;
            this.buttonFrFwd.TabStop = false;
            this.buttonFrFwd.UseVisualStyleBackColor = true;
            this.buttonFrFwd.Click += new System.EventHandler(this.buttonFrFwd_Click);
            // 
            // checkBoxRenderer
            // 
            this.checkBoxRenderer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxRenderer.AutoSize = true;
            this.checkBoxRenderer.Enabled = false;
            this.checkBoxRenderer.Location = new System.Drawing.Point(729, 583);
            this.checkBoxRenderer.Name = "checkBoxRenderer";
            this.checkBoxRenderer.Size = new System.Drawing.Size(61, 17);
            this.checkBoxRenderer.TabIndex = 176;
            this.checkBoxRenderer.Text = "Output:";
            this.checkBoxRenderer.UseVisualStyleBackColor = true;
            this.checkBoxRenderer.CheckedChanged += new System.EventHandler(this.checkBox_renderer_CheckedChanged);
            // 
            // comboBoxRenrerers
            // 
            this.comboBoxRenrerers.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxRenrerers.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxRenrerers.Enabled = false;
            this.comboBoxRenrerers.FormattingEnabled = true;
            this.comboBoxRenrerers.Location = new System.Drawing.Point(811, 581);
            this.comboBoxRenrerers.Name = "comboBoxRenrerers";
            this.comboBoxRenrerers.Size = new System.Drawing.Size(121, 21);
            this.comboBoxRenrerers.TabIndex = 175;
            this.comboBoxRenrerers.SelectedIndexChanged += new System.EventHandler(this.renderersCombobox_SelectedIndexChanged);
            // 
            // checkBoxAudio
            // 
            this.checkBoxAudio.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxAudio.AutoSize = true;
            this.checkBoxAudio.Checked = true;
            this.checkBoxAudio.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxAudio.Location = new System.Drawing.Point(779, 552);
            this.checkBoxAudio.Name = "checkBoxAudio";
            this.checkBoxAudio.Size = new System.Drawing.Size(61, 17);
            this.checkBoxAudio.TabIndex = 177;
            this.checkBoxAudio.Text = "Volume";
            this.checkBoxAudio.UseVisualStyleBackColor = true;
            this.checkBoxAudio.CheckedChanged += new System.EventHandler(this.checkBoxAudio_CheckedChanged);
            // 
            // trackBarVolume
            // 
            this.trackBarVolume.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBarVolume.Location = new System.Drawing.Point(837, 549);
            this.trackBarVolume.Margin = new System.Windows.Forms.Padding(2);
            this.trackBarVolume.Maximum = 100;
            this.trackBarVolume.Name = "trackBarVolume";
            this.trackBarVolume.Size = new System.Drawing.Size(84, 45);
            this.trackBarVolume.TabIndex = 175;
            this.trackBarVolume.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBarVolume.Value = 100;
            this.trackBarVolume.Scroll += new System.EventHandler(this.trackBar1_Scroll);
            // 
            // buttonLogo
            // 
            this.buttonLogo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLogo.Location = new System.Drawing.Point(853, 26);
            this.buttonLogo.Name = "buttonLogo";
            this.buttonLogo.Size = new System.Drawing.Size(75, 23);
            this.buttonLogo.TabIndex = 174;
            this.buttonLogo.Text = "Add logo";
            this.buttonLogo.UseVisualStyleBackColor = true;
            this.buttonLogo.Click += new System.EventHandler(this.buttonLogo_Click);
            // 
            // pictureLogo
            // 
            this.pictureLogo.Cursor = System.Windows.Forms.Cursors.Hand;
            this.pictureLogo.Image = ((System.Drawing.Image)(resources.GetObject("pictureLogo.Image")));
            this.pictureLogo.InitialImage = ((System.Drawing.Image)(resources.GetObject("pictureLogo.InitialImage")));
            this.pictureLogo.Location = new System.Drawing.Point(10, 12);
            this.pictureLogo.Name = "pictureLogo";
            this.pictureLogo.Size = new System.Drawing.Size(193, 51);
            this.pictureLogo.TabIndex = 170;
            this.pictureLogo.TabStop = false;
            // 
            // comboBoxVideoFormat
            // 
            this.comboBoxVideoFormat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxVideoFormat.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxVideoFormat.FormattingEnabled = true;
            this.comboBoxVideoFormat.Location = new System.Drawing.Point(729, 613);
            this.comboBoxVideoFormat.Name = "comboBoxVideoFormat";
            this.comboBoxVideoFormat.Size = new System.Drawing.Size(203, 21);
            this.comboBoxVideoFormat.TabIndex = 22;
            this.comboBoxVideoFormat.SelectedIndexChanged += new System.EventHandler(this.comboVF_SelectedIndexChanged);
            // 
            // checkBoxVideo
            // 
            this.checkBoxVideo.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxVideo.AutoSize = true;
            this.checkBoxVideo.Checked = true;
            this.checkBoxVideo.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxVideo.Location = new System.Drawing.Point(729, 552);
            this.checkBoxVideo.Name = "checkBoxVideo";
            this.checkBoxVideo.Size = new System.Drawing.Size(44, 17);
            this.checkBoxVideo.TabIndex = 178;
            this.checkBoxVideo.Text = "Vid.";
            this.checkBoxVideo.UseVisualStyleBackColor = true;
            this.checkBoxVideo.CheckedChanged += new System.EventHandler(this.checkBoxVideo_CheckedChanged);
            // 
            // checkBoxCG
            // 
            this.checkBoxCG.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxCG.AutoSize = true;
            this.checkBoxCG.Enabled = false;
            this.checkBoxCG.Location = new System.Drawing.Point(726, 59);
            this.checkBoxCG.Name = "checkBoxCG";
            this.checkBoxCG.Size = new System.Drawing.Size(41, 17);
            this.checkBoxCG.TabIndex = 179;
            this.checkBoxCG.Text = "CG";
            this.checkBoxCG.UseVisualStyleBackColor = true;
            this.checkBoxCG.CheckedChanged += new System.EventHandler(this.checkBoxCG_CheckedChanged);
            // 
            // buttonCGProps
            // 
            this.buttonCGProps.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonCGProps.Enabled = false;
            this.buttonCGProps.Location = new System.Drawing.Point(853, 55);
            this.buttonCGProps.Name = "buttonCGProps";
            this.buttonCGProps.Size = new System.Drawing.Size(75, 23);
            this.buttonCGProps.TabIndex = 180;
            this.buttonCGProps.Text = "CG Props";
            this.buttonCGProps.UseVisualStyleBackColor = true;
            this.buttonCGProps.Click += new System.EventHandler(this.buttonCGProps_Click);
            // 
            // checkBoxCC
            // 
            this.checkBoxCC.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxCC.AutoSize = true;
            this.checkBoxCC.Location = new System.Drawing.Point(643, 59);
            this.checkBoxCC.Name = "checkBoxCC";
            this.checkBoxCC.Size = new System.Drawing.Size(70, 17);
            this.checkBoxCC.TabIndex = 183;
            this.checkBoxCC.Text = "Show CC";
            this.checkBoxCC.UseVisualStyleBackColor = true;
            // 
            // timerUpdatePos
            // 
            this.timerUpdatePos.Enabled = true;
            this.timerUpdatePos.Interval = 10;
            this.timerUpdatePos.Tick += new System.EventHandler(this.timerUpdatePos_Tick);
            // 
            // comboBoxAudioRenderers
            // 
            this.comboBoxAudioRenderers.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxAudioRenderers.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxAudioRenderers.DropDownWidth = 75;
            this.comboBoxAudioRenderers.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.comboBoxAudioRenderers.FormattingEnabled = true;
            this.comboBoxAudioRenderers.Location = new System.Drawing.Point(916, 549);
            this.comboBoxAudioRenderers.Name = "comboBoxAudioRenderers";
            this.comboBoxAudioRenderers.Size = new System.Drawing.Size(16, 24);
            this.comboBoxAudioRenderers.TabIndex = 175;
            this.comboBoxAudioRenderers.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // CGEditorButton
            // 
            this.CGEditorButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.CGEditorButton.Enabled = false;
            this.CGEditorButton.Location = new System.Drawing.Point(773, 55);
            this.CGEditorButton.Name = "CGEditorButton";
            this.CGEditorButton.Size = new System.Drawing.Size(75, 23);
            this.CGEditorButton.TabIndex = 184;
            this.CGEditorButton.Text = "CG Editor";
            this.CGEditorButton.UseVisualStyleBackColor = true;
            this.CGEditorButton.Click += new System.EventHandler(this.CGEditorButton_Click);
            // 
            // checkBoxVirtDev
            // 
            this.checkBoxVirtDev.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxVirtDev.AutoSize = true;
            this.checkBoxVirtDev.Location = new System.Drawing.Point(540, 59);
            this.checkBoxVirtDev.Name = "checkBoxVirtDev";
            this.checkBoxVirtDev.Size = new System.Drawing.Size(81, 17);
            this.checkBoxVirtDev.TabIndex = 185;
            this.checkBoxVirtDev.Text = "Virtual Dev.";
            this.checkBoxVirtDev.UseVisualStyleBackColor = true;
            this.checkBoxVirtDev.CheckedChanged += new System.EventHandler(this.checkBoxVirtDev_CheckedChanged);
            // 
            // buttonHTMLProps
            // 
            this.buttonHTMLProps.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonHTMLProps.Enabled = false;
            this.buttonHTMLProps.Location = new System.Drawing.Point(853, 80);
            this.buttonHTMLProps.Name = "buttonHTMLProps";
            this.buttonHTMLProps.Size = new System.Drawing.Size(75, 23);
            this.buttonHTMLProps.TabIndex = 193;
            this.buttonHTMLProps.Text = "Props";
            this.buttonHTMLProps.UseVisualStyleBackColor = true;
            this.buttonHTMLProps.Click += new System.EventHandler(this.buttonHTMLProps_Click);
            // 
            // textBoxHTMLURL
            // 
            this.textBoxHTMLURL.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.textBoxHTMLURL.Enabled = false;
            this.textBoxHTMLURL.Location = new System.Drawing.Point(643, 82);
            this.textBoxHTMLURL.Name = "textBoxHTMLURL";
            this.textBoxHTMLURL.Size = new System.Drawing.Size(205, 20);
            this.textBoxHTMLURL.TabIndex = 192;
            this.textBoxHTMLURL.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBoxHTMLURL_KeyDown);
            // 
            // checkBoxHTML
            // 
            this.checkBoxHTML.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.checkBoxHTML.AutoSize = true;
            this.checkBoxHTML.Enabled = false;
            this.checkBoxHTML.Location = new System.Drawing.Point(540, 84);
            this.checkBoxHTML.Name = "checkBoxHTML";
            this.checkBoxHTML.Size = new System.Drawing.Size(93, 17);
            this.checkBoxHTML.TabIndex = 191;
            this.checkBoxHTML.Text = "HTML overlay";
            this.checkBoxHTML.UseVisualStyleBackColor = true;
            this.checkBoxHTML.CheckedChanged += new System.EventHandler(this.checkBoxHTML_CheckedChanged);
            // 
            // mAudioMeter1
            // 
            this.mAudioMeter1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mAudioMeter1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(237)))), ((int)(((byte)(242)))), ((int)(((byte)(255)))));
            this.mAudioMeter1.BackColorHi = System.Drawing.Color.FromArgb(((int)(((byte)(222)))), ((int)(((byte)(232)))), ((int)(((byte)(254)))));
            this.mAudioMeter1.ColorGainSlider = System.Drawing.Color.Red;
            this.mAudioMeter1.ColorLevelBack = System.Drawing.Color.DarkGray;
            this.mAudioMeter1.ColorLevelHi = System.Drawing.Color.Red;
            this.mAudioMeter1.ColorLevelLo = System.Drawing.Color.DarkBlue;
            this.mAudioMeter1.ColorLevelMid = System.Drawing.Color.Blue;
            this.mAudioMeter1.ColorLevelOrg = System.Drawing.Color.LightGray;
            this.mAudioMeter1.ColorOutline = System.Drawing.Color.Black;
            this.mAudioMeter1.Location = new System.Drawing.Point(853, 106);
            this.mAudioMeter1.Margin = new System.Windows.Forms.Padding(2);
            this.mAudioMeter1.Name = "mAudioMeter1";
            this.mAudioMeter1.Size = new System.Drawing.Size(79, 439);
            this.mAudioMeter1.TabIndex = 177;
            // 
            // Add3TL_btn
            // 
            this.Add3TL_btn.Location = new System.Drawing.Point(726, 12);
            this.Add3TL_btn.Name = "Add3TL_btn";
            this.Add3TL_btn.Size = new System.Drawing.Size(84, 23);
            this.Add3TL_btn.TabIndex = 194;
            this.Add3TL_btn.Text = "Add3timelines";
            this.Add3TL_btn.UseVisualStyleBackColor = true;
            this.Add3TL_btn.Click += new System.EventHandler(this.Add3TL_btn_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(940, 642);
            this.Controls.Add(this.Add3TL_btn);
            this.Controls.Add(this.buttonLogo);
            this.Controls.Add(this.checkBoxVirtDev);
            this.Controls.Add(this.checkBoxCC);
            this.Controls.Add(this.buttonHTMLProps);
            this.Controls.Add(this.textBoxHTMLURL);
            this.Controls.Add(this.checkBoxHTML);
            this.Controls.Add(this.CGEditorButton);
            this.Controls.Add(this.checkBoxAudio);
            this.Controls.Add(this.comboBoxAudioRenderers);
            this.Controls.Add(this.buttonCGProps);
            this.Controls.Add(this.checkBoxCG);
            this.Controls.Add(this.checkBoxVideo);
            this.Controls.Add(this.checkBoxRenderer);
            this.Controls.Add(this.comboBoxVideoFormat);
            this.Controls.Add(this.comboBoxRenrerers);
            this.Controls.Add(this.mAudioMeter1);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.labelDescr);
            this.Controls.Add(this.pictureLogo);
            this.Controls.Add(this.labelStatus);
            this.Controls.Add(this.buttonSetFile);
            this.Controls.Add(this.panelPreview);
            this.Controls.Add(this.trackBarVolume);
            this.Name = "MainForm";
            this.Text = "Sample File Playback";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.SizeChanged += new System.EventHandler(this.MainForm_SizeChanged);
            ((System.ComponentModel.ISupportInitialize)(this.trackBarSeek)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panelPos.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.trackBarVolume)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureLogo)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button buttonPlay;
		private System.Windows.Forms.Button buttonFrBack;
		private System.Windows.Forms.Button buttonFrFwd;
		private System.Windows.Forms.TrackBar trackBarSeek;
		private System.Windows.Forms.Panel panelPreview;
		private System.Windows.Forms.Button buttonSetFile;
		private System.Windows.Forms.OpenFileDialog openFileDialog;
		private System.Windows.Forms.Label labelStatus;
		private System.Windows.Forms.PictureBox pictureLogo;
		private System.Windows.Forms.Label labelDescr;
		private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label labelPos;
		private System.Windows.Forms.CheckBox checkBoxLoop;
		private System.Windows.Forms.Button buttonLogo;
		private System.Windows.Forms.Button buttonReverse;
        private System.Windows.Forms.TextBox textBoxToPos;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox comboBoxAudioTrack;
		private System.Windows.Forms.Button buttonFastForward;
		private System.Windows.Forms.Button buttonFastBackward;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ComboBox comboBoxTSProg;
        private System.Windows.Forms.Panel panelPos;
        private System.Windows.Forms.TrackBar trackBarVolume;
        private MAudioMeter mAudioMeter1;
        private System.Windows.Forms.CheckBox checkBoxAudio;
        private System.Windows.Forms.ComboBox comboBoxRenrerers;
        private System.Windows.Forms.CheckBox checkBoxRenderer;
        private System.Windows.Forms.ComboBox comboBoxVideoFormat;
        private System.Windows.Forms.CheckBox checkBoxVideo;
        private System.Windows.Forms.CheckBox checkBoxCG;
        private System.Windows.Forms.Button buttonCGProps;
        private System.Windows.Forms.CheckBox checkBoxCC;
        private System.Windows.Forms.Timer timerUpdatePos;
        private System.Windows.Forms.ComboBox comboBoxAudioRenderers;
        private System.Windows.Forms.Button CGEditorButton;
        private System.Windows.Forms.CheckBox checkBoxVirtDev;
        private System.Windows.Forms.Button buttonHTMLProps;
        private System.Windows.Forms.TextBox textBoxHTMLURL;
        private System.Windows.Forms.CheckBox checkBoxHTML;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxTC;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button Add3TL_btn;
    }
}

