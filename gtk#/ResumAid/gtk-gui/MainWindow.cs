
// This file has been generated by the GUI designer. Do not modify.

public partial class MainWindow
{
	private global::Gtk.UIManager UIManager;

	private global::Gtk.Action DatabaseAction;

	private global::Gtk.Action DocumentsAction;

	private global::Gtk.Action DocumentAction;

	private global::Gtk.Action AddKeywordAction;

	private global::Gtk.VBox vbox1;

	private global::Gtk.MenuBar menubar2;

	private global::Gtk.HPaned hpaned1;

	private global::Gtk.VPaned vpaned1;

	private global::Gtk.Frame frame2;

	private global::Gtk.Alignment GtkAlignment;

	private global::Gtk.ScrolledWindow scrolledwindow7;

	private global::Gtk.NodeView documentsNV;

	private global::Gtk.Label GtkLabel3;

	private global::Gtk.Frame frame3;

	private global::Gtk.Alignment GtkAlignment1;

	private global::Gtk.ScrolledWindow scrolledwindow8;

	private global::Gtk.NodeView nodeview2;

	private global::Gtk.Label GtkLabel4;

	private global::Gtk.VPaned vpaned2;

	private global::Gtk.Frame frame4;

	private global::Gtk.Alignment GtkAlignment2;

	private global::Gtk.ScrolledWindow scrolledwindow9;

	private global::Gtk.TextView documentText;

	private global::Gtk.Label GtkLabel5;

	private global::Gtk.Frame frame5;

	private global::Gtk.Alignment GtkAlignment3;

	private global::Gtk.ScrolledWindow scrolledwindow10;

	private global::Gtk.TextView keywordsTV;

	private global::Gtk.Label GtkLabel6;

