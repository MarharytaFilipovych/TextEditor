#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_SIZE_ROWS 10
#define INITIAL_SIZE_COLUMNS 10
#include <io.h>

typedef struct {
    size_t lines;
    size_t symbolsPerLine;
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

void MakeLineLonger(text* editor, size_t currentLength, size_t newLength)
{
    size_t newCapacity = currentLength + newLength + 1;
    char* temp = (char*)realloc(editor->text[editor->currentLine], newCapacity * sizeof(char));
    if (temp == NULL) {
        printf("Memory reallocation failed\n");
        exit(0);
    }
    editor->text[editor->currentLine] = temp;
    editor->symbolsPerLine = newCapacity;
}

void MakeMoreLines(text* editor, int line) {
    size_t newLinesCapacity = line + 1;
    char** temp = (char**)realloc(editor->text, newLinesCapacity * sizeof(char*));
    if (temp == NULL) {
        printf("Out of Memory\n");
        exit(EXIT_FAILURE);
    }
    editor->text = temp;
    for (size_t i = editor->lines; i < newLinesCapacity; i++) {
        editor->text[i] = (char*)malloc(editor->symbolsPerLine * sizeof(char));
        if (editor->text[i] == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        editor->text[i][0] = '\0';
    }
    editor->lines = newLinesCapacity;
}

void AppendToEnd(text* editor, char newText[])
{
    size_t currentLength = strlen(editor->text[editor->currentLine]);
    size_t newLength = strlen(newText);

    if (newLength > 0 && newText[newLength - 1] == '\n') {
        newText[newLength - 1] = '\0';
        newLength--;
    }

    if (currentLength + newLength >= editor->symbolsPerLine)
    {
        MakeLineLonger(editor, currentLength, newLength);
    }
    strcat(editor->text[editor->currentLine], newText);
}

void help()
{
    printf("There is an explanation for every command:\n"      
        "1 - append text symbols to the end\n"
        "2 - start the new line\n"
        "3 - use files to saving the information\n"
        "4 - print the current text to console\n"
        "5 - insert the text by line and symbol index\n"
        "6 - search\n"
        "7 - use files to loading the information\n"
        "8 - switch line\n"
        "9 - explain every command;\n"
        "10 - clear editor\n"
        "11 - clear console\n");
}

void Print(text* editor)
{
    for (int i = 0; i < editor->lines; i++) {  
        printf("%s\n", editor->text[i]);
    }
}


void StartNewLine(text* editor)
{
    if (editor->currentLine + 1 > editor->lines)
    {
        MakeMoreLines(editor, editor->currentLine);
    }
    editor->currentLine++;
}


void SaveToFile(text* editor, char fileName[])
{
    FILE* file;
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        ("This file cannot be opened or something bad happened to it!\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < editor->lines; i++)
    {
        fputs(editor->text[i], file);
        fputc('\n', file);
    }
    fclose(file);
    printf("Text has been saved successfully!\n");

}
int fileExists(char* fileName)
{
    return _access(fileName, 0) == 0;
}

void InitializeEditor(text* editor)
{
    editor->text = createArray(INITIAL_SIZE_ROWS, INITIAL_SIZE_COLUMNS);
    editor->currentLine = 0;
    editor->lines = INITIAL_SIZE_ROWS;
    editor->symbolsPerLine = INITIAL_SIZE_COLUMNS;
}

void CleanEditor(text* editor)
{
    if (editor == NULL || editor->text == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < editor->lines; i++)
    {
          free(editor->text[i]);
            //editor->text[i] = NULL;
    }

    free(editor->text);
    //editor->text = NULL;
    //editor->lines = 0;
}

void LoadFromFile(text* editor, char fileName[]) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    size_t bufferCapacity = INITIAL_SIZE_COLUMNS;
    char* buffer = (char*)malloc(bufferCapacity * sizeof(char));
    if (buffer == NULL) {
        printf("Memory allocation failed\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    InitializeEditor(editor);
    while (fgets(buffer, bufferCapacity, file) != NULL) {
        while (buffer[strlen(buffer) - 1] != '\n' && !feof(file)) {
            bufferCapacity *= 2;
            char* temp = (char*)realloc(buffer, bufferCapacity * sizeof(char));
            if (temp == NULL) {
                printf("Memory reallocation failed\n");
                free(buffer);
                fclose(file);
                exit(EXIT_FAILURE);
            }
            buffer = temp;
            fgets(buffer + strlen(buffer), bufferCapacity - strlen(buffer), file);
        }
        buffer[strcspn(buffer, "\n")] = '\0'; 

        if (editor->currentLine >= editor->lines) {
            MakeMoreLines(editor, editor->currentLine + 1);
        }
        AppendToEnd(editor, buffer);
        editor->currentLine++;
    }
    free(buffer);
    fclose(file);
    printf("File has been loaded successfully!\n");
}

void InsertAtIndex(text* editor, int line, int place, char newText[]) {

    if (line >= editor->lines) {
        MakeMoreLines(editor, line);
        editor->currentLine = line;
    }
    editor->currentLine = line;

    size_t currentLength = strlen(editor->text[line]);
    size_t newLength = strlen(newText);

    if (currentLength + newLength >= editor->symbolsPerLine) {
        MakeLineLonger(editor, currentLength, newLength);
    }

    if (place > currentLength) {
        for (size_t i = currentLength; i < place; i++) {
            editor->text[line][i] = ' ';
        }
        editor->text[line][place] = '\0';
        currentLength = place;
    }

    for (size_t i = currentLength; i >= place; i--) {
        editor->text[line][i + newLength] = editor->text[line][i];
    }

    for (int i = 0; i < newLength; i++) {
        editor->text[line][place + i] = newText[i];
    }

    editor->text[line][currentLength + newLength] = '\0';

}
void LineToModify(text* editor)
{
    int line;
    printf("Your cuurent line is: %d. Enter a line which you want to modify:\n", editor->currentLine);
    scanf("%d", &line); 
    while (getchar() != '\n');
    if (line >= editor->lines) 
    {
        MakeMoreLines(editor, line);
    }
    editor->currentLine = line ; 
    printf("Your line: %d\n", editor->currentLine);
}
void LPSArray(char pattern[], size_t patternLength, int* lps)
{
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < patternLength)
    {
        if (pattern[i] == pattern[len])
        {
            len++;
            lps[i] = len;
            i++;
        }
        else
        {
            if (len != 0)
            {
                len = lps[len - 1];
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}
void KMPSearch(text* editor, char* pattern)
{
    size_t M = strlen(pattern);
    if (M == 0) return;
    int* lps = (int*)malloc(M * sizeof(int));
    if (lps == NULL) {
        perror("Failed to allocate memory for LPS array");
        return;
    }    LPSArray(pattern, M, lps);


    for (int k = 0; k < editor->lines; k++)
    {
        size_t N = strlen(editor->text[k]);
        int i = 0;
        int j = 0;
        while ((N - i) >= (M - j))
        {
            if (pattern[j] == editor->text[k][i])
            {
                i++;
                j++;
            }
            if (j == M)
            {
                printf("Your pattern is found at location (line, place): %d %d\n", k, i - j);
                j = lps[j - 1];
            }
            else if (i < N && pattern[j] != editor->text[k][i])
            {
                if (j != 0)
                {
                    j = lps[j - 1];
                }
                else
                {
                    i++;
                }
            }
        }
    }
    free(lps);
}
void ProcessCommand(int command, text* editor)
{
    char newText[100];

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
        fgets(newText, sizeof(newText), stdin);
        AppendToEnd(editor, newText);
        break;
    case 2:
        printf("Starting new line...\n");
        StartNewLine(editor);
        printf("Current line:%d\n", editor->currentLine);
        break;
    case 3:
        printf("Enter a filename in which you want to store the text:\n");
        char fileName[50];
        fgets(fileName, sizeof(fileName), stdin);

        if (strlen(fileName) > 0)
        {
            fileName[strcspn(fileName, "\n")] = '\0';
        }
        SaveToFile(editor, fileName);
        break;
    case 4:
        Print(editor);
        break;
    case 5:
        printf("Choose line and index:\n");
        int line;
        int place;

        scanf("%d %d", &line, &place);
        if (line < 0 || place < 0)
        {
            printf("Line index and place index cannot be negative!\n");
            exit(EXIT_FAILURE);
        }
        while (getchar() != '\n');
        printf("Enter text to insert:\n");
        fgets(newText, sizeof(newText), stdin);
        newText[strcspn(newText, "\n")] = '\0';

        InsertAtIndex(editor, line, place, newText);

        break;
    case 6:
        printf("Enter a pattern:\n");
        char pattern[100];
        fgets(pattern, sizeof(pattern), stdin);
        pattern[strcspn(pattern, "\n")] = '\0';
        KMPSearch(editor, pattern);

        break;
    case 7:
        do {
            printf("Enter a filename from which you want to load data into the text editor:\n ");
            fgets(fileName, sizeof(fileName), stdin);

            if (strlen(fileName) > 0 && fileName[strlen(fileName) - 1] == '\n') {
                fileName[strlen(fileName) - 1] = '\0';
            }
            if (!fileExists(fileName))
            {
                printf("This file does not exist!\n");
            }
        } while (!fileExists(fileName));
        LoadFromFile(editor, fileName);
        break;
    case 8:
        LineToModify(editor);
        break;
    case 10:
        CleanEditor(editor);
        text newEditor;
        InitializeEditor(&newEditor);
        *editor = newEditor;
        break;
    case 11:
        system("cls");
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
    InitializeEditor(&editor);
    do {
        printf("Enter command: ");
        
        if (scanf("%d", &command) != 1)
        {
            printf("The command is not an integer!\n");
            while (getchar() != '\n'); 
            continue;
           
        }
        while (getchar() != '\n'); 
        ProcessCommand(command, &editor);
    } while (command != 0);
    CleanEditor(&editor);
    return 0;
}