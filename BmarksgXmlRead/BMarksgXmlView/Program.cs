﻿using System;
using Gtk;
using System.Linq;
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
            }
            win.Show();
            Application.Run();
            //var findit = document.Descendants("Group").FirstOrDefault(el => el.Attribute("Id")?.Value == "5");
            //Console.WriteLine(findit);
            document.Save("../../../../in_out/test.xml");
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
