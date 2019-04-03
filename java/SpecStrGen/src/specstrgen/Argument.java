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

    public void lFlag(Argumenter<String> arg) {
	// Sets currentStr.
	Integer i = intArg(arg.get(0));
	if (i == null) {
	    return;
	}
	if (product.hasStr(i)) {
	    currentStr = product.getStr(i);
	}
	arg.next(1);
    }

    public void pFlag() {
	// Plain part of last/current str.
	if (currentStr != null) {
	    product.addPart(builder.MakePlainPart(currentStr));
	}
    }

    public void pFlag(Argumenter<String> arg){
	// Plain part of nth string.
	Integer i = intArg(arg.get(0));
	if (i == null) {
	    pFlag();
	} else {
	    if (!product.hasStr(i)) {
		pFlag();
	    } else {
		product.addPart(builder.MakePlainPart(product.getStr(i)));
	    }
	    arg.next(1);
	}
    }
    
    public void PFlag(Argumenter<String> arg) {
	// Creates a literal and plain part.
	// Equal to '-s ABC -p'
	sFlag(arg);
	pFlag();
    }

    public void rFlag(Argumenter<String> arg) {
	// Random part of last/current str with length n
	if (currentStr == null) {
	    return;
	}
	Integer i = intArg(arg.get(0));
	if (i == null) {
	    return;
	}
	if (i > 0) {
	    product.addPart(builder.MakeRandomPartWithSymbols(currentStr, i));
	}
	arg.next(1);
    }

    public void RFlag(Argumenter<String> arg) {
	// Random part of last/current str with length upto n
	if (currentStr == null) {
	    return;
	}
	Integer i = intArg(arg.get(0));
	if (i == null) {
	    return;
	}
	if (i > 0) {
	    product.addPart(builder.MakeRandomLengthPart(currentStr, i));
	}
	arg.next(1);
    }

    public void cFlag(Argumenter<String> arg) {
	// Color part.
	List<Integer> ints = intArgs(arg.get(0), 10); // can be more but 10 ATM.
	List<String> colors = new ArrayList<>();
	for (int i : ints) {
	    if (product.hasStr(i)) {
		colors.add(product.getStr(i));
	    }
	}
	if (colors.size() > 1) {
	    product.addPart(builder.MakeColorPart(colors));
	}
	arg.next(1);
    }

    public void CFlag(Argumenter<String> arg) {
	// Delimited Color part.
	// Equal to -S / A/B/C -c 1,2,3
	int i = product.literalsCount();
	if (SFlag(arg)) {
	    List<String> colors = new ArrayList<>();
	    while (i < product.literalsCount()) {
		colors.add(product.getStr(i));
		++i;
	    }
	    product.addPart(builder.MakeColorPart(colors));
	}
    }

    public void xxCFlag(Argumenter<String> arg) {
	// Creates copy of nth literal.
	Integer i = intArg(arg.get(0));
	if (product.hasStr(i)) {
	    currentStr = product.getStr(i);
	    product.addStr(currentStr);
	    arg.next(1);
	}
    }

    public void sFlag(Argumenter<String> arg) {
	// Adds new literal.
	currentStr = arg.get(0);
	product.addStr(currentStr);
	arg.next(1);
    }

    public boolean SFlag(Argumenter<String> arg) {
	// Adds new delimited literals.
	String del = arg.get(0);
	if (del.length() < 1) {
	    return false;
	}
	String list = arg.get(1);
	String[] strs = list.split(del);
	for (String str : strs) {
	    product.addStr(str);
	}
	if (strs.length > 0) {
	    currentStr = strs[strs.length - 1];
	}
	arg.next(2);
	return true;
    }

    public void gFlag(Argumenter<String> arg) {
	// Generates a literal based on 4 len bool
	// Extra chars are appended.
	String b = arg.get(0);
	if (b.length() < 4) {
	    return;
	}
	if (b.charAt(0) == '1') {
	    si.add(CharSet.LOWER);
	}
	if (b.charAt(1) == '1') {
	    si.add(CharSet.UPPER);
	}
	if (b.charAt(2) == '1') {
	    si.add(CharSet.DIGITS);
	}
	if (b.charAt(3) == '1') {
	    si.add(CharSet.SYMBOLS);
	}
	if (b.length() > 4) {
	    si.addChars(b.substring(4));
	}
	if (!si.isEmpty()) {
	    currentStr = si.toString();
	    product.addStr(currentStr);
	    si.clear();
	}
	arg.next(1);
    }

    public CompositePartString product;
    PartBuilder builder;
    String currentStr;
    StringIncludes si;
    Matcher numberMatcher;
}
