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
    }
}
