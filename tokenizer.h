#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "vector.h"

typedef enum {
    TOKEN_LITERAL,
    TOKEN_NUMERICAL,
    TOKEN_LROUND,
    TOKEN_RROUND,
    TOKEN_LSQUARE,
    TOKEN_RSQUARE,
    TOKEN_LCURLY,
    TOKEN_RCURLY,
    TOKEN_LTHAN,
    TOKEN_GTHAN,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_FSLASH,
    TOKEN_ASSGN,
    TOKEN_EQUAL,
    TOKEN_NEQUAL,
    TOKEN_EXCLAM,
    TOKEN_AMPERSAND,
    TOKEN_ASTERISK,
    TOKEN_EOF,
} TokenType;

typedef struct {
    uint8_t row;
    uint8_t col;
} SymbolPos;

typedef enum {
    FROM_FILE = 1, // otherwise from string
    HIGHLIGHT = 2,
//  DEBUG     = 4, // use compiler process debug flag instead 
    HEAP_ONLY = 8, // TODO
} ScanFlags;

#define TOKENIZER_BUFFER_LENGTH 4096
#define TOKENIZER_BUFFER_SIZE   sizeof(char)

typedef struct {
    ScanFlags  flags;
    Vector*    tokens; // Vector of Token
    char*      active;
    char       buffer[TOKENIZER_BUFFER_LENGTH]; // buffer for fread if input is a file
} Tokenizer;

typedef struct {
    char value[64];
    SymbolPos pos;
    TokenType type;
} Token;

void tokenize_source(char* source);

#endif