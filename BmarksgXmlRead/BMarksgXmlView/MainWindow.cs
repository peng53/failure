using System;
using Gtk;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Xml;

using BMarksgXmlView;

public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel)
    {
        Build();
        QuitAction.Activated += QuitAction_Activated;
        name_entry.Activated += (o, e) => url_entry.GrabFocus();
        url_entry.Activated += (o, e) => GetEntry();
        data = new NodeStore(typeof(LinkNode));
        nodeview1.NodeStore = data;
        nodeview1.AppendColumn("Group", new CellRendererText(), "text", 0).Resizable = true;
        nodeview1.AppendColumn("Name", new CellRendererText(), "text",1).Resizable = true;
        nodeview1.AppendColumn("URL", new CellRendererText(), "text",2).Resizable = true;

    }
    private void QuitAction_Activated(object sender, EventArgs e)
    {
        Application.Quit();
    }
    protected void OnDeleteEvent(object sender, DeleteEventArgs a)
    {
        Application.Quit();
        a.RetVal = true;
    }
    public void AddLink(XElement link)
    {
        data.AddNode(new LinkNode(link));
    }
    public void AddGroup(XElement mgroup)
    {
        var xes = new Queue<XElement>();
        xes.Enqueue(mgroup);
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
                    AddLink(link);
                }
            }
        }
    }
    private void GetEntry()
    {
        Console.WriteLine($"{name_entry.Text}, {url_entry.Text}");
    }
    NodeStore data;
}