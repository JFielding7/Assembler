#ifndef ASSEMBLY_GENERATOR_H
#define ASSEMBLY_GENERATOR_H

#include "ast_node.h"

void generate_assembly(ast_node *);

void function_assembly(ast_node*);

void assignment_assembly(ast_node*);

void mul_assembly(ast_node*);

void div_assembly(ast_node*);

void mod_assembly(ast_node*);

void add_assembly(ast_node*);

void sub_assembly(ast_node*);

void load_assembly(ast_node*);

void literal_assembly(ast_node*);

#endif //ASSEMBLY_GENERATOR_H
