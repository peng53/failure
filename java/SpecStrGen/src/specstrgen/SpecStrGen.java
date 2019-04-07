/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import argumenter.Argumenter;
import argumenter.StandardArgumenter;
import flags.DefaultFlagProcessor;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;

/**
 *
 * @author lm
 */
public class SpecStrGen {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
	List<String> testArgs = new ArrayList<>(
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
	while (!arm.atEnd()){
	    amk.process(arm);
	}
	System.out.println("With 'Argument'.");
	for (int c = 5; c > 0; --c) {
	    System.out.println(amk.product);
	}
	StandardArgumenter stdArg = new StandardArgumenter(
	    args.length==0 ? testArgs : Arrays.asList(args)
	);
	CompositeState cs = new CompositeState();
	DefaultFlagProcessor fh = new DefaultFlagProcessor();
	while (!stdArg.atEnd()){
	    fh.process(stdArg, cs);
	}
	System.out.println("With 'FlagProcessor'.");
	for (int c = 5; c > 0; --c) {
	    System.out.println(cs);
	}
    }
}
