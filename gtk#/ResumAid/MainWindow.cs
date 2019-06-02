using System;
using Gtk;
using System.Reflection;
using System.Collections.Generic;

public partial class MainWindow : Gtk.Window
{
    ResumAid.AddKeywordDialog addKeywordDialog;
    NodeStore documentsNodes;
    public MainWindow() : base(Gtk.WindowType.Toplevel)
    {
        Build();
        Action<string> printAction = s => Console.WriteLine(s);
        addKeywordDialog = new ResumAid.AddKeywordDialog(AddAKeyword);
        documentsNodes = new NodeStore(typeof(ResumAid.Keyword));
        documentsNV.NodeStore = documentsNodes;
        typeof(NodeView).GetField("store", BindingFlags.Instance | BindingFlags.NonPublic).SetValue(documentsNV, documentsNodes);
        SetDocument(new ResumAid.RichDoc {
            Text = "Sample Text\nPoops",
            Highlights = new List<ResumAid.Highlight> {
                new ResumAid.Highlight { Start = 3, Length = 3 },
                new ResumAid.Highlight { Start = 9, Length = 5 }
            }
            });
    }

    protected void OnDeleteEvent(object sender, DeleteEventArgs a)
    {
        Application.Quit();
        a.RetVal = true;
    }

    protected void AddKeywordToDocument(object sender, EventArgs e)
    {
        addKeywordDialog.Use();
    }

    protected void AddAKeyword(string keyword)
    {
        // ATM only adds to visual pane and not DB
        if (keywordsTV.Buffer.Text.Length > 0)
        {
            keywordsTV.Buffer.Text += ", ";
        }
        keywordsTV.Buffer.Text += keyword;
    }
    protected void SetDocument(ResumAid.RichDoc richDoc)
    {
        documentText.Buffer.Text = richDoc.Text;
        var tag = new TextTag("poop");
        tag.Background = "red";
        documentText.Buffer.TagTable.Add(tag);
        foreach (var hl in richDoc.Highlights)
        {
            documentText.Buffer.ApplyTag("poop", documentText.Buffer.GetIterAtOffset(hl.Start), documentText.Buffer.GetIterAtOffset(hl.Start+hl.Length));
        }

    }

}
