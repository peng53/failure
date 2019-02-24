using System;
using System.Collections.Generic;
using System.Xml.Linq;

namespace HierarchXml
{
    public struct Stats
    {
        public string Name;
        public int Id;
        public int Count;
        public static Stats Empty
        {
            get { return new Stats { Name = "", Id = int.MinValue, Count = 0 }; }
        }
    }
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
            var xGroup = new XElement("Group", new XAttribute("NAME", Name), new XAttribute("id", Id));
            if (links != null)
            {
                foreach (var link in links)
                {
                    xGroup.Add(link.GetXElement());
                }
            }
            return xGroup;
        }
        public void Overwrite(Group group)
        {
            if (group != this)
            {
                Name = group.Name;
                Id = group.Id;
                Pid = group.Pid;
                if (links == null)
                {
                    links = group.links;
                }
                else if (group.links != null)
                {
                    links.AddRange(group.links);
                }
            }
        }
        public Stats GetStats() => new Stats { Name = Name, Count = links?.Count ?? 0, Id = Id };
        public string Name;
        public int Id;
        public int Pid;
        List<LinkItem> links;
    }
}

