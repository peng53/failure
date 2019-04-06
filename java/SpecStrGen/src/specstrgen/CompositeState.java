/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package specstrgen;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author lm
 */
public class CompositeState {
    public void add(String s){
	if (s != null){
	    literals.add(s);
	    lastStr = literals.size()-1;
	}
    }
    public boolean hasStr(int i){
	return i>=0 && literals.size()>i;
    }
    public String getStr(int i){
	if (i<0){
	    i = lastStr;
	}
	return literals.get(i);
    }
    public String getLast(){
	return (lastStr==null ? null : literals.get(lastStr));
    }
    public int literalsCount(){
	return literals.size();
    }
    public void add(Part p){
	if (p != null){
	    parts.add(p);
	}
    }
    public Part getPart(int i){
	return parts.get(i);
    }
    public void chooseStr(int i){
	if (hasStr(i)){
	    lastStr = i;
	}
    }
    Integer lastStr;
    List<Part> parts = new ArrayList<>();
    List<String> literals = new ArrayList<>();
}
