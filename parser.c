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

void hash_input(char* input_file, char ***lines, int* cant_lineas){
	(*lines) = read_lines(input_file, cant_lineas);
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
	char** lines;
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



