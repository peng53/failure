using System;
using System.Xml;
using System.Xml.Linq;
using Gtk;

namespace BMarksgXmlView
{
    public class LinkNode : TreeNode
    {
        public LinkNode(XElement link)
        {
            element = link;
}
        [Gtk.TreeNodeValue (Column = 0)]
        public string Group
        {
            get => element.Parent.Attribute("Id").Value;
        }
        [Gtk.TreeNodeValue (Column = 1)]
        public string Title
        {
            get => element.Attribute("NAME").Value;
            set => element.Attribute("NAME").SetValue(value);
        }
        [Gtk.TreeNodeValue(Column = 2)]
        public string Url
        {
            get => element.Attribute("URL").Value;
            set => element.Attribute("URL").SetValue(value);
        }
        XElement element;
    }
}
