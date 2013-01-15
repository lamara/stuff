package com.example.calculator.Tree;

public abstract class Node {
	protected Node leftChild;
	protected Node rightChild;
	
	//can pass in null to signify that there is no right or
	//no left child
	public Node(Node leftChild, Node rightChild) {
		this.rightChild = rightChild;
		this.leftChild = leftChild;
	}
	
	public abstract float evaluate();
	
	public Node getLeftChild() {return leftChild;}
	public Node getRightChild() {return rightChild;}
	
	public void setLeftChild(Node leftChild) {this.leftChild = leftChild;}
	public void setRightChild(Node rightChild) {this.rightChild = rightChild;}
	
	public boolean hasLeftChild() {
		if (leftChild != null) {
			return true;
		}
		else {return false;}
	}
	public boolean hasRightChild() {
		if (rightChild != null) {
			return true;
		}
		else {return false;}
	}
}
