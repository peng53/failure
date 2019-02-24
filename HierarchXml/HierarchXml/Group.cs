using System;
using System.Collections.Generic;
using System.Xml.Linq;

namespace HierarchXml
{
    public class Group
    {
        public Group()
        {
            links = null; 
        }
        public void LinkAdd(LinkItem link)
        {
            if (links == null)
            {
                links = new List<LinkItem>();
            }
            links.Add(link);
        }
        public XElement GetXElement()
        {
            var xGroup = new XElement("Group", new XAttribute("NAME", Name), new XAttribute("Id", Id));
            if (links != null)
            {
                foreach (var link in links)
                {
                    xGroup.Add(link.GetXElement());
                }
            }
            return xGroup;
        }
        public string Name;
        public int Id;
        public int Pid;
        List<LinkItem> links;
    }
}
