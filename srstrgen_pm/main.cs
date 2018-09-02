using System;

namespace PartedStringNS {
	public class SrStrGen {
		public static void Main(string[] args){
			PartedString s = new PartedString();
			s.add_literal("0123456789");
			s.add_literal("-");
			s.add_part(PartedString.PartT.RandomPool,0,5);
			s.add_part(PartedString.PartT.Plain,1);
			s.add_part(PartedString.PartT.Clone,0);
			Console.WriteLine("test: {0}", s.pout());
			//Console.WriteLine("here {0}",a.t);
		}	
	}
}