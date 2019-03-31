package specstrgen;

import java.util.List;
import java.util.ArrayList;

public class CompositePartString {
	public CompositePartString(){
		literals = new ArrayList<>();
		parts = new ArrayList<>();
	}
	public void addPart(Part part){
		parts.add(part);
	}
	public Part getPart(int n){
		if (n<0 || parts.size()<n){
			return null;
		}
		return parts.get(n);
	}
	
	public void addStr(String str){
		literals.add(new StringBuilder(str));
	}
	public boolean hasStr(int n){
		return (n<literals.size() && n>=0);

	}
	public String getStr(int n){
		if (hasStr(n)){
			return literals.get(n).toString();
		}
		return null;
	}
	public int literalsCount(){
		return literals.size();
	}
	
	public String toString(){
		StringBuilder sb = new StringBuilder();
        for (Part p : parts){
            p.out(sb);
        }
        return sb.toString();
	}
	
	List<StringBuilder> literals;
	List<Part> parts;
}
