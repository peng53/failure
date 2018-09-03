using System;
using System.Collections.Generic;
using System.Text;

public enum PartT { Plain, RandomPool, RandomN, SubStr, Clone };
public class PartedString {
	/* Currently (as in pout):
	 * Plain
	 * a -> literal index
	 * b -> literal index (delimiter) optional
	 * c -> # times to repeat, default 1
	 * d -> delimiter index
	 *
	 * RandomPool
	 * a -> literal index
	 * b -> a length (# of random chars (of a) to be printed)
	 * c -> literal index
	 * d -> # times to repeat, default 1
	 *
	 * RandomN
	 * a -> min number, if b not present, default to 0
	 * b -> max number, if not present, defaults to a
	 *
	 * SubStr
	 * a -> literal index
	 * b -> start index
	 * c -> end index, last index of char to print
	 *
	 * Clone
	 * a -> part index
	 */
	struct Part {
		public int a,b,c,d;
		public PartT t;
		public Part(PartT _t,int _a,int _b,int _c,int _d){
			t = _t;
			a = _a;
			b = _b;
			c = _c;
			d = _d;
		}
	}
	List<string> literals;
	List<Part> parts;
	Random r;
	public PartedString(){
		literals = new List<string>();
		parts = new List<Part>();
		r = new Random();
	}
	public void add_part(PartT i,int a,int b=-1,int c=-1,int d=-1){
		switch (i){
			case PartT.Plain:
				if (a<0 || a>=literals.Count || c>=literals.Count) return;
				if (c!=-1 && d>literals[c].Length) return;
				if (b<1) b = 1;
				break;
			case PartT.RandomN:
				if (a>b) return;
				++b; // since Random.Next is [) and we want [a,b] as int
				break;
			case PartT.RandomPool:
				if (a<0 || a>=literals.Count || c>=literals.Count) return;
				if (d==-1) d = 1;
				if (b==-1) b = 1;
				break;
			case PartT.SubStr:
				if (a<0 || b==-1 || b>=c || a>=literals.Count || c>(literals[a].Length)) return;
				break;
			case PartT.Clone:
				if (a<0 || a>=parts.Count) return;
				i = parts[a].t;
				b = parts[a].b;
				c = parts[a].c;
				d = parts[a].d;
				a = parts[a].a;
				break;
			default:
				return;
		}
		parts.Add(new Part(i,a,b,c,d));
	}
	public void add_literal(string s){
		literals.Add(s);
	}
	public void add_literal(char c){
		literals.Add(c.ToString());
	}
	public void lit_out(){
		foreach (string s in literals){
			Console.WriteLine(s);
		}
	}
	public override string ToString(){
		StringBuilder sb = new StringBuilder();
		foreach (Part p in parts){
			switch (p.t){
				case PartT.Plain: // string delimited c times.
					for (int i=p.b; i>0; --i){
						sb.Append(literals[p.a]);
						if (p.c>0 && i!=1) sb.Append((p.d==-1) ? literals[p.c] : literals[p.c][p.d].ToString());
					}
					break;
				case PartT.RandomPool: // RANA-DELIMIT-RANA..
					int l = literals[p.a].Length;
					for (int i=p.d; i>0; --i){
						for (int j=p.b; j>0; --j){
							sb.Append(literals[p.a][r.Next(l)]);
						}
						if (p.c!=-1 && i!=1) sb.Append(literals[p.c]);
					}
					break;
				case PartT.SubStr: // substring
					sb.Append(literals[p.a].Substring(p.b,p.c-p.b));
					break;
				case PartT.RandomN:
					sb.Append(r.Next(p.a,p.b));
					break;
			}
		}
		return sb.ToString();
	}

}
