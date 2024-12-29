#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "vec.h"

#define MIN_FILENAME_LEN 4
#define FILE_EXT ".ro"

#define TOKEN_REGEX "\n[ \t]*|[-+*/%|&~^()=,]|\\w+|\".*?[^\\\\]\""
#define TOKEN_REGEX_FLAGS (REG_EXTENDED)

vec *tokenize_source_code_files(char **filenames);

#endif //TOKENIZER_H
