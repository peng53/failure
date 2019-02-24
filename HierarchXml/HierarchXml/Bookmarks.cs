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
        /* Might delete this
        public void LinkAdd(int gid, string title, string target)
        {
            if (Groups.ContainsKey(gid))
            {
                Groups[gid].Element.Add(
                    new XElement("Link",
                        new XAttribute("Title", title),
                        new XAttribute("Target", target)
                    )
                );
            }
            else
            {
                throw new InvalidOperationException("Cannot add link to non-existent group.");
            }
        }
        */
        public void LinkAdd(int gid, LinkItem linkItem)
        {
            if (Links.ContainsKey(gid) == false)
            {
                Links.Add(gid, new List<LinkItem>());
            }
            Links[gid].Add(linkItem);
        }
		public Dictionary<int, Group> Groups;
        public Dictionary<int, List<LinkItem>> Links;
	}
}
