#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char** read(char* file) {
    FILE *fp = fopen(file, "r");
    if(fp == NULL) {
        perror("Unable to open file!");
        exit(1);
	}
// Read lines using POSIX function getline
// This code won't work on Windows
	char *line = NULL;
	size_t len = 0;
	char** lines=malloc(sizeof(char*)*100);
	int i =0;
 	
	while(getline(&line, &len, fp) != -1) 
	{
		//printf("%s", line);
		lines[i]=malloc(sizeof(char)*len);
		strcpy(lines[i], line);
		i+=1;
		
	}
    //printf("\n\nMax line size: %zd\n", len);
	fclose(fp);
    free(line);
   	return lines;
}


int
main(int argc, char * const argv[])
{
	
	char** lines = read(argv[1]);
	int i = sizeof(lines)-1;
	for(int x=0;x<i;x++){
    printf("%s", lines[x]);
    }
	return 0;
}



