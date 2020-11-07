#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "hash.h"

char** read_lines(char* file, int* cant_lineas);
void destruir_lineas(char** lines, int cantidad_lineas);
void hash_input(char* input_file, char ***lines, int* cant_lineas);
void output_hash(const char* output_file, char **lines, int cant_lineas);
void parseAnswer(int argc, const char** argv);

#endif /*PARSER_H*/


