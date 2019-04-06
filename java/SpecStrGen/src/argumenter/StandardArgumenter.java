/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package argumenter;

import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.util.List;
import java.util.ArrayList;

/**
 *
 * @author lm
 */
public class StandardArgumenter {
    public StandardArgumenter(List<String> args, int startIndex){
	this.args = args;
	this.index = startIndex;
    }
    public StandardArgumenter(List<String> args){
	this(args,0);
    }
    private int indexFromOffset(int offset){
	return index + offset;
    }
    public String get(int offset){
	int i = indexFromOffset(offset);
	if (i>0 | i < args.size()){
	    return args.get(i);
	} else {
	    return null;
	}
    }
    public Integer getInt(int offset){
	String s = get(offset);
	if (s == null){
	    return null;
	}
	numberMatcher.reset(s);
	return (numberMatcher.find() ? Integer.parseInt(numberMatcher.group()) : null);
    }
  
    public List<Integer> intArgs(int offset) {
	String s = get(offset);
	if (s == null){
	    return null;
	}
	List<Integer> ints = new ArrayList<>();
	while (numberMatcher.find()) {
	    ints.add(Integer.parseInt(numberMatcher.group()));
	}
	return ints;
    }
    public void next(int offset){
	index += offset;
    }
    public boolean atEnd(){
	return index>=args.size();
    }
    public int remaining(){
	return args.size()-1-index;
    }
    
    Matcher numberMatcher = Pattern.compile("\\d+").matcher("");
    final List<String> args;
    int index;
}
