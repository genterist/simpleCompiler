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



Treeptr insertNewNode(char data[25], char value [25], Treeptr parent_node){
	Treeptr newNode = (Treeptr) malloc(sizeof(struct BSTNode));
	strcpy(newNode->data,data);
	strcpy(newNode->value,value);
	if (parent_node==NULL) newNode->count=1;
	else newNode->count = parent_node->count + 1;
	newNode->left=newNode->right=NULL;
	if (parent_node == NULL) newNode->parent = NULL;
	else newNode->parent = parent_node;
	if (parent_node==NULL) newNode->scope = 0;
	else if (strstr(newNode->value,"<block>")!=NULL) newNode->scope = parent_node->scope + 1;
	else newNode->scope = parent_node->scope;
	return newNode;
}

Treeptr buildTree(char data[25], char value[25], Treeptr parent_node){
	Treeptr tempNode;
	if(parent_node==NULL){               //if the current spot root
		theTree=insertNewNode(data, value, theTree);
		tempNode = theTree;
	} else {
	    if (parent_node->left == NULL){
	        parent_node->left = insertNewNode (data, value, parent_node );
	        tempNode = parent_node->left;
	    } else if (parent_node->right == NULL){
	        parent_node->right = insertNewNode (data, value, parent_node );
	        tempNode = parent_node->right;
	    } else tempNode =  parent_node;
	}
	return tempNode;
}
