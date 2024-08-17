// tokenizer for template engine

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

ccalculus compiler = { 0 };

bool is_invalid(char ch) {
    return isspace(ch);
}

void advance_buffer() {
    compiler.tokenizer->active = getc(compiler.buffer);
    compiler.pos.row++;
}

void skip_whitespace(Tokenizer* tokenizer) {
    while (isspace(tokenizer->active))
        advance_buffer();
}

void advance_characters(Tokenizer* tokenizer) {
    do { advance_buffer(tokenizer); }
    while (is_invalid(tokenizer->active));
}

void read_identifier(Tokenizer* tokenizer, Token* token) {
    size_t i = 0;

    while (isalpha(tokenizer->active)) {
        token->value[i++] = tokenizer->active;
        advance_buffer(tokenizer);
    }

    token->value[i] = '\0';
}

Token next_token(Tokenizer* tokenizer) {
    Token token = { 0 };

    skip_whitespace(tokenizer);

    token.value[0] = tokenizer->active;
    token.value[1] = 0;

    switch (tokenizer->active) {
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
            if (!isalpha(tokenizer->active)) break;

            token.type = TOKEN_LITERAL;
            read_identifier(tokenizer, &token);

            return token;
    }

    advance_buffer(tokenizer);

    return token;
}

void argparse(int argc, char** argv) {
    char* argname;
    char* argval;

    for (int i = 1; i < argc; i++) {
        argname = argv[i];
        argval = argv[++i];

        if (strcmp(argname, "-o") == 0) {
            compiler.flags |= OUTFILE;
            compiler.outname = argval; 
        }
        else
        if (strcmp(argname, "-s") == 0) {
            // currently source file needs to be appended
            // after this flag, will fix later because this is dumb as fuck
            compiler.flags |= INFILE;
            compiler.buffname = argval;
        }
    }

    // TODO: finish this
    if (compiler.flags & INFILE == 0)
        fprintf(stderr, "no source provided with -s\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: cc2 -s [source] -o [out]\n");
        fprintf(stderr, "expected argument, received none\n");
        return -1;
    }

    argparse(argc, argv);

    compiler.buffer = fopen(compiler.buffname, "rb");
    compiler.tokenizer = &(Tokenizer) { 0 };

    Token token;
    while ((token = next_token(compiler.tokenizer)).type != TOKEN_EOF)
        printf("%s\n", token.value);

    fclose(compiler.buffer);
    return 0;
}
