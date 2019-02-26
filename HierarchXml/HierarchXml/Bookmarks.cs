using System;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Linq;

namespace HierarchXml
{
    public class BookmarkStats
    {
        public int Groups;
        public int Links;
    }
	public class Bookmarks
	{
		public Bookmarks()
		{
            groups = new Dictionary<int, Group>
            {
                { 0, new Group { Id = 0, Name = "Root", Pid = 0 } }
            };
        }
        public void GroupAdd(int gid, Group group, bool overwrite=false)
        {
            if (groups.ContainsKey(gid))
            {
                if (!overwrite)
                {
                    throw new InvalidOperationException("Cannot add duplicate gid");
                }
                else
                {
                    groups[gid].Overwrite(group);
                }
            }
            else
            {
                groups.Add(gid, group);
            }
        }

        public Stats GetStats(int gid)
        {
            if (groups.ContainsKey(gid))
            {
                return groups[gid].GetStats;
            }
            else
            {
                return Stats.Empty;
            }
        }
        public void LinkAdd(int gid, LinkItem linkItem, bool force=false)
        {
            if (groups.ContainsKey(gid) == false)
            {
                if (!force)
                {
                    throw new KeyNotFoundException("No group with matching GID");
                }
                else
                {
                    GroupAdd(gid, new Group { Id = gid, Pid = 0 });

                }
            }
            groups[gid].LinkAdd(linkItem);
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
        public BookmarkStats Counts
        {
            get
            {
                int gCount = 0, lCount = 0;
                foreach (var group in groups)
                {
                    ++gCount;
                    if (group.Key != 0)
                    {
                        lCount += group.Value.GetStats.Count;
                    }
                }
                return new BookmarkStats { Groups = gCount, Links = lCount };
            }
        }
		Dictionary<int, Group> groups;
    }
}
