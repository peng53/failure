package specstrgen;

import argumenter.Argumenter;
import java.util.List;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import stringincludes.StringIncludes;
import stringincludes.CharSet;

class Argument {

    public Argument() {
	product = new CompositePartString();
	builder = new PartBuilder();
	si = new StringIncludes();
	numberMatcher = Pattern.compile("\\d+").matcher("");
    }

    public Integer intArg(String s) {
	// Returns first whole number from string.
	numberMatcher.reset(s);
	return (numberMatcher.find() ? Integer.parseInt(numberMatcher.group()) : null);
    }

    public List<Integer> intArgs(String s, int max) {
	numberMatcher.reset(s);
	List<Integer> ints = new ArrayList<>(max);
	while (numberMatcher.find() && ints.size() < max) {
	    ints.add(Integer.parseInt(numberMatcher.group()));
	}
	return ints;
    }

    public int lFlag(String s) {
	// Sets currentStr.
	Integer i = intArg(s);
	if (i == null) {
	    return 0;
	}
	if (product.hasStr(i)) {
	    currentStr = product.getStr(i);
	}
	return 1;
    }

    public int pFlag() {
	// Plain part of last/current str.
	if (currentStr != null) {
	    product.addPart(builder.MakePlainPart(currentStr));
	}
	return 0;
    }

    public int pFlag(String s) {
	// Plain part of nth string.
	Integer i = intArg(s);
	if (i == null) {
	    pFlag();
	    return 0;
	} else if (!product.hasStr(i)) {
	    pFlag();
	} else {
	    product.addPart(builder.MakePlainPart(product.getStr(i)));
	}
	return 1;
    }

    public void pFlag(Argumenter<String> arg){
	// Plain part of nth string.
	Integer i = intArg(arg.get(0));
	if (i == null) {
	    pFlag();
	} else if (!product.hasStr(i)) {
	    pFlag();
	    arg.next(1);
	} else {
	    product.addPart(builder.MakePlainPart(product.getStr(i)));
	    arg.next(1);
	}
    }
    
    public int PFlag(String s) {
	// Creates a literal and plain part.
	// Equal to '-s ABC -p'
	sFlag(s);
	pFlag();
	return 1;
    }

    public int rFlag(String s) {
	// Random part of last/current str with length n
	if (currentStr == null) {
	    return 0;
	}
	Integer i = intArg(s);
	if (i == null) {
	    return 0;
	}
	if (i > 0) {
	    product.addPart(builder.MakeRandomPartWithSymbols(currentStr, i));
	}
	return 1;
    }

    public int RFlag(String s) {
	// Random part of last/current str with length upto n
	if (currentStr == null) {
	    return 0;
	}
	Integer i = intArg(s);
	if (i == null) {
	    return 0;
	}
	if (i > 0) {
	    product.addPart(builder.MakeRandomLengthPart(currentStr, i));
	}
	return 1;
    }

    public int cFlag(String s) {
	// Color part.
	List<Integer> ints = intArgs(s, 10); // can be more but 10 ATM.
	List<String> colors = new ArrayList<>();
	for (int i : ints) {
	    if (product.hasStr(i)) {
		colors.add(product.getStr(i));
	    }
	}
	if (colors.size() > 1) {
	    product.addPart(builder.MakeColorPart(colors));
	}
	return 1;
    }

    public int CFlag(String del, String list) {
	// Delimited Color part.
	// Equal to -S / A/B/C -c 1,2,3
	int i = product.literalsCount();
	if (SFlag(del, list) == 2) {
	    List<String> colors = new ArrayList<>();
	    while (i < product.literalsCount()) {
		colors.add(product.getStr(i));
		++i;
	    }
	    product.addPart(builder.MakeColorPart(colors));
	    return 2;
	} else {
	    return 0;
	}
    }

    public int xxCFlag(String s) {
	// Creates copy of nth literal.
	Integer i = intArg(s);
	if (product.hasStr(i)) {
	    currentStr = product.getStr(i);
	    product.addStr(currentStr);
	    return 1;
	}
	return 0;
    }

    public int sFlag(String s) {
	// Adds new literal.
	currentStr = s;
	product.addStr(currentStr);
	return 1;
    }

    public int SFlag(String del, String list) {
	// Adds new delimited literals.
	if (del.length() < 1) {
	    return 0;
	}
	String[] strs = list.split(del);
	for (String str : strs) {
	    product.addStr(str);
	}
	if (strs.length > 0) {
	    currentStr = strs[strs.length - 1];
	}
	return 2;
    }

    public int gFlag(String s) {
	// Generates a literal based on 4 len bool
	// Extra chars are appended.
	if (s.length() < 4) {
	    return 0;
	}
	if (s.charAt(0) == '1') {
	    si.add(CharSet.LOWER);
	}
	if (s.charAt(1) == '1') {
	    si.add(CharSet.UPPER);
	}
	if (s.charAt(2) == '1') {
	    si.add(CharSet.DIGITS);
	}
	if (s.charAt(3) == '1') {
	    si.add(CharSet.SYMBOLS);
	}
	if (s.length() > 4) {
	    si.addChars(s.substring(4));
	}
	if (!si.isEmpty()) {
	    currentStr = si.toString();
	    product.addStr(currentStr);
	    si.clear();
	}
	return 1;
    }

    public CompositePartString product;
    PartBuilder builder;
    String currentStr;
    StringIncludes si;
    Matcher numberMatcher;
}
