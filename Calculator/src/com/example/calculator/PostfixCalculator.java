package com.example.calculator;

import java.util.Stack;
import java.util.EmptyStackException;

import com.example.calculator.Tree.*;

public class PostfixCalculator {
	
	public static void main(String args[]) {
		System.out.println(calculate("3+4+5"));
	}
	
	//takes an infix expression (i.e. "3+4*5) and outputs
	//a numerical result (as a string!) following order of operations
	public static String calculate(String infixExpression) {
		try {
		String postfixExpression = translateToPostfix(infixExpression);
		Tree postfixTree = parseExpression(postfixExpression);
		String result = Float.toString(postfixTree.calculate());
		return result;
		}
		//the two exceptions below will catch badly formatted input
		//strings (i.e. "432+" or "32*43..2"
		catch (NumberFormatException e) {
			return "Error";
		}
		catch (EmptyStackException e) {
			return "Error";
		}
	}
	
	
	//parses the postfix expression into a tree that contains the
	//expression's operators and operands in the correct postfix order
	private static Tree parseExpression(String expression) {
		Stack<Tree> expressionStack = new Stack<Tree>();
		
		String[] terms = expression.split("\\s");
		
		for (String term : terms) {
			//process: if not an operator (+,-,*,/), creates a tree consisting solely
			//of the term's value (it must be a numeric value) and pushes it onto the stack
			if (!term.matches("[+\\-*/]")) {
				float value = Float.parseFloat(term);
				Node valueNode = new ValueNode(null, null, value);
				Tree tree = new Tree(valueNode);
				expressionStack.push(tree);
			}
			//process: if an operator, pops the first two trees on the stack 
			//and then creates a new tree with the operator as the
			//root and the two trees as its two children, and then pushes it back
			//onto the stack
			else {
				char operator = term.charAt(0);
				Node operatorNode = new OperatorNode(null, null, operator);
				Tree tree = new Tree(operatorNode);
				
				Tree rightTree = expressionStack.pop();
				Tree leftTree = expressionStack.pop();
				Node rightNode = rightTree.getRootNode();
				Node leftNode = leftTree.getRootNode();
				
				tree.getRootNode().setRightChild(rightNode);
				tree.getRootNode().setLeftChild(leftNode);
				
				expressionStack.push(tree);
			}
		}
		//the only item that remains in the stack after the above steps
		//is the completed operation tree, which we return
		return expressionStack.pop();
	}
	
	//takes a string of a standard infix expression (i.e. "4+3*2")
	//and parses it into a postfix expression.
	private static String translateToPostfix(String infix) {
		Stack<Character> operatorStack = new Stack<Character>();
		String postfix = "";
		String[] terms = infix.split("[+\\-*/]");
		char[] operators = infix.replaceAll("[0-9.]", "").toCharArray();
		int operatorIndex = 0;
		
		//the algorithm for translating infix to postfix can get complicated, following
		//the steps on this link can help with comprehending it:
		//http://scriptasylum.com/tutorials/infix_postfix/algorithms/infix-postfix/index.htm
		for (String term : terms) {
			//adds the numeric term being parsed to the postfix
			postfix += (term + " ");
			//if we reach this condition, then we are at the end
			//of the infix expression
			if (operatorIndex >= operators.length) {break;}
			//if the stack is empty then we just add the current operator
			//being parsed to it
			if (operatorStack.empty()) {
				operatorStack.push(operators[operatorIndex]);
				operatorIndex++;
			}
			//if the stack is not empty then things get a lot more complicated,
			//if you are having trouble following then read the link posted above
			else {
				while (!operatorStack.empty()) {
					char prevOperator = operatorStack.pop();
					char currentOperator = operators[operatorIndex];
					//tests whether the current operator that we are parsing
					//is higher on the order of operations list than the proceeding
					//operator, if so then we add the previous operator back onto
					//the loop and then break it
					if ((currentOperator == '*' || currentOperator == '/') &&
						(prevOperator == '+' || prevOperator == '-')) {
						operatorStack.push(prevOperator);
						operatorIndex++;
						break;
					}
					//otherwise, the current operator hold less or equal precedence,
					//so we write it to postfix string and continue the loop
					else {
						postfix += (prevOperator + " ");
						operatorIndex++;
					}
				}
				//at the end of the loop we have to add the last operator
				//we parsed (currentOperator from above) to the stack
				operatorStack.push(operators[operatorIndex - 1]);
			}
		}
		//reached end of infix expression, all that we have left is to pop
		//the remaining operators from the stack onto the postfix expression
		while (!operatorStack.empty()) {
			char operator = operatorStack.pop();
			postfix += (operator + " ");
		}
		return postfix;
	}
	
}






