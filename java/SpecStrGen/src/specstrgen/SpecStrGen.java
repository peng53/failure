/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import argumenter.Argumenter;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.function.Function;

/**
 *
 * @author lm
 */
public class SpecStrGen {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
	List<String> testArgs = new ArrayList<String>(
		Arrays.asList(new String[]{
	    "-s", "ABC",
	    "-C", "/", "hector/mike/tony/robert",
	    "-P", "_",
	    "-g", "0010",
	    "-r", "3",
	    "-s", "XRT",
	    "-R", "3",
	    "-P", "@",
	    "-C", "/", "hmail/coldmail/hooray/hmx",
	    "-P", ".com"
	}
		)
	);
	Argumenter<String> arm = new Argumenter<>(
		args.length==0 ? testArgs : Arrays.asList(args)
	);
	Argument amk = new Argument();
	while (!arm.atEnd()) {
	    String flag = arm.get(0);
	    arm.next(1);
	    if (flag.length() != 2) {
		System.out.println("ignored");
		System.out.println(flag);
		continue;
	    }
	    char c = flag.charAt(1);
	    switch (c) {
		case 'l':
		    amk.lFlag(arm);
		    break;
		case 'p':
		    amk.pFlag(arm);
		    break;
		case 'P':
		    amk.PFlag(arm);
		    break;
		case 's':
		    amk.sFlag(arm);
		    break;
		case 'S':
		    amk.SFlag(arm);
		    break;
		case 'R':
		    amk.RFlag(arm);
		    break;
		case 'r':
		    amk.rFlag(arm);
		    break;
		case 'C':
		    amk.CFlag(arm);
		    break;
		case 'g':
		    amk.gFlag(arm);
		    break;
		default:
		    System.out.println("need case for ");
		    System.out.println(c);
	    }
	}

	for (int c = 5; c > 0; --c) {
	    System.out.println(amk.product);
	}

    }
}
