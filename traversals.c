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

char asmFooter[100];

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
			printf("%s:%d \n", node->data, node->count);
		} else
		{
			for (temp = 0; temp<=theLevel; temp++) strcat(message,"  ");
			//sprintf(buffer, "%s(%d) %s \n", node->data,string2int(node->data), node->value);
			sprintf(buffer, "%s %s \n", node->data, node->value);
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

//generate ASM codes based on pre-order tree traversal
void generateASM (Treeptr node, const char *filename){
	char asmBody[100];
	if (node != NULL){
		if (filename == NULL) {
			fprintf(stderr,"[ERROR] Name of ASM file out put not specified.\n");
		} else
		{
		   memset(asmBody,0,sizeof(asmBody));           //preventing mem issues
           
           //Generating body of ASM file
           if (strstr(node->data,"<INT>")!=NULL) sprintf(asmBody, "%s\n", node->value);
           
           if (strstr(node->data,"<Out>")!=NULL) sprintf(asmBody, "WRITE ");
           
           if (strstr(node->data,"<In>")!=NULL) sprintf(asmBody, "READ ");
           
           if (strstr(node->data,"<ID>")!=NULL) {
               if (strstr(node->parent->data,"<vars>")!=NULL || strstr(node->parent->data,"<mvars>")!=NULL)
               {
                   char temp[25];
                   snprintf(temp, sizeof(temp), "%s 0\n", node->value);
                   strcat(asmFooter, temp);
               } 
               else if (strstr(node->parent->data,"<assign>")!=NULL) {}
               else
               {
                   char temp[25];
                   snprintf(temp, sizeof(temp), "%s\n", node->value);
                   sprintf(asmBody, temp);
               }
           }
           
           if (strstr(node->data,"<assign>")!=NULL) {
                    //load the right hand side here
                    generateASM (node->right, filename);
                    //right hand side will be calculated and loaded into accumulator
                    
                    char temp[25];
                    snprintf(temp, sizeof(temp), "STORE %s\n", node->left->value);
                
            }

            if (strstr(node->data,"<expr>")!=NULL) {                  
                    char temp[25];
                    snprintf(temp, sizeof(temp), "STORE %s\n", node->left->value);
                
            }

		   if (strcmp(asmBody, "") != 0) saveLog (filename, asmBody);
		   //--end of ASM body----
		   		   
		   memset(asmBody,0,sizeof(asmBody));           //preventing mem issues
		}
		if (node->left != NULL) generateASM(node->left,filename);
		if (node->right != NULL) generateASM(node->right,filename);
		
        //Generating footer of ASM file
		if (node->parent == NULL) {
		    if (strcmp(asmFooter, "") != 0) saveLog (filename, asmFooter);}
		//--end of ASM footer----
	}
	
}
