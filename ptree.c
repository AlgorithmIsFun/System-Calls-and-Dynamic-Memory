#include <stdio.h>
// Add your system includes here.
#include <stdlib.h>
#include "ptree.h"
#include <string.h>
#include <sys/stat.h>
// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests 
// directory for PIDs, instead of /proc.
#ifdef TEST
    const char *PROC_ROOT = "tests";
#else
    const char *PROC_ROOT = "/proc";
#endif

/*
 * Creates a PTree rooted at the process pid. The root of the tree is
 * placed in root. The function returns 0 if the tree was created 
 * successfully and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an 
 * executing process.
 */


int lstat(const char *restrict path, struct stat *restrict buf);
int generate_ptree(struct TreeNode **root, pid_t pid) {
    	// Here's a trick to generate the name of a file to open. Note 
    	// that it uses the PROC_ROOT variable
    	char procfile[MAX_PATH_LENGTH+1];
    	struct stat buffer, buffer2;
	if (snprintf(procfile, MAX_PATH_LENGTH + 1, "%s/%d", PROC_ROOT, pid) < 0) {
		//fprintf(stderr, "snprintf failed to produce a filename\n");
		return 1;
	}
	if (lstat(procfile, &buffer) == -1) {
		//fprintf(stderr, "file does not exist\n");
		return 1;
	}
	if (snprintf(procfile, MAX_PATH_LENGTH + 1, "%s/%d/exe", PROC_ROOT, pid) < 0) {
		//fprintf(stderr, "snprintf failed to produce a filename\n");
        	return 1;
    	}	

    		   
	if (lstat(procfile, &buffer) == -1) {
        	//fprintf(stderr, "file does not exist\n");
                return 1;

    	}
	char cmdline[MAX_PATH_LENGTH+5];
    	if (snprintf(cmdline, MAX_PATH_LENGTH + 5, "%s/%d/cmdline", PROC_ROOT, pid) < 0) {
        	//fprintf(stderr, "snprintf failed to produce a filename\n");
        	return 0;
    	}
    	
    	FILE *file;
    	file = fopen(cmdline, "r");
    	(*root) = malloc(sizeof(struct TreeNode));
	//(*root)->name = malloc(sizeof(char)*300);
	if (file == NULL){
		//fprintf(stderr, "Error opening file\n");
     		//strcpy((*root)->name, "");
		//(*root)->name = "";
	}
	lstat(cmdline, &buffer2);
	if (buffer2.st_size == 0){
		(*root)->name = NULL;
	}
	else{	
		(*root)->name = malloc(sizeof(char)*350);
		fscanf(file, "%s", (*root)->name);
		//printf("%s %ld\n", (*root)->name, buffer2.st_size);
	}
	if (fclose(file) != 0){
		//fprintf(stderr, "fclose failed\n");
		
		return 0;
    	 }
	(*root)->pid = pid;
    	if (snprintf(cmdline, MAX_PATH_LENGTH + 4, "%s/%d/task", PROC_ROOT, pid) < 0){
		return 0;
	}
	if (lstat(cmdline, &buffer) == 0 && S_ISDIR(buffer.st_mode)){
		
	}
	else{
		return 0;
	}
	if(snprintf(cmdline, MAX_PATH_LENGTH + 30, "%s/%d/task/%d/children", PROC_ROOT, pid, pid) < 0) {
        	//fprintf(stderr, "snprintf failed to produce a filename\n");
        	return 0;
    	}
    	file = fopen(cmdline, "r");
    	if (file == NULL){
        	//fprintf(stderr, "Error opening file\n");
        	return 0;
    	 }
     
     	char child_pid[MAX_PATH_LENGTH];
     
	int counter = 0;

        int test = 0;	
     	struct TreeNode *child = NULL, *temp = NULL;
 	while(fscanf(file, "%s", child_pid) != EOF){
        	test = generate_ptree(&child, strtol(child_pid, NULL, 10));
		if (counter == 0){
			(*root)->child_procs = child;
			counter = 1;
		}else{
			if (test == 0){
			temp = (*root)->child_procs;
			while (temp->next_sibling != NULL){
				temp = temp->next_sibling;
			}
			temp->next_sibling = child;
		} }
	}
     	if (fclose(file) != 0){
        	//fprintf(stderr, "fclose failed\n");
        	return 0;
     	}
    // Your implementation goes here.
   
    
    	return 0;
}

void free_tree(struct TreeNode *root){
    if (root != NULL) {
        free_tree(root->next_sibling);
        free_tree(root->child_procs);
        free(root);
     }
}

/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the 
 * entire tree is printed.
 */

void print_ptree(struct TreeNode *root, int max_depth) {
    	// Here's a trick for remembering what depth (in the tree) you're at
    	// and printing 2 * that many spaces at the beginning of the line.
    	static int depth = 0;
    	if (root == NULL){return;}
	printf("%*s", depth*2, "");
	if (root->name == NULL){
		printf("%d\n", root->pid);
	}else{
    	printf("%ld: %s\n", (long) root->pid, root->name);}
    	if (root->child_procs != NULL && max_depth != 1){ 
		depth += 1;
		print_ptree(root->child_procs, max_depth-1);
		depth -= 1;}
    	if (root->next_sibling != NULL){
        	print_ptree(root->next_sibling, max_depth);}
}
