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
public class StringFlagHandler implements FlagHandler {

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	String s = arg.get(0);
	arg.next(1);
	cs.add(s);
	return 0;
    }
}
