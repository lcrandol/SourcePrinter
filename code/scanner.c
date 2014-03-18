//
//  scanner.c
//  Lab3
//
//  Created by Bryce Holton.
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include <stdio.h>
#include "scanner.h"

/*******************
 Static functions needed for the scanner
 You need to design the proper parameter list and
 return types for functions with ???.
 ******************/
static char* get_char(char source_buffer[MAX_TOKEN_STRING_LENGTH], char *token_ptr);
static char* skip_comment(char *token_ptr);
static char* skip_blanks(char *token_ptr);
static char* get_word(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr);
static char* get_number(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr);
static char* get_string(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr);
static char* get_special(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr);
static char* downshift_word(char wordToLower[]);
static BOOLEAN is_reserved_word(Token *theToken);

typedef enum
{
    LETTER, DIGIT, QUOTE, SPECIAL, EOF_CODE,
}
CharCode;

/*********************
 Static Variables for Scanner
 Must be initialized in the init_scanner function.
 *********************/
static FILE *src_file;
static char src_name[MAX_FILE_NAME_LENGTH];
static char todays_date[DATE_STRING_LENGTH];
static CharCode char_table[256];  // The character table

typedef struct
{
    char *string;
    TokenCode token_code;
}
RwStruct;

const RwStruct rw_table[9][10] = {
    {{"do",DO},{"if",IF},{"in",IN},{"of",OF},{"or",OR},{"to",TO},{NULL,0}}, //Reserved words of size 2
    {{"and",AND},{"div",DIV},{"end",END},{"for",FOR},{"mod",MOD},{"nil",NIL},{"not",NOT},{"set",SET},{"var",VAR},{NULL,0}}, //Reserved words of size 3
    {{"case",CASE},{"else",ELSE},{"file",FFILE},{"goto",GOTO},{"then",THEN},{"type",TYPE},{"with",WITH},{NULL,0}}, //Reserved words of size 4
    {{"array",ARRAY},{"begin",BEGIN},{"const",CONST},{"label",LABEL},{"until",UNTIL},{"while",WHILE},{NULL,0}},  //Reserved words of size 5
    {{"downto",DOWNTO}, {"packed",PACKED},{"record",RECORD}, {"repeat",REPEAT},{NULL,0}},  // Reserved words of size 6
    {{"program", PROGRAM},{NULL,0}}, // Reserved words of size 7
    {{"function", FUNCTION},{NULL,0}}, // Reserved words of size 8
    {{"procedure", PROCEDURE},{NULL,0}}  // Reserved words of size 9
};

/****************************
Symbol Strings 1
****************************/
const char* const SYMBOL_STRINGS1[] =
{
    "<no token>", "<IDENTIFIER>", "<NUMBER>", "<STRING>",
    "^","*","(",")","-","+","=","[","]",":",";",
    "<",">",",",".","/",":=","<=",">=","<>","..",
    "<END OF FILE>", "<ERROR>",
    "AND","ARRAY","BEGIN","CASE","CONST","DIV","DO","DOWNTO",
    "ELSE","END","FILE","FOR","FUNCTION","GOTO","IF","IN",
    "LABEL","MOD","NIL","NOT","OF","OR","PACKED","PROCEDURE",
    "PROGRAM","RECORD","REPEAT","SET","THEN","TO","TYPE","UNTIL",
    "VAR","WHILE","WITH",
};

void init_scanner(FILE *source_file, char source_name[], char date[])
{
    src_file = source_file;
    strcpy(src_name, source_name);
    strcpy(todays_date, date);

    /*******************
     initialize character table, this table is useful for identifying what type of character
     we are looking at by setting our array up to be a copy the ascii table.  Since C thinks of
     a char as like an int you can use ch in get_token as an index into the table.
     *******************/
     int x;
     for(x = 0;x <= 127;x++)
     {
         if((x > 64 && x < 91 )||(x > 96 && x < 123))
         {
             char_table[x] = LETTER;
         }else if(x > 47 && x < 58)
         {
             char_table[x] = DIGIT;
         }else if(x == 34 || x == 39)
         {
             char_table[x] = QUOTE;
         }else if(x == 46)
         {
             char_table[x] = END_OF_FILE;
         }else
         {
             char_table[x] = SPECIAL;
         }
     }


}

