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
		List<Integer> ints = new ArrayList<>(max);
		while (matcher.find() && ints.size()<max){
			ints.add(Integer.parseInt(matcher.group()));
		}
		return ints;
	}
	
	public void lFlag(String s){
		// Sets currentStr.
		Integer i = intArg(s);
		if (i != null || product.hasStr(i)){
			currentStr = product.getStr(i);
		}
	}
	public void pFlag(){
		// Plain part of last/current str.
		if (currentStr == null){
			return;
		}
		product.addPart(builder.MakePlainPart(currentStr));
	}
	public void pFlag(String s){
		// Plain part of nth string.
		Integer i = intArg(s);
		if (i == null || !product.hasStr(i)){
			pFlag();
		} else {
			product.addPart(builder.MakePlainPart(product.getStr(i)));
		}		
	}

	public void rFlag(String s){
		// Random part of last/current str with length n
		if (currentStr == null){
			return;
		}
		Integer i = intArg(s);
		if (i > 0){
			product.addPart(builder.MakeRandomPartWithSymbols(currentStr, i));
		}
	}
	public void RFlag(String s){
		// Random part of last/current str with length upto n
		if (currentStr == null){
			return;
		}
		Integer i = intArg(s);
		if (i > 0){
			product.addPart(builder.MakeRandomLengthPart(currentStr, i));
		}
	}
	
	public void cFlag(String s){
		// Color part.
		List<Integer> ints = intArgs(s,10); // can be more but 10 ATM.
		List<String> colors = new ArrayList<>();
		for (int i : ints){
			if (product.hasStr(i)){
				colors.add(product.getStr(i));
			}
		}
		if (colors.size() > 0){
			product.addPart(builder.MakeColorPart(colors));
			currentStr = colors.get(colors.size()-1);
		}
	}
	
	public void CFlag(String s){
		// Creates copy of nth literal.
		Integer i = intArg(s);
		if (product.hasStr(i)){
			currentStr = product.getStr(i);
			product.addStr(currentStr);
		}
	}
	
	public void sFlag(String s){
		// Adds new literal.
		currentStr = s;
		product.addStr(currentStr);
	}
	
	
	public CompositePartString product;
	PartBuilder builder;
	String currentStr;
	Pattern digit = Pattern.compile("\\d+");
}
