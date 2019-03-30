/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

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
		"-s ABC -C / hector/mike/tony/robert -P _"
		"-g 0010 -r 3 -s XRT -R 3 -P @ -C / hmail/coldmail/hooray/hmx
		"-P .com"
		* Would produce: 
		*/
		amk.sFlag("ABC"); // 0
		//amk.RFlag("1");
		amk.CFlag("/", "hector/mike/tony/robert"); // 1 - 4
		amk.PFlag("_"); // 5
		amk.gFlag("0010"); // 6
		amk.rFlag("3");
		amk.sFlag("XRT"); // 7
		amk.RFlag("3");
		amk.PFlag("@"); // 8
		amk.CFlag("/", "hmail/coldmail/hooray/hmx"); // 9 - 12
		amk.PFlag(".com"); // 13
		for (int c = 10; c>0; --c){
			System.out.println(amk.product);
		}
    }
}
