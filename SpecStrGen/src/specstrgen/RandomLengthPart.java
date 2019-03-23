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
public class RandomLengthPart implements Part {
    public RandomLengthPart(char[] symbols, int max, Random rand) {
        this.alphabet = symbols;
        this.max = max;
        this.rand = rand;
    }
    @Override
    public String out() {
        StringBuilder sb = new StringBuilder(max);
        this.out(sb);
        return sb.toString();
    }
    @Override
    public void out(StringBuilder sb) {
        for (int i = rand.nextInt(max)+1; i>0; --i){
            sb.append(alphabet[rand.nextInt(alphabet.length)]);
        }
    }
    char[] alphabet;
    int max;
    Random rand;
}
