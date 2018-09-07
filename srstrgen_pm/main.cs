using System;

public class SrStrGen {
	public static void Main(string[] args){
		PartedString s = new PartedString();
		s.add_literal("0123456789");
		s.add_literal('-');
		s.add_part(PartT.RandomPool,0,5);
		s.add_part(PartT.Plain,1,2);
		s.add_part(PartT.Clone,0);
		//Console.WriteLine("test: {0}", s.pout());
		for (int i=0;i<10; ++i){
			Console.WriteLine(s);
		}
		Console.WriteLine("I found {0}.", s.firsts("123"));
		//Console.WriteLine("here {0}",a.t);
	}
}