BOOLEAN get_source_line(char source_buffer[])
{
    char print_buffer[MAX_SOURCE_LINE_LENGTH + 9];
//    char source_buffer[MAX_SOURCE_LINE_LENGTH];  //I've moved this to a function parameter.  Why did I do that?
    static int line_number = 0;


    if (fgets(source_buffer, MAX_SOURCE_LINE_LENGTH, src_file) != NULL)
    {
        ++line_number;
        sprintf(print_buffer, "%4d: %s", line_number, source_buffer);
        print_line(print_buffer, src_name, todays_date);
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

Token* get_token()
{
    char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
    static char source_buffer[MAX_SOURCE_LINE_LENGTH] = {NULL};
    Token *theToken = (struct Token*)malloc(sizeof(Token));//I am missing the most important variable in the function, what is it?  Hint: what should I return?
    static char *token_ptr;
    token_ptr = get_char(source_buffer,token_ptr);
    switch(char_table[*(token_ptr)])
    {
    case LETTER:
        token_ptr = get_word(theToken,token_string,token_ptr);
        break;

    case DIGIT:
        token_ptr = get_number(theToken,token_string,token_ptr);
        break;

    case QUOTE:
        token_ptr = get_string(theToken,token_string,token_ptr);
        break;

    case SPECIAL:
        token_ptr = get_special(theToken,token_string,token_ptr);
        break;

    case END_OF_FILE:
        theToken->token_code = PERIOD;
        theToken->literal_value = ".";
        theToken->nextToken = NULL;
        break;

    default:
        theToken->token_code = PERIOD;
        theToken->literal_value = ".";
        theToken->nextToken = NULL;
        break;
    }
    //2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch
    //3.  Call the appropriate function to deal with the cases in 2.
    return theToken; //What should be returned here?
}

static char* get_char(char source_buffer[MAX_TOKEN_STRING_LENGTH], char *token_ptr)
{
    if(source_buffer[0] == NULL)
    {
        if(!get_source_line(source_buffer))
        {
            return '.';
        }
        token_ptr = &source_buffer[0];
    }
    if((*(token_ptr)) == 10)
    {
        if(!get_source_line(source_buffer))
        {
            return '.';
        }
        token_ptr = source_buffer;
        if(*(token_ptr) == '\n')
        {
            token_ptr = get_char(source_buffer,token_ptr);
        }
    }
    if((*(token_ptr)) == 46)
    {
        *token_ptr = '.';
        return token_ptr;
    }
    if((*(token_ptr)) == 123)
    {
        token_ptr = skip_comment(token_ptr);
    }
    if(*token_ptr == 9) // Horizontal Tabs are a pain in my rump!!!
    {
        token_ptr++;
        if(*(token_ptr) == 9) //Recursively make them go away.
        {
            token_ptr = get_char(source_buffer,token_ptr);
        }
    }
    if((*(token_ptr)) == 32)
    {
        token_ptr = skip_blanks(token_ptr); //1.  Skip past all of the blanks
    }
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */
     return token_ptr;
}

static char* skip_blanks(char *token_ptr)
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */

     while((*(token_ptr)) == 32)
     {
         token_ptr++;
     }
     return token_ptr;
}

static char* skip_comment(char *token_ptr)
{
    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */

     while((*(token_ptr)) != 125)
     {
         token_ptr++;
     }
     token_ptr++;
     return token_ptr;
}

static char* get_word(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr)
{
    /*
     Write some code to Extract the word
     */
     int charCount = 0;
     while((char_table[(*(token_ptr))] == LETTER || char_table[(*(token_ptr))] == DIGIT) && charCount < (MAX_TOKEN_STRING_LENGTH-1))
     {
        token_string[charCount] = *(token_ptr);
        token_string[charCount+1] = '\0';
        charCount++;
        token_ptr++;
     }

    theToken->literal_value = downshift_word(token_string); //Downshift the word, to make it lower case

    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
     if(is_reserved_word(theToken))
     {
         theToken->literal_type = STRING_LIT;
     }else
     {
         theToken->literal_type = STRING_LIT;
         theToken->token_code = IDENTIFIER;
         theToken->nextToken = NULL;
     }



     return token_ptr;
}

static char* get_number(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr)
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
     int charCount = 0;
     int realCount = 0;
     while((*(token_ptr) == 'e' || *(token_ptr) == '-' || *(token_ptr) == '.' || char_table[(*(token_ptr))] == DIGIT) && charCount < (MAX_TOKEN_STRING_LENGTH-1))
     {
        token_string[charCount] = *(token_ptr);
        token_string[charCount+1] = '\0';
        if((*(token_ptr) == 'e' || *(token_ptr) == '.'))
        {
            realCount++;
        }
        charCount++;
        token_ptr++;
     }

     if(realCount > 0)
     {
         theToken->literal_type = REAL_LIT;
     }else
     {
         theToken->literal_type = INTEGER_LIT;
     }
     theToken->literal_value = token_string;
     theToken->token_code = NUMBER;
     theToken->nextToken = NULL;

     return token_ptr;
}

