/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.StandardArgumenter;
import java.util.ArrayList;
import java.util.List;
import specstrgen.CompositeState;

/**
 *
 * @author lm
 */
public class ColorFlagHandler implements FlagHandler {

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	List<Integer> ints = arg.intArgs(0); // can be more but 10 ATM.
	List<String> colors = new ArrayList<>();
	for (int i : ints) {
	    if (cs.hasStr(i)) {
		colors.add(cs.getStr(i));
	    }
	}
	arg.next(1);
	return 0;
    }
       
}
