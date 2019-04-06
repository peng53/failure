/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.StandardArgumenter;
import specstrgen.CompositeState;

/**
 *
 * @author lm
 */
public class SplitStringHandler implements FlagHandler {

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	String del = arg.get(0);
	if (del.length() < 1) {
	    return 1;
	}
	String list = arg.get(1);
	String[] strs = list.split(del);
	if (strs.length == 0){
	    return 1;
	}
	for (String str : strs) {
	    cs.add(str);
	}
	arg.next(2);
	return 0;
    }
    
}
