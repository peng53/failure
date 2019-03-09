using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TrapperKeeper
{
    public partial class AddDialog : Form
    {
        public AddDialog()
        {
            InitializeComponent();
        }
        private void AddDialog_Load(object sender, EventArgs e)
        {

        }
        public void UseAddDialog(TreeNodeCollection nodes)
        {
            Nodes = nodes;
            Show();
        }
        private void button_cancel_Click(object sender, EventArgs e)
        {
            EndDialog();
        }
        private void button_ok_Click(object sender, EventArgs e)
        {
            if (textbox_label.Text.Length > 0)
            {
                CreateNewNode(textbox_label);
                textbox_label.Clear();
                EndDialog();
            }
        }
        private void CreateNewNode(TextBox tb)
        {
            Nodes.Add(tb.Text);
        }
        private void EndDialog()
        {
            Hide();
        }
        TreeNodeCollection Nodes;

        private void textbox_label_KeyDown(object sender, KeyEventArgs e)
        {
            if (sender is TextBox) {
                var tb = (TextBox)sender;
                switch (e.KeyCode)
                {
                    case Keys.Enter:
                        if (tb.Text.Length > 0)
                        {
                            CreateNewNode(tb);
                            tb.Clear();
                            EndDialog();
                        }
                    break;
                }
            }
        }
    }
}
