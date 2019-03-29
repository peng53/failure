package SrStrGen;
import SrStrGen.BIncString.BIncString;
import SrStrGen.PartedString.PartedString;
import java.util.ArrayList;

enum CharLib {
	NONE, // 0
	SETS_RELCOM, // 2 b
	SETS, // 3 B
	EXACT,
	COUNT, // 4 c
	SETS_UNION, // 5 a
	NULL // 0
}

public class SrStrGen {
	static CharLib whatFlag(String s){
		if (s.length()==0){
			return CharLib.NONE;
		} else if (s.length()<2){
			return CharLib.EXACT;
		} else if (s.length()==2){
			switch (s.charAt(1)){
				case 's': return CharLib.NONE;
				case 'b': return CharLib.SETS_RELCOM;
				case 'B': return CharLib.SETS;
				case 'a': return CharLib.SETS_UNION;
				case 'c': return CharLib.COUNT;
				default: return CharLib.EXACT;
			}
		} else if (s.charAt(0)=='-' && s.charAt(1)=='-'){
			// Long Form.
			return whatLongFlag(s);
		}
		return CharLib.EXACT;
	}
	
	static CharLib whatLongFlag(String s){
		String sn = s.substring(2).toUpperCase();
		switch (sn){
			case "SET": return CharLib.SETS;
			case "BUT": return CharLib.SETS_RELCOM;
			case "AND": return CharLib.SETS_UNION;
			case "THIS": return CharLib.EXACT;
			case "START": return CharLib.NONE;
			case "COUNT": return CharLib.COUNT;
		}
		return CharLib.EXACT;
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
			switch (whatFlag(A[I])){
				case NULL: break;
				case NONE: return ++I;
				case SETS_RELCOM:
					if (I+2<A.length && A[I+1].length()==6){
						bs = new BIncString(parseBool(A[++I]));
						bs.remove_excs(A[++I].toCharArray());
						M.add_lit(bs.toString());
					}
					break;
				case SETS:
					if (I+1<A.length && A[I+1].length()==6){
						bs = new BIncString(parseBool(A[++I]));
						M.add_lit(bs.toString());
					}
					break;
				case SETS_UNION:
					if (I+2<A.length && A[I+1].length()==6){
						bs = new BIncString(parseBool(A[++I]));
						bs.add_chars(A[++I].toCharArray());
						M.add_lit(bs.toString());
					}
					break;
				case EXACT:
					M.add_lit(A[I]);
					break;
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
	static void help(){
		System.out.println("Usage: program [ -c || --count N ] [strings] [ -s || --start ] [parts]");
		System.out.println();
		System.out.println("If -c / --count is omitted, 1 is assumed.");
		System.out.println();
		System.out.println("Describing 'Strings':");
		System.out.println();
		System.out.println("Exact strings can be used, e.g. 'abc'");
		System.out.println("However, it cannot be the same as a 'special-string flag'.");
		System.out.println("This includes the shorthand b s B a c.");
		System.out.println("And long form: 'set and this but start count'.");
		System.out.println("At the moment, there is no way of using the above other than having the first char not be '-'.");
		System.out.println();
		System.out.println("After one the above flags, follows that option's argument(s).");
		System.out.println("set or B");
		System.out.println("  Creates a string based on includes (6 len boolean).");
		System.out.println("  1/0 = true/false. Positions are lower/UPPER/digits/TOP_SYMBOLS/side_symbols/OTHER_SYMBOLS");
		System.out.println("--");
		System.out.println("but or b");
		System.out.println("  Similar to set but has second parameter which is a string of characters to be removed from the resultant set.");
		System.out.println("--");
		System.out.println("and or a");
		System.out.println("  Similar to set but has second parameter which is a string of characters to be added to the resultant set.");
		System.out.println("--");
		System.out.println("After describing the strings section, you must use the s or start flag.");
		System.out.println("Failing to do so will only print out the strings described.");
		System.out.println();
		System.out.println("--");
		System.out.println("Parts follow s or start:");
		System.out.println();
		System.out.println("Parts are 0 to 4 numbers seperated by comma.");
		System.out.println("Possibilities include:");
		System.out.println("  0 numbers but not 0 length:");
		System.out.println("    Part is intepreted as a Exact String. E.g. as if you described a literal.");
		System.out.println("  1 number n:");
		System.out.println("    Exact literal of nth string you described.");
		System.out.println("  2 numbers m,n:");
		System.out.println("    n random characters from mth string.");
		System.out.println("  3 numbers m,n,o:");
		System.out.println("    mth literal o times with nth literal as a delimiter.");
		System.out.println("  4 numbers m,n,o,p:");
		System.out.println("    n random chars from mth string with o string as delimiter p times.");
		System.out.println("  4 numbers m,n,o,p where p is 0:");
		System.out.println("    mth or oth string with 50-50 chance of either.");
	}
	public static void main(String[] args){
		if (args.length<2){
			help();
			return;
		}
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
