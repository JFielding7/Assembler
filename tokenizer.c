#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

/**
 * Opens a source code file
 * @param name name of the file
 * @return FILE* for the opened file
 */
FILE *open_source_file(const char *name) {
    // validating the file name
    const size_t len = strlen(name);
    if (len < MIN_FILENAME_LEN || strcmp(name + len - MIN_FILENAME_LEN + 1, FILE_EXT) != 0) {
        fprintf(stderr, "Invalid file: %s\n", name);
        return NULL;
    }

    // open the file
    FILE *source_file = fopen(name, "r");
    if (source_file == NULL) {
        fprintf(stderr, "File not found: %s\n", name);
        return NULL;
    }

    return source_file;
}

/**
 * Gets the size of the file
 * @param file The file to caclualte the size of
 * @return ssize_t size of the file
 */
ssize_t get_file_size(FILE *file) {
    // seek to the end of the file
    const int ret = fseek(file, 0, SEEK_END);
    if (ret != 0) {
        return -1;
    }

    // calculate the size as the difference between the end and start
    const ssize_t offset = ftell(file);
    rewind(file);
    return offset;
}

/**
 * Reads in the contents of a source code file
 * @param name name of the file
 * @return char* contents of the file
 */
char *read_source_file(const char *name) {
    // open the file
    FILE *file = open_source_file(name);
    if (file == NULL) {
        return NULL;
    }

    // get the size of the file
    const ssize_t source_file_size = get_file_size(file);
    if (source_file_size < 0) {
        fprintf(stderr, "Failed to calculate size of file: %s\n", name);
        fclose(file);
        return NULL;
    }

    // allocate memory for the file contents
    char *buffer = malloc(source_file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate buffer for file: %s\n", name);
        fclose(file);
        return NULL;
    }

    // read the contents into the allocated buffer
    const size_t bytes_read = fread(buffer, sizeof(char), source_file_size, file);
    fclose(file);

    if (bytes_read != source_file_size) {
        fprintf(stderr, "Failed to read from file: %s\n", name);
        free(buffer);
        return NULL;
    }

    buffer[bytes_read] = '\0';
    return buffer;
}

int tokenize(regex_t *regex, regmatch_t* match, char *source_code_cursor, vec *tokenv) {
    while (regexec(regex, source_code_cursor, 1, match, 0) == 0) {
        source_code_cursor += match->rm_so;

        size_t token_len = match->rm_eo - match->rm_so;
        char* token = malloc(token_len + 1);
        strncpy(token, source_code_cursor, token_len);
        token[token_len] = '\0';
        printf("%s %lu\n", token, token_len);

        vec_add(tokenv, char*, token);
        source_code_cursor += token_len;
    }

    return 0;
}

/**
 *
 * @param files
 * @return
 */
vec *tokenize_source_code_files(char **filenames) {
    regex_t regex;
    const int ret = regcomp(&regex, TOKEN_REGEX, TOKEN_REGEX_FLAGS);
    if (ret != 0) {
        perror("Failed to compile token regex\n");
        return NULL;
    }

    vec *tokenv = vec_new(char*);
    for (char **source_file = filenames; *source_file != NULL; source_file++) {
        char *source_file_content = read_source_file(*source_file);
        if (source_file_content == NULL) {
            vec_free_all(tokenv);
            regfree(&regex);
            return NULL;
        }

        char *new_line = malloc(2);
        new_line[0] = '\n';
        new_line[1] = '\0';
        vec_add(tokenv, char*, new_line);

        regmatch_t match[1];
        tokenize(&regex, match, source_file_content, tokenv);

        free(source_file_content);
    }

    regfree(&regex);

    return tokenv;
}
