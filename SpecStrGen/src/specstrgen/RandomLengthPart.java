/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.Random;

/**
 *
 * @author lm
 */
public class RandomLengthPart extends RandomPart {
    public RandomLengthPart(char[] symbols, int max, Random rand) {
        super(symbols,0,rand);
        this.max = max+1;
    }
    @Override
    public void out(StringBuilder sb) {
        length = gen.nextInt(max);
        super.out(sb);
    }
    int max;
}
