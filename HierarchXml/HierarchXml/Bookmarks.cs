using System;
using System.Collections.Generic;
using System.Xml.Linq;

namespace HierarchXml
{
	public class Bookmarks
	{
		public Bookmarks()
		{
			Groups = new Dictionary<int, Group>();
            Links = new Dictionary<int, List<LinkItem>>();
		}
        public void GroupAdd(int gid, Group group)
        {
            if (Groups.ContainsKey(gid))
            {
                throw new InvalidOperationException("Cannot add duplicate gid");
            }
            else
            {
                Groups.Add(gid, group);
            }
        }
        public void LinkAdd(int gid, LinkItem linkItem)
        {
            if (gid != 0 && Groups.ContainsKey(gid) == false)
            {
                throw new KeyNotFoundException("No group with matching GID");
            }
            if (Links.ContainsKey(gid) == false)
            {
                Links.Add(gid, new List<LinkItem>());
            }
            Links[gid].Add(linkItem);
        }
        public bool Enclosed()
        {
            foreach (var group in Links)
            {
                if (Groups.ContainsKey(group.Key) == false)
                {
                    return false;
                }
            }
            return true;
        }
        public XElement ToXml()
        {
            var bookmarks = new XElement("Bookmarks");
            var elements = new Dictionary<int, XElement>();
            foreach (var group in Groups)
            {
                var xGroup = group.Value.GetXElement();
                if (Links.ContainsKey(group.Key))
                {
                    foreach (var link in Links[group.Key])
                    {
                        xGroup.Add(link.GetXElement());
                    }
                }
                if (group.Value.Pid == 0)
                {
                    bookmarks.Add(xGroup);
                }
                elements.Add(group.Key, xGroup);

            }
            foreach (var group in elements)
            {
                var pid = Groups[group.Key].Pid;
                if (pid != 0 && elements.ContainsKey(pid))
                {
                    elements[pid].Add(group.Value);
                }
            }
            return bookmarks;
        }
		public Dictionary<int, Group> Groups;
        public Dictionary<int, List<LinkItem>> Links;
	}
}
