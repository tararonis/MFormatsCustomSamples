namespace CGEditor_WinForms
{
    partial class MainWindow
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
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.PageCommon = new System.Windows.Forms.TabPage();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.BasicMiscEditor = new CGEditor_WinForms.CustomPropertyEditors.MiscEditor();
            this.BasicColorEditor = new CGEditor_WinForms.CustomPropertyEditors.SimpleColorEditor();
            this.BasicPositionEditor = new CGEditor_WinForms.CustomPropertyEditors.PositionEditor();
            this.BasicVisibilityEditor = new CGEditor_WinForms.CustomPropertyEditors.VisibilityEditor();
            this.PageEffects = new System.Windows.Forms.TabPage();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.EffectsGlowEditor = new CGEditor_WinForms.CustomPropertyEditors.GlowEditor();
            this.EffectsBlurEditor = new CGEditor_WinForms.CustomPropertyEditors.BlurEditor();
            this.EffectsShadowEditor = new CGEditor_WinForms.CustomPropertyEditors.ShadowEditor();
            this.EffectsSpeedEditor = new CGEditor_WinForms.CustomPropertyEditors.SpeedEditor();
            this.PageText = new System.Windows.Forms.TabPage();
            this.label12 = new System.Windows.Forms.Label();
            this.label11 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.TextOutlineEditor = new CGEditor_WinForms.CustomPropertyEditors.OutlineColorEditor();
            this.TextColorEditor = new CGEditor_WinForms.CustomPropertyEditors.GradientEditor();
            this.TextFontEditor = new CGEditor_WinForms.CustomPropertyEditors.FontEditor();
            this.TextContEditor = new CGEditor_WinForms.CustomPropertyEditors.TextContentEditor();
            this.PageGraphics = new System.Windows.Forms.TabPage();
            this.label15 = new System.Windows.Forms.Label();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.GraphicsOutlineEditor = new CGEditor_WinForms.CustomPropertyEditors.OutlineColorEditor();
            this.GraphicsColorEditor = new CGEditor_WinForms.CustomPropertyEditors.GradientEditor();
            this.GraphicsShapeEditor = new CGEditor_WinForms.CustomPropertyEditors.ShapeEditor();
            this.PageTickerContent = new System.Windows.Forms.TabPage();
            this.label19 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label16 = new System.Windows.Forms.Label();
            this.TickerTextFontEditor = new CGEditor_WinForms.CustomPropertyEditors.FontEditor();
            this.TickerTextOutlineEditor = new CGEditor_WinForms.CustomPropertyEditors.TextOutlineColorEditor();
            this.TickerTextColorEditor = new CGEditor_WinForms.CustomPropertyEditors.TextGradientEditor();
            this.TickerNewContentEditor = new CGEditor_WinForms.CustomPropertyEditors.TickerContentEditor();
            this.PageTickerProps = new System.Windows.Forms.TabPage();
            this.label23 = new System.Windows.Forms.Label();
            this.label22 = new System.Windows.Forms.Label();
            this.label21 = new System.Windows.Forms.Label();
            this.label20 = new System.Windows.Forms.Label();
            this.TickerOutlineEditor = new CGEditor_WinForms.CustomPropertyEditors.OutlineColorEditor();
            this.TickerColorEditor = new CGEditor_WinForms.CustomPropertyEditors.GradientEditor();
            this.TickerShapeEditor = new CGEditor_WinForms.CustomPropertyEditors.ShapeEditor();
            this.TickerPropsEditor = new CGEditor_WinForms.CustomPropertyEditors.TickerEditor();
            this.PageImage = new System.Windows.Forms.TabPage();
            this.label24 = new System.Windows.Forms.Label();
            this.ImageEditor = new CGEditor_WinForms.CustomPropertyEditors.OpenImageEditor();
            this.PageVideo = new System.Windows.Forms.TabPage();
            this.label25 = new System.Windows.Forms.Label();
            this.openVideoEditor1 = new CGEditor_WinForms.CustomPropertyEditors.OpenVideoEditor();
            this.PageFlash = new System.Windows.Forms.TabPage();
            this.label26 = new System.Windows.Forms.Label();
            this.FlashEditor = new CGEditor_WinForms.CustomPropertyEditors.OpenFlashEditor();
            this.PageActions = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.ListBoxTools = new System.Windows.Forms.ListBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.listBoxItems = new System.Windows.Forms.ListBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.XMLDescription = new CGEditor_WinForms.XMLDescriptionControl();
            this.MainPreview = new CGEditor_WinForms.PreviewControl();
            this.buttonGroup = new System.Windows.Forms.Button();
            this.buttonUngroup = new System.Windows.Forms.Button();
            this.buttonLayerUp = new System.Windows.Forms.Button();
            this.buttonLayerDown = new System.Windows.Forms.Button();
            this.buttonBringToFront = new System.Windows.Forms.Button();
            this.buttonSendToBack = new System.Windows.Forms.Button();
            this.buttonALeft = new System.Windows.Forms.Button();
            this.buttonACenter = new System.Windows.Forms.Button();
            this.buttonAright = new System.Windows.Forms.Button();
            this.buttonATop = new System.Windows.Forms.Button();
            this.buttonAMiddle = new System.Windows.Forms.Button();
            this.buttonABottom = new System.Windows.Forms.Button();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.loadConfigurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveConfigurationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabControl1.SuspendLayout();
            this.PageCommon.SuspendLayout();
            this.PageEffects.SuspendLayout();
            this.PageText.SuspendLayout();
            this.PageGraphics.SuspendLayout();
            this.PageTickerContent.SuspendLayout();
            this.PageTickerProps.SuspendLayout();
            this.PageImage.SuspendLayout();
            this.PageVideo.SuspendLayout();
            this.PageFlash.SuspendLayout();
            this.PageActions.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.PageCommon);
            this.tabControl1.Controls.Add(this.PageEffects);
            this.tabControl1.Controls.Add(this.PageText);
            this.tabControl1.Controls.Add(this.PageGraphics);
            this.tabControl1.Controls.Add(this.PageTickerContent);
            this.tabControl1.Controls.Add(this.PageTickerProps);
            this.tabControl1.Controls.Add(this.PageImage);
            this.tabControl1.Controls.Add(this.PageVideo);
            this.tabControl1.Controls.Add(this.PageFlash);
            this.tabControl1.Controls.Add(this.PageActions);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Top;
            this.tabControl1.Location = new System.Drawing.Point(0, 33);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1717, 162);
            this.tabControl1.TabIndex = 0;
            // 
            // PageCommon
            // 
            this.PageCommon.Controls.Add(this.label4);
            this.PageCommon.Controls.Add(this.label3);
            this.PageCommon.Controls.Add(this.label2);
            this.PageCommon.Controls.Add(this.label1);
            this.PageCommon.Controls.Add(this.BasicMiscEditor);
            this.PageCommon.Controls.Add(this.BasicColorEditor);
            this.PageCommon.Controls.Add(this.BasicPositionEditor);
            this.PageCommon.Controls.Add(this.BasicVisibilityEditor);
            this.PageCommon.Location = new System.Drawing.Point(4, 29);
            this.PageCommon.Name = "PageCommon";
            this.PageCommon.Padding = new System.Windows.Forms.Padding(3);
            this.PageCommon.Size = new System.Drawing.Size(1709, 129);
            this.PageCommon.TabIndex = 0;
            this.PageCommon.Text = "Common Properties";
            this.PageCommon.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(822, 80);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(109, 20);
            this.label4.TabIndex = 25;
            this.label4.Text = "Miscellaneous";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(388, 80);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(140, 20);
            this.label3.TabIndex = 24;
            this.label3.Text = "Alpha/Background";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(193, 80);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(100, 20);
            this.label2.TabIndex = 23;
            this.label2.Text = "Position/Size";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(32, 80);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 20);
            this.label1.TabIndex = 22;
            this.label1.Text = "Visibility";
            // 
            // BasicMiscEditor
            // 
            this.BasicMiscEditor.BackColor = System.Drawing.Color.White;
            this.BasicMiscEditor.Enabled = false;
            this.BasicMiscEditor.Location = new System.Drawing.Point(564, 6);
            this.BasicMiscEditor.Name = "BasicMiscEditor";
            this.BasicMiscEditor.Size = new System.Drawing.Size(502, 71);
            this.BasicMiscEditor.TabIndex = 21;
            // 
            // BasicColorEditor
            // 
            this.BasicColorEditor.BackColor = System.Drawing.Color.White;
            this.BasicColorEditor.Enabled = false;
            this.BasicColorEditor.Location = new System.Drawing.Point(356, 3);
            this.BasicColorEditor.Name = "BasicColorEditor";
            this.BasicColorEditor.Size = new System.Drawing.Size(202, 74);
            this.BasicColorEditor.TabIndex = 20;
            // 
            // BasicPositionEditor
            // 
            this.BasicPositionEditor.BackColor = System.Drawing.Color.White;
            this.BasicPositionEditor.Enabled = false;
            this.BasicPositionEditor.Location = new System.Drawing.Point(122, 3);
            this.BasicPositionEditor.Name = "BasicPositionEditor";
            this.BasicPositionEditor.Size = new System.Drawing.Size(228, 74);
            this.BasicPositionEditor.TabIndex = 19;
            // 
            // BasicVisibilityEditor
            // 
            this.BasicVisibilityEditor.BackColor = System.Drawing.Color.White;
            this.BasicVisibilityEditor.Enabled = false;
            this.BasicVisibilityEditor.Location = new System.Drawing.Point(8, 6);
            this.BasicVisibilityEditor.Name = "BasicVisibilityEditor";
            this.BasicVisibilityEditor.Size = new System.Drawing.Size(108, 71);
            this.BasicVisibilityEditor.TabIndex = 18;
            // 
            // PageEffects
            // 
            this.PageEffects.Controls.Add(this.label8);
            this.PageEffects.Controls.Add(this.label7);
            this.PageEffects.Controls.Add(this.label6);
            this.PageEffects.Controls.Add(this.label5);
            this.PageEffects.Controls.Add(this.EffectsGlowEditor);
            this.PageEffects.Controls.Add(this.EffectsBlurEditor);
            this.PageEffects.Controls.Add(this.EffectsShadowEditor);
            this.PageEffects.Controls.Add(this.EffectsSpeedEditor);
            this.PageEffects.Location = new System.Drawing.Point(4, 29);
            this.PageEffects.Name = "PageEffects";
            this.PageEffects.Padding = new System.Windows.Forms.Padding(3);
            this.PageEffects.Size = new System.Drawing.Size(1709, 129);
            this.PageEffects.TabIndex = 1;
            this.PageEffects.Text = "Effects";
            this.PageEffects.UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(977, 85);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(45, 20);
            this.label8.TabIndex = 29;
            this.label8.Text = "Glow";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(739, 85);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(37, 20);
            this.label7.TabIndex = 28;
            this.label7.Text = "Blur";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(395, 85);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(67, 20);
            this.label6.TabIndex = 27;
            this.label6.Text = "Shadow";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(110, 85);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(56, 20);
            this.label5.TabIndex = 26;
            this.label5.Text = "Speed";
            // 
            // EffectsGlowEditor
            // 
            this.EffectsGlowEditor.BackColor = System.Drawing.Color.White;
            this.EffectsGlowEditor.Enabled = false;
            this.EffectsGlowEditor.Location = new System.Drawing.Point(889, 6);
            this.EffectsGlowEditor.Name = "EffectsGlowEditor";
            this.EffectsGlowEditor.Size = new System.Drawing.Size(223, 76);
            this.EffectsGlowEditor.TabIndex = 25;
            // 
            // EffectsBlurEditor
            // 
            this.EffectsBlurEditor.BackColor = System.Drawing.Color.White;
            this.EffectsBlurEditor.Enabled = false;
            this.EffectsBlurEditor.Location = new System.Drawing.Point(660, 6);
            this.EffectsBlurEditor.Name = "EffectsBlurEditor";
            this.EffectsBlurEditor.Size = new System.Drawing.Size(223, 76);
            this.EffectsBlurEditor.TabIndex = 24;
            // 
            // EffectsShadowEditor
            // 
            this.EffectsShadowEditor.BackColor = System.Drawing.Color.White;
            this.EffectsShadowEditor.Enabled = false;
            this.EffectsShadowEditor.Location = new System.Drawing.Point(235, 3);
            this.EffectsShadowEditor.Name = "EffectsShadowEditor";
            this.EffectsShadowEditor.Size = new System.Drawing.Size(419, 79);
            this.EffectsShadowEditor.TabIndex = 23;
            // 
            // EffectsSpeedEditor
            // 
            this.EffectsSpeedEditor.BackColor = System.Drawing.Color.White;
            this.EffectsSpeedEditor.Enabled = false;
            this.EffectsSpeedEditor.Location = new System.Drawing.Point(8, 6);
            this.EffectsSpeedEditor.Name = "EffectsSpeedEditor";
            this.EffectsSpeedEditor.Size = new System.Drawing.Size(221, 76);
            this.EffectsSpeedEditor.TabIndex = 22;
            // 
            // PageText
            // 
            this.PageText.Controls.Add(this.label12);
            this.PageText.Controls.Add(this.label11);
            this.PageText.Controls.Add(this.label10);
            this.PageText.Controls.Add(this.label9);
            this.PageText.Controls.Add(this.TextOutlineEditor);
            this.PageText.Controls.Add(this.TextColorEditor);
            this.PageText.Controls.Add(this.TextFontEditor);
            this.PageText.Controls.Add(this.TextContEditor);
            this.PageText.Location = new System.Drawing.Point(4, 29);
            this.PageText.Name = "PageText";
            this.PageText.Size = new System.Drawing.Size(1709, 129);
            this.PageText.TabIndex = 2;
            this.PageText.Text = "Text Properties";
            this.PageText.UseVisualStyleBackColor = true;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(1508, 85);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(42, 20);
            this.label12.TabIndex = 31;
            this.label12.Text = "Font";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(1115, 89);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(59, 20);
            this.label11.TabIndex = 30;
            this.label11.Text = "Outline";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(689, 89);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(77, 20);
            this.label10.TabIndex = 29;
            this.label10.Text = "Text color";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(241, 89);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(66, 20);
            this.label9.TabIndex = 28;
            this.label9.Text = "Content";
            // 
            // TextOutlineEditor
            // 
            this.TextOutlineEditor.BackColor = System.Drawing.Color.White;
            this.TextOutlineEditor.Enabled = false;
            this.TextOutlineEditor.Location = new System.Drawing.Point(919, 3);
            this.TextOutlineEditor.Name = "TextOutlineEditor";
            this.TextOutlineEditor.Size = new System.Drawing.Size(381, 79);
            this.TextOutlineEditor.TabIndex = 27;
            // 
            // TextColorEditor
            // 
            this.TextColorEditor.BackColor = System.Drawing.Color.White;
            this.TextColorEditor.Enabled = false;
            this.TextColorEditor.Location = new System.Drawing.Point(548, 3);
            this.TextColorEditor.Name = "TextColorEditor";
            this.TextColorEditor.Size = new System.Drawing.Size(365, 76);
            this.TextColorEditor.TabIndex = 26;
            // 
            // TextFontEditor
            // 
            this.TextFontEditor.BackColor = System.Drawing.Color.White;
            this.TextFontEditor.Enabled = false;
            this.TextFontEditor.Location = new System.Drawing.Point(1306, 3);
            this.TextFontEditor.Name = "TextFontEditor";
            this.TextFontEditor.Size = new System.Drawing.Size(462, 79);
            this.TextFontEditor.TabIndex = 25;
            // 
            // TextContEditor
            // 
            this.TextContEditor.BackColor = System.Drawing.Color.White;
            this.TextContEditor.Enabled = false;
            this.TextContEditor.Location = new System.Drawing.Point(8, 3);
            this.TextContEditor.Name = "TextContEditor";
            this.TextContEditor.Size = new System.Drawing.Size(534, 83);
            this.TextContEditor.TabIndex = 24;
            // 
            // PageGraphics
            // 
            this.PageGraphics.Controls.Add(this.label15);
            this.PageGraphics.Controls.Add(this.label14);
            this.PageGraphics.Controls.Add(this.label13);
            this.PageGraphics.Controls.Add(this.GraphicsOutlineEditor);
            this.PageGraphics.Controls.Add(this.GraphicsColorEditor);
            this.PageGraphics.Controls.Add(this.GraphicsShapeEditor);
            this.PageGraphics.Location = new System.Drawing.Point(4, 29);
            this.PageGraphics.Name = "PageGraphics";
            this.PageGraphics.Size = new System.Drawing.Size(1709, 129);
            this.PageGraphics.TabIndex = 3;
            this.PageGraphics.Text = "Graphics Properties";
            this.PageGraphics.UseVisualStyleBackColor = true;
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Location = new System.Drawing.Point(835, 82);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(59, 20);
            this.label15.TabIndex = 32;
            this.label15.Text = "Outline";
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(440, 82);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(46, 20);
            this.label14.TabIndex = 31;
            this.label14.Text = "Color";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(111, 82);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(56, 20);
            this.label13.TabIndex = 30;
            this.label13.Text = "Shape";
            // 
            // GraphicsOutlineEditor
            // 
            this.GraphicsOutlineEditor.BackColor = System.Drawing.Color.White;
            this.GraphicsOutlineEditor.Enabled = false;
            this.GraphicsOutlineEditor.Location = new System.Drawing.Point(660, 3);
            this.GraphicsOutlineEditor.Name = "GraphicsOutlineEditor";
            this.GraphicsOutlineEditor.Size = new System.Drawing.Size(381, 79);
            this.GraphicsOutlineEditor.TabIndex = 29;
            // 
            // GraphicsColorEditor
            // 
            this.GraphicsColorEditor.BackColor = System.Drawing.Color.White;
            this.GraphicsColorEditor.Enabled = false;
            this.GraphicsColorEditor.Location = new System.Drawing.Point(288, 3);
            this.GraphicsColorEditor.Name = "GraphicsColorEditor";
            this.GraphicsColorEditor.Size = new System.Drawing.Size(366, 76);
            this.GraphicsColorEditor.TabIndex = 28;
            // 
            // GraphicsShapeEditor
            // 
            this.GraphicsShapeEditor.BackColor = System.Drawing.Color.White;
            this.GraphicsShapeEditor.Enabled = false;
            this.GraphicsShapeEditor.Location = new System.Drawing.Point(8, 9);
            this.GraphicsShapeEditor.Name = "GraphicsShapeEditor";
            this.GraphicsShapeEditor.Size = new System.Drawing.Size(277, 67);
            this.GraphicsShapeEditor.TabIndex = 27;
            // 
            // PageTickerContent
            // 
            this.PageTickerContent.Controls.Add(this.label19);
            this.PageTickerContent.Controls.Add(this.label18);
            this.PageTickerContent.Controls.Add(this.label17);
            this.PageTickerContent.Controls.Add(this.label16);
            this.PageTickerContent.Controls.Add(this.TickerTextFontEditor);
            this.PageTickerContent.Controls.Add(this.TickerTextOutlineEditor);
            this.PageTickerContent.Controls.Add(this.TickerTextColorEditor);
            this.PageTickerContent.Controls.Add(this.TickerNewContentEditor);
            this.PageTickerContent.Location = new System.Drawing.Point(4, 29);
            this.PageTickerContent.Name = "PageTickerContent";
            this.PageTickerContent.Size = new System.Drawing.Size(1709, 129);
            this.PageTickerContent.TabIndex = 4;
            this.PageTickerContent.Text = "Ticker Content";
            this.PageTickerContent.UseVisualStyleBackColor = true;
            // 
            // label19
            // 
            this.label19.AutoSize = true;
            this.label19.Location = new System.Drawing.Point(1524, 88);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(88, 20);
            this.label19.TabIndex = 62;
            this.label19.Text = "Ticker Font";
            // 
            // label18
            // 
            this.label18.AutoSize = true;
            this.label18.Location = new System.Drawing.Point(1104, 88);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(59, 20);
            this.label18.TabIndex = 61;
            this.label18.Text = "Outline";
            // 
            // label17
            // 
            this.label17.AutoSize = true;
            this.label17.Location = new System.Drawing.Point(710, 88);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(80, 20);
            this.label17.TabIndex = 60;
            this.label17.Text = "Text Color";
            // 
            // label16
            // 
            this.label16.AutoSize = true;
            this.label16.Location = new System.Drawing.Point(215, 88);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(112, 20);
            this.label16.TabIndex = 59;
            this.label16.Text = "Ticker Content";
            // 
            // TickerTextFontEditor
            // 
            this.TickerTextFontEditor.BackColor = System.Drawing.Color.White;
            this.TickerTextFontEditor.Enabled = false;
            this.TickerTextFontEditor.Location = new System.Drawing.Point(1305, 3);
            this.TickerTextFontEditor.Name = "TickerTextFontEditor";
            this.TickerTextFontEditor.Size = new System.Drawing.Size(463, 75);
            this.TickerTextFontEditor.TabIndex = 58;
            // 
            // TickerTextOutlineEditor
            // 
            this.TickerTextOutlineEditor.BackColor = System.Drawing.Color.White;
            this.TickerTextOutlineEditor.Enabled = false;
            this.TickerTextOutlineEditor.Location = new System.Drawing.Point(921, 3);
            this.TickerTextOutlineEditor.Name = "TickerTextOutlineEditor";
            this.TickerTextOutlineEditor.Size = new System.Drawing.Size(378, 77);
            this.TickerTextOutlineEditor.TabIndex = 57;
            // 
            // TickerTextColorEditor
            // 
            this.TickerTextColorEditor.BackColor = System.Drawing.Color.White;
            this.TickerTextColorEditor.Enabled = false;
            this.TickerTextColorEditor.Location = new System.Drawing.Point(553, 3);
            this.TickerTextColorEditor.Name = "TickerTextColorEditor";
            this.TickerTextColorEditor.Size = new System.Drawing.Size(362, 76);
            this.TickerTextColorEditor.TabIndex = 56;
            // 
            // TickerNewContentEditor
            // 
            this.TickerNewContentEditor.BackColor = System.Drawing.Color.White;
            this.TickerNewContentEditor.Enabled = false;
            this.TickerNewContentEditor.Location = new System.Drawing.Point(8, 3);
            this.TickerNewContentEditor.Name = "TickerNewContentEditor";
            this.TickerNewContentEditor.Size = new System.Drawing.Size(539, 82);
            this.TickerNewContentEditor.TabIndex = 55;
            // 
            // PageTickerProps
            // 
            this.PageTickerProps.Controls.Add(this.label23);
            this.PageTickerProps.Controls.Add(this.label22);
            this.PageTickerProps.Controls.Add(this.label21);
            this.PageTickerProps.Controls.Add(this.label20);
            this.PageTickerProps.Controls.Add(this.TickerOutlineEditor);
            this.PageTickerProps.Controls.Add(this.TickerColorEditor);
            this.PageTickerProps.Controls.Add(this.TickerShapeEditor);
            this.PageTickerProps.Controls.Add(this.TickerPropsEditor);
            this.PageTickerProps.Location = new System.Drawing.Point(4, 29);
            this.PageTickerProps.Name = "PageTickerProps";
            this.PageTickerProps.Size = new System.Drawing.Size(1709, 129);
            this.PageTickerProps.TabIndex = 5;
            this.PageTickerProps.Text = "Ticker Properties";
            this.PageTickerProps.UseVisualStyleBackColor = true;
            // 
            // label23
            // 
            this.label23.AutoSize = true;
            this.label23.Location = new System.Drawing.Point(1124, 81);
            this.label23.Name = "label23";
            this.label23.Size = new System.Drawing.Size(107, 20);
            this.label23.TabIndex = 39;
            this.label23.Text = "Shape outline";
            // 
            // label22
            // 
            this.label22.AutoSize = true;
            this.label22.Location = new System.Drawing.Point(739, 81);
            this.label22.Name = "label22";
            this.label22.Size = new System.Drawing.Size(97, 20);
            this.label22.TabIndex = 38;
            this.label22.Text = "Shape Color";
            // 
            // label21
            // 
            this.label21.AutoSize = true;
            this.label21.Location = new System.Drawing.Point(410, 81);
            this.label21.Name = "label21";
            this.label21.Size = new System.Drawing.Size(102, 20);
            this.label21.TabIndex = 37;
            this.label21.Text = "Ticker Shape";
            // 
            // label20
            // 
            this.label20.AutoSize = true;
            this.label20.Location = new System.Drawing.Point(102, 81);
            this.label20.Name = "label20";
            this.label20.Size = new System.Drawing.Size(127, 20);
            this.label20.TabIndex = 36;
            this.label20.Text = "Ticker Properties";
            // 
            // TickerOutlineEditor
            // 
            this.TickerOutlineEditor.BackColor = System.Drawing.Color.White;
            this.TickerOutlineEditor.Enabled = false;
            this.TickerOutlineEditor.Location = new System.Drawing.Point(984, 3);
            this.TickerOutlineEditor.Name = "TickerOutlineEditor";
            this.TickerOutlineEditor.Size = new System.Drawing.Size(381, 75);
            this.TickerOutlineEditor.TabIndex = 35;
            // 
            // TickerColorEditor
            // 
            this.TickerColorEditor.BackColor = System.Drawing.Color.White;
            this.TickerColorEditor.Enabled = false;
            this.TickerColorEditor.Location = new System.Drawing.Point(613, 3);
            this.TickerColorEditor.Name = "TickerColorEditor";
            this.TickerColorEditor.Size = new System.Drawing.Size(365, 75);
            this.TickerColorEditor.TabIndex = 34;
            // 
            // TickerShapeEditor
            // 
            this.TickerShapeEditor.BackColor = System.Drawing.Color.White;
            this.TickerShapeEditor.Enabled = false;
            this.TickerShapeEditor.Location = new System.Drawing.Point(327, 3);
            this.TickerShapeEditor.Name = "TickerShapeEditor";
            this.TickerShapeEditor.Size = new System.Drawing.Size(280, 75);
            this.TickerShapeEditor.TabIndex = 33;
            // 
            // TickerPropsEditor
            // 
            this.TickerPropsEditor.BackColor = System.Drawing.Color.White;
            this.TickerPropsEditor.Enabled = false;
            this.TickerPropsEditor.Location = new System.Drawing.Point(8, 3);
            this.TickerPropsEditor.Name = "TickerPropsEditor";
            this.TickerPropsEditor.Size = new System.Drawing.Size(313, 75);
            this.TickerPropsEditor.TabIndex = 32;
            // 
            // PageImage
            // 
            this.PageImage.Controls.Add(this.label24);
            this.PageImage.Controls.Add(this.ImageEditor);
            this.PageImage.Location = new System.Drawing.Point(4, 29);
            this.PageImage.Name = "PageImage";
            this.PageImage.Size = new System.Drawing.Size(1709, 129);
            this.PageImage.TabIndex = 6;
            this.PageImage.Text = "Image Properties";
            this.PageImage.UseVisualStyleBackColor = true;
            // 
            // label24
            // 
            this.label24.AutoSize = true;
            this.label24.Location = new System.Drawing.Point(196, 79);
            this.label24.Name = "label24";
            this.label24.Size = new System.Drawing.Size(109, 20);
            this.label24.TabIndex = 37;
            this.label24.Text = "Image Source";
            // 
            // ImageEditor
            // 
            this.ImageEditor.BackColor = System.Drawing.Color.White;
            this.ImageEditor.Location = new System.Drawing.Point(8, 30);
            this.ImageEditor.Name = "ImageEditor";
            this.ImageEditor.Size = new System.Drawing.Size(493, 46);
            this.ImageEditor.TabIndex = 35;
            this.ImageEditor.Value = null;
            // 
            // PageVideo
            // 
            this.PageVideo.Controls.Add(this.label25);
            this.PageVideo.Controls.Add(this.openVideoEditor1);
            this.PageVideo.Location = new System.Drawing.Point(4, 29);
            this.PageVideo.Name = "PageVideo";
            this.PageVideo.Size = new System.Drawing.Size(1709, 129);
            this.PageVideo.TabIndex = 7;
            this.PageVideo.Text = "Video Properties";
            this.PageVideo.UseVisualStyleBackColor = true;
            // 
            // label25
            // 
            this.label25.AutoSize = true;
            this.label25.Location = new System.Drawing.Point(200, 72);
            this.label25.Name = "label25";
            this.label25.Size = new System.Drawing.Size(105, 20);
            this.label25.TabIndex = 37;
            this.label25.Text = "Video Source";
            // 
            // openVideoEditor1
            // 
            this.openVideoEditor1.Location = new System.Drawing.Point(8, 29);
            this.openVideoEditor1.Name = "openVideoEditor1";
            this.openVideoEditor1.Size = new System.Drawing.Size(495, 40);
            this.openVideoEditor1.TabIndex = 0;
            this.openVideoEditor1.Value = null;
            // 
            // PageFlash
            // 
            this.PageFlash.Controls.Add(this.label26);
            this.PageFlash.Controls.Add(this.FlashEditor);
            this.PageFlash.Location = new System.Drawing.Point(4, 29);
            this.PageFlash.Name = "PageFlash";
            this.PageFlash.Size = new System.Drawing.Size(1709, 129);
            this.PageFlash.TabIndex = 8;
            this.PageFlash.Text = "Flash Properties";
            this.PageFlash.UseVisualStyleBackColor = true;
            // 
            // label26
            // 
            this.label26.AutoSize = true;
            this.label26.Location = new System.Drawing.Point(202, 77);
            this.label26.Name = "label26";
            this.label26.Size = new System.Drawing.Size(103, 20);
            this.label26.TabIndex = 37;
            this.label26.Text = "Flash Source";
            // 
            // FlashEditor
            // 
            this.FlashEditor.BackColor = System.Drawing.Color.White;
            this.FlashEditor.Location = new System.Drawing.Point(8, 30);
            this.FlashEditor.Name = "FlashEditor";
            this.FlashEditor.Size = new System.Drawing.Size(493, 44);
            this.FlashEditor.TabIndex = 36;
            this.FlashEditor.Value = null;
            // 
            // PageActions
            // 
            this.PageActions.Controls.Add(this.buttonABottom);
            this.PageActions.Controls.Add(this.buttonAMiddle);
            this.PageActions.Controls.Add(this.buttonATop);
            this.PageActions.Controls.Add(this.buttonAright);
            this.PageActions.Controls.Add(this.buttonACenter);
            this.PageActions.Controls.Add(this.buttonALeft);
            this.PageActions.Controls.Add(this.buttonSendToBack);
            this.PageActions.Controls.Add(this.buttonBringToFront);
            this.PageActions.Controls.Add(this.buttonLayerDown);
            this.PageActions.Controls.Add(this.buttonLayerUp);
            this.PageActions.Controls.Add(this.buttonUngroup);
            this.PageActions.Controls.Add(this.buttonGroup);
            this.PageActions.Location = new System.Drawing.Point(4, 29);
            this.PageActions.Name = "PageActions";
            this.PageActions.Size = new System.Drawing.Size(1709, 129);
            this.PageActions.TabIndex = 9;
            this.PageActions.Text = "Actions";
            this.PageActions.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.ListBoxTools);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Left;
            this.groupBox1.Location = new System.Drawing.Point(0, 195);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(312, 658);
            this.groupBox1.TabIndex = 3;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "New Items";
            // 
            // ListBoxTools
            // 
            this.ListBoxTools.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ListBoxTools.FormattingEnabled = true;
            this.ListBoxTools.ItemHeight = 20;
            this.ListBoxTools.Location = new System.Drawing.Point(3, 22);
            this.ListBoxTools.Name = "ListBoxTools";
            this.ListBoxTools.Size = new System.Drawing.Size(306, 633);
            this.ListBoxTools.TabIndex = 0;
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.listBoxItems);
            this.groupBox2.Dock = System.Windows.Forms.DockStyle.Right;
            this.groupBox2.Location = new System.Drawing.Point(1420, 195);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(297, 658);
            this.groupBox2.TabIndex = 4;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Current Items";
            // 
            // listBoxItems
            // 
            this.listBoxItems.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxItems.FormattingEnabled = true;
            this.listBoxItems.ItemHeight = 20;
            this.listBoxItems.Location = new System.Drawing.Point(3, 22);
            this.listBoxItems.Name = "listBoxItems";
            this.listBoxItems.Size = new System.Drawing.Size(291, 633);
            this.listBoxItems.TabIndex = 0;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.XMLDescription);
            this.groupBox3.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.groupBox3.Location = new System.Drawing.Point(312, 667);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(1108, 186);
            this.groupBox3.TabIndex = 5;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "XML Description";
            // 
            // XMLDescription
            // 
            this.XMLDescription.Dock = System.Windows.Forms.DockStyle.Fill;
            this.XMLDescription.Location = new System.Drawing.Point(3, 22);
            this.XMLDescription.Name = "XMLDescription";
            this.XMLDescription.Size = new System.Drawing.Size(1102, 161);
            this.XMLDescription.TabIndex = 0;
            // 
            // MainPreview
            // 
            this.MainPreview.AllowDrop = true;
            this.MainPreview.AutoSize = true;
            this.MainPreview.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.MainPreview.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainPreview.Editor = null;
            this.MainPreview.eMode = CGEditor_WinForms.eMode.eM_Default;
            this.MainPreview.Location = new System.Drawing.Point(312, 195);
            this.MainPreview.Name = "MainPreview";
            this.MainPreview.ResizeAreaRatio = 0.08D;
            this.MainPreview.Size = new System.Drawing.Size(1108, 472);
            this.MainPreview.TabIndex = 6;
            // 
            // buttonGroup
            // 
            this.buttonGroup.Image = global::CGEditor_WinForms.Properties.Resources.Group;
            this.buttonGroup.Location = new System.Drawing.Point(8, 3);
            this.buttonGroup.Name = "buttonGroup";
            this.buttonGroup.Size = new System.Drawing.Size(89, 123);
            this.buttonGroup.TabIndex = 0;
            this.buttonGroup.Text = "Group";
            this.buttonGroup.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonGroup.UseVisualStyleBackColor = true;
            this.buttonGroup.Click += new System.EventHandler(this.BtnGroup_Click);
            // 
            // buttonUngroup
            // 
            this.buttonUngroup.Image = global::CGEditor_WinForms.Properties.Resources.Ungroup;
            this.buttonUngroup.Location = new System.Drawing.Point(103, 3);
            this.buttonUngroup.Name = "buttonUngroup";
            this.buttonUngroup.Size = new System.Drawing.Size(89, 123);
            this.buttonUngroup.TabIndex = 1;
            this.buttonUngroup.Text = "Ungroup";
            this.buttonUngroup.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonUngroup.UseVisualStyleBackColor = true;
            this.buttonUngroup.Click += new System.EventHandler(this.BtnUnGroup_Click);
            // 
            // buttonLayerUp
            // 
            this.buttonLayerUp.Image = global::CGEditor_WinForms.Properties.Resources.Layer_Up;
            this.buttonLayerUp.Location = new System.Drawing.Point(219, 3);
            this.buttonLayerUp.Name = "buttonLayerUp";
            this.buttonLayerUp.Size = new System.Drawing.Size(89, 123);
            this.buttonLayerUp.TabIndex = 2;
            this.buttonLayerUp.Text = "Layer Up";
            this.buttonLayerUp.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonLayerUp.UseVisualStyleBackColor = true;
            this.buttonLayerUp.Click += new System.EventHandler(this.BtnUp_Click);
            // 
            // buttonLayerDown
            // 
            this.buttonLayerDown.Image = global::CGEditor_WinForms.Properties.Resources.Layer_Down;
            this.buttonLayerDown.Location = new System.Drawing.Point(314, 3);
            this.buttonLayerDown.Name = "buttonLayerDown";
            this.buttonLayerDown.Size = new System.Drawing.Size(89, 123);
            this.buttonLayerDown.TabIndex = 3;
            this.buttonLayerDown.Text = "Layer Down";
            this.buttonLayerDown.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonLayerDown.UseVisualStyleBackColor = true;
            this.buttonLayerDown.Click += new System.EventHandler(this.BtnDown_Click);
            // 
            // buttonBringToFront
            // 
            this.buttonBringToFront.Image = global::CGEditor_WinForms.Properties.Resources.Bring_To_Front;
            this.buttonBringToFront.Location = new System.Drawing.Point(409, 3);
            this.buttonBringToFront.Name = "buttonBringToFront";
            this.buttonBringToFront.Size = new System.Drawing.Size(89, 123);
            this.buttonBringToFront.TabIndex = 4;
            this.buttonBringToFront.Text = "Bring to Front";
            this.buttonBringToFront.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonBringToFront.UseVisualStyleBackColor = true;
            this.buttonBringToFront.Click += new System.EventHandler(this.BtnToFront_Click);
            // 
            // buttonSendToBack
            // 
            this.buttonSendToBack.Image = global::CGEditor_WinForms.Properties.Resources.Send_To_Back;
            this.buttonSendToBack.Location = new System.Drawing.Point(504, 4);
            this.buttonSendToBack.Name = "buttonSendToBack";
            this.buttonSendToBack.Size = new System.Drawing.Size(89, 123);
            this.buttonSendToBack.TabIndex = 5;
            this.buttonSendToBack.Text = "Send to Back";
            this.buttonSendToBack.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonSendToBack.UseVisualStyleBackColor = true;
            this.buttonSendToBack.Click += new System.EventHandler(this.BtnToBack_Click);
            // 
            // buttonALeft
            // 
            this.buttonALeft.Image = global::CGEditor_WinForms.Properties.Resources.Align_Left;
            this.buttonALeft.Location = new System.Drawing.Point(619, 3);
            this.buttonALeft.Name = "buttonALeft";
            this.buttonALeft.Size = new System.Drawing.Size(89, 123);
            this.buttonALeft.TabIndex = 6;
            this.buttonALeft.Text = "Align Lefts";
            this.buttonALeft.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonALeft.UseVisualStyleBackColor = true;
            this.buttonALeft.Click += new System.EventHandler(this.AlignLefts_Click);
            // 
            // buttonACenter
            // 
            this.buttonACenter.Image = global::CGEditor_WinForms.Properties.Resources.Align_Center;
            this.buttonACenter.Location = new System.Drawing.Point(714, 3);
            this.buttonACenter.Name = "buttonACenter";
            this.buttonACenter.Size = new System.Drawing.Size(89, 123);
            this.buttonACenter.TabIndex = 7;
            this.buttonACenter.Text = "Align Centers";
            this.buttonACenter.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonACenter.UseVisualStyleBackColor = true;
            this.buttonACenter.Click += new System.EventHandler(this.AlignCenters_Click);
            // 
            // buttonAright
            // 
            this.buttonAright.Image = global::CGEditor_WinForms.Properties.Resources.Align_Right;
            this.buttonAright.Location = new System.Drawing.Point(809, 4);
            this.buttonAright.Name = "buttonAright";
            this.buttonAright.Size = new System.Drawing.Size(89, 123);
            this.buttonAright.TabIndex = 8;
            this.buttonAright.Text = "Align Rights";
            this.buttonAright.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonAright.UseVisualStyleBackColor = true;
            this.buttonAright.Click += new System.EventHandler(this.AlignRights_Click);
            // 
            // buttonATop
            // 
            this.buttonATop.Image = global::CGEditor_WinForms.Properties.Resources.Align_Top;
            this.buttonATop.Location = new System.Drawing.Point(904, 4);
            this.buttonATop.Name = "buttonATop";
            this.buttonATop.Size = new System.Drawing.Size(89, 123);
            this.buttonATop.TabIndex = 9;
            this.buttonATop.Text = "Align Tops";
            this.buttonATop.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonATop.UseVisualStyleBackColor = true;
            this.buttonATop.Click += new System.EventHandler(this.AlignTops_Click);
            // 
            // buttonAMiddle
            // 
            this.buttonAMiddle.Image = global::CGEditor_WinForms.Properties.Resources.Align_Middle;
            this.buttonAMiddle.Location = new System.Drawing.Point(999, 3);
            this.buttonAMiddle.Name = "buttonAMiddle";
            this.buttonAMiddle.Size = new System.Drawing.Size(89, 123);
            this.buttonAMiddle.TabIndex = 10;
            this.buttonAMiddle.Text = "Align Middles";
            this.buttonAMiddle.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonAMiddle.UseVisualStyleBackColor = true;
            this.buttonAMiddle.Click += new System.EventHandler(this.AlignMiddles_Click);
            // 
            // buttonABottom
            // 
            this.buttonABottom.Image = global::CGEditor_WinForms.Properties.Resources.Align_Bottom;
            this.buttonABottom.Location = new System.Drawing.Point(1094, 4);
            this.buttonABottom.Name = "buttonABottom";
            this.buttonABottom.Size = new System.Drawing.Size(89, 123);
            this.buttonABottom.TabIndex = 11;
            this.buttonABottom.Text = "Align Bottoms";
            this.buttonABottom.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.buttonABottom.UseVisualStyleBackColor = true;
            this.buttonABottom.Click += new System.EventHandler(this.AlignBottoms_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1717, 33);
            this.menuStrip1.TabIndex = 7;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadConfigurationToolStripMenuItem,
            this.saveConfigurationToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(50, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // loadConfigurationToolStripMenuItem
            // 
            this.loadConfigurationToolStripMenuItem.Name = "loadConfigurationToolStripMenuItem";
            this.loadConfigurationToolStripMenuItem.Size = new System.Drawing.Size(250, 30);
            this.loadConfigurationToolStripMenuItem.Text = "Load Configuration";
            this.loadConfigurationToolStripMenuItem.Click += new System.EventHandler(this.LoadConfig_Click);
            // 
            // saveConfigurationToolStripMenuItem
            // 
            this.saveConfigurationToolStripMenuItem.Name = "saveConfigurationToolStripMenuItem";
            this.saveConfigurationToolStripMenuItem.Size = new System.Drawing.Size(250, 30);
            this.saveConfigurationToolStripMenuItem.Text = "Save Configuration";
            this.saveConfigurationToolStripMenuItem.Click += new System.EventHandler(this.SaveConfig_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(250, 30);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.ribbonOrbOptionButton1_Click);
            // 
            // MainWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1717, 853);
            this.Controls.Add(this.MainPreview);
            this.Controls.Add(this.groupBox3);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainWindow";
            this.Text = "MainForm";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainWindow_FormClosed);
            this.ResizeBegin += new System.EventHandler(this.MainForm_ResizeBegin);
            this.ResizeEnd += new System.EventHandler(this.MainForm_ResizeEnd);
            this.tabControl1.ResumeLayout(false);
            this.PageCommon.ResumeLayout(false);
            this.PageCommon.PerformLayout();
            this.PageEffects.ResumeLayout(false);
            this.PageEffects.PerformLayout();
            this.PageText.ResumeLayout(false);
            this.PageText.PerformLayout();
            this.PageGraphics.ResumeLayout(false);
            this.PageGraphics.PerformLayout();
            this.PageTickerContent.ResumeLayout(false);
            this.PageTickerContent.PerformLayout();
            this.PageTickerProps.ResumeLayout(false);
            this.PageTickerProps.PerformLayout();
            this.PageImage.ResumeLayout(false);
            this.PageImage.PerformLayout();
            this.PageVideo.ResumeLayout(false);
            this.PageVideo.PerformLayout();
            this.PageFlash.ResumeLayout(false);
            this.PageFlash.PerformLayout();
            this.PageActions.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage PageCommon;
        private System.Windows.Forms.TabPage PageEffects;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ListBox ListBoxTools;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ListBox listBoxItems;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.TabPage PageText;
        private System.Windows.Forms.TabPage PageGraphics;
        private System.Windows.Forms.TabPage PageTickerContent;
        private System.Windows.Forms.TabPage PageTickerProps;
        private System.Windows.Forms.TabPage PageImage;
        private System.Windows.Forms.TabPage PageVideo;
        private System.Windows.Forms.TabPage PageFlash;
        private System.Windows.Forms.TabPage PageActions;
        private CustomPropertyEditors.MiscEditor BasicMiscEditor;
        private CustomPropertyEditors.SimpleColorEditor BasicColorEditor;
        private CustomPropertyEditors.PositionEditor BasicPositionEditor;
        private CustomPropertyEditors.VisibilityEditor BasicVisibilityEditor;
        private CustomPropertyEditors.GlowEditor EffectsGlowEditor;
        private CustomPropertyEditors.BlurEditor EffectsBlurEditor;
        private CustomPropertyEditors.ShadowEditor EffectsShadowEditor;
        private CustomPropertyEditors.SpeedEditor EffectsSpeedEditor;
        private CustomPropertyEditors.OutlineColorEditor TextOutlineEditor;
        private CustomPropertyEditors.GradientEditor TextColorEditor;
        private CustomPropertyEditors.FontEditor TextFontEditor;
        private CustomPropertyEditors.TextContentEditor TextContEditor;
        private CustomPropertyEditors.OutlineColorEditor GraphicsOutlineEditor;
        private CustomPropertyEditors.GradientEditor GraphicsColorEditor;
        private CustomPropertyEditors.ShapeEditor GraphicsShapeEditor;
        private CustomPropertyEditors.FontEditor TickerTextFontEditor;
        private CustomPropertyEditors.TextOutlineColorEditor TickerTextOutlineEditor;
        private CustomPropertyEditors.TextGradientEditor TickerTextColorEditor;
        private CustomPropertyEditors.TickerContentEditor TickerNewContentEditor;
        private CustomPropertyEditors.OutlineColorEditor TickerOutlineEditor;
        private CustomPropertyEditors.GradientEditor TickerColorEditor;
        private CustomPropertyEditors.ShapeEditor TickerShapeEditor;
        private CustomPropertyEditors.TickerEditor TickerPropsEditor;
        private CustomPropertyEditors.OpenImageEditor ImageEditor;
        private CustomPropertyEditors.OpenFlashEditor FlashEditor;
        private XMLDescriptionControl XMLDescription;
        private PreviewControl MainPreview;
        private CustomPropertyEditors.OpenVideoEditor openVideoEditor1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label19;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label20;
        private System.Windows.Forms.Label label23;
        private System.Windows.Forms.Label label22;
        private System.Windows.Forms.Label label21;
        private System.Windows.Forms.Label label24;
        private System.Windows.Forms.Label label25;
        private System.Windows.Forms.Label label26;
        private System.Windows.Forms.Button buttonAright;
        private System.Windows.Forms.Button buttonACenter;
        private System.Windows.Forms.Button buttonALeft;
        private System.Windows.Forms.Button buttonSendToBack;
        private System.Windows.Forms.Button buttonBringToFront;
        private System.Windows.Forms.Button buttonLayerDown;
        private System.Windows.Forms.Button buttonLayerUp;
        private System.Windows.Forms.Button buttonUngroup;
        private System.Windows.Forms.Button buttonGroup;
        private System.Windows.Forms.Button buttonATop;
        private System.Windows.Forms.Button buttonABottom;
        private System.Windows.Forms.Button buttonAMiddle;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadConfigurationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveConfigurationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
    }
}