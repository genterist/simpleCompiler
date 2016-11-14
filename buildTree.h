/*
 ============================================================================
 Name        : buildTree.h
 Author      : NGUYEN, TAM N
 Created on  : 09SEP16
 Version     : 1
 Copyright   : (CC)
 Summary	 : data type declarations and all (i/o) methods required
 	 	 	 	 to successfully build and display a Binary Search Tree
 ============================================================================
 */

#ifndef BUILDTREE_H_
#define BUILDTREE_H_

//declare data type for a specific node in a BSTree
typedef struct BSTNode* Treeptr;
struct BSTNode{
	int data;
	int count;
	Treeptr left;
	Treeptr right;
	Treeptr parent;
};
//--

Treeptr buildTree(Treeptr ,int , Treeptr );

#endif /* BUILDTREE_H_ */
