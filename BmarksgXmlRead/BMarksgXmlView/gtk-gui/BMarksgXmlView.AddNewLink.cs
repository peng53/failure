
// This file has been generated by the GUI designer. Do not modify.
namespace BMarksgXmlView
{
	public partial class AddNewLink
	{
		private global::Gtk.Frame frame1;

		private global::Gtk.Alignment GtkAlignment2;

		private global::Gtk.Notebook notebook4;

		private global::Gtk.VBox vbox3;

		private global::Gtk.Frame frame2;

		private global::Gtk.Alignment GtkAlignment3;

		private global::Gtk.Entry entry3;

		private global::Gtk.Label GtkLabel;

		private global::Gtk.Frame frame3;

		private global::Gtk.Alignment GtkAlignment4;

		private global::Gtk.Entry entry4;

		private global::Gtk.Label GtkLabel4;

		private global::Gtk.Frame frame4;

		private global::Gtk.Alignment GtkAlignment5;

		private global::Gtk.ComboBox combobox1;

		private global::Gtk.Label GtkLabel5;

		private global::Gtk.Label label4;

		private global::Gtk.VBox vbox4;

		private global::Gtk.Frame frame5;

		private global::Gtk.Alignment GtkAlignment6;

		private global::Gtk.Entry entry5;

		private global::Gtk.Label GtkLabel1;

		private global::Gtk.Frame frame6;

		private global::Gtk.Alignment GtkAlignment7;

		private global::Gtk.ComboBox combobox2;

		private global::Gtk.Label GtkLabel7;

		private global::Gtk.Label label5;

		private global::Gtk.Label GtkLabel2;

		private global::Gtk.Button buttonCancel;

		private global::Gtk.Button buttonOk;

