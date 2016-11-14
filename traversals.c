/*
 ============================================================================
 Name        : traversals.c
 Author      : NGUYEN, TAM N
 Created on  : 09SEP16
 Version     : 1
 Copyright   : (CC)
 Summary	 : data type declarations and all (i/o) methods required
 	 	 	 	 to successfully build and display a Binary Search Tree
 Functions	 :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buildTree.h"
#include "traversals.h"

//save results to file
int saveLog (const char *filename, char *message) {
	FILE *f_ptr;
	f_ptr=fopen( filename,"a");

	fprintf(f_ptr, "%s",message);

	return fclose(f_ptr);
}

void TravInOrder (Treeptr node, int level, const char *filename){
	int theLevel = level;
	int temp = 0;
	char message[1000];
	char buffer[1000];
	if (node != NULL){
		TravInOrder(node->left, theLevel+1,filename);
		if (filename == NULL) {
			for (temp = 0; temp<=theLevel; temp++) printf("  ");
			printf("%d:%d \n", node->data, node->count);
		} else
		{
			for (temp = 0; temp<=theLevel; temp++) strcat(message,"  ");
			sprintf(buffer, "%d:%d \n", node->data, node->count);
			strcat (message, buffer);
			saveLog (filename, message);
			strncpy(buffer, "", sizeof(buffer));
			strncpy(message, "", sizeof(message));
		}
		TravInOrder(node->right, theLevel+1,filename);
	}
}

void TravPreOrder (Treeptr node, int level, const char *filename){
	int theLevel = level;
	int temp = 0;
	char message[1000];
	char buffer[1000];
	if (node != NULL){
		if (filename == NULL) {
			for (temp = 0; temp<=theLevel; temp++) printf("  ");
			printf("%d:%d \n", node->data, node->count);
		} else
		{
			for (temp = 0; temp<=theLevel; temp++) strcat(message,"  ");
			sprintf(buffer, "%d:%d \n", node->data, node->count);
			strcat (message, buffer);
			saveLog (filename, message);
			strncpy(buffer, "", sizeof(buffer));
			strncpy(message, "", sizeof(message));
		}
		TravPreOrder(node->left, theLevel+1,filename);
		TravPreOrder(node->right, theLevel+1,filename);
	}
}

void TravPostOrder (Treeptr node, int level, const char *filename){
	int theLevel = level;
	int temp = 0;
	char message[1000];
	char buffer[1000];

	if (node != NULL){
		TravPostOrder(node->left, theLevel+1,filename);
		TravPostOrder(node->right, theLevel+1,filename);
		if (filename == NULL) {
			for (temp = 0; temp<=theLevel; temp++) printf("  ");
			printf("%d:%d \n", node->data, node->count);
		} else
		{
			for (temp = 0; temp<=theLevel; temp++) strcat(message,"  ");
			sprintf(buffer, "%d:%d \n", node->data, node->count);
			strcat (message, buffer);
			saveLog (filename, message);
			strncpy(buffer, "", sizeof(buffer));
			strncpy(message, "", sizeof(message));
		}
	}
}

