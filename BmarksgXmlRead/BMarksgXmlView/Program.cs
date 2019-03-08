using System;
using Gtk;
using System.Xml.Linq;

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
                win.AddGroup(node);
            }
            win.Show();
            Application.Run();

        }
    }
}
