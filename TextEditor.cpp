// TextEditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define INITIAL_CAPACITY 50
#define MAX_WORDS_PER_LINE 80

void help()
{
    printf("There is an explanation for every command:\n"
        "h - explain every command;\n"
        "1 - append text symbols to the end\n"
        "2 - start the new line\n"
        "3 - use files to loading/saving the information\n"
        "4 - print the current text to console\n"
        "5 - insert the text by line and symbol index\n"
        "6 - search\n");
}
typedef struct {
    int length;
    char* text;
}line;

typedef struct
{
    line* lines;
    int capacity;
    int linesNum;
}text;

void ProcessCommand(char command)
{
    switch (command)
    {
    case 'h':
        help();
        break;
    case '0':
        printf("Bye!");
        break;
    case '1':
        printf("...");
        break;
    case '2':
        printf("...");
        break;
    case '3':
        printf("...");
        break;
    case '4':
        printf("...");
        break;
    case '5':
        printf("...");
        break;
    case '6':
        printf("...");
        break;
    default:
        printf("The command is not implemented. Type 'h' in case you are lost!\n");
    }
}
int main()
{
    printf("Hello! Welcome to the Text Editor! Enter 'h' to see the available list of commands :)\n");
    char command;
    text Text;
    Text.capacity = INITIAL_CAPACITY;
    Text.linesNum = 0;
    Text.lines = (line*)malloc(sizeof(line) * INITIAL_CAPACITY);

    do {
        scanf(" %c", &command);
        ProcessCommand(command);
    } while (command != '0');
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
