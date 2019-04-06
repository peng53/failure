/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package flags;

import argumenter.StandardArgumenter;
import specstrgen.CompositeState;
import flags.StringFlagHandler;

/**
 *
 * @author lm
 */
public class PlainFlagHandlerSH implements FlagHandler {
    public PlainFlagHandlerSH(PlainFlagHandler p, StringFlagHandler s){
	this.s = s;
	this.p = p;
    }
    
    PlainFlagHandler p;
    StringFlagHandler s;

    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	s.handle(arg, cs);
	p.handle(arg, cs);
	return 0;
    }
    
}
