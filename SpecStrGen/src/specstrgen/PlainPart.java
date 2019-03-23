/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

/**
 *
 * @author lm
 */
public class PlainPart implements Part {
    public PlainPart(String str){
        this.str = str;
    }
    @Override
    public String out() {
        StringBuilder sb = new StringBuilder(str.length());
        this.out(sb);
        return sb.toString();        
    }
    @Override
    public void out(StringBuilder sb) {
        sb.append(str);
    }
    String str;
}
