package specstrgen;

import java.util.List;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Argument {
	public Argument(){
		product = new CompositePartString();
		builder = new PartBuilder();
	}
	public Integer intArg(String s){
		// Returns first whole number from string.
		Matcher matcher = digit.matcher(s);
		return (matcher.find() ? Integer.parseInt(matcher.group()) : null);
	}
	public List<Integer> intArgs(String s, int max){
		Matcher matcher = digit.matcher(s);
		List<Integer> ints = new ArrayList<Integer>(max);
		while (matcher.find() && ints.size()<max){
			ints.add(Integer.parseInt(matcher.group()));
		}
		return ints;
	}
	
	public void pFlag(){
		if (currentStr == null){
			return;
		}
		product.addPart(builder.MakePlainPart(currentStr));
	}
	public void pFlag(String s){
		Integer i = intArg(s);
		if (i == null || product.getStr(i) == null){
			pFlag();
		} else {
			product.addPart(builder.MakePlainPart(product.getStr(i)));
		}		
	}
	public void lFlag(String s){
		// Sets currentStr.
		Integer i = intArg(s);
		if (i != null || product.getStr(i) != null){
			currentStr = product.getStr(i);
		}
	}
	public void RFlag(String s){
		List<Integer> args = intArgs(s, 2);
		if (args.size() == 1 && args.get(0) > 0){
			product.addPart(builder.MakeRandomPartWithSymbols(currentStr, args.get(0)));
		}
	}
	
	
	CompositePartString product;
	PartBuilder builder;
	String currentStr;
	Pattern digit = Pattern.compile("\\d+");
}
