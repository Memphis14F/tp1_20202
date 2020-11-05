#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"


char** read(char* file, int* cant_lineas) {
    FILE *fp = fopen(file, "r");
    if(fp == NULL) {
        perror("Unable to open file!");
        exit(1);
	}
	char *line = NULL;
	size_t len = 0;
	char** lines = NULL;
	while(getline(&line, &len, fp) != -1) 
	{
		lines = realloc(lines, sizeof(char*)*((*cant_lineas)+1));
		lines[(*cant_lineas)]=malloc(sizeof(char)*len);
		strcpy(lines[(*cant_lineas)], line);
		(*cant_lineas)++;
		
	}
	fclose(fp);
    free(line);
   	return lines;
}

void destruir_lineas(char** lines, int cantidad_lineas){
	for (int i=0; i<cantidad_lineas; i++)
		free(lines[i]);
	free(lines);
}

int
main(int argc, char * const argv[])
{
	int cant_lineas = 0;
	char** lines = read("input.txt", &cant_lineas);
	//char** lines = read(argv[1]);
	for(int x=0; x < cant_lineas; x++){
    	printf("%s", lines[x]);
    }
	//string_hash_more();
	destruir_lineas(lines, cant_lineas);
	return 0;
}



