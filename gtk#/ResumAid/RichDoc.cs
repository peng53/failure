using System;
using System.Collections.Generic;

namespace ResumAid
{
    public class RichDoc
    {
        public RichDoc()
        {
        }
        public string Text { get; set; }
        public List<Highlight> Highlights { get; set; }
        public List<int> Keywords { get; set; }
    }

    public class Highlight
    {
        public int Start { get; set; }
        public int Length { get; set; }
    }
}
