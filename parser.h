#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <getopt.h>
#include "hash.h"

char** read_lines(char* file, int* cant_lineas);
void destruir_lineas(char** lines, int cantidad_lineas);
void destruir_hashes(int* hashes);
void hash_input(char* input_file, char ***lines, int* cant_lineas, int32_t** hashes);
void output_hash(const char* output_file, char **lines, int cant_lineas, int32_t* hashes);
void parseAnswer(int argc, const char** argv);
int get_hash(char *msg);
int32_t get_hash_(string_hash *sh, char *msg, size_t len, size_t stride);
void salida_std(char** lines, int cant_lineas, int32_t* hashes);
void std_input(char** lines, int* cant_lineas, bool hay_std_output);

#endif /*PARSER_H*/


