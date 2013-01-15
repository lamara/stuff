package com.example.calculator.Tree;

public class OperatorNode extends Node {

	private char operator;
	
	public OperatorNode(Node leftChild, Node rightChild, char operator) {
		super(leftChild, rightChild);
		this.operator = operator;
	}

	@Override
	public float evaluate() {
		switch(operator)
		{
			case '+': 
				return leftChild.evaluate() + rightChild.evaluate();
			case '-':
				return leftChild.evaluate() - rightChild.evaluate();
			case '*':
				return leftChild.evaluate() * rightChild.evaluate();
			case '/': 
				return leftChild.evaluate() / rightChild.evaluate();
		}
		return 0;
	}

}
