package SrStrGen.PartedString;

import java.util.ArrayList;
import java.util.Random;

public class PartedString {
	private ArrayList<int[]> parts = new ArrayList<int[]>();
	private ArrayList<String> lits = new ArrayList<String>();
	public int lit_size(){
		return lits.size();
	}
	public void add_lit(String s){
		lits.add(s);
	}
	public void add_part(String s){
		lits.add(s);
		parts.add(new int[] {lits.size()-1,0,-1,1});
	}
	public void add_part(int i){
		parts.add(new int[] {i,0,-1,1});
	}
	public void add_part(int i, int l){
		parts.add(new int[] {i,l,-1,1});
	}
	public void add_part(int i, int d, int w){
		parts.add(new int[] {i,0,d,w});
	}
	public void add_part(int i, int l, int d, int w){
		parts.add(new int[] {i,l,d,w});
	}
	private Random rand = new Random();
	private void out_part(StringBuilder b, int[] P){
		if (P[3]==0){ //case w=0
			b.append(lits.get(rand.nextBoolean() ? P[0] : P[2] ));
		} else if (P[1]!=0){ //random
			int rlen = lits.get(P[0]).length();
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
	public String toString(){
		StringBuilder b = new StringBuilder();
		for (int[] P : parts){ out_part(b,P);}
		return b.toString();
	}
}
