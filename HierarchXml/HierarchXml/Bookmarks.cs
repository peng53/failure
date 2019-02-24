using System;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Linq;

namespace HierarchXml
{
	public class Bookmarks
	{
		public Bookmarks()
		{
            groups = new Dictionary<int, Group>
            {
                { 0, new Group { Id = 0, Name = "Root", Pid = 0 } }
            };
        }
        public void GroupAdd(int gid, Group group)
        {
            if (groups.ContainsKey(gid))
            {
                throw new InvalidOperationException("Cannot add duplicate gid");
            }
            else
            {
                groups.Add(gid, group);
            }
        }
        public void LinkAdd(int gid, LinkItem linkItem)
        {
            if (groups.ContainsKey(gid) == false)
            {
                throw new KeyNotFoundException("No group with matching GID");
            }
            else
            {
                groups[gid].LinkAdd(linkItem);
            }
        }
        public XElement ToXml()
        {
            var xBookmarks = new XElement("Bookmarks");
            var elements = new Dictionary<int, XElement>();
            foreach (var group in groups)
            {
                elements.Add(group.Key, group.Value.GetXElement());
            }
            foreach (var group in elements)
            {
                var pid = groups[group.Key].Pid;
                if (pid == 0)
                {
                    xBookmarks.Add(group.Value);
                }
                else if (elements.ContainsKey(pid))
                {
                    elements[pid].Add(group.Value);
                }
                else
                {
                    throw new KeyNotFoundException();
                }
            }
            return xBookmarks;
        }
		Dictionary<int, Group> groups;
    }
}
