using NUnit.Framework;
using System;
namespace HierarchXml
{
    [TestFixture()]
    public class UnitTests
    {
        [Test()]
        public void BasicStatTest()
        {
            var bookmarks = new Bookmarks();
            bookmarks.GroupAdd(200, new Group { Id = 200, Name = "200g", Pid = 0 });
            bookmarks.LinkAdd(200, new LinkItem { Title = "First", Target = "Target!" });
            bookmarks.LinkAdd(200, new LinkItem { Title = "Poop", Target = "OffTarget!" });
            var stats = bookmarks.GetStats(200);
            Assert.AreEqual(stats.Count, 2);
        }
        [Test()]
        public void BasicStatTestNonGid()
        {
            var bookmarks = new Bookmarks();
            bookmarks.GroupAdd(200, new Group { Id = 200, Name = "200g", Pid = 0 });
            bookmarks.LinkAdd(200, new LinkItem { Title = "First", Target = "Target!" });
            var stats = bookmarks.GetStats(2000);
            Assert.AreEqual(0, stats.Count);
            Assert.AreEqual("", stats.Name);
        }
        [Test()]
        public void BMStats()
        {
            var bookmarks = new Bookmarks();
            bookmarks.GroupAdd(200, new Group { Id = 200, Name = "200g", Pid = 0 });
            for (int i = 0; i < 30; ++i)
            {
                bookmarks.LinkAdd(200, new LinkItem { Title = "f" + i.ToString(), Target = "Target!" });
            }
            var counts = bookmarks.Counts;
            Assert.AreEqual(2, counts.Groups);
            Assert.AreEqual(30, counts.Links);
        }
        [Test()]
        public void Overwrites()
        {
            var bookmarks = new Bookmarks();
            bookmarks.LinkAdd(523, new LinkItem { Title = "VIP", Target = "123 Land" }, force:true);
            var counts = bookmarks.Counts;
            Assert.AreEqual(2, counts.Groups);
            Assert.AreEqual(1, counts.Links);
            bookmarks.GroupAdd(523, new Group { Id = 523, Name = "Milli", Pid = 0 }, overwrite:true);
            counts = bookmarks.Counts;
            Assert.AreEqual(2, counts.Groups);
            Assert.AreEqual(1, counts.Links);
        }
        [Test()]
        public void OverwriteSelf()
        {
            var group = new Group { };
            group.Overwrite(group);
        }
        [Test()]
        public void OverwriteAppend()
        {
            var group = new Group { };
            group.LinkAdd(new LinkItem { Title = "A", Target = "A" });
            group.LinkAdd(new LinkItem { Title = "B", Target = "B" });
            var group_z = new Group { };
            group_z.LinkAdd(new LinkItem { Title = "Z", Target = "Z" });
            group_z.LinkAdd(new LinkItem { Title = "X", Target = "X" });
            group.Overwrite(group_z);
            var stats = group.GetStats;
            Assert.AreEqual(4, stats.Count);
        }
        [Test()]
        public void EmptyNullStat()
        {
            var bookmarks = new Bookmarks();
            var stats = bookmarks.GetStats(101);
            var stats_2 = bookmarks.GetStats(102);
            Assert.AreEqual(0, stats.Count);
            Assert.AreSame(stats, stats_2);
        }
    }
}
