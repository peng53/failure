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
            Application.Init();
            MainWindow win = new MainWindow();

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
                        win.AddLink(link.Attribute("NAME").Value, link.Attribute("URL").Value);
                        Console.WriteLine($"\t{link}");
                    }

                }
            }

            win.Show();
            Application.Run();

        }
    }
}
