#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <stdint.h>
#include <stdbool.h>
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

void destruir_hashes(int* hashes){
	free(hashes);
}

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

void hash_input(char* input_file, char ***lines, int* cant_lineas, int32_t**hashes){
	(*lines) = read_lines(input_file, cant_lineas);
	(*hashes) = malloc(sizeof(int32_t*)*(size_t)(*cant_lineas));
	for (int i=0; i<(*cant_lineas);i++){
		(*hashes)[i]=get_hash((*lines)[i]);
	}
}


void std_input(char*** lines, int* cant_lineas, bool hay_std_output, char* path_file){
	char* buffer = NULL;
	size_t buffer_size = 0;
	int leido = getline(&buffer, &buffer_size, stdin);
	FILE* fp = NULL;
	if(!hay_std_output){
		fp = fopen(path_file, "w");
		if(fp == NULL){
        	perror("No se pudo leer el archivo. Imprimiendo el resultado por consola.");
			hay_std_output = true;
		}
	}
	while (leido != EOF){
		(*lines)=realloc((*lines), sizeof(char*)*(*cant_lineas+1));
		int hash = get_hash(buffer);
		if (!hay_std_output)
			fprintf(fp, "%s 0x%08x\n", buffer, hash);
		else
			printf ("%s 0x%08x\n", buffer, hash);
		leido = getline(&buffer, &buffer_size, stdin);
	}
	if(!hay_std_output)
		fclose(fp);
	free(buffer);
}

void salida_std(char** lines, int cant_lineas, int32_t* hashes){
	printf("\n\nRESULTADO DEL HASHING (FORMATO-> Linea N°: LINEA HASH)\n------------------------------------------------\n");
	for (int i=0; i<cant_lineas; i++){
		printf("Linea %i: %s 0x%08x\n",i, lines[i],hashes[i]);
	}
}


void parseAnswer(int argc, const char** argv) {
	int option;
	int cIndex = 0;
	int32_t* hashes = NULL;
	char** lines = NULL;
	int cant_lineas = 0;
	bool hubo_input = false;
	bool hubo_output = false;

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
				hash_input(optarg, &lines, &cant_lineas, &hashes);
				hubo_input = true;
				break;

			case 'o':
				hubo_output=true;
				if (!hubo_input){
					std_input(&lines, &cant_lineas, false, optarg);
				}
				break;

			default:
				printf("Comando inválido, pruebe con ./tp1 -h");
				break;
				
		}
	}
	if((!hubo_input && !hubo_output)){
		std_input(&lines, &cant_lineas, true, NULL);
	}
	if(hubo_input && !hubo_output){
		salida_std(lines, cant_lineas, hashes);
		destruir_lineas(lines, cant_lineas);
		destruir_hashes(hashes);
	}
}



