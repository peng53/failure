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
        //groupIters = new Dictionary<string, TreeIter>();
        groupPaths = new Dictionary<string, TreePath>();

        treeview1.AppendColumn("Name", new CellRendererText(), "text", 0);
        treeview1.AppendColumn("URL", new CellRendererText(), "text", 1);
        treeview1.Model = treeStore;

    }
    public void AddGroup(string name, string id, string pid=null)
    {
        if (pid == null)
        {
            groupPaths.Add(id, treeStore.GetPath(treeStore.AppendValues(name, id)));
        }
        else
        {
            if (groupPaths.ContainsKey(id) == false && groupPaths.ContainsKey(pid))
            {
                treeStore.GetIter(out TreeIter iter, groupPaths[pid]);
                groupPaths.Add(id, 
                    treeStore.GetPath(
                        treeStore.AppendValues(iter, name + id, id)
                        ));
            }
        }
    }
    public void AddLink(string name, string url, string id)
    {
        if (treeStore != null && groupPaths.ContainsKey(id))
        {
            treeStore.GetIter(out TreeIter iter, groupPaths[id]);
            treeStore.AppendValues(iter, name, url);
        }
    }
    public void GetEntry()
    {
        Console.WriteLine($"{name_entry.Text}, {url_entry.Text}");
    }
    private void AddNewDialog()
    {
    }
    TreeStore treeStore;
    Dictionary<string, TreePath> groupPaths;

}