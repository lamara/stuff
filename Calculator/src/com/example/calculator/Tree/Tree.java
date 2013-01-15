package com.example.calculator.Tree;

public class Tree {
	private Node rootNode;
	
	public Tree(Node rootNode) {
		this.rootNode = rootNode;
	}
	
	//true inserts left child from root, false inserts right child
	//(due to the logic behind the PostfixCalculator class, we only need to be able
	//to attach children to the very first level (the root node))
	public void insert(boolean left, Node node) {
		if (left) {
			rootNode.setLeftChild(node);
		}
		else {
			rootNode.setRightChild(node);
		}
	}
	
	public Node getRootNode() {return rootNode;};
	
	//takes advantage of polymorphism to to evaluate the rootnode
	//and every value underneath the rootnode giving us a correct
	//result as long as the tree is properly ordered
	public float calculate() {
		return rootNode.evaluate();
	}
	
}
