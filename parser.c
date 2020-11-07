#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "hash.h"

char** read_lines(char* file, int* cant_lineas) {
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

/*
int32_t get_hash_more(string_hash *str_hash, char * linea, size_t length, size_t stride){
	size_t delta;
	size_t rem = length;

	string_hash_init(str_hash);

	while (rem > 0) {
		if (rem >= stride)
			delta = stride;
		else
			delta = rem;
	
		string_hash_more(str_hash, linea, delta);
		rem -= delta;
		linea += delta;
	}
	string_hash_done(str_hash);

	return string_hash_value(str_hash);
}

int get_hash(char* linea, int cant_lineas){
	size_t length = strlen(linea);
	string_hash str_hash;
	int32_t valor_hash;

	if (length > 1) {
		valor_hash = get_hash_more(&str_hash, linea, length, length);

		for (size_t i = length; i >= 1; i--) {
			int32_t h = get_hash_more(&str_hash, linea, length, i);
			assert(valor_hash == h);
		}
	}

	return valor_hash;
}
*/

int32_t get_hash_(string_hash *sh, char *msg, size_t len, size_t stride)
{
	char *ptr = msg;
	size_t delta;
	size_t rem;

	string_hash_init(sh);
	for (rem = len; rem > 0; ) {
		if (rem >= stride)
			delta = stride;
		else
			delta = rem;
	
		string_hash_more(sh, ptr, delta);
		rem -= delta;
		ptr += delta;
	}
	string_hash_done(sh);

	return string_hash_value(sh);
}

int get_hash(char *msg)
{
	size_t len = strlen(msg);
	size_t stride;
	string_hash sh;
	int32_t h0;
	int32_t h;

	if (len > 1) {
		h0 = get_hash_(&sh, msg, len, len);

		for (stride = len; stride >= 1; stride--) {
			h = get_hash_(&sh, msg, len, stride);
			assert(h0 == h);
		}
	}

	return h0;
}

void hash_input(char* input_file, char ***lines, int* cant_lineas){
	int* hashes = malloc(sizeof(int*)*(size_t)(*cant_lineas));
	(*lines) = read_lines(input_file, cant_lineas);
	for (int i=0; i<(*cant_lineas);i++){
		hashes[i]=get_hash((*lines)[i]);
		printf("%p\n", hashes[i]);
	}
}

void output_hash(const char* output_file, char **lines, int cant_lineas){
	for(int i=0; i < cant_lineas; i++){
		printf("%s", lines[i]);
	}
	destruir_lineas(lines, cant_lineas);
}

void parseAnswer(int argc, const char** argv) {
	int option;
	int cIndex = 0;
	char** lines = NULL;
	int cant_lineas = 0;

	static struct option long_answer[] = {
		{"version", no_argument,       NULL, 'V'},
		{"help",    no_argument,       NULL, 'h'},
		{"input",   required_argument, NULL, 'i'},
		{"output",  required_argument, NULL, 'o'},
	};
	
	while ((option = getopt_long(argc, (char**)argv, "Vhi:o:",long_answer ,&cIndex)) != -1){

		if (option == -1)
			break;

		switch (option) {
			case 'V':
				printf("Version actual: 1.");
				exit(0);

			case 'h':
				printf("Usage: \ntp1 -h\ntp1 -v\ntp1 -i in_file -o out_file\nOptions:\n-V, --version Print version and quit.\n-h, --help Print this information and quit.\n");
				printf("-i, --input Specify input stream/file, '-' for stdin.\n-o, --output Specify output stream/file, '-' for stdout.");
				exit(0);

			case 'i':
				hash_input(optarg, &lines, &cant_lineas);
				break;

			case 'o':
				output_hash(optarg, lines, cant_lineas);
				break;

			default:
				printf("Comando invalido. Pruebe utilizando tp1 -h.");
				abort();
		}
	}
}



