

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_SIZE_ROWS 10
#define INITIAL_SIZE_COLUMNS 10

typedef struct {
    int lines;
    int symbolsPerLine;
    char** text;
    int currentLine;
} text;

char** createArray(int rows, int columns)
{
    char** array = (char**)malloc(rows * sizeof(char*));
    if (array == NULL)
    {
        printf("Memory allocation failed\n");
        exit(0);
    }
    for (int i = 0; i < rows; i++)
    {
        array[i] = (char*)malloc(columns * sizeof(char));
        if (array[i] == NULL)
        {
            printf("Memory allocation failed\n");
            exit(0);
        }
        array[i][0] = '\0';  
    }
    return array;
}

void AppendToEnd(text* editor, char newText[])
{
    int currentLength = strlen(editor->text[editor->currentLine]);
    int newLength = strlen(newText);

    
    if (newLength > 0 && newText[newLength - 1] == '\n') {
        newText[newLength - 1] = '\0';
        newLength--;
    }

    if (currentLength + newLength >= editor->symbolsPerLine)
    {
        int newCapacity = (currentLength + newLength + 1) * 2;
        editor->text[editor->currentLine] = (char*)realloc(editor->text[editor->currentLine], newCapacity * sizeof(char));
        if (editor->text[editor->currentLine] == NULL) {
            printf("Memory reallocation failed\n");
            exit(0);
        }
        editor->symbolsPerLine = newCapacity;
    }
    strcat(editor->text[editor->currentLine], newText);
}

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

void Print(text* editor)
{
    for (int i = 0; i < editor->currentLine + 1; i++) {  
        printf("%s\n", editor->text[i]);
    }
}

void StartNewLine(text* editor)
{
    
    if (editor->currentLine + 1 == editor->lines)
    {
        int newLinesCapacity = editor->lines * 2;
        char** temp = editor->text;
        editor->text = (char**)realloc(editor->text, newLinesCapacity * sizeof(char*));
        if (editor->text == NULL)
        {
            printf("Out of Memory\n");
            editor->text = temp;
            return;
        }
        for (int i = editor->lines; i < newLinesCapacity; i++)
        {
            editor->text[i] = (char*)malloc(INITIAL_SIZE_COLUMNS * sizeof(char));
            if (editor->text[i] == NULL)
            {
                printf("Memory allocation failed\n");
                exit(0);
            }
            editor->text[i][0] = '\0';  
        }
        editor->lines = newLinesCapacity;
    }
    editor->currentLine++;
}

void ProcessCommand(int command, text* editor)
{
    switch (command)
    {
    case 9:
        help();
        break;
    case 0:
        printf("Exiting text editor. Bye!\n");
        break;
    case 1:
        printf("Enter text to append:\n");
        char newText[100];
        fgets(newText, sizeof(newText), stdin);
        AppendToEnd(editor, newText);
        break;
    case 2:
        printf("Starting new line...\n");
        StartNewLine(editor);
        printf("Current line:%d\n", editor->currentLine);
        break;
    case 3:
        printf("...");
        break;
    case 4:
        Print(editor);
        break;
    case 5:
        printf("...");
        break;
    case 6:
        printf("...");
        break;
    default:
        printf("The command is not implemented. Type '9' for help.\n");
    }
}


int main()
{
    printf("Hello! Welcome to the Text Editor! Enter '9' to see the available list of commands :)\n");
    text editor;
    int command;
    editor.text = createArray(INITIAL_SIZE_ROWS, INITIAL_SIZE_COLUMNS);
    editor.symbolsPerLine = INITIAL_SIZE_COLUMNS;
    editor.lines = INITIAL_SIZE_ROWS;
    editor.currentLine = 0;
    do {
        printf("Enter command: ");
        
        if (scanf("%d", &command) != 1)
        {
            printf("The command is not an integer!\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
           
        }
        while (getchar() != '\n'); 
        ProcessCommand(command, &editor);
    } while (command != 0);
    for (int i = 0; i < editor.lines; i++)
    {
        free(editor.text[i]);
    }
    free(editor.text);
    return 0;
}