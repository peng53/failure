using System;
using Gtk;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Xml;

public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel)
    {
        Build();
        PrepareTreeView();
        QuitAction.Activated += QuitAction_Activated;
        add_button.Clicked += (o,e) => GetEntry();
        name_entry.Activated += (o, e) => url_entry.GrabFocus();
        url_entry.Activated += (o, e) => GetEntry();
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
    private void PrepareTreeView()
    {
        treeStore = new TreeStore(typeof (string), typeof(string));
        groupIters = new Dictionary<string, TreeIter>();

        treeview1.AppendColumn("Name", new CellRendererText(), "text", 0);
        treeview1.AppendColumn("URL", new CellRendererText(), "text", 1);
        treeview1.Model = treeStore;

    }
    public void AddGroup(string name, string id, string pid=null)
    {
        if (pid == null)
        {
            groupIters.Add(id, treeStore.AppendValues(name, id));
        }
        else
        {
            if (groupIters.ContainsKey(id) == false && groupIters.ContainsKey(pid))
            {
                groupIters.Add(id, treeStore.AppendValues(groupIters[pid], name+id, id));
            }
        }
    }
    public void AddLink(string name, string url, string id)
    {
        if (treeStore != null && groupIters.ContainsKey(id))
        {
            treeStore.AppendValues(groupIters[id], name, url);
        }
    }
    public void GetEntry()
    {
        Console.WriteLine($"{name_entry.Text}, {url_entry.Text}");
    }
    TreeStore treeStore;
    Dictionary<string, TreeIter> groupIters;

}