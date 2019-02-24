using System;
using System.Xml.Linq;
namespace HierarchXml
{
    public struct LinkItem
    {
        public string Title;
        public string Target;
        public XElement GetXElement()
        {
            return new XElement("Link", new XAttribute("NAME", Title), new XAttribute("URL", Target));
        }
    }
}
