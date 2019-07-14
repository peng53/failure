using System;
using Gtk;
using System.Xml.Linq;
using System.Collections.Generic;

namespace BMarksgXmlView
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            Application.Init();
            MainWindow win = new MainWindow();

            var document = XDocument.Load("../../../../in_out/test.xml");
            foreach (XElement node in document.Elements("Bookmarks").Elements("Group"))
            {
                AddGroup(node,win);
                //Console.WriteLine("start");
                //Console.WriteLine(node);
                //Console.WriteLine("end");
            }
            win.Show();
            Application.Run();

        }
        public static void AddGroup(XElement groups, MainWindow win)
        {
            var xes = new Queue<XElement>();
            xes.Enqueue(groups);
            while (xes.Count > 0)
            {
                var group = xes.Dequeue();
                foreach (var link in group.Elements())
                {
                    if (link.Name == "Group")
                    {
                        xes.Enqueue(link);
                    }
                    else
                    {
                        win.AddLink(link);
                        Console.WriteLine("Adding link for: {0}", link);
                    }
                }
            }
        }
    }
}
