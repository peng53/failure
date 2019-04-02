/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package argumenter;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 *
 * @author lm
 */
public class Argumenter <T> {
    public Argumenter(List<T> args, int startIndex){
	this.args = args;
	this.index = startIndex;
    }
    public Argumenter(List<T> args){
	this(args,0);
    }
    public T get(int offset){
	int i = index + offset;
	if (i>0 | i < args.size()){
	    return args.get(i);
	} else {
	    return null;
	}
    }
    public void next(int offset){
	index += offset;
    }
    public boolean atEnd(){
	return index>=args.size();
    }
    
    final List<T> args;
    int index;
    
    public static void main(String[] args){
	List<String> listArgs = new ArrayList<String>(args.length);
	for (String s : args){
	    listArgs.add(s);
	}
	listArgs.add("hello");
	listArgs.add("world");
	Argumenter argu = new Argumenter<String>(listArgs);
	while (!argu.atEnd()){
	    System.out.println(argu.get(0));
	    argu.next(1);
	}
    }
}
