/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.LinkedList;
import java.util.ArrayList;
import java.util.List;
import stringincludes.StringIncludes;
/**
 *
 * @author lm
 */
public class SpecStrGen {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
		Argument amk = new Argument();
		System.out.println(amk.intArgs("12,20,50,hello",2));
        PartBuilder builder = new PartBuilder();
        List<Part> parts = new LinkedList<Part>();
        StringIncludes si = new StringIncludes();
        si.addChars("ABC");
        Part grade = builder.MakeRandomLengthPart(si.toString(), 1);
        // might not have grade at all.
        parts.add(builder.MakeAlignedPart(grade, true, 2, ' '));
        StringBuilder sb = new StringBuilder();
        parts.add(builder.MakeColorPart(
                new String[] {
                    "hector",
                    "mike",
                    "tony",
                    "robert"
                }
        ));
        Part underscore = builder.MakePlainPart("_");
        parts.add(underscore);
        parts.add(builder.MakeRandomPartDigits(3));
        parts.add(builder.MakeRandomLengthPart(StringIncludes.DIGITS, 2));
        parts.add(underscore);
        si.clear();
        si.addChars("XRT");
        parts.add(builder.MakeRandomLengthPart(si.toString(), 3));
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
        for (int c = 10; c > 0; --c){
            for (Part p : parts){
                p.out(sb);
            }
            sb.append('\n');
        }
        System.out.println(sb.toString());
    }
    List<String> literals;
	List<Part> parts;
}
