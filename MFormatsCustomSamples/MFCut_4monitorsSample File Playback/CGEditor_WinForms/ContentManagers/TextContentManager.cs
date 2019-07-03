using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CGEditor_WinForms.ContentManagers
{
    public partial class TextContentManager : Form
    {
        public TextContentManager()
        {
            InitializeComponent();
        }

        private string m_strNewContent;
        public string NewContent
        {
            get { return m_strNewContent; }
            set { m_strNewContent = value; }
        }

        private void TextContentManager_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                ButtonCancel_Click(this, null);
            }
        }

        private void ButtonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void ButtonOk_Click(object sender, EventArgs e)
        {
            m_strNewContent = TextBoxContent.Text;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            OpenFileDialog openFile = new OpenFileDialog();
            openFile.Multiselect = false;
            openFile.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";

            if (openFile.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    string text = System.IO.File.ReadAllText(openFile.FileName);
                    TextBoxContent.Text = text;
                }
                catch {}
            }
        }
    }
}
