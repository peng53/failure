package srstrgenj;

import java.util.ArrayList;
import java.util.Random;

public class PartedString {
	class StC {
		public int I, D;
		public int L = 0, W = 1;
		public StC(int i){
			I = i;
		}
		public StC(int i,int l){
			I = i;
			L = l;
		}
		public StC(int i,int d,int w){
			I = i;
			D = d;
			W = w;
		}
		public StC(int i,int l,int d,int w){
			I = i;
			L = l;
			D = d;
			W = w;
		}
	}
	private ArrayList<StC> parts;
	private ArrayList<String> lits;
	public PartedString(){
		lits = new ArrayList<String>();
		parts = new ArrayList<StC>();
	}
	public int lit_size(){
		return lits.size();
	}
	public void add_lit(String s){
		lits.add(s);
	}
	public String get_lit(int i){
		return lits.get(i);
	}
	public void add_part(String s){
		lits.add(s);
		parts.add(new StC(lits.size()-1));
	}
	public void add_part(int i){
		if (i<lits.size()){
			parts.add(new StC(i));
		}
		//System.out.println(Integer.toString(i));
	}
	public void add_part(int i, int l){
		if (i<lits.size()){
			parts.add(new StC(i,l));
		}
	}
	public void add_part(int i, int d, int w){
		if (i<lits.size() && d<lits.size()){
			parts.add(new StC(i,d,w));
		}
	}
	public void add_part(int i, int l, int d, int w){
		if (i<lits.size() && d<lits.size()){
			parts.add(new StC(i,l,d,w));
		}
	}
	private void out_part(StringBuilder b, int i){
		if (i<parts.size()){
			//StringBuilder b = new StringBuilder();
			StC P = parts.get(i);
			if (P.W==0){ //case w=0
				Random coin = new Random();
				b.append(lits.get(coin.nextInt(1)==1 ? P.I : P.D ));
			} else if (P.L!=0){ //random
				int rlen = lits.get(P.I).length();
				Random rand = new Random();
				for (int w=P.W;w>0;--w){
					for (int l=0;l<P.L;++l){
						b.append(lits.get(P.I).charAt(rand.nextInt(rlen)));
					}
					if (w!=1) b.append(lits.get(P.D));
				}
			} else { //case constant
				for (int w=P.W;w>0;--w){
					b.append(lits.get(P.I));
					if (w!=1) b.append(lits.get(P.D));
				}
			}
			//return b.toString();
		}
		//return "";
	}
	public String toString(){
		int l = parts.size();
		StringBuilder b = new StringBuilder();
		for (int i=0;i<l;++i){
			//b.append(out_part(i));
			out_part(b,i);
		}
		return b.toString();
	}
	/*
	public static void main(String[] arg){
		String s = "hello world";
		PartedString M = new PartedString();
		M.add_lit(s);
		M.add_lit("abc");
		M.add_part(0);
		M.add_part(1,25);
		System.out.println(M);
	}
	*/
}
