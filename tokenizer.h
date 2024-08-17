#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

bool is_whitespace(char ch);

// chasm template is tokenized and parsed
// any source code within a template is not parsed

typedef enum {
    TOKEN_LITERAL,
    TOKEN_NUMERICAL,
    TOKEN_LCURLY,
    TOKEN_RCURLY,
    TOKEN_LTHAN,
    TOKEN_GTHAN,
    TOKEN_COMMA,
    TOKEN_FSLASH,
    TOKEN_BSLASH,
    TOKEN_EQUAL,
    TOKEN_EXCLAM,
    TOKEN_DOLLAR,
    TOKEN_EOF,
} TokenType;

typedef struct {
    char**     filename;
    uint8_t    row;
    uint8_t    col;
} SymbolPos;

typedef struct {
    char       active;
    // token sequence
} Tokenizer;

typedef struct {
    FILE*      buffer;
    char*      buffname; 
    Tokenizer* tokenizer;
    SymbolPos  pos;
} chasm;

typedef struct {
    char       value[64];
    SymbolPos  pos;
    TokenType  type;
    TokenFlags flags;
} Token;

extern ccalculus compiler;

bool  is_invalid(char ch);
void  advance_buffer();
void  advance_characters(Tokenizer* tokenizer);
void  read_literal(Tokenizer* tokenizer, Token* token);
Token next_token(Tokenizer* tokenizer);

#endif
