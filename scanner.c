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
static char get_char(char token_string[MAX_TOKEN_STRING_LENGTH], char *token_ptr);
static char* skip_comment();
static char* skip_blanks();
static Token get_word(char* start);
static Token get_number(char* start);
static Token get_string(char* start);
static Token get_special(char* start);
static char* downshift_word(char* wordToLower);
static BOOLEAN is_reserved_word(char* wordToCheck);

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
             char_table[x] = 0;
         }else if(x > 47 && x < 58)
         {
             char_table[x] = 1;
         }else if(x == 34 || x == 39)
         {
             char_table[x] = 2;
         }else if(x == 46)
         {
             char_table[x] = 4;
         }else
         {
             char_table[x] = 3;
         }
     }
     for(x = 65;x <= 90;x++)
     {
         char_table[x] = 1;
     }
     for(x = 97;x <= 122;x++)
     {
         char_table[x] = 1;
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
    char ch; //This can be the current character you are examining during scanning.
    char token_string[MAX_TOKEN_STRING_LENGTH]; //Store your token here as you build it.
    char *token_ptr = token_string[0]; //write some code to point this to the beginning of token_string
    Token* theToken;  //I am missing the most important variable in the function, what is it?  Hint: what should I return?

    //1.  Skip past all of the blanks
    //2.  figure out which case you are dealing with LETTER, DIGIT, QUOTE, EOF, or special, by examining ch
    //3.  Call the appropriate function to deal with the cases in 2.

    return theToken; //What should be returned here?
}
static char get_char(char token_string[MAX_TOKEN_STRING_LENGTH], char *token_ptr)
{
    /*
     If at the end of the current line (how do you check for that?),
     we should call get source line.  If at the EOF (end of file) we should
     set the character ch to EOF and leave the function.
     */

    /*
     Write some code to set the character ch to the next character in the buffer
     */
}
static char* skip_blanks()
{
    /*
     Write some code to skip past the blanks in the program and return a pointer
     to the first non blank character
     */

}
static char* skip_comment()
{
    /*
     Write some code to skip past the comments in the program and return a pointer
     to the first non blank character.  Watch out for the EOF character.
     */
}
static Token get_word(char* start)
{
    /*
     Write some code to Extract the word
     */

    //Downshift the word, to make it lower case

    /*
     Write some code to Check if the word is a reserved word.
     if it is not a reserved word its an identifier.
     */
}
static Token get_number(char* start)
{
    /*
     Write some code to Extract the number and convert it to a literal number.
     */
}
static Token get_string(char* start)
{
    /*
     Write some code to Extract the string
     */
}
static Token get_special(char* start)
{
    /*
     Write some code to Extract the special token.  Most are single-character
     some are double-character.  Set the token appropriately.
     */
}
static char* downshift_word(char* wordToLower)
{
    /*
     Make all of the characters in the incoming word lower case.
     */
}
static BOOLEAN is_reserved_word(char* wordToCheck)
{
    /*
     Examine the reserved word table and determine if the function input is a reserved word.
     */
    return FALSE;
}
