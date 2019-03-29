package SrStrGen.BIncString;

import java.util.Arrays;

public class BIncString {
	static final String CHAR_SEL[]= {
		"abcdefghijklmnopqrstuvwxyz",
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"0123456789",
		"!#$%&*+-=@^_`~",
		"\"',./:;?\\|",
		" ()<>[]{}"
	};
	private String C;
	public BIncString(boolean[] I){
		StringBuilder b = new StringBuilder();
		for (int i=0;i<6;++i){
			if (I[i]) b.append(CHAR_SEL[i]);
		}
		C = b.toString();
	}
	public void remove_excs(char[] E){
		char[] temp_i = C.toCharArray();
		Arrays.sort(temp_i);
		Arrays.sort(E);
		int ii = 0;
		int il = C.length();
		StringBuilder b = new StringBuilder();
		for (char e : E){
			if (temp_i[ii]<=e){
				for (;ii<il;b.append(temp_i[ii++])){
					if (temp_i[ii]==e){
						++ii;
						break;
					}
				}
			}
		}
		for (;ii<il;++ii){
			b.append(temp_i[ii]);
		}
		C = b.toString();
	}
	public void add_chars(char[] A){
		String temp_a = String.valueOf(A);
		C += temp_a;
	}
	public final String toString(){
		return C;
	}
}
