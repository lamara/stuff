package com.example.calculator.Tree;

public class ValueNode extends Node {
	
	private float value;

	public ValueNode(Node leftChild, Node rightChild, float value) {
		super(leftChild, rightChild);
		this.value = value;
	}

	@Override
	public float evaluate() {
		return value;
	}

}
