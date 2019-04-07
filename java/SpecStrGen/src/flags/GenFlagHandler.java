/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.StandardArgumenter;
import specstrgen.CompositeState;
import stringincludes.CharSet;
import stringincludes.StringIncludes;

/**
 *
 * @author lm
 */
public class GenFlagHandler implements FlagHandler {

    public GenFlagHandler(StringIncludes si) {
	this.si = si;
    }

    public GenFlagHandler() {
	this.si = new StringIncludes();
    }
    

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	String b = arg.get(0);
	if (b.length() < 4) {
	    return 0;
	}
	if (b.charAt(0) == '1') {
	    si.add(CharSet.LOWER);
	}
	if (b.charAt(1) == '1') {
	    si.add(CharSet.UPPER);
	}
	if (b.charAt(2) == '1') {
	    si.add(CharSet.DIGITS);
	}
	if (b.charAt(3) == '1') {
	    si.add(CharSet.SYMBOLS);
	}
	if (b.length() > 4) {
	    si.addChars(b.substring(4));
	}
	if (!si.isEmpty()) {
	    String s = si.toString();
	    cs.add(s);
	    si.clear();
	}
	arg.next(1);
	return 0;
    }
    StringIncludes si;
    
}
