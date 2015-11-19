package codegen.prof.flow;

public class Range {

	private int min; // inclusive
	private int max; // inclusive
	
	
	public Range(){
		
	}
	
	
	public Range(int min, int max){
		this.min = min;
		this.max = max;
	}
	
	public Range(int value) {
		min = value;
		max = value;
	}


	public void setMin(int min){
		this.min = min;
	}
	
	public void setMax(int max){
		this.max = max;
	}
	
	public int getMin(){
		return min;
	}
	
	public int getMax(){
		return max;
	}


	public void update(int value) {
		if(value < min){
			min = value;
		} else if (value > max){
			max = value;
		}
		
	}
	
	public String toString(){
		return "[" + min + "," + max + "]";
	}


	public boolean isNegative() {
		return max < 0;
	}
	
	public boolean isPositive() {
		return min > 0;
	}
}
