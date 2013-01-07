import java.math.BigInteger;
import java.util.HashMap;
import java.util.Map;

//Uses Euler's formula for generating partition numbers
//link to project euler 78:  http://projecteuler.net/problem=78
//partition numbers:		 http://mathworld.wolfram.com/PartitionFunctionP.html
public class Euler78 {
	
	static Map<Integer, BigInteger> values = new HashMap();
	
	static void loadValues()
	{
		//p(0) is defined as equaling 1
		BigInteger i = BigInteger.valueOf(1);
		values.put(0, i);
	}
	
	static BigInteger partition(int n)
	{
		//retrieves values that we have already computed
		if (values.containsKey(n)) {
			return values.get(n);
		}
		//p(n) where n < 0 is defined as equaling 0
		if (n < 0) {
			return BigInteger.valueOf(0);
		}
		BigInteger sum;
		sum = BigInteger.valueOf(0);
		int k, m, p;
		k = 1;
		//generates the first two generalized pentagonal numbers
		m = (int)((1.0/2)*k*(3.0*k - 1));
		p = (int)((1.0/2)*k*(3.0*k + 1));
		//keep generating the generalized pentagonal numbers
		//until it becomes redundant (where we have p(n) with n < 0)
		while (n - m >= 0) {
			//general equation is p(n) = -1^(k+1)[P(n-m) + P(n-p)]
			//where k is increased infinitely
			BigInteger power = BigInteger.valueOf((long) Math.pow(-1, k + 1));
			//distributes -1^(k + 1) to the terms in brackets
			sum = sum.add(power.multiply(partition(n - m)));
			sum = sum.add(power.multiply(partition(n - p)));
			
			k++;
			m = (int)((1.0/2)*k*(3.0*k - 1));
			p = (int)((1.0/2)*k*(3.0*k + 1));
		}
		values.put(n, sum);
		return sum;
	}


	public static void main(String [] args)
	{
		loadValues();
		boolean condition = false;
		int n = 1;
		BigInteger zero = BigInteger.valueOf(0);
		BigInteger partitionValue;
		BigInteger compareValue;
		while (!condition) {
			partitionValue = partition(n);
			compareValue = partitionValue.mod(BigInteger.valueOf(1000000));
			if (compareValue.equals(zero)) {
				condition = true;
				break;
			}
			n++;
		}
		
		System.out.println(n);
	}
}