static char* get_string(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr)
{
    /*
     Write some code to Extract the string
     */
     int charCount = 0;
     token_ptr++;
     while(*(token_ptr) != 39 && charCount < (MAX_TOKEN_STRING_LENGTH-1))
     {
        token_string[charCount] = *(token_ptr);
        token_string[charCount+1] = '\0';
        charCount++;
        token_ptr++;
     }
     theToken->token_code = STRING;
     theToken->literal_value = token_string;
     theToken->literal_type = STRING_LIT;
     theToken->nextToken = NULL;
     token_ptr++;

     return token_ptr;
}

static char* get_special(Token* theToken, char token_string[MAX_TOKEN_STRING_LENGTH],char *token_ptr)
{
    /*
     Write some code to Extract the special token.  Most are single-character
     some are double-character.  Set the token appropriately.
     */
     /*switch(*token_ptr)
     {
     case 94:
        theToken->literal_value = "^";
        theToken->token_code = UPARROW;
        break;
     case 42:
        theToken->literal_value = "*";
        theToken->token_code = STAR;
        break;
     case 40:
        theToken->literal_value = "(";
        theToken->token_code = LPAREN;
        break;
     case 41:
        theToken->literal_value = ")";
        theToken->token_code = RPAREN;
        break;
     case 45:
        theToken->literal_value = "-";
        theToken->token_code = MINUS;
        break;
     case 43:
        theToken->literal_value = "+";
        theToken->token_code = PLUS;
        break;
     case 61:
        theToken->literal_value = "=";
        theToken->token_code = EQUAL;
        break;
     case 91:
        theToken->literal_value = "[";
        theToken->token_code = LBRACKET;
        break;
     case 93:
        theToken->literal_value = "]";
        theToken->token_code = RBRACKET;
        break;
     case 58:
        if(*(token_ptr+1) == '=')
        {
            theToken->literal_value = ":=";
            theToken->token_code = COLONEQUAL;
            token_ptr++;
        }else
        {
            theToken->literal_value = ":";
            theToken->token_code = COLON;
        }
        break;
     case 59:
        theToken->literal_value = ";";
        theToken->token_code = SEMICOLON;
        break;
     case 60:
        if(*(token_ptr+1) == '=')
        {
            theToken->literal_value = "<=";
            theToken->token_code = LE;
            token_ptr++;
        }else if(*(token_ptr+1) == '>')
        {
            theToken->literal_value = "<>";
            theToken->token_code = NE;
            token_ptr++;
        }else
        {
            theToken->literal_value = "<";
            theToken->token_code = LT;
        }
        break;
     case 62:
        if(*(token_ptr+1) == '=')
        {
            theToken->literal_value = ">=";
            theToken->token_code = GE;
            token_ptr++;
        }else
        {
            theToken->literal_value = ">";
            theToken->token_code = GT;
        }
        break;
     default:
        theToken->literal_value = ".";
        theToken->token_code = PERIOD;
        break;
     }*/
     int x;
     for(x = 4; x < 20;x++)
     {
         if( *(SYMBOL_STRINGS1[x]) == *(token_ptr))
         {
             theToken->literal_value = SYMBOL_STRINGS1[x];
             theToken->nextToken = NULL;
             theToken->token_code = x;
             token_ptr++;
             return token_ptr;
         }
     }
     theToken->nextToken = NULL;
     token_ptr++;

     return token_ptr;
}

static char* downshift_word(char wordToLower[])
{
    /*
     Make all of the characters in the incoming word lower case.
     */
     int x = 0;
     while(wordToLower[x] != '\0')
     {
        if(wordToLower[x] > 64 && wordToLower[x] < 91)
        {
            wordToLower[x] = wordToLower[x] + 32;
        }
        x++;
     }

     return wordToLower;
}

static BOOLEAN is_reserved_word(Token *theToken)
{
    /*
     Examine the reserved word table and determine if the function input is a reserved word.
     */
     char wordToCheck[MAX_TOKEN_STRING_LENGTH];
     strcpy(wordToCheck,theToken->literal_value);
     int x = 0;
     int count = 0;
     while(wordToCheck[x] != '\0')
     {
         count++;
         x++;
     }
     if(count > 1 && count < 10)
     {
         int y = 0;
         while(rw_table[count-2][y].string != NULL)
         {
             if(strcmp(rw_table[count-2][y].string,wordToCheck) == 0)
             {
                 theToken->nextToken = NULL;
                 theToken->token_code = rw_table[count-2][y].token_code;
                 return (TRUE);
             }
             y++;
         }
     }
    return (FALSE);
}
