/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.List;
import java.util.Random;

/**
 *
 * @author lm
 */
public class ColorPart implements Part {

    /**
     *
     * @param colors List of strings to be used as 'colors'
     * @param rand Random function.
     */
    public ColorPart(List<String> colors, Random rand) {
        this.colors = colors;
        this.rand = rand;
    }

    /**
     *
     * @return As a string.
     */
    @Override
    public String out() {
        StringBuilder sb = new StringBuilder();
        this.out(sb);
        return sb.toString();        
    }

    /**
     *
     * @param sb Where to output the part to.
     */
    @Override
    public void out(StringBuilder sb) {
        sb.append(colors.get(rand.nextInt(colors.size())));
    }
    List<String> colors;
    Random rand;
}
