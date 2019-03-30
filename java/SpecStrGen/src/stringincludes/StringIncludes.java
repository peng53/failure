/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package stringincludes;

import java.util.HashSet;

/**
 *
 * @author lm
 */
public class StringIncludes {
    public StringIncludes(){
        alphabet = new HashSet<Character>();
    }
    public void addChars(String s){
        for (char c : s.toCharArray()){
            alphabet.add(c);
        }
    }
    public void add(CharSet... csets){
        for (CharSet cs : csets){
            switch (cs){
                case UPPER:
                    addChars(UALPHA);
                    break;
                case LOWER:
                    addChars(LALPHA);
                    break;
                case DIGITS:
                    addChars(DIGITS);
                    break;
                case SYMBOLS:
                    addChars(SYMBOLS);
                    break;
                case WHITESPACE:
                    addChars(WHITESPACE);
                    break;
            }
        }
    }
    public void removeChars(char... cs){
        for (char c : cs){
            if (alphabet.contains(c)){
                alphabet.remove(c);
            }
        }
    }
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder(alphabet.size());
        for (char c : alphabet){
            sb.append(c);
        }
        return sb.toString();
    }
    public char[] toCharArray(){
        char[] chars = new char[alphabet.size()];
        int i = 0;
        for (char c : alphabet){
            chars[i++] = c;
        }
        return chars;
    }
    public void clear(){
        alphabet.clear();
    }

    public static final String LALPHA = "abcdefghijklmnopqrstuvwxyz";
    public static final String UALPHA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    public static final String DIGITS = "01234567890";
    public static final String SYMBOLS = "!@#$%^&*()-=_+";
    public static final String WHITESPACE = " \t\n";
    HashSet<Character> alphabet;
    
    /*
    public static void main(String[] args){
        StringIncludes si = new StringIncludes();
        si.add(CharSet.LOWER,CharSet.UPPER);
        si.removeChars('a','b');
        si.addChars(StringIncludes.SYMBOLS);
        System.out.println(si);
    }
    */
}
