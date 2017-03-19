package srstrgenj;

import java.util.ArrayList;
import java.util.Random;

public class PartedString {
	private ArrayList<int[]> parts;
	private ArrayList<String> lits;
	public PartedString(){
		lits = new ArrayList<String>();
		parts = new ArrayList<int[]>();
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
		parts.add({lits.size()-1,0,-1,1});
	}
	public void add_part(int i){
		parts.add({i,0,-1,1});
	}
	public void add_part(int i, int l){
		parts.add({i,l,-1,1});
	}
	public void add_part(int i, int d, int w){
		parts.add({i,0,d,w});
	}
	public void add_part(int i, int l, int d, int w){
		parts.add({i,l,d,w});
	}
	private void out_part(StringBuilder b, int i){
		if (i<parts.size()){
			int[] P = parts.get(i);
			if (P[3]==0){ //case w=0
				Random coin = new Random();
				b.append(lits.get(coin.nextInt(1)==1 ? P[0] : P[2] ));
			} else if (P[1]!=0){ //random
				int rlen = lits.get(P[0]).length();
				Random rand = new Random();
				for (int w=P[3];w>0;--w){
					for (int l=0;l<P[1];++l){
						b.append(lits.get(P[0]).charAt(rand.nextInt(rlen)));
					}
					if (w!=1) b.append(lits.get(P[2]));
				}
			} else { //case constant
				for (int w=P[3];w>0;--w){
					b.append(lits.get(P[0]));
					if (w!=1) b.append(lits.get(P[2]));
				}
			}
		}
	}
	public String toString(){
		StringBuilder b = new StringBuilder();
		for (int i=0,l=parts.size();i<l;++i){
			out_part(b,i);
		}
		return b.toString();
	}
}
