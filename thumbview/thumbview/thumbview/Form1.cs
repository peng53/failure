using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace thumbview
{
    public partial class Form1 : Form
    {
        System.Drawing.Bitmap loadedImage;
        //System.Windows.Forms.ImageList imglist;
        public Form1()
        {
            InitializeComponent();
        }

        private void CleanUpImages()
        {
            if (this.loadedImage != null) { 
                this.loadedImage.Dispose();
                for (int c = imageList1.Images.Count-1; c>=0; --c)
                {
                    imageList1.Images[c].Dispose();
                }
                imageList1.Images.Clear();
                listView1.Items.Clear();
            }
        }

        private void loadImageButton_Click(object sender, EventArgs e)
        {
            var openfiledialog = new OpenFileDialog
            {
                Filter = "tif files|*.tif"
            };
            if (openfiledialog.ShowDialog() == DialogResult.OK)
            {
                if (this.loadedImage != null){ 
                    CleanUpImages();
                }
                this.loadedImage = new System.Drawing.Bitmap(openfiledialog.FileName);
                this.pictureBox1.Image = this.loadedImage;
                for (int i = 0, c = this.loadedImage.GetFrameCount(System.Drawing.Imaging.FrameDimension.Page); i < c; ++i)
                {
                    this.loadedImage.SelectActiveFrame(System.Drawing.Imaging.FrameDimension.Page, i);
                    imageList1.Images.Add(this.loadedImage.GetThumbnailImage(128, 128, null, IntPtr.Zero));
                    this.listView1.Items.Add(new ListViewItem(String.Format("Page {0}",i), i));
                }
            }
            openfiledialog.Dispose();
        }

        private void Form1_Closed(object sender, EventArgs e)
        {
            CleanUpImages();
            MessageBox.Show("Done!");
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listView1.SelectedIndices.Count > 0)
            {
                int i = listView1.SelectedIndices[0];
                loadedImage.SelectActiveFrame(System.Drawing.Imaging.FrameDimension.Page, i);
                this.pictureBox1.Image = this.loadedImage;
            }
            
        }

        private void checkAllButton_Click(object sender, EventArgs e)
        {
            bool checkAll = checkAllButton.Checked;
            foreach (ListViewItem item in listView1.Items) {
                item.Checked = checkAll;
            }
        }
    }
}
