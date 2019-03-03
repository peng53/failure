using System;
using Gtk;

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

        treeview1.Model = treeStore;
    }
    public void AddLink(string name, string url)
    {
        if (treeStore != null)
        {
            treeStore.AppendValues(name, url);
        }
    }
    TreeStore treeStore;
    TreeViewColumn columnName, columnUrl;
    CellRendererText rendererTextName, rendererTextUrl;

}
