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
public class RandomPart implements Part {
    public RandomPart(char[] alphabet, int length, Random rand){
        this.alphabet = alphabet;
        this.length = length;
        this.gen = rand;
    }
    @Override
    public String out(){
        StringBuilder sb = new StringBuilder(length);
        this.out(sb);
        return sb.toString();
    }
    @Override
    public void out(StringBuilder sb) {
        for (int i = length; i>0; --i){
            sb.append(alphabet[gen.nextInt(alphabet.length)]);
        }
    }    
    char[] alphabet;
    protected int length;
    protected Random gen;
}
