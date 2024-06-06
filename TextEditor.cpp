#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INITIAL_SIZE 10

class NodeForClipboard
{
public:
    char* text;
    NodeForClipboard* next;

    NodeForClipboard(char* newText)
        : text(newText), next(NULL) {}

};

class Clipboard
{
    int size;
public:
    NodeForClipboard* top;
    Clipboard()
        : size(0), top(NULL) {}

    void DisplayContentsOfClipboard()
    {
        NodeForClipboard* current = NULL;
        current = top;
        if (current == NULL)
        {
            printf("Your clipboard is empty!\n");
            return;
        }
        printf("Contents of your clipboard:\n");
        while (current != NULL)
        {
            printf("%s\n", current->text);
            current = current->next;
        }

    }
    ~Clipboard()
    {
        NodeForClipboard* current = top;
        while (current != NULL)
        {
            NodeForClipboard* next = current->next;
            free(current->text);
            free(current);
            current = next;
        }
        top = NULL;
    }
    void PushToStack(char* text) {
        NodeForClipboard* node = (NodeForClipboard*)malloc(sizeof(NodeForClipboard));
        if (node == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        node->text = _strdup(text);

        node->next = top;
        top = node;
        size++;
    }
    NodeForClipboard* PopFromClipboardAndReturnLastValue()
    {
        NodeForClipboard* temp = top;
        if (temp == NULL)
        {
            return NULL;
        }
        top = top->next;
        size--;
        return temp;
    }

};

class TextEditor
{

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

