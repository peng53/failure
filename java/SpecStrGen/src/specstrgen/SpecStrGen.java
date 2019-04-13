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
	StandardArgumenter stdArg = new StandardArgumenter(
	    Arrays.asList(args)
	);
	CompositeState cs = new CompositeState();
	DefaultFlagProcessor fh = new DefaultFlagProcessor();
	Integer times = 1;
	if ("-t".equals(stdArg.get(0))){
	    times = stdArg.getInt(1);
	    if (times != null){
		stdArg.next(2);
	    } else {
		times = 1;
	    }
	}
	// System.out.printf("Will print %d times\n", times); // debug only
	while (!stdArg.atEnd()){
	    fh.process(stdArg, cs);
	}
	for (int c = times; c > 0; --c) {
	    System.out.println(cs);
	}
    }
}
