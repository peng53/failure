using System;
namespace ResumAid
{
    public partial class AddKeywordDialog : Gtk.Dialog
    {
        private Action<string> addKeyword;

        public AddKeywordDialog(Action<string> action)
        {   
            this.Build();
            this.addKeyword = action;
            this.Hide();
        }
        public void Use()
        {
            Show();
            keywordEntry.GrabFocus();
        }

        protected void ReturnKeyword(object sender, EventArgs e)
        {
            if (keywordEntry.Text.Length > 0)
            {
                addKeyword(keywordEntry.Text);
            }
            ClearAndHide(sender, e);
        }

        protected void ClearAndHide(object sender, EventArgs e)
        {
            Hide();
            keywordEntry.Text = "";
        }
    }
}