	protected virtual void Build()
	{
		global::Stetic.Gui.Initialize(this);
		// Widget MainWindow
		this.UIManager = new global::Gtk.UIManager();
		global::Gtk.ActionGroup w1 = new global::Gtk.ActionGroup("Default");
		this.DatabaseAction = new global::Gtk.Action("DatabaseAction", global::Mono.Unix.Catalog.GetString("Database"), null, null);
		this.DatabaseAction.ShortLabel = global::Mono.Unix.Catalog.GetString("Database");
		w1.Add(this.DatabaseAction, null);
		this.DocumentsAction = new global::Gtk.Action("DocumentsAction", global::Mono.Unix.Catalog.GetString("Documents"), null, null);
		this.DocumentsAction.ShortLabel = global::Mono.Unix.Catalog.GetString("Documents");
		w1.Add(this.DocumentsAction, null);
		this.DocumentAction = new global::Gtk.Action("DocumentAction", global::Mono.Unix.Catalog.GetString("Document"), null, null);
		this.DocumentAction.ShortLabel = global::Mono.Unix.Catalog.GetString("Document");
		w1.Add(this.DocumentAction, null);
		this.AddKeywordAction = new global::Gtk.Action("AddKeywordAction", global::Mono.Unix.Catalog.GetString("Add Keyword"), null, null);
		this.AddKeywordAction.ShortLabel = global::Mono.Unix.Catalog.GetString("Add Keyword");
		w1.Add(this.AddKeywordAction, null);
		this.UIManager.InsertActionGroup(w1, 0);
		this.AddAccelGroup(this.UIManager.AccelGroup);
		this.Name = "MainWindow";
		this.Title = global::Mono.Unix.Catalog.GetString("MainWindow");
		this.WindowPosition = ((global::Gtk.WindowPosition)(4));
		// Container child MainWindow.Gtk.Container+ContainerChild
		this.vbox1 = new global::Gtk.VBox();
		this.vbox1.Name = "vbox1";
		this.vbox1.Spacing = 6;
		// Container child vbox1.Gtk.Box+BoxChild
		this.UIManager.AddUiFromString(@"<ui><menubar name='menubar2'><menu name='DatabaseAction' action='DatabaseAction'/><menu name='DocumentsAction' action='DocumentsAction'/><menu name='DocumentAction' action='DocumentAction'><menuitem name='AddKeywordAction' action='AddKeywordAction'/></menu></menubar></ui>");
		this.menubar2 = ((global::Gtk.MenuBar)(this.UIManager.GetWidget("/menubar2")));
		this.menubar2.Name = "menubar2";
		this.vbox1.Add(this.menubar2);
		global::Gtk.Box.BoxChild w2 = ((global::Gtk.Box.BoxChild)(this.vbox1[this.menubar2]));
		w2.Position = 0;
		w2.Expand = false;
		w2.Fill = false;
		// Container child vbox1.Gtk.Box+BoxChild
		this.hpaned1 = new global::Gtk.HPaned();
		this.hpaned1.CanFocus = true;
		this.hpaned1.Name = "hpaned1";
		this.hpaned1.Position = 102;
		// Container child hpaned1.Gtk.Paned+PanedChild
		this.vpaned1 = new global::Gtk.VPaned();
		this.vpaned1.CanFocus = true;
		this.vpaned1.Name = "vpaned1";
		this.vpaned1.Position = 112;
		// Container child vpaned1.Gtk.Paned+PanedChild
		this.frame2 = new global::Gtk.Frame();
		this.frame2.Name = "frame2";
		this.frame2.ShadowType = ((global::Gtk.ShadowType)(0));
		// Container child frame2.Gtk.Container+ContainerChild
		this.GtkAlignment = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
		this.GtkAlignment.Name = "GtkAlignment";
		this.GtkAlignment.LeftPadding = ((uint)(12));
		// Container child GtkAlignment.Gtk.Container+ContainerChild
		this.scrolledwindow7 = new global::Gtk.ScrolledWindow();
		this.scrolledwindow7.CanFocus = true;
		this.scrolledwindow7.Name = "scrolledwindow7";
		this.scrolledwindow7.ShadowType = ((global::Gtk.ShadowType)(1));
		// Container child scrolledwindow7.Gtk.Container+ContainerChild
		this.documentsNV = new global::Gtk.NodeView();
		this.documentsNV.CanFocus = true;
		this.documentsNV.Name = "documentsNV";
		this.scrolledwindow7.Add(this.documentsNV);
		this.GtkAlignment.Add(this.scrolledwindow7);
		this.frame2.Add(this.GtkAlignment);
		this.GtkLabel3 = new global::Gtk.Label();
		this.GtkLabel3.Name = "GtkLabel3";
		this.GtkLabel3.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Documents</b>");
		this.GtkLabel3.UseMarkup = true;
		this.frame2.LabelWidget = this.GtkLabel3;
		this.vpaned1.Add(this.frame2);
		global::Gtk.Paned.PanedChild w6 = ((global::Gtk.Paned.PanedChild)(this.vpaned1[this.frame2]));
		w6.Resize = false;
		// Container child vpaned1.Gtk.Paned+PanedChild
		this.frame3 = new global::Gtk.Frame();
		this.frame3.Name = "frame3";
		this.frame3.ShadowType = ((global::Gtk.ShadowType)(0));
		// Container child frame3.Gtk.Container+ContainerChild
		this.GtkAlignment1 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
		this.GtkAlignment1.Name = "GtkAlignment1";
		this.GtkAlignment1.LeftPadding = ((uint)(12));
		// Container child GtkAlignment1.Gtk.Container+ContainerChild
		this.scrolledwindow8 = new global::Gtk.ScrolledWindow();
		this.scrolledwindow8.CanFocus = true;
		this.scrolledwindow8.Name = "scrolledwindow8";
		this.scrolledwindow8.ShadowType = ((global::Gtk.ShadowType)(1));
		// Container child scrolledwindow8.Gtk.Container+ContainerChild
		this.nodeview2 = new global::Gtk.NodeView();
		this.nodeview2.CanFocus = true;
		this.nodeview2.Name = "nodeview2";
		this.scrolledwindow8.Add(this.nodeview2);
		this.GtkAlignment1.Add(this.scrolledwindow8);
		this.frame3.Add(this.GtkAlignment1);
		this.GtkLabel4 = new global::Gtk.Label();
		this.GtkLabel4.Name = "GtkLabel4";
		this.GtkLabel4.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Highlights</b>");
		this.GtkLabel4.UseMarkup = true;
		this.frame3.LabelWidget = this.GtkLabel4;
		this.vpaned1.Add(this.frame3);
		this.hpaned1.Add(this.vpaned1);
		global::Gtk.Paned.PanedChild w11 = ((global::Gtk.Paned.PanedChild)(this.hpaned1[this.vpaned1]));
		w11.Resize = false;
		// Container child hpaned1.Gtk.Paned+PanedChild
		this.vpaned2 = new global::Gtk.VPaned();
		this.vpaned2.CanFocus = true;
		this.vpaned2.Name = "vpaned2";
		this.vpaned2.Position = 193;
		// Container child vpaned2.Gtk.Paned+PanedChild
		this.frame4 = new global::Gtk.Frame();
		this.frame4.Name = "frame4";
		this.frame4.ShadowType = ((global::Gtk.ShadowType)(0));
		// Container child frame4.Gtk.Container+ContainerChild
		this.GtkAlignment2 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
		this.GtkAlignment2.Name = "GtkAlignment2";
		this.GtkAlignment2.LeftPadding = ((uint)(12));
		// Container child GtkAlignment2.Gtk.Container+ContainerChild
		this.scrolledwindow9 = new global::Gtk.ScrolledWindow();
		this.scrolledwindow9.CanFocus = true;
		this.scrolledwindow9.Name = "scrolledwindow9";
		this.scrolledwindow9.ShadowType = ((global::Gtk.ShadowType)(1));
		// Container child scrolledwindow9.Gtk.Container+ContainerChild
		this.documentText = new global::Gtk.TextView();
		this.documentText.CanFocus = true;
		this.documentText.Name = "documentText";
		this.scrolledwindow9.Add(this.documentText);
		this.GtkAlignment2.Add(this.scrolledwindow9);
		this.frame4.Add(this.GtkAlignment2);
		this.GtkLabel5 = new global::Gtk.Label();
		this.GtkLabel5.Name = "GtkLabel5";
		this.GtkLabel5.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Document</b>");
		this.GtkLabel5.UseMarkup = true;
		this.frame4.LabelWidget = this.GtkLabel5;
		this.vpaned2.Add(this.frame4);
		global::Gtk.Paned.PanedChild w15 = ((global::Gtk.Paned.PanedChild)(this.vpaned2[this.frame4]));
		w15.Resize = false;
		// Container child vpaned2.Gtk.Paned+PanedChild
		this.frame5 = new global::Gtk.Frame();
		this.frame5.Name = "frame5";
		this.frame5.ShadowType = ((global::Gtk.ShadowType)(0));
		// Container child frame5.Gtk.Container+ContainerChild
		this.GtkAlignment3 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
		this.GtkAlignment3.Name = "GtkAlignment3";
		this.GtkAlignment3.LeftPadding = ((uint)(12));
		// Container child GtkAlignment3.Gtk.Container+ContainerChild
		this.scrolledwindow10 = new global::Gtk.ScrolledWindow();
		this.scrolledwindow10.CanFocus = true;
		this.scrolledwindow10.Name = "scrolledwindow10";
		this.scrolledwindow10.ShadowType = ((global::Gtk.ShadowType)(1));
		// Container child scrolledwindow10.Gtk.Container+ContainerChild
		this.keywordsTV = new global::Gtk.TextView();
		this.keywordsTV.CanFocus = true;
		this.keywordsTV.Name = "keywordsTV";
		this.keywordsTV.Editable = false;
		this.keywordsTV.CursorVisible = false;
		this.keywordsTV.WrapMode = ((global::Gtk.WrapMode)(2));
		this.scrolledwindow10.Add(this.keywordsTV);
		this.GtkAlignment3.Add(this.scrolledwindow10);
		this.frame5.Add(this.GtkAlignment3);
		this.GtkLabel6 = new global::Gtk.Label();
		this.GtkLabel6.Name = "GtkLabel6";
		this.GtkLabel6.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Keywords</b>");
		this.GtkLabel6.UseMarkup = true;
		this.frame5.LabelWidget = this.GtkLabel6;
		this.vpaned2.Add(this.frame5);
		this.hpaned1.Add(this.vpaned2);
		this.vbox1.Add(this.hpaned1);
		global::Gtk.Box.BoxChild w21 = ((global::Gtk.Box.BoxChild)(this.vbox1[this.hpaned1]));
		w21.Position = 1;
		this.Add(this.vbox1);
		if ((this.Child != null))
		{
			this.Child.ShowAll();
		}
		this.DefaultWidth = 472;
		this.DefaultHeight = 300;
		this.Show();
		this.DeleteEvent += new global::Gtk.DeleteEventHandler(this.OnDeleteEvent);
		this.AddKeywordAction.Activated += new global::System.EventHandler(this.AddKeywordToDocument);
	}
}
