/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.Random;
import stringincludes.StringIncludes;

/**
 *
 * @author lm
 */
public class PartBuilder {
    public PartBuilder(){
        rand = new Random();
    }
    public Part MakeRandomPartWithSymbols(char[] symbols, int count){
        if (symbols.length == 0 || count == 0){
            throw new IllegalArgumentException("Cannot make random part without length or symbols.");
        }
        return new RandomPart(symbols, count, rand);
    }
    public Part MakeRandomPartLowerAlpha(int count){
        if (count == 0){
            throw new IllegalArgumentException("Cannot make random part without length.");
        }
        return new RandomPart(StringIncludes.LALPHA, count, rand);
    }
    public Part MakeRandomPartUpperAlpha(int count){
        if (count == 0){
            throw new IllegalArgumentException("Cannot make random part without length.");
        }        
        return new RandomPart(StringIncludes.UALPHA, count, rand);
    }
    public Part MakeRandomPartDigits(int count){
        if (count == 0){
            throw new IllegalArgumentException("Cannot make random part without length.");
        }
        return new RandomPart(StringIncludes.DIGITS, count, rand);
    }
    public Part MakePlainPart(String str){
        return new PlainPart(str);
    }
    public Part MakeColorPart(String[] strs){
        if (strs.length==0){
            throw new IllegalArgumentException("Cannot have ColorPart without colors.");
        }
        return new ColorPart(strs,rand);
    }
    public Part MakeRandomLengthPart(char[] symbols, int count){
        if (count<1){
            throw new IllegalArgumentException("Can't make random length part with count less than 1.");
        }
        return new RandomLengthPart(symbols, count, rand);
    }
    public Part MakeAlignedPart(Part part, boolean leftAlign, int length, char fillChar){
        if (length<1){
            throw new IllegalArgumentException("Can't make alignment with no length.");
        }
        return new SpacedPart(part,leftAlign,length,fillChar);
    }
    Random rand;
}
