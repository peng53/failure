/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.StandardArgumenter;
import java.util.HashMap;
import specstrgen.CompositeState;

/**
 *
 * @author lm
 */
public class DefaultFlagProcessor implements FlagProcessor {

    public DefaultFlagProcessor() {
//	flags = new HashMap<>();

	PlainFlagHandler p = new PlainFlagHandler();
	FLAGS.put('p', p);

	StringFlagHandler s = new StringFlagHandler();
	FLAGS.put('s', s);

	FLAGS.put('P', new PlainFlagHandlerSH(p, s));

	SplitStringHandler S = new SplitStringHandler();
	FLAGS.put('S', S);

	FLAGS.put('C', new ColorFlagHandlerSH(S));

	FLAGS.put('r', new RandomFlagHandler());
	FLAGS.put('R', new RandomLengthHandler());
	FLAGS.put('c', new ColorFlagHandler());
	FLAGS.put('g', new GenFlagHandler());
	FLAGS.put('l', new SetStringHandler());

    }

    @Override
    public void process(StandardArgumenter arg, CompositeState cs) {
	if (arg.atEnd()){
	    return;
	}
	String f = arg.get(0);
	// Currently only processes short flags.
	System.out.println(f);
	if (f.length() != 2) {
	    // take it as if -s was before it.
	    FLAGS.get('s').handle(arg, cs);
	    System.out.printf("'%s' was treated as a literal.",f);
	    return;
	}
	arg.next(1);
	int r = arg.remaining();
	if (r < 1) {
	    // All flags ATM require atleast 1 extra arg.
	    return;
	}
	char c = f.charAt(1);
	
	if (FLAGS.containsKey(c)){
	    if ((c=='C' || c=='S') && r<2){
		System.out.printf("Not enough arguments for %c",c);
		return;
	    }
	    FLAGS.get(c).handle(arg,cs);
	} else {
	    arg.next(-1);
	    FLAGS.get('s').handle(arg, cs);
	    System.out.printf("'%s' was treated as a literal.",f);
	}
    }

    static final HashMap<Character, FlagHandler> FLAGS = new HashMap<>();
}
