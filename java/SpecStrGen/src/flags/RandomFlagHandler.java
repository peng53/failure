/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.StandardArgumenter;
import specstrgen.CompositeState;
import specstrgen.PartBuilder;

/**
 *
 * @author lm
 */
public class RandomFlagHandler implements FlagHandler {

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	String s = cs.getLast();
	if (s == null){
	    return 0;
	}
	Integer i = arg.getInt(0);
	if (i != null && i > 0){
	    cs.add(PartBuilder.MakeRandomPartWithSymbols(s, i));
	    arg.next(1);
	}
	return 0;
    }
    
}
