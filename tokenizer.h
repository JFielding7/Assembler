#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "vec.h"

#include <stdbool.h>

bool invalid_symbol(char *symbol);

vec *tokenize_source_code_files(char **filenames);

#endif //TOKENIZER_H
