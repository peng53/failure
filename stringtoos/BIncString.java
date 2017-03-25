package stringtoos;

import java.util.Arrays;

// boo

public class BIncString {
	private String C;
	public String toString(){
		return C;
	}
	public BIncString(boolean[] I){
		String C2[]= {
			"abcdefghijklmnopqrstuvwxyz",
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
			"0123456789",
			"!#$%&*+-=@^_`~",
			"\"',./:;?\\|",
			" ()<>[]{}"
		};
		StringBuilder b = new StringBuilder();
		for (int i=0;i<6;++i){
			if (I[i]) b.append(C2[i]);
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
}
