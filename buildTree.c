/*
 ============================================================================
 Name        : buildTree.c
 Author      : NGUYEN, TAM N
 Created on  : 09SEP16
 Version     : 1
 Copyright   : (CC)
 Summary	 : all functions related to a successfully built BSTree
 Functions	 :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include "buildTree.h"



Treeptr insertNewNode(int data, Treeptr parent_node){
	Treeptr newNode = (Treeptr) malloc(sizeof(struct BSTNode));
	newNode->data=data;
	newNode->count=1;
	newNode->left=newNode->right=NULL;
	newNode->parent = parent_node;
	return newNode;
}

Treeptr buildTree(Treeptr root,int data, Treeptr parent_node){
	if(root==NULL){
		root=insertNewNode(data, parent_node);
	} else if(data<root->data){
		root->left=buildTree(root->left,data, root);
	} else if (data>root->data) {
		root->right=buildTree(root->right,data, root);
	} else {
		root->count +=1;
	}
	return root;
}

