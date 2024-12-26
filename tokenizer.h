#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "vec.h"

#define TOKEN_REGEX "\n[ \t]*|[-+*/%|&~^()=]|\\w+"
// #define TOKEN_REGEX "\n"
#define TOKEN_REGEX_FLAGS (REG_EXTENDED)

vec *tokenize_source_code_files(vec *files);

#endif //TOKENIZER_H
