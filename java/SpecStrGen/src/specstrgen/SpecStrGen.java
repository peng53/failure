/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.Arrays;
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
		/**
		"-s ABC -s hector -s mike -s tony -s robert -c 1,2,3,4 -s _ -p"
		"-s 1234567890 -r 3 -s XRT -R 3 -s @ -p -s hmail -s coldmail -s hooray"
		"-s hmx -c 9,10,11,12 -s .com -p"
		* Would produce: 
		*/
		amk.sFlag("ABC"); // 0
		//amk.RFlag("1");
		amk.sFlag("hector"); // 1
		amk.sFlag("mike"); // 2
		amk.sFlag("tony"); // 3
		amk.sFlag("robert"); // 4
		amk.cFlag("1,2,3,4");
		amk.sFlag("_"); // 5
		amk.pFlag();
		amk.sFlag(StringIncludes.DIGITS); // 6
		amk.rFlag("3");
		amk.sFlag("XRT"); // 7
		amk.RFlag("3");
		amk.sFlag("@"); // 8
		amk.pFlag();
		amk.sFlag("hmail"); // 9
		amk.sFlag("coldmail"); // 10
		amk.sFlag("hooray"); // 11
		amk.sFlag("hmx"); // 12
		amk.cFlag("9,10,11,12");
		amk.sFlag(".com"); // 13
		amk.pFlag();
		for (int c = 10; c>0; --c){
			System.out.println(amk.product);
		}
    }
    List<String> literals;
	List<Part> parts;
}
