using System;
using Gtk;
using System.Xml.Linq;
using System.Linq;
using System.Collections.Generic;

namespace BMarksgXmlView
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            var document = XDocument.Load("../../../../in_out/test.xml");
            Queue<XElement> xes = new Queue<XElement>();

            foreach (var node in document.Elements("Bookmarks").Elements("Group"))
            {
                xes.Enqueue(node);
            }
            while (xes.Count > 0)
            {
                var group = xes.Peek();
                xes.Dequeue();
                Console.WriteLine($"group : {group.Attribute("NAME")}");
                foreach (var link in group.Elements())
                {
                    if (link.Name == "Group")
                    {
                        xes.Enqueue(link);
                    }
                    else
                    {
                        Console.WriteLine($"\t{link}");
                    }

                }
            }

            /*
            Application.Init();
            MainWindow win = new MainWindow();
            win.Show();
            Application.Run();
            */
        }
    }
}
