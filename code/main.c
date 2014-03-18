//
//  main.c
//  Lab3
//
//  Created by Bryce Holton. Modified by Levi Randolph-Roble
//  Copyright (c) 2014 Bryce Holton. All rights reserved.
//

#include "common.h"
#include "print.h"
#include "scanner.h"

FILE *init_lister(const char *name, char source_file_name[], char dte[]);
void quit_scanner(FILE *src_file, Token *list);
void add_token_to_list(Token *list, Token *new_token);

int main(int argc, const char * argv[])
{
    Token *token;
    Token *token_list = (struct Token*)malloc(sizeof(Token)); //This needs to be implemented as a linked list in scanner.h.
    token_list->nextToken = NULL;
    char source_name[MAX_FILE_NAME_LENGTH];
    char date[DATE_STRING_LENGTH];
    FILE *source_file = init_lister("NEWTON.PAS", source_name, date);
    init_scanner(source_file, source_name, date);

    do
    {
        token = get_token();
        add_token_to_list(token_list, token);
        print_token(token,source_name,date);
    }
    while (token->token_code != 18);//What is the sentinel value that ends this loop?

    quit_scanner(source_file, token_list);
    return 0;
}
void add_token_to_list(Token *list, Token *new_token)
{
    // Add new_token to the list knowing that list is a linked list.
    if(list->nextToken == NULL)
    {
        list->nextToken = new_token;
    }else
    {
        add_token_to_list(list->nextToken,new_token);
    }
}
void quit_scanner(FILE *src_file, Token *list)
{
    if(list->nextToken == NULL)
    {
        free(list);
        return;
    }else
    {
        quit_scanner(src_file,list->nextToken);
    }
    //write code to free all of the memory for the token list

    fclose(src_file);
}
FILE *init_lister(const char *name, char source_file_name[], char dte[])
{
    time_t timer;
    FILE *file;

    strcpy(source_file_name, name);
    file = fopen(source_file_name, "r");
    time(&timer);
    strcpy(dte, asctime(localtime(&timer)));
    return file;
}

