#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ptree.h"
#include <ctype.h>
int isNumber(char *s){
	for (int i = 0; i < strlen(s); i++){
		if (!isdigit(s[i])){
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
    // TODO: Update error checking and add support for the optional -d flag
    // printf("Usage:\n\tptree [-d N] PID\n");
	
    // NOTE: This only works if no -d option is provided and does not
    // error check the provided argument or generate_ptree. Fix this!
	int m;
	if (argc == 2 && isNumber(argv[1])== 0){
 		struct TreeNode *root = NULL;
    		m=generate_ptree(&root, strtol(argv[1], NULL, 10));
		print_ptree(root, 0);
	}
	else if (argc == 4 && strcmp(argv[1], "-d") == 0 && isNumber(argv[2]) == 0 && isNumber(argv[3]) == 0){
		struct TreeNode *root = NULL;
    		m=generate_ptree(&root, strtol(argv[3], NULL, 10));
    		print_ptree(root, strtol(argv[2], NULL, 10));
	}
        /*else if (argc == 4 && strcmp(argv[2], "-d") != 0){
                struct TreeNode *root = NULL;
                m=generate_ptree(&root, strtol(argv[1], NULL, 10));
                print_ptree(root, strtol(argv[3], NULL, 10));
        }*/

	else{    	
		fprintf(stderr, "Incorrect Command Line Arguments\n");
	return 1;	
    }
	if (m==1){return 2;}
    return 0;
}

