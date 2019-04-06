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
import specstrgen.PartBuilder;

/**
 *
 * @author lm
 */
public class ColorFlagHandlerSH implements FlagHandler {
    public ColorFlagHandlerSH(SplitStringHandler ss){
	this.ss = ss;
    }
    
    @Override
    public int handle(StandardArgumenter arg, CompositeState cs) {
	int i = cs.literalsCount();
	ss.handle(arg, cs);
	List<String> colors = new ArrayList<>();
	for (int j=cs.literalsCount(); j>i; ++i){
	    colors.add(cs.getStr(i));
	}
        cs.add(PartBuilder.MakeColorPart(colors));
	return 0;
    }
    
    SplitStringHandler ss;
    
}
