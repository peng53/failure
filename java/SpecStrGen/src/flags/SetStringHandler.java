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
public class SetStringHandler implements FlagHandler {

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	Integer i = arg.getInt(0);
	if (i != null && i > 0){
	    cs.chooseStr(i);
	}
	return 0;
    }
    
}
