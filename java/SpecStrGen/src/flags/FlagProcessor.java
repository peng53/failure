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
public interface FlagProcessor {
    
    public void process(StandardArgumenter arg, CompositeState cs);
    
}
