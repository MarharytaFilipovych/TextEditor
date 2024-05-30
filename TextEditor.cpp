#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_SIZE_ROWS 10
#define INITIAL_SIZE_COLUMNS 10
#define INITIAL_SIZE_OF_SYMBOLS_FOR_USER 10

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
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++)
    {
        array[i] = (char*)malloc(columns * sizeof(char));
        if (array[i] == NULL)
        {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
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
        exit(EXIT_FAILURE);
    }
    editor->text[editor->currentLine] = temp;
    editor->symbolsPerLine = newCapacity;
}

void MakeMoreLines(text* editor, int line) {
    size_t newLinesCapacity = line + 1;
    char** temp = (char**)realloc(editor->text, newLinesCapacity * sizeof(char*));
    if (temp == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    editor->text = temp;
    for (size_t i = editor->lines; i < newLinesCapacity; i++) {
        editor->text[i] = (char*)malloc(editor->symbolsPerLine * sizeof(char));
        if (editor->text[i] == NULL)
        {
            printf("Memory reallocation failed\n");
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
        "11 - clear console\n"
        "12 - delete some data\n"
        "13 - undo\n");
}

void Print(text* editor)
{
    for (int i = 0; i < editor->lines; i++) {  
        printf("%s\n", editor->text[i]);
    }
}


void StartNewLine(text* editor)
{
    printf("You current line: %d. Starting new line...\n", editor->currentLine);
    if (editor->currentLine + 1 > editor->lines)
    {
        MakeMoreLines(editor, editor->currentLine);
    }
    editor->currentLine++;
    printf("Your new line: %d\n", editor->currentLine);

}


void SaveToFile(text* editor, char fileName[])
{
    FILE* file;
    file = fopen(fileName, "w");
    if (file == NULL)
    {
        ("This file cannot be opened or something bad happened to it!\n");
        return;
    }
    for (int i = 0; i < editor->lines; i++)
    {
        fputs(editor->text[i], file);
        fputc('\n', file);
    }
    fclose(file);
    printf("Text has been saved successfully!\n");

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
          editor->text[i] = NULL;
    }
    free(editor->text);
    editor->text = NULL;
    printf("Editor has been cleand!\n");
    }

void LoadFromFile(text* editor, char fileName[]) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file. It looks like it does not exist!\n");
        return;
    }

    size_t bufferCapacity = INITIAL_SIZE_COLUMNS;
    char* buffer = (char*)malloc(bufferCapacity * sizeof(char));
    if (buffer == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    InitializeEditor(editor);
    while (fgets(buffer, bufferCapacity, file) != NULL) {
        while (buffer[strlen(buffer) - 1] != '\n' && !feof(file)) {
            bufferCapacity += bufferCapacity;
            char* temp = (char*)realloc(buffer, bufferCapacity * sizeof(char));
            if (temp == NULL)
            {
                printf("Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }            buffer = temp;
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

void InsertAtIndex(text* editor, int line, int place, char* newText) {

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

    for (int i = currentLength; i >= place; i--) {
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
    if (scanf("%d", &line) != 1) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    };

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

    bool foundPattern = false;
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
                foundPattern = true;
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
    if (!foundPattern)
    {
        printf("No pattern like yours can be found!\n");
    }
    free(lps);
}
typedef struct {
    char* text;
    size_t capacity;
} arrayForUserInput;

char* CreateArrayForUserInput(arrayForUserInput* userInput) {
    char* text = (char*)malloc(INITIAL_SIZE_OF_SYMBOLS_FOR_USER * sizeof(char));
    if (text == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    text[0] = '\0';
    userInput->text = text;
    userInput->capacity = INITIAL_SIZE_OF_SYMBOLS_FOR_USER;
    return text;
}

void MakeUserArrayLonger(arrayForUserInput* userInput) {
    size_t newCapacity = userInput->capacity * 2;
    char* temp = (char*)realloc(userInput->text, newCapacity * sizeof(char));
    if (temp == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }   
    userInput->text = temp;
    userInput->capacity = newCapacity;
}

void FreeUserInput(arrayForUserInput* userInput)
{
    free(userInput->text);
    userInput->text = NULL;
}

void AdjustNeededSizeForUserArray(arrayForUserInput* userInput, size_t realLength)
{
    char* temp = (char*)realloc(userInput->text, (realLength + 1) * sizeof(char));
    if (temp == NULL)
    {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    userInput->text = temp;
    userInput->capacity = realLength + 1;
}

void TakeUserInput(arrayForUserInput* userInput)
{
    size_t currentLength = 0;
    while (fgets(userInput->text + currentLength, userInput->capacity - currentLength, stdin))
    {
        currentLength += strlen(userInput->text + currentLength);
        if (userInput->text[currentLength - 1] == '\n')
        {
            break;
        }
        MakeUserArrayLonger(userInput);
    }
    userInput->text[strcspn(userInput->text, "\n")] = '\0';
    AdjustNeededSizeForUserArray(userInput, currentLength);
}

void DoCommand1(text* editor, arrayForUserInput* userInput)
{
    printf("Enter text to append:\n");
    TakeUserInput(userInput);
    AppendToEnd(editor, userInput->text);
    FreeUserInput(userInput);
}

void DoCommand3(text* editor, arrayForUserInput* userInput)
{
    do {
        printf("Enter a filename in which you want to store the text:\n");
        TakeUserInput(userInput);
    } while (strlen(userInput->text) == 0);
    SaveToFile(editor, userInput->text);
    FreeUserInput(userInput);
}

void DoCommand5(text* editor, arrayForUserInput* userInput)
{
    printf("Choose line and index:\n");
    int place, line;
    if (scanf("%d %d", &line, &place) != 2 || line < 0 || place < 0) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');
    printf("Enter text to insert:\n");
    TakeUserInput(userInput);
    InsertAtIndex(editor, line, place, userInput->text);
    FreeUserInput(userInput);
}

void DoCommand6(text* editor, arrayForUserInput* userInput)
{
    printf("Enter a pattern:\n");
    TakeUserInput(userInput);
    KMPSearch(editor, userInput->text);
    FreeUserInput(userInput);
}
void DoCommand7(text* editor, arrayForUserInput* userInput)
{
    printf("Enter a filename from which you want to load data into the text editor:\n");
    TakeUserInput(userInput);
    LoadFromFile(editor, userInput->text);
    FreeUserInput(userInput);
}
void AdjustSizeOfLine(text* editor, int line, int length) {
    char* temp = (char*)realloc(editor->text[line], (length + 1) * sizeof(char));
    if (temp == NULL) {
        printf("Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    editor->text[line] = temp; 
    editor->text[line][length] = '\0'; 
}
void DeleteSymbols(text* editor, int line, int index, int number, int currentLength)
{
    for (int i = index; i < currentLength; i++)
    {
        editor->text[line][i] = editor->text[line][i + number];
    }
    currentLength -= number;
    AdjustSizeOfLine(editor, line, currentLength);
}
void DoCommand12(text* editor) {
    
    printf("Choose line, index and number of symbols:\n");
    int line, index, number;
    if (scanf("%d %d %d", &line, &index, &number) != 3 || line < 0 || index < 0 || number < 0)
    {
        printf("Invalid input!\n");
        while (getchar() != '\n');
        return;
    }
    int currentLength = strlen(editor->text[line]);
    printf("There are %d lines and, on your chosen line there are %d symbols\n", editor->lines, currentLength);
    if (number > currentLength || line > editor->lines || index > currentLength || currentLength == 0)
    {   
        printf("Something is wrong in your numbers! Look higher!\n");
        return;
    }
    DeleteSymbols(editor, line, index, number, currentLength);
}
void InsertWithReplacement(text* editor, int line, int index, char* newText)
{
    size_t currentLength = strlen(editor->text[line]);
    size_t newTextLength = strlen(newText);
    printf("%d - current length", currentLength);
    printf("%d - new", newTextLength);
    if (index + newTextLength > currentLength)
    {
        printf("You cannot replace more symbols than present!\n");
        return;
    }
    
    for (int i = 0; i < newTextLength; i++) {
         editor->text[line][index + i] = newText[i];
    }

   }
void DoCommand14(text* editor, arrayForUserInput* userInput)
{
    printf("Choose line and index:\n");
    int index, line;
    if (scanf("%d %d", &line, &index) != 2 || line < 0 || index < 0) {
        printf("Invalid input\n");
        while (getchar() != '\n');
        return;
    }
    int currentLength = strlen(editor->text[line]);
    if (line > editor->lines || index > currentLength || currentLength == 0)
    {
        printf("Something is wrong in your numbers! \n");
        return;
    }    
    while (getchar() != '\n');
    printf("Enter text to insert:\n");
    TakeUserInput(userInput);
    InsertWithReplacement(editor, line, index, userInput->text);
    FreeUserInput(userInput);
}
void ProcessCommand(int command, text* editor) {
    arrayForUserInput userInput;
    userInput.text = CreateArrayForUserInput(&userInput);
    switch (command) {
    case 9:
        help();
        break;
    case 0:
        printf("Exiting text editor. Bye!\n");
        break;
    case 1:
        DoCommand1(editor, &userInput);
        break;
    case 2:
        StartNewLine(editor);
        break;
    case 3:
        DoCommand3(editor, &userInput);
        break;
    case 4:
        Print(editor);
        break;
    case 5:
        DoCommand5(editor, &userInput);
        break;
    case 6:
        DoCommand6(editor, &userInput);
        break;
    case 7:
        DoCommand7(editor, &userInput);
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
    case 12:
        DoCommand12(editor);
        break;
    case 13:
        break;
    case 14:
        DoCommand14(editor, &userInput);
        break;
    default:
        printf("The command is not implemented. Type '9' for help.\n");
    }
    if (userInput.text != NULL)
    {
        FreeUserInput(&userInput);
    }

}

int main() {
    printf("Hello! Welcome to the Text Editor! Enter '9' to see the available list of commands :)\n");
    text editor;
    int command;
    InitializeEditor(&editor);
    do {
        printf("Enter command: ");
        if (scanf("%d", &command) != 1) {

            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        ProcessCommand(command, &editor);
    } while (command != 0);
    CleanEditor(&editor);
    return 0;
}