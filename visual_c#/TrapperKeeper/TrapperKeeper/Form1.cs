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
    public partial class Form1 : Form
    {
        TreeNode treeviewRoot;
        public Form1()
        {
            InitializeComponent();
            addDialog = new AddDialog();
        }

        private void InitRootNode()
        {
            treeviewRoot = new TreeNode("Root");
            treeView1.Nodes.Add(treeviewRoot);
            treeviewRoot.Nodes.Add("test");
            treeviewRoot.Nodes.Add("group1", "fight");
            if (treeviewRoot.Nodes.ContainsKey("group1"))
            {
                Console.WriteLine(treeviewRoot.Nodes["group1"]);
            }
        }
        private void AddNode(string text, TreeNode key=null)
        {
            if (key == null)
            {
                treeView1.Nodes.Add(new TreeNode(text));
            }
            else
            {
                key.Nodes.Add(text);
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void menu_file_new_Click(object sender, EventArgs e)
        {
            InitRootNode();
        }
        private void menu_file_close_Click(object sender, EventArgs e)
        {
            treeView1.Nodes.Clear();
        }
        private void menu_file_quit_Click(object sender, EventArgs e)
        {
            ExitApp();
        }
        private void menu_edit_add_child_Click(object sender, EventArgs e)
        {
            TreeNodeCollection tree = (treeView1.SelectedNode == null) ? treeView1.Nodes : treeView1.SelectedNode.Nodes;
            addDialog.UseAddDialog(tree);
        }
        private void menu_edit_add_sibling_Click(object sender, EventArgs e)
        {
            TreeNodeCollection tree;
            if (treeView1.SelectedNode == null || treeView1.SelectedNode.Parent == null)
            {
                tree = treeView1.Nodes;
            }
            else
            {
                tree = treeView1.SelectedNode.Parent.Nodes;
            }
            addDialog.UseAddDialog(tree);
        }
        private void menu_edit_delete_node_Click(object sender, EventArgs e)
        {
            if (treeView1.SelectedNode != null)
            {
                treeView1.Nodes.Remove(treeView1.SelectedNode);
            }
        }
        private void ExitApp()
        {
            Close();
        }
        private void toolbar_unselect_Click(object sender, EventArgs e)
        {
            treeView1.SelectedNode = null;
        }
        AddDialog addDialog;
    }
}
