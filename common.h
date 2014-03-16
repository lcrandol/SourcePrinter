//
//  common.c
//  Lab3
//
//  Created by Bryce Holton. Modified by Levi Randolph-Roble
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#ifndef CSE220_project2_common_h
#define CSE220_project2_common_h

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#define FORM_FEED_CHAR          '\f'
#define MAX_FILE_NAME_LENGTH    32
#define MAX_SOURCE_LINE_LENGTH  256
#define MAX_TOKEN_STRING_LENGTH MAX_SOURCE_LINE_LENGTH
#define MAX_PRINT_LINE_LENGTH   80
#define MAX_LINES_PER_PAGE      50
#define DATE_STRING_LENGTH      26

typedef enum
{
    FALSE, TRUE,
}
BOOLEAN;

/***************************
 Token Codes
 ***************************/
typedef enum
{
    NO_TOKEN, IDENTIFIER, NUMBER, STRING, UPARROW, STAR, LPAREN,
    RPAREN, MINUS, PLUS, EQUAL, LBRACKET,
    RBRACKET, COLON, SEMICOLON, LT, GT, COMMA, PERIOD, SLASH,
    COLONEQUAL, LE, GE, NE, DOTDOT, END_OF_FILE, ERROR, AND,
    ARRAY, BEGIN, CASE, CONST, DIV, DO, DOWNTO, ELSE, END, FFILE,
    FOR, FUNCTION, GOTO, IF, IN, LABEL, MOD, NIL, NOT, OF, OR, PACKED,
    PROCEDURE, PROGRAM, RECORD, REPEAT, SET, THEN, TO, TYPE, UNTIL,
    VAR, WHILE, WITH,
}
TokenCode;

/*****************************
 Literal Type
 *****************************/
typedef enum
{
    INTEGER_LIT, REAL_LIT, STRING_LIT,
}
LiteralType;

/**************
 this is a valid Pascal token.  A token must have a literal type,
 a literal value, and a token code.  It also must have a link to
 another token since this must be stored as a linked list.
 ***************/
typedef struct
{
    //Missing code goes here
    char* token_string;
    int tokenType;
    struct Token *nextToken;
}
Token;

#endif
