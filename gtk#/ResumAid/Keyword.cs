using System;
namespace ResumAid
{
    [Gtk.TreeNode(ListOnly = true)]
    public class Keyword : Gtk.TreeNode
    {
        public Keyword(string keyword)
        {
            this.keyword = keyword;
        }

        [Gtk.TreeNodeValue(Column = 0)]
        string keyword;
    }
}
