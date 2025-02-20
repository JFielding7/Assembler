#ifndef ASSEMBLY_GENERATOR_H
#define ASSEMBLY_GENERATOR_H

#include "ast_node.h"

void generate_assembly(ast_node *);

void function_assembly(void *);

void assignment_assembly(void *);

void add_assembly(void *);

void load_assembly(void *);

void literal_assembly(void *);

#endif //ASSEMBLY_GENERATOR_H
