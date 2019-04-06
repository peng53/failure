/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.Argumenter;
import argumenter.StandardArgumenter;
import specstrgen.CompositeState;
import specstrgen.PartBuilder;

/**
 *
 * @author lm
 */
public class PlainFlagHandler implements FlagHandler {

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	Integer i = arg.getInt(0);
	if (i == null || i < 0) {
	    handleNoArg(cs);
	} else {
	    String s = cs.getStr(i);
	    if (s == null) {
		handleNoArg(cs);
	    } else {
		cs.add(PartBuilder.MakePlainPart(s));
	    }
	    arg.next(1);
	}
	return 0;
    }
    void handleNoArg(CompositeState cs){
	String s = cs.getLast();
	if (s != null){
	    cs.add(PartBuilder.MakePlainPart(s));
	}
    }
    
}