    void AdjustSizeOfLine(int line, int length) {
        char* temp = (char*)realloc(text[line], (length + 1) * sizeof(char));
        if (temp == NULL) {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        text[line] = temp;
        text[line][length] = '\0';
    }
public:
    size_t lines;
    size_t symbolsPerLine;
    char** text;
    int currentLine;

    TextEditor() {
        lines = INITIAL_SIZE;
        currentLine = 0;
        symbolsPerLine = INITIAL_SIZE;
        text = createArray(INITIAL_SIZE, INITIAL_SIZE);

    }

    void Print()
    {
        for (int i = 0; i < lines; i++) {
            printf("%s\n", text[i]);
        }
    }
    void MakeLineLonger(size_t currentLength, size_t newLength)
    {
        size_t newCapacity = currentLength + newLength + 1;
        char* temp = (char*)realloc(text[currentLine], newCapacity * sizeof(char));
        if (temp == NULL) {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        text[currentLine] = temp;
        symbolsPerLine = newCapacity;
    }
    void MakeMoreLines(int line) {
        size_t newLinesCapacity = line + 1;
        char** temp = (char**)realloc(text, newLinesCapacity * sizeof(char*));
        if (temp == NULL)
        {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        text = temp;
        for (size_t i = lines; i < newLinesCapacity; i++) {
            text[i] = (char*)malloc(symbolsPerLine * sizeof(char));
            if (text[i] == NULL)
            {
                printf("Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
            text[i][0] = '\0';
        }
        lines = newLinesCapacity;
    }
    void AppendToEnd(char newText[])
    {
        size_t currentLength = strlen(text[currentLine]);
        size_t newLength = strlen(newText);

        if (currentLength + newLength >= symbolsPerLine)
        {
            MakeLineLonger(currentLength, newLength);
        }
        strcat(text[currentLine], newText);
    }
    void InsertAtIndex(int line, int place, char* newText) {

        if (line >= lines) {
            MakeMoreLines(line);
            currentLine = line;
        }
        currentLine = line;

        size_t currentLength = strlen(text[line]);
        size_t newLength = strlen(newText);

        if (currentLength + newLength >= symbolsPerLine) {
            MakeLineLonger(currentLength, newLength);
        }

        if (place > currentLength) {
            for (size_t i = currentLength; i < place; i++) {
                text[line][i] = ' ';
            }
            text[line][place] = '\0';
            currentLength = place;
        }

        for (int i = currentLength; i >= place; i--) {
            text[line][i + newLength] = text[line][i];
        }

        for (int i = 0; i < newLength; i++) {
            text[line][place + i] = newText[i];
        }

        text[line][currentLength + newLength] = '\0';

    }

    void DeleteSymbols(int line, int index, int number, int currentLength)
    {
        for (int i = index; i < currentLength; i++)
        {
            text[line][i] = text[line][i + number];
        }
        currentLength -= number;
        AdjustSizeOfLine(line, currentLength);
    }
    void StartNewLine()
    {
        printf("You current line: %d. Starting new line...\n", currentLine);
        if (currentLine + 1 > lines)
        {
            MakeMoreLines(currentLine);
        }
        currentLine++;
        printf("Your new line: %d\n", currentLine);

    }
    void InsertWithReplacement(int line, int index, char* newText)
    {
        size_t currentLength = strlen(text[line]);
        size_t newTextLength = strlen(newText);

        if (index + newTextLength > currentLength)
        {
            printf("You cannot replace more symbols than present!\n");
            return;
        }

        for (int i = 0; i < newTextLength; i++) {
            text[line][index + i] = newText[i];
        }

    }
    void CutOrCopy(Clipboard* clipboard, bool needCut) {
        int line, index, number;
        size_t currentLength;
        if (ChooseLineIndexNumber(&line, &index, &number, &currentLength)) {
            return;
        }
        char* textToBuffer = (char*)malloc((number + 1) * sizeof(char));
        if (textToBuffer == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        strncpy(textToBuffer, &text[line][index], number);
        textToBuffer[number] = '\0';
        clipboard->PushToStack(textToBuffer);
        if (needCut)
        {
            DeleteSymbols(line, index, number, currentLength);
        }
        free(textToBuffer);
    }
    void Paste(Clipboard* clipboard)
    {
        if (clipboard->top == NULL)
        {
            printf("There is nothing to paste, your clipboard is empty!\n");
            return;
        }
        int line, index;
        size_t currentLength;
        if (!ChooseLineIndex(&line, &index)) {
            return;
        }
        NodeForClipboard* node = clipboard->PopFromClipboardAndReturnLastValue();
        InsertAtIndex(line, index, node->text);

    }
    ~TextEditor()
    {
        if (text == NULL)
            exit(EXIT_FAILURE);
        for (int i = 0; i < lines; i++)
        {
            free(text[i]);
            text[i] = NULL;
        }
        free(text);
        text = NULL;
        printf("Editor has been cleand!\n");
    }

    int ChooseLineIndexNumber(int* line, int* index, int* number, size_t* currentLength)
    {
        printf("Choose line, index and number of symbols:\n");
        if (scanf("%d %d %d", line, index, number) != 3 || *line < 0 || *index < 0 || *number < 0) {
            printf("Invalid input!\n");
            while (getchar() != '\n');
            return 0;
        }
        *currentLength = strlen(text[*line]);
        printf("There are %d lines and, on your chosen line there are %d symbols\n", lines, *currentLength);

        if (*index >= *currentLength || *number > *currentLength - *index || *currentLength == 0 || *line >= lines) {
            printf("Something is wrong in your numbers! Look higher!\n");
            return 0;
        }
        return 1;
    }
    int ChooseLineIndex(int* line, int* index) {
        printf("Choose line and index:\n");
        if (scanf("%d %d", line, index) != 2 || *line < 0 || *index < 0) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            return 0;
        }
        while (getchar() != '\n');
        return 1;
    }
    void Clear() {
        if (text == NULL)
            exit(EXIT_FAILURE);
        for (int i = 0; i < lines; i++)
        {
            free(text[i]);
            text[i] = NULL;
        }
        free(text);
        text = NULL;
        printf("Editor has been cleand!\n");
        lines = INITIAL_SIZE; 
        symbolsPerLine = INITIAL_SIZE;// Set the number of lines for the new editor
        text = createArray(INITIAL_SIZE, INITIAL_SIZE);
        currentLine = 0;
       
    }
};



class NodeHistory
{
public:
    TextEditor* textToSave;
    NodeHistory* next;

    NodeHistory(TextEditor* newText)
        : next(NULL), textToSave(newText) {};
};

class HistoryStack
{
public:
    NodeHistory* top;
    int size;
    HistoryStack()
        : size(0), top(NULL) {}

    void PushToStack(TextEditor* editor) {
        NodeHistory* node = (NodeHistory*)malloc(sizeof(NodeHistory));
        if (node == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        TextEditor* editorCopy = (TextEditor*)malloc(sizeof(TextEditor));
        if (editorCopy == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        *editorCopy = *editor;

        editorCopy->text = (char**)malloc(editor->lines * sizeof(char*));
        if (editorCopy->text == NULL) {
            printf("Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        for (int i = 0; i < editor->lines; i++) {
            editorCopy->text[i] = (char*)malloc(editor->symbolsPerLine * sizeof(char));
            if (editorCopy->text[i] == NULL) {
                printf("Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            strcpy(editorCopy->text[i], editor->text[i]);
        }

        node->textToSave = editorCopy;
        node->next = top;
        top = node;
        size++;

    }
    void PopFromStack() {
        if (top == NULL) {
            return;
        }
        NodeHistory* temp = top;
        top = top->next;
        size--;
    }
    void DisplayContentsOfStack() {
        NodeHistory* current = top;
        if (current == NULL) {
            printf("The stack is empty!\n");
            return;
        }
        printf("Contents of the stack:\n");
        while (current != NULL) {
            if (current->textToSave != NULL) {
                current->textToSave->Print();
            }
            else {
                printf("Empty text\n");
            }
            current = current->next;
        }
    }
    ~HistoryStack() {
        NodeHistory* current = top;
        while (current != NULL) {
            NodeHistory* next = current->next;
            free(current);
            current = next;
        }
        top = NULL;
        size = 0;
    }

};

class UserInput
{
    void MakeUserArrayLonger() {
        size_t newCapacity = capacity * 2;
        char* temp = (char*)realloc(text, newCapacity * sizeof(char));
        if (temp == NULL)
        {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        text = temp;
        capacity = newCapacity;
    }
    void AdjustNeededSizeForUserArray(size_t realLength)
    {
        char* temp = (char*)realloc(text, (realLength + 1) * sizeof(char));
        if (temp == NULL)
        {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        text = temp;
        capacity = realLength + 1;
    }
public:
    char* text;
    size_t capacity;
    char* CreateArrayForUserInput() {
        char* text = (char*)malloc(INITIAL_SIZE * sizeof(char));
        if (text == NULL)
        {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        text[0] = '\0';
        return text;
    }
    UserInput()
    {
        text = CreateArrayForUserInput();
        capacity = INITIAL_SIZE;
    }
    ~UserInput()
    {
        if (text != NULL)
        {
            free(text);
            text = NULL; 
        }
    }
    void TakeUserInput()
    {
        size_t currentLength = 0;
        while (fgets(text + currentLength, capacity - currentLength, stdin))
        {
            currentLength += strlen(text + currentLength);
            if (text[currentLength - 1] == '\n')
            {
                break;
            }
            MakeUserArrayLonger();
        }
        text[strcspn(text, "\n")] = '\0';
        AdjustNeededSizeForUserArray(currentLength);
    }


};

class Command
{
    static void SaveToFile(TextEditor* editor, char fileName[])
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
    static void LoadFromFile(TextEditor* editor, char fileName[]) {
        FILE* file = fopen(fileName, "r");
        if (file == NULL) {
            printf("Error opening file. It looks like it does not exist!\n");
            return;
        }

        size_t bufferCapacity = INITIAL_SIZE;
        char* buffer = (char*)malloc(bufferCapacity * sizeof(char));
        if (buffer == NULL) {
            printf("Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        
        
        editor->Clear(); 
        while (fgets(buffer, bufferCapacity, file) != NULL) {
            while (buffer[strlen(buffer) - 1] != '\n' && !feof(file)) {
                bufferCapacity += bufferCapacity;
                char* temp = (char*)realloc(buffer, bufferCapacity * sizeof(char));
                if (temp == NULL) {
                    printf("Memory reallocation failed\n");
                    exit(EXIT_FAILURE);
                }
                buffer = temp;
                fgets(buffer + strlen(buffer), bufferCapacity - strlen(buffer), file);
            }
            buffer[strcspn(buffer, "\n")] = '\0';

            if (editor->currentLine >= editor->lines) {
                editor->MakeMoreLines(editor->currentLine + 1);
            }
            editor->AppendToEnd(buffer);
            editor->currentLine++;
        }
        free(buffer);
        fclose(file);
        printf("File has been loaded successfully!\n");

    }
    static void LineToModify(TextEditor* editor)
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
            editor->MakeMoreLines(line);
        }
        editor->currentLine = line;
        printf("Your line: %d\n", editor->currentLine);
    }
    static void help()
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
            "13 - undo\n"
            "14 - insert with replacement\n"
            "15 - cut\n"
            "16 - copy\n"
            "17 - paste\n"
            "18 - undo\n"
            "19 - redo\n"
            "20 - display contents of your clipboard\n");

    }
    static void LPSArray(char pattern[], size_t patternLength, int* lps)
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
    static void KMPSearch(TextEditor* editor, char* pattern)
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
    static void DoCommand1(TextEditor* editor, UserInput* userInput)
    {
        printf("Enter text to append:\n");
        userInput->TakeUserInput();
        editor->AppendToEnd(userInput->text);
    }

    static void DoCommand3(TextEditor* editor, UserInput* userInput)
    {
        do {
            printf("Enter a filename in which you want to store the text:\n");
            userInput->TakeUserInput();
        } while (strlen(userInput->text) == 0);
        SaveToFile(editor, userInput->text);
    }

    static void DoCommand5(TextEditor* editor, UserInput* userInput)
    {
        int line, index;
        if (!editor->ChooseLineIndex(&line, &index)) {
            return;
        }
        printf("Enter text to insert:\n");
        userInput->TakeUserInput();
        editor->InsertAtIndex(line, index, userInput->text);
    }

    static void DoCommand6(TextEditor* editor, UserInput* userInput)
    {
        printf("Enter a pattern:\n");
        userInput->TakeUserInput();
        KMPSearch(editor, userInput->text);
    }
    static void DoCommand7(TextEditor* editor, UserInput* userInput)
    {
        printf("Enter a filename from which you want to load data into the text editor:\n");
        userInput->TakeUserInput();
        LoadFromFile(editor, userInput->text);
    }

    static void DoCommand12(TextEditor* editor) {
        int line, index, number;
        size_t currentLength;

        if (!editor->ChooseLineIndexNumber(&line, &index, &number, &currentLength)) {
            return;
        }

        editor->DeleteSymbols(line, index, number, currentLength);
    }
    static void DoCommand14(TextEditor* editor, UserInput* userInput)
    {
        int line, index;
        if (!editor->ChooseLineIndex(&line, &index))
        {
            return;
        }
        int currentLength = strlen(editor->text[line]);
        if (line > editor->lines || index > currentLength || currentLength == 0)
        {
            printf("Something is wrong in your numbers! \n");
            return;
        }
        printf("Enter text to insert:\n");
        userInput->TakeUserInput();
        editor->InsertWithReplacement(line, index, userInput->text);
    }

    static void Undo(TextEditor* editor, HistoryStack* stackRedo, HistoryStack* stackUndo) {
        if (stackUndo->size > 1) {
            stackUndo->PopFromStack();
            stackRedo->PushToStack(editor);
            *editor = *(stackUndo->top->textToSave);

        }
        else {
            printf("Nothing more to undo.\n");
        }
    }

    static void Redo(TextEditor* editor, HistoryStack* stackRedo, HistoryStack* stackUndo) {
        if (stackRedo->size > 0) {
            *editor = *(stackRedo->top->textToSave);
            stackUndo->PushToStack(editor);
            stackRedo->PopFromStack();
        }
        else {
            printf("Nothing more to redo.\n");
        }
    }


public:
    int command;

    Command(int newCommand)
        :command(newCommand) {}

    static void ProcessCommand(int command, TextEditor* editor, Clipboard* clipboard, HistoryStack* stackRedo, HistoryStack* stackUndo) {
        UserInput userInput;
        bool needCut = false;

        switch (command) {
        case 9:
            help();
            break;
        case 0:
            printf("Exiting text editor. Bye!\n");
            break;
        case 1:
            DoCommand1(editor, &userInput);
            stackUndo->PushToStack(editor);
            break;
        case 2:
            editor->StartNewLine();
            stackUndo->PushToStack(editor);
            break;
        case 3:
            DoCommand3(editor, &userInput);
            break;
        case 4:
            editor->Print();
            break;
        case 5:
            DoCommand5(editor, &userInput);
            stackUndo->PushToStack(editor);
            break;
        case 6:
            DoCommand6(editor, &userInput);
            break;
        case 7:
            DoCommand7(editor, &userInput);
            break;
        case 8:
            LineToModify(editor);
            stackUndo->PushToStack(editor);
            break;
        case 10:
            editor->Clear();
            stackUndo->PushToStack(editor);
            break;
        case 11:
            system("cls");
            break;
        case 12:
            DoCommand12(editor);
            stackUndo->PushToStack(editor);
            break;
        case 13:
            break;
        case 14:
            DoCommand14(editor, &userInput);
            stackUndo->PushToStack(editor);
            break;
        case 15:
            needCut = true;
            editor->CutOrCopy(clipboard, needCut);
            stackUndo->PushToStack(editor);
            break;
        case 16:
            editor->CutOrCopy(clipboard, needCut);
            break;
        case 17:
            editor->Paste(clipboard);
            stackUndo->PushToStack(editor);
            break;
        case 18:
            Undo(editor, stackRedo, stackUndo);
            break;

        case 19:
            Redo(editor, stackRedo, stackUndo);
            break;

        case 20:
            clipboard->DisplayContentsOfClipboard();
            break;
        case 21:
            stackRedo->DisplayContentsOfStack();
            break;
        case 22:
            stackUndo->DisplayContentsOfStack();
            break;
        default:
            printf("The command is not implemented. Type '9' for help.\n");
        }
        /*if (stackUndo->size > 5)
        {

            InitializeStackForSaving(stackUndo);
        }*/


    }

};

int main() {
    printf("Hello! Welcome to the Text Editor! Enter '9' to see the available list of commands :)\n");
    Clipboard clipboard;
    HistoryStack stackUndo;
    HistoryStack stackRedo;
    TextEditor editor;
    int command;
    do {
        printf("Enter command: ");
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        Command commandUser(command);
        commandUser.ProcessCommand(commandUser.command, &editor, &clipboard, &stackRedo, &stackUndo);
    } while (command != 0);
    return 0;
}