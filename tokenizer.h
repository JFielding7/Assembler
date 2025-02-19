#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "vec.h"

#include <stdbool.h>

bool valid_symbol(char *symbol);

vec tokenize_file(char *filename);

#endif //TOKENIZER_H
