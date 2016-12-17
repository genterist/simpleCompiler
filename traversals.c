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
	char temp[100];
	if (node != NULL){
		if (filename == NULL) {
			fprintf(stderr,"[ERROR] Name of ASM file out put not specified.\n");
		} else
		{
		   memset(temp,0,sizeof(temp));           //preventing mem issues
           
           //Generating body of ASM file
           if (strstr(node->data,"<INT>")!=NULL && node->asmFlag==0) {
               sprintf(temp, "LOAD %s\n", node->value);
               saveLog(filename, temp);
               node->asmFlag = 1;
           }

           
           if (strstr(node->data,"<Out>")!=NULL && node->asmFlag==0) {
               if (node->left != NULL) generateASM (node->left, filename);
               if (node->right != NULL) generateASM (node->right, filename);
               snprintf(temp, sizeof(temp), "STORE T%d\nWRITE T%d\n", node->scope, node->scope, node->value);
               saveLog(filename, temp);
               memset (temp,0,sizeof(temp));
               snprintf(temp, sizeof(temp), "T%d 0\n", node->scope);
               strcat(asmFooter,temp);
               node->asmFlag = 1;
           }

           
           if (strstr(node->data,"<ID>")!=NULL && node->asmFlag==0) {
               if (strstr(node->parent->data,"<vars>")!=NULL || strstr(node->parent->data,"<mvars>")!=NULL)
               {
                   snprintf(temp, sizeof(temp), "%s%d 0\n", node->value, node->scope);
                   strcat(asmFooter, temp);
                   node->asmFlag = 1;
               } 
               else if (strstr(node->parent->data,"<In>")!=NULL)
               {
                   memset (temp,0,sizeof(temp));
                   sprintf(temp, "READ %s%d\n", node->value, node->scope);
                   saveLog(filename, temp);
                   node->asmFlag = 1;
               }
               else
               {
                   snprintf(temp, sizeof(temp), "LOAD %s%d\n", node->value, node->scope);
                   saveLog(filename, temp);
                   node->asmFlag = 1;
               }
               
           }
           
           if (strstr(node->data,"<expr>")!=NULL && node->asmFlag==0) {
                    if (strstr(node->value,"+")!=NULL && node->asmFlag==0)
                    {
                        generateASM (node->right, filename);
                        snprintf(temp, sizeof(temp), "STORE T%d\n", node->count);
                        saveLog(filename, temp);
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "T%d 0\n", node->count);
                        strcat(asmFooter, temp);
                        //right hand side will be calculated and loaded into accumulator
                        generateASM (node->left, filename);
                        snprintf(temp, sizeof(temp), "ADD T%d\n", node->count);
                        saveLog(filename, temp);
    
                        node->asmFlag = 1;
                    }               
            }
            
            if (strstr(node->data,"<M>")!=NULL && node->asmFlag==0) {
                    if (strstr(node->value,"-")!=NULL && node->asmFlag==0)
                    {
                        generateASM (node->right, filename);
                        snprintf(temp, sizeof(temp), "STORE T%d\n", node->count);
                        saveLog(filename, temp);
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "T%d 0\n", node->count);
                        strcat(asmFooter, temp);
                        //right hand side will be calculated and loaded into accumulator
                        generateASM (node->left, filename);
                        snprintf(temp, sizeof(temp), "SUB T%d\n", node->count);
                        saveLog(filename, temp);
    
                        node->asmFlag = 1;
                    }               
            }
            
            if (strstr(node->data,"<T>")!=NULL && node->asmFlag==0) {
                    if (strstr(node->value,"*")!=NULL && node->asmFlag==0)
                    {
                        generateASM (node->right, filename);
                        snprintf(temp, sizeof(temp), "STORE T%d\n", node->count);
                        saveLog(filename, temp);
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "T%d 0\n", node->count);
                        strcat(asmFooter, temp);
                        //right hand side will be calculated and loaded into accumulator
                        generateASM (node->left, filename);
                        snprintf(temp, sizeof(temp), "MUL T%d\n", node->count);
                        saveLog(filename, temp);
    
                        node->asmFlag = 1;
                    }
                    
                    if (strstr(node->value,"/")!=NULL && node->asmFlag==0)
                    {
                        generateASM (node->right, filename);
                        snprintf(temp, sizeof(temp), "STORE T%d\n", node->count);
                        saveLog(filename, temp);
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "T%d 0\n", node->count);
                        strcat(asmFooter, temp);
                        //right hand side will be calculated and loaded into accumulator
                        generateASM (node->left, filename);
                        snprintf(temp, sizeof(temp), "DIV T%d\n", node->count);
                        saveLog(filename, temp);
    
                        node->asmFlag = 1;
                    } 
            }
            
            if (strstr(node->data,"<assign>")!=NULL && node->asmFlag==0) {
                    //load the right hand side here
                    generateASM (node->right, filename);
                    //right hand side will be calculated and loaded into accumulator
                    snprintf(temp, sizeof(temp), "STORE %s\n", node->left->value);
                    saveLog(filename, temp);
                    node->asmFlag = 1;
                
            }
            
            if (strstr(node->data,"<F>")!=NULL && node->asmFlag==0) {
                if (strstr(node->value,"-")!=NULL){
                    //load the right hand side here
                    generateASM (node->left, filename);
                    snprintf(temp, sizeof(temp), "MUL -1\n", node->count);
                    saveLog(filename, temp);
                    memset (temp,0,sizeof(temp));
                    node->asmFlag = 1; 
                }
            }
            
            if (strstr(node->data,"<If>")!=NULL && node->asmFlag==0) {
                    //load the condition
                    generateASM (node->left, filename);
                    //value of <RO> will be loaded to Accumulator
                    //left side minus right side
                    if (strcmp(node->left->value,"<")==0)
                    {
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "BRNEG BLOCK%d\nBR END%d\n", node->count, node->count);
                        saveLog(filename, temp);
                    }
                    
                    if (strcmp(node->left->value,"<=<")==0)
                    {
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "BRZNEG BLOCK%d\nBR END%d\n", node->count, node->count);
                        saveLog(filename, temp);
                    }
                    
                    if (strcmp(node->left->value,">")==0)
                    {
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "BRPOS BLOCK%d\nBR END%d\n", node->count, node->count);
                        saveLog(filename, temp);
                    }
                    
                    if (strcmp(node->left->value,">=>")==0)
                    {
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "BRZPOS BLOCK%d\nBR END%d\n", node->count, node->count);
                        saveLog(filename, temp);
                    }
                    
                    if (strcmp(node->left->value,"=!=")==0)
                    {
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "BRPOS BLOCK%d\nBRNEG BLOCK%d\nBR END\n", node->count, node->count);
                        saveLog(filename, temp);
                    }
                    
                    if (strcmp(node->left->value,"=")==0)
                    {
                        memset (temp,0,sizeof(temp));
                        snprintf(temp, sizeof(temp), "BRZERO BLOCK%d\nBR END\n", node->count);
                        saveLog(filename, temp);
                    }
                    
                    memset (temp,0,sizeof(temp));
                    snprintf(temp, sizeof(temp), "BLOCK%d: NOOP\n", node->count);
                    saveLog(filename, temp);
                    
                    
                    //load the right hand side
                    generateASM (node->right, filename);
                    //end of right hand side
                    
                    memset (temp,0,sizeof(temp));
                    snprintf(temp, sizeof(temp), "END%d: NOOP\n", node->count);
                    saveLog(filename, temp);
                    
                    memset (temp,0,sizeof(temp));
                    
                    node->asmFlag = 1;
                
            }
            
            if (strstr(node->data,"<RO>")!=NULL && node->asmFlag==0) {
                    generateASM (node->right, filename);
                    snprintf(temp, sizeof(temp), "STORE T%d\n", node->count);
                    saveLog(filename, temp);
                    memset (temp,0,sizeof(temp));
                    snprintf(temp, sizeof(temp), "T%d 0\n", node->count);
                    strcat(asmFooter, temp);
                    //right hand side will be calculated and loaded into accumulator
                    generateASM (node->left, filename);
                    snprintf(temp, sizeof(temp), "SUB T%d\n", node->count);
                    saveLog(filename, temp);

                    node->asmFlag = 1;              
            }

		}
		if (node->left != NULL && node->asmFlag==0) generateASM(node->left,filename);
		if (node->right != NULL&& node->asmFlag==0) generateASM(node->right,filename);
		
        //Generating footer of ASM file
		if (node->parent == NULL) {
		    
		    saveLog(filename, "STOP\n");
		    if (strcmp(asmFooter, "") != 0) 
		    {saveLog (filename, asmFooter);}
	     }

		//--end of ASM footer----
	}
	
}
