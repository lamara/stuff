package com.example.calculator;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


//provides for the ability to parse through strings that contain
//only numeric characters as well as +, -, /, x, and . (ex. "323x5+6/2")
//calculating the given string and returning a new string giving the result.
//strings must begin and end with numbers, i.e. "+432/15" is illegal.
public class Calculator {
	
	public static String calculate(String expression) {
		
		//checks case where there are no operators
		if (!(expression.contains("+") || expression.contains("-") ||
			  expression.contains("/") || expression.contains("*"))) {
			return expression;
		}
		
		String[] terms = getTerms(expression);
		char[] operators = getOperators(expression);
		
		
		//checks whether there are multiplication or division
		//operators, and if so completes those operations first
		if (expression.contains("/") || expression.contains("*")) {
			terms = calculateMultiDiv(terms, operators);
		}
		
		float term;
		float result = Float.parseFloat(terms[0]);
		for (int i = 1, j = 0; i < terms.length; i++, j++) {
			//checks if the current operator has already been
			//taken care of by calculateMultiDiv()
			if (operators[j] == ',') {
				i--;
				continue;
			}
			
			term = Float.parseFloat(terms[i]);
			//parses through the operators
			switch(operators[j]) {
				case '+': result += term; break;
				case '-': result -= term; break;
				case '/': result /= term; break;
				case 'x': result *= term; break;
			}
		}
		
		return Float.toString(result);
	}
	
	
	//Iterates over the multiplication or division operators completes the necessary
	//operations on terms. Returns an updated terms list with the new calculated values,
	//and also replaces multiplication and division operators in the operators list
	//with a ',' character as they are used, denoting a null operation.
	private static String[] calculateMultiDiv(String[] terms, char[] operators) {
		List<Integer> operatorIndexes = new ArrayList<Integer>();
		List<String>  calculatedTerms = new ArrayList<String>(Arrays.asList(terms));
		float term;
		float secondTerm;
		
		int index = 0;
		for (char c : operators) {
			if (c == 'x' || c == '/') {
				operatorIndexes.add(index);
			}
			index++;
		}
		

		//keeps the value of the previous index stored to use
		//in comparisons between iterations
		int previousOpIndex = operatorIndexes.get(0);
		//initializes term to the first value we need to compute
		term = Float.parseFloat(terms[previousOpIndex]);
		int cnt = 0;
		for (int opIndex : operatorIndexes) {
			//checks that the two operators are not 
			//in succession, if they are then the previous
			//iteration's result will be used in computation
			if (opIndex - previousOpIndex != 1) {
				//assigns the first term used in computation
				term = Float.parseFloat(terms[opIndex]);
			}
			secondTerm = Float.parseFloat(terms[opIndex + 1]);
			
			switch(operators[opIndex]) {
				case '/': term /= secondTerm; break;
				case 'x': term *= secondTerm; break;
			}
			//using (opIndex - cnt) because we are removing elements
			//from the arraylist, shortening its range of indexes
			calculatedTerms.set(opIndex - cnt, Float.toString(term));
			calculatedTerms.remove(opIndex - cnt + 1);
			operators[opIndex] = ',';
			previousOpIndex = opIndex;
			cnt++;
		}
		
		
		return calculatedTerms.toArray(new String[calculatedTerms.size()]);
	}
	
	
	private static String[] getTerms(String string) {
		return string.split("[+\\-/x]");
	}
	
	private static char[] getOperators(String string) {
		String resultingOperators = string.replaceAll("[0-9\\.]", "");
		return resultingOperators.toCharArray();
	}
}
