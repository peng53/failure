/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author lm
 */
public class SpecStrGen {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        PartBuilder builder = new PartBuilder();
        List<Part> parts = new LinkedList<Part>();
        parts.add(builder.MakeColorPart(
                new String[] {
                    "hector",
                    "mike",
                    "tony",
                    "robert"
                }
        ));
        parts.add(builder.MakePlainPart("_"));
        parts.add(builder.MakeRandomPartDigits(3));
        parts.add(builder.MakeRandomLengthPart(builder.digits, 2));
        parts.add(builder.MakePlainPart("@"));
        parts.add(builder.MakeColorPart(
                new String[] {
                    "hmail",
                    "coldmail",
                    "hooray",
                    "hmx"
                }
        ));
        parts.add(builder.MakePlainPart(".com"));
        StringBuilder sb = new StringBuilder();
        for (int c = 10; c > 0; --c){
            for (Part p : parts){
                p.out(sb);
            }
            sb.append('\n');
        }
        System.out.println(sb.toString());
    }
    
}
