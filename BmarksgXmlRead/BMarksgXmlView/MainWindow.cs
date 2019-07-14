using System;
using Gtk;
using System.Collections.Generic;
using System.Xml.Linq;
using System.Xml;

using BMarksgXmlView;
using System.Reflection;

public partial class MainWindow : Gtk.Window
{
    public MainWindow() : base(Gtk.WindowType.Toplevel)
    {
        Build();
        QuitAction.Activated += QuitAction_Activated;
        name_entry.Activated += (o, e) => url_entry.GrabFocus();
        url_entry.Activated += (o, e) => GetEntry();
        TreeViewColumn groupColumn = new TreeViewColumn { Title = "Group1" };
        TreeViewColumn nameColumn = new TreeViewColumn { Title = "Name2" };
        TreeViewColumn urlColumn = new TreeViewColumn { Title = "Url3" };
        treeview1.AppendColumn(groupColumn);
        treeview1.AppendColumn(nameColumn);
        treeview1.AppendColumn(urlColumn);
        linksListStore = new TreeStore(typeof(LinkNode));
        treeview1.Model = linksListStore;
        Gtk.CellRendererText groupCell = new CellRendererText();
        groupColumn.PackStart(groupCell, true);

        Gtk.CellRendererText nameCell = new CellRendererText();
        nameColumn.PackStart(nameCell, true);

        Gtk.CellRendererText urlCell = new CellRendererText();
        urlColumn.PackStart(urlCell, true);

        groupColumn.SetCellDataFunc(groupCell, new TreeCellDataFunc(RenderGroupText));
        nameColumn.SetCellDataFunc(nameCell, new TreeCellDataFunc(RenderNameText));
        urlColumn.SetCellDataFunc(urlCell, new TreeCellDataFunc(RenderUrlText));
    }

    private void RenderGroupText(TreeViewColumn tree_column, CellRenderer cell, TreeModel tree_model, TreeIter iter)
    {
        LinkNode link = (LinkNode)tree_model.GetValue(iter, 0);
        (cell as CellRendererText).Text = link.Group;
    }
    private void RenderNameText(TreeViewColumn tree_column, CellRenderer cell, TreeModel tree_model, TreeIter iter)
    {
        LinkNode link = (LinkNode)tree_model.GetValue(iter, 0);
        (cell as CellRendererText).Text = link.Title;
    }
    private void RenderUrlText(TreeViewColumn tree_column, CellRenderer cell, TreeModel tree_model, TreeIter iter)
    {
        LinkNode link = (LinkNode)tree_model.GetValue(iter, 0);
        (cell as CellRendererText).Text = link.Url;
    }
    private void Cwout(object sender, EventArgs e)
    {
        /*
        var selection = (NodeSelection)sender;
        foreach (var sel in selection.SelectedNodes)
        {
            var link = (LinkNode)sel;
            Console.WriteLine($"{link.Group},{link.Title},{link.Url}");
        }
        */
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
        //data.AddNode(new LinkNode(link));
        //store.AppendValues(new LinkNode(link));
        linksListStore.AppendValues(new LinkNode(link));
    }
private void GetEntry()
    {
        Console.WriteLine($"{name_entry.Text}, {url_entry.Text}");
    }
    TreeStore linksListStore;

}