		protected virtual void Build()
		{
			global::Stetic.Gui.Initialize(this);
			// Widget BMarksgXmlView.AddNewLink
			this.Name = "BMarksgXmlView.AddNewLink";
			this.WindowPosition = ((global::Gtk.WindowPosition)(4));
			// Internal child BMarksgXmlView.AddNewLink.VBox
			global::Gtk.VBox w1 = this.VBox;
			w1.Name = "dialog1_VBox";
			w1.BorderWidth = ((uint)(2));
			// Container child dialog1_VBox.Gtk.Box+BoxChild
			this.frame1 = new global::Gtk.Frame();
			this.frame1.Name = "frame1";
			this.frame1.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child frame1.Gtk.Container+ContainerChild
			this.GtkAlignment2 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
			this.GtkAlignment2.Name = "GtkAlignment2";
			this.GtkAlignment2.LeftPadding = ((uint)(12));
			// Container child GtkAlignment2.Gtk.Container+ContainerChild
			this.notebook4 = new global::Gtk.Notebook();
			this.notebook4.CanFocus = true;
			this.notebook4.Name = "notebook4";
			this.notebook4.CurrentPage = 1;
			// Container child notebook4.Gtk.Notebook+NotebookChild
			this.vbox3 = new global::Gtk.VBox();
			this.vbox3.Name = "vbox3";
			this.vbox3.Spacing = 6;
			// Container child vbox3.Gtk.Box+BoxChild
			this.frame2 = new global::Gtk.Frame();
			this.frame2.Name = "frame2";
			this.frame2.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child frame2.Gtk.Container+ContainerChild
			this.GtkAlignment3 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
			this.GtkAlignment3.Name = "GtkAlignment3";
			this.GtkAlignment3.LeftPadding = ((uint)(12));
			// Container child GtkAlignment3.Gtk.Container+ContainerChild
			this.entry3 = new global::Gtk.Entry();
			this.entry3.CanFocus = true;
			this.entry3.Name = "entry3";
			this.entry3.IsEditable = true;
			this.entry3.InvisibleChar = '●';
			this.GtkAlignment3.Add(this.entry3);
			this.frame2.Add(this.GtkAlignment3);
			this.GtkLabel = new global::Gtk.Label();
			this.GtkLabel.Name = "GtkLabel";
			this.GtkLabel.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Title</b>");
			this.GtkLabel.UseMarkup = true;
			this.frame2.LabelWidget = this.GtkLabel;
			this.vbox3.Add(this.frame2);
			global::Gtk.Box.BoxChild w4 = ((global::Gtk.Box.BoxChild)(this.vbox3[this.frame2]));
			w4.Position = 0;
			w4.Expand = false;
			w4.Fill = false;
			// Container child vbox3.Gtk.Box+BoxChild
			this.frame3 = new global::Gtk.Frame();
			this.frame3.Name = "frame3";
			this.frame3.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child frame3.Gtk.Container+ContainerChild
			this.GtkAlignment4 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
			this.GtkAlignment4.Name = "GtkAlignment4";
			this.GtkAlignment4.LeftPadding = ((uint)(12));
			// Container child GtkAlignment4.Gtk.Container+ContainerChild
			this.entry4 = new global::Gtk.Entry();
			this.entry4.CanFocus = true;
			this.entry4.Name = "entry4";
			this.entry4.IsEditable = true;
			this.entry4.InvisibleChar = '●';
			this.GtkAlignment4.Add(this.entry4);
			this.frame3.Add(this.GtkAlignment4);
			this.GtkLabel4 = new global::Gtk.Label();
			this.GtkLabel4.Name = "GtkLabel4";
			this.GtkLabel4.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Url</b>");
			this.GtkLabel4.UseMarkup = true;
			this.frame3.LabelWidget = this.GtkLabel4;
			this.vbox3.Add(this.frame3);
			global::Gtk.Box.BoxChild w7 = ((global::Gtk.Box.BoxChild)(this.vbox3[this.frame3]));
			w7.Position = 1;
			w7.Expand = false;
			w7.Fill = false;
			// Container child vbox3.Gtk.Box+BoxChild
			this.frame4 = new global::Gtk.Frame();
			this.frame4.Name = "frame4";
			this.frame4.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child frame4.Gtk.Container+ContainerChild
			this.GtkAlignment5 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
			this.GtkAlignment5.Name = "GtkAlignment5";
			this.GtkAlignment5.LeftPadding = ((uint)(12));
			// Container child GtkAlignment5.Gtk.Container+ContainerChild
			this.combobox1 = global::Gtk.ComboBox.NewText();
			this.combobox1.Name = "combobox1";
			this.GtkAlignment5.Add(this.combobox1);
			this.frame4.Add(this.GtkAlignment5);
			this.GtkLabel5 = new global::Gtk.Label();
			this.GtkLabel5.Name = "GtkLabel5";
			this.GtkLabel5.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Group</b>");
			this.GtkLabel5.UseMarkup = true;
			this.frame4.LabelWidget = this.GtkLabel5;
			this.vbox3.Add(this.frame4);
			global::Gtk.Box.BoxChild w10 = ((global::Gtk.Box.BoxChild)(this.vbox3[this.frame4]));
			w10.Position = 2;
			w10.Expand = false;
			w10.Fill = false;
			this.notebook4.Add(this.vbox3);
			// Notebook tab
			this.label4 = new global::Gtk.Label();
			this.label4.Name = "label4";
			this.label4.LabelProp = global::Mono.Unix.Catalog.GetString("Link");
			this.notebook4.SetTabLabel(this.vbox3, this.label4);
			this.label4.ShowAll();
			// Container child notebook4.Gtk.Notebook+NotebookChild
			this.vbox4 = new global::Gtk.VBox();
			this.vbox4.Name = "vbox4";
			this.vbox4.Spacing = 6;
			// Container child vbox4.Gtk.Box+BoxChild
			this.frame5 = new global::Gtk.Frame();
			this.frame5.Name = "frame5";
			this.frame5.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child frame5.Gtk.Container+ContainerChild
			this.GtkAlignment6 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
			this.GtkAlignment6.Name = "GtkAlignment6";
			this.GtkAlignment6.LeftPadding = ((uint)(12));
			// Container child GtkAlignment6.Gtk.Container+ContainerChild
			this.entry5 = new global::Gtk.Entry();
			this.entry5.CanFocus = true;
			this.entry5.Name = "entry5";
			this.entry5.IsEditable = true;
			this.entry5.InvisibleChar = '●';
			this.GtkAlignment6.Add(this.entry5);
			this.frame5.Add(this.GtkAlignment6);
			this.GtkLabel1 = new global::Gtk.Label();
			this.GtkLabel1.Name = "GtkLabel1";
			this.GtkLabel1.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Name</b>");
			this.GtkLabel1.UseMarkup = true;
			this.frame5.LabelWidget = this.GtkLabel1;
			this.vbox4.Add(this.frame5);
			global::Gtk.Box.BoxChild w14 = ((global::Gtk.Box.BoxChild)(this.vbox4[this.frame5]));
			w14.Position = 0;
			w14.Expand = false;
			w14.Fill = false;
			// Container child vbox4.Gtk.Box+BoxChild
			this.frame6 = new global::Gtk.Frame();
			this.frame6.Name = "frame6";
			this.frame6.ShadowType = ((global::Gtk.ShadowType)(0));
			// Container child frame6.Gtk.Container+ContainerChild
			this.GtkAlignment7 = new global::Gtk.Alignment(0F, 0F, 1F, 1F);
			this.GtkAlignment7.Name = "GtkAlignment7";
			this.GtkAlignment7.LeftPadding = ((uint)(12));
			// Container child GtkAlignment7.Gtk.Container+ContainerChild
			this.combobox2 = global::Gtk.ComboBox.NewText();
			this.combobox2.Name = "combobox2";
			this.GtkAlignment7.Add(this.combobox2);
			this.frame6.Add(this.GtkAlignment7);
			this.GtkLabel7 = new global::Gtk.Label();
			this.GtkLabel7.Name = "GtkLabel7";
			this.GtkLabel7.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Parent Group</b>");
			this.GtkLabel7.UseMarkup = true;
			this.frame6.LabelWidget = this.GtkLabel7;
			this.vbox4.Add(this.frame6);
			global::Gtk.Box.BoxChild w17 = ((global::Gtk.Box.BoxChild)(this.vbox4[this.frame6]));
			w17.Position = 1;
			w17.Expand = false;
			w17.Fill = false;
			this.notebook4.Add(this.vbox4);
			global::Gtk.Notebook.NotebookChild w18 = ((global::Gtk.Notebook.NotebookChild)(this.notebook4[this.vbox4]));
			w18.Position = 1;
			// Notebook tab
			this.label5 = new global::Gtk.Label();
			this.label5.Name = "label5";
			this.label5.LabelProp = global::Mono.Unix.Catalog.GetString("Group");
			this.notebook4.SetTabLabel(this.vbox4, this.label5);
			this.label5.ShowAll();
			this.GtkAlignment2.Add(this.notebook4);
			this.frame1.Add(this.GtkAlignment2);
			this.GtkLabel2 = new global::Gtk.Label();
			this.GtkLabel2.Name = "GtkLabel2";
			this.GtkLabel2.LabelProp = global::Mono.Unix.Catalog.GetString("<b>Add new..</b>");
			this.GtkLabel2.UseMarkup = true;
			this.frame1.LabelWidget = this.GtkLabel2;
			w1.Add(this.frame1);
			global::Gtk.Box.BoxChild w21 = ((global::Gtk.Box.BoxChild)(w1[this.frame1]));
			w21.Position = 0;
			w21.Expand = false;
			w21.Fill = false;
			// Internal child BMarksgXmlView.AddNewLink.ActionArea
			global::Gtk.HButtonBox w22 = this.ActionArea;
			w22.Name = "dialog1_ActionArea";
			w22.Spacing = 10;
			w22.BorderWidth = ((uint)(5));
			w22.LayoutStyle = ((global::Gtk.ButtonBoxStyle)(4));
			// Container child dialog1_ActionArea.Gtk.ButtonBox+ButtonBoxChild
			this.buttonCancel = new global::Gtk.Button();
			this.buttonCancel.CanDefault = true;
			this.buttonCancel.CanFocus = true;
			this.buttonCancel.Name = "buttonCancel";
			this.buttonCancel.UseStock = true;
			this.buttonCancel.UseUnderline = true;
			this.buttonCancel.Label = "gtk-cancel";
			this.AddActionWidget(this.buttonCancel, -6);
			global::Gtk.ButtonBox.ButtonBoxChild w23 = ((global::Gtk.ButtonBox.ButtonBoxChild)(w22[this.buttonCancel]));
			w23.Expand = false;
			w23.Fill = false;
			// Container child dialog1_ActionArea.Gtk.ButtonBox+ButtonBoxChild
			this.buttonOk = new global::Gtk.Button();
			this.buttonOk.CanDefault = true;
			this.buttonOk.CanFocus = true;
			this.buttonOk.Name = "buttonOk";
			this.buttonOk.UseStock = true;
			this.buttonOk.UseUnderline = true;
			this.buttonOk.Label = "gtk-ok";
			this.AddActionWidget(this.buttonOk, -5);
			global::Gtk.ButtonBox.ButtonBoxChild w24 = ((global::Gtk.ButtonBox.ButtonBoxChild)(w22[this.buttonOk]));
			w24.Position = 1;
			w24.Expand = false;
			w24.Fill = false;
			if ((this.Child != null))
			{
				this.Child.ShowAll();
			}
			this.DefaultWidth = 400;
			this.DefaultHeight = 300;
			this.Show();
		}
	}
}
