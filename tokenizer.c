#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"

static Tokenizer* tokenizer;
// forwards
static Token next_token         ();
static void  advance_buffer     ();
static void  advance_characters ();
static void  read_literal       (Token* token);

struct {
    struct {
        int row;
        int col;
    } pos;
} compiler;

#define TOKENIZER_ACTIVE_CHAR *tokenizer->active

static void advance_buffer() {
    tokenizer->active++;
    compiler.pos.row++;
}

static void advance_characters() {
    while (isspace(TOKENIZER_ACTIVE_CHAR))
        advance_buffer(tokenizer);
}

static void read_identifier(Token* token) {
    size_t i = 0;

    while (isalpha(TOKENIZER_ACTIVE_CHAR)) {
        token->value[i++] = TOKENIZER_ACTIVE_CHAR;
        advance_buffer(tokenizer);
    }

    token->value[i] = '\0';
}

static Token next_token() {
    Token token = { 0 };

    advance_characters(tokenizer);

    token.value[0] = TOKENIZER_ACTIVE_CHAR;
    token.value[1] = 0;

    switch (TOKENIZER_ACTIVE_CHAR) {
        case '(':
            token.type = TOKEN_LROUND;
            break;
        case ')':
            token.type = TOKEN_RROUND;
            break;
        case '[':
            token.type = TOKEN_LSQUARE;
            break;
        case ']':
            token.type = TOKEN_RSQUARE;
            break;
        case '<':
            token.type = TOKEN_LTHAN;
            break;
        case '>':
            token.type = TOKEN_GTHAN;
            break;
        case '{':
            token.type = TOKEN_LCURLY;
            break;
        case '}':
            token.type = TOKEN_RCURLY;
            break;
        case ';':
            token.type = TOKEN_SEMICOLON;
            break;
        case ',':
            token.type = TOKEN_COMMA;
            break;
        case '+':
            token.type = TOKEN_PLUS;
            break;
        case '-':
            token.type = TOKEN_MINUS;
            break;
        case '=':
            token.type = TOKEN_ASSGN;
            break;
        case '!':
            token.type = TOKEN_EXCLAM;
            break;
        case '/':
            token.type = TOKEN_FSLASH;
            break;
        case '&':
            token.type = TOKEN_AMPERSAND;
            break;
        case '*':
            token.type = TOKEN_ASTERISK;
            break;

        case EOF:
            token.type = TOKEN_EOF;
            token.value[0] = 0;
            break;

        default:
            if (!isalpha(TOKENIZER_ACTIVE_CHAR)) break;

            token.type = TOKEN_LITERAL;
            read_identifier(&token);

            return token;
    }

    advance_buffer(tokenizer);

    return token;
}

void thicc_tokenize_source(char* source) {
    assert(tokenizer);
    
    Tokenizer tok = { 0 };
    tokenizer = &tok;
    tokenizer->tokens = new_vector(sizeof(Token) * 128);

    Token token;
    for (tokenizer->active = source; tokenizer->active != 0; tokenizer->active++) {
        if (TOKENIZER_ACTIVE_CHAR == '\0') return;
        token = next_token(tokenizer);
        push_vector(tokenizer->tokens, &token);
    }
}