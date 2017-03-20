import stringtoos.BIncString;
import stringtoos.PartedString;
import java.util.ArrayList;

public class SrStrGen {
	private static int what_flag(String s){
		switch (s.length()){
		case 0: return 0;
		case 2:
			switch (s.charAt(1)){
			case 's': return 1;
			case 'b': return 2;
			case 'B': return 3;
			case 'c': return 4;
			case 'a': return 5;
			}
		}
		return 9;
	}
	private static boolean[] parseBool(String s){
		boolean r[] = {false,false,false,false,false,false};
		for (int i=0;i<6;++i){
			if (s.charAt(i)=='1') r[i]=true;
		}
		return r;
	}
	private static void dry_run(PartedString M){
		int l = M.lit_size();
		M.add_lit("\n");
		for (int i=0;i<l;++i){
			M.add_part(i);
			M.add_part(l);
		}
	}
	private static int literal_stor(int I,String[] A,PartedString M){
		for (BIncString bs;I<A.length;++I){
			switch (what_flag(A[I])){
			case 0: break;
			case 1: return ++I;
			case 2:
				if (I+2<A.length && A[I+1].length()==6){
					bs = new BIncString(parseBool(A[++I]));
					bs.remove_excs(A[++I].toCharArray());
					M.add_lit(bs.toString());
					break;
				}
			case 3:
				if (I+1<A.length && A[I+1].length()==6){
					bs = new BIncString(parseBool(A[++I]));
					M.add_lit(bs.toString());
					break;
				}
			case 5:
				if (I+2<A.length && A[I+1].length()==6){
					bs = new BIncString(parseBool(A[++I]));
					bs.add_chars(A[++I].toCharArray());
					M.add_lit(bs.toString());
					break;
				}
			default: M.add_lit(A[I]);
			}
		}
		return A.length;
	}
	private static void part_stor(int I,String[] A,PartedString M){
		int[] U = new int[4];
		int count,n,b,i,l;
		for (int lit_s=M.lit_size();I<A.length;++I){
			count=0;
			for (i=0,l=A[I].length();i<l && count<4;++count){
				for (n=1;(i<l && !Character.isDigit(A[I].charAt(i)));++i) continue;
				if (i>=l) break;
				for (b=i++;(i<l && Character.isDigit(A[I].charAt(i++)));++n) continue;
				U[count] = Integer.parseInt(A[I].substring(b,b+n));
			}
			if (count==0){
				M.add_part(A[I]);
				++lit_s;
			} else if (U[0]<lit_s){
				switch (count){
				case 1: M.add_part(U[0]); break;
				case 2: M.add_part(U[0],U[1]); break;
				case 3: if (U[1]<lit_s) M.add_part(U[0],U[1],U[2]); break;
				case 4: if (U[2]<lit_s) M.add_part(U[0],U[1],U[2],U[3]); break;
				}
			}
		}
	}
	public static void main(String[] args){
		if (args.length<2) return;
		int C=1,I=0;
		if (what_flag(args[0])==4){
			C = args[1].length();
			if (C==0 || args[1].charAt(0)=='0') return;
			for (int i=0;i<C;++i){
				if (!Character.isDigit(args[1].charAt(i))) return;
			}
			C = Integer.parseInt(args[1]);
			I+=2;
		}
		PartedString M = new PartedString();
		I = literal_stor(I,args,M);
		if (I==args.length) dry_run(M); else part_stor(I,args,M);
		for (;C>0;--C){ System.out.println(M);}
	}
}
