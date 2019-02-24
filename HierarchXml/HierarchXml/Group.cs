using System;
using System.Xml.Linq;

namespace HierarchXml
{
    public class Group
    {
        public string Name;
        public int Id;
        public int Pid;
        public XElement GetXElement()
        {
            return new XElement("Group", new XAttribute("NAME", Name), new XAttribute("Id", Id));
        }
    }
}
