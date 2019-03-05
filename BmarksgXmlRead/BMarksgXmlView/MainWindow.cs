using System;
using Gtk;
using System.Collections.Generic;

public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel)
    {
        Build();
        PrepareTreeView();
        QuitAction.Activated += QuitAction_Activated;
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
        treeStore = new TreeStore(typeof(string), typeof(string));
        columnName = new TreeViewColumn { Title = "Name" };
        columnUrl = new TreeViewColumn { Title = "URL" };
        rendererTextName = new CellRendererText();
        rendererTextUrl = new CellRendererText();
        columnName.PackStart(rendererTextName, true);
        columnUrl.PackStart(rendererTextUrl, true);
        treeview1.AppendColumn(columnName);
        treeview1.AppendColumn(columnUrl);
        columnName.AddAttribute(rendererTextName, "text", 0);
        columnUrl.AddAttribute(rendererTextUrl, "text", 1);
        groupIters = new Dictionary<string, TreeIter>();
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
        if (treeStore != null)
        {
            if (groupIters.ContainsKey(id))
            {
                treeStore.AppendValues(groupIters[id], name, url);
                //treeStore.AppendValues(name, url);
            }
        }
    }
    TreeStore treeStore;
    TreeViewColumn columnName, columnUrl;
    CellRendererText rendererTextName, rendererTextUrl;
    Dictionary<string, TreeIter> groupIters;

}
