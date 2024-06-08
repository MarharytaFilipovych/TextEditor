#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#define INITIAL_SIZE 10

class CommandHistory
{
public:
    int command;
    int line;
    int index;
    int number;
    char* text;
    char* pText;
    CommandHistory(int userCommand, int userLine, int userIndex, size_t userNumber, char* userText, char* previousText)
        : command(userCommand), line(userLine), index(userIndex), number(userNumber), pText(NULL) {
        if (userText != NULL) {
            text = new char[strlen(userText) + 1];
            strcpy(text, userText);
        }
        else {
            text = NULL;
        }
        if (previousText != NULL) {
            pText = new char[strlen(previousText) + 1];
            strcpy(pText, previousText);
        }
        else {
            pText = NULL;
        }
    }
    void Print()
    {
        std::cout << "command: " << command
            << ", line: " << line
            << ", index: " << index
            << ", number of symbols: " << number
            << ", new text: " << (text ? text : "-")
            << ", previous text: " << (pText ? pText : "-") << std::endl;
    }
    ~CommandHistory() {
        delete[] text;
        delete[] pText;
    }
};

class NodeHistory
{
public:
    CommandHistory commandInfo;
    NodeHistory* next;

    NodeHistory(int userCommand, int userLine, int userIndex, size_t userNumber, char* userText, char* previousText)
        : next(NULL), commandInfo(userCommand, userLine, userIndex, userNumber, userText, previousText) {};

};

class HistoryStack
{
public:
    NodeHistory* top;
    size_t size;
    HistoryStack()
        : size(0), top(NULL) {}

    void PushToStack(CommandHistory* commandInfo) {
        NodeHistory* node = new NodeHistory(commandInfo->command, commandInfo->line, commandInfo->index, commandInfo->number, commandInfo->text, commandInfo->pText);
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
        delete temp; // ?
    }
    void DisplayContentsOfStack() {
        NodeHistory* current = top;
        if (current == NULL) {
            std::cout << "The stack is empty!" << std::endl;
            return;
        }
        std::cout << "Contents of the stack:" << std::endl;
        while (current != NULL) {
            current->commandInfo.Print();
            current = current->next;
        }
    }
    ~HistoryStack() {
        NodeHistory* current = top;
        while (current != NULL) {
            NodeHistory* next = current->next;
            delete current;
            current = next;

        }
        top = NULL;
        size = 0;
    }

};
class NodeForClipboard
{
public:
    char* text;
    NodeForClipboard* next;
    NodeForClipboard() : text(NULL), next(NULL) {}

    NodeForClipboard(char* newText)
        : text(newText), next(NULL) {}

};

class Clipboard
{
    size_t size;
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
            std::cout << "Your clipboard is empty!" << std::endl;
            return;
        }
        std::cout << "Contents of your clipboard:" << std::endl;
        while (current != NULL)
        {
            std::cout << current->text << std::endl;
            current = current->next;
        }

    }
    ~Clipboard()
    {
        NodeForClipboard* current = top;
        while (current != NULL)
        {
            NodeForClipboard* next = current->next;
            delete[] current->text;
            delete current;
            current = next;
        }
        top = NULL;
    }
    void PushToStack(char* text) {
        NodeForClipboard* node = new NodeForClipboard;
        if (node == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
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
        char** array = new char*[rows];
        if (array == NULL)
        {
            std::cerr << "Memory allocation failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < rows; i++)
        {
            array[i] = new char[columns];
            if (array[i] == NULL)
            {
                std::cerr << "Memory allocation failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            array[i][0] = '\0';
        }
        return array;
    }

    void AdjustSizeOfLine(int line, int length) {
        char* temp = (char*)realloc(text[line], (length + 1) * sizeof(char));
        if (temp == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        text[line] = temp;
        text[line][length] = '\0';

    }

    void MakeSizeOfEditorSmaller(int line) {
        if (line < 0 || line >= lines) {
            printf("Invalid line number\n");
            return;
        }
        size_t newLinesCapacity = lines - line;
        if (newLinesCapacity > lines) {
            printf("Invalid newLinesCapacity calculated\n");
            return;
        }
        for (size_t i = lines; i > newLinesCapacity; i--) {
            if (text[i - 1] != NULL) {
                free(text[i - 1]);
                text[i - 1] = NULL;
            }
        }
        char** temp = (char**)realloc(text, newLinesCapacity * sizeof(char*));
        if (temp == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        text = temp;
        lines = newLinesCapacity;
    }
    char* SavePreviousText(int line, int index, size_t number)
    {
        char* previousText = new char[number + 1];
        strncpy(previousText, &text[line][index], number);
        previousText[number] = '\0';
        return previousText;
    }
public:
    size_t lines;
    size_t symbolsPerLine;
    char** text;
    int currentLine;
    HistoryStack stackRedo;
    HistoryStack stackUndo;

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
            std::cerr << "Memory allocation failed" << std::endl;
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
            std::cerr << "Memory allocation failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        text = temp;
        for (size_t i = lines; i < newLinesCapacity; i++) {
            text[i] = (char*)malloc(symbolsPerLine * sizeof(char));
            if (text[i] == NULL)
            {
                std::cerr << "Memory allocation failed" << std::endl;
                exit(EXIT_FAILURE);
            }
            text[i][0] = '\0';
        }
        lines = newLinesCapacity;
    }
    void AppendToEnd(char newText[], bool user)
    {
        size_t currentLength = strlen(text[currentLine]);
        size_t newLength = strlen(newText);

        if (currentLength + newLength >= symbolsPerLine)
        {
            MakeLineLonger(currentLength, newLength);
        }
        strcat(text[currentLine], newText);
        if (user)
        {
            CommandHistory commandInfo(1, currentLine, currentLength, newLength, newText, NULL);
            stackUndo.PushToStack(&commandInfo);
        }
    }
    void InsertAtIndex(int line, int place, char* newText, bool user) {

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
        if (user)
        {
            CommandHistory commandInfo(5, line, place, newLength, newText, NULL);
            stackUndo.PushToStack(&commandInfo);
        }
    }

    void DeleteSymbols(int line, int index, size_t number, int currentLength, bool user)
    {
        char* previousText = SavePreviousText(line, index, number);
        for (int i = index; i < currentLength; i++)
        {
            text[line][i] = text[line][i + number];
        }
        currentLength -= number;
        AdjustSizeOfLine(line, currentLength);

        if (user)
        {
            CommandHistory commandInfo(12, line, index, number, previousText, NULL);
            stackUndo.PushToStack(&commandInfo);
        }
    }
    void StartNewLine(bool user)
    {
        printf("You current line: %d. Starting new line...\n", currentLine);
        if (currentLine + 1 > lines)
        {
            MakeMoreLines(currentLine);
        }
        currentLine++;
        printf("Your new line: %d\n", currentLine);
        if (user)
        {
            CommandHistory commandInfo(2, currentLine, 0, 0, NULL, NULL);
            stackUndo.PushToStack(&commandInfo);
        }
    }
    void DeleteLine(int line)
    {
        if (currentLine == 0)
        {
            printf("nothing to delete!\n");
            return;
        }
        MakeSizeOfEditorSmaller(line);
        currentLine--;
    }
    void CutOrCopy(Clipboard* clipboard, bool needCut, bool user, int line, int number, int index, size_t currentLength) 
    {       
        char* textToBuffer = SavePreviousText(line, index, number);      
        clipboard->PushToStack(textToBuffer);

        if (needCut)
        {
            DeleteSymbols(line, index, number, currentLength, false);
            if (user)
            {
                CommandHistory commandInfo(15, line, index, number, textToBuffer, NULL);
                stackUndo.PushToStack(&commandInfo);
            }
        }
        delete[] textToBuffer;
    }

    void InsertWithReplacement(int line, int index, char* newText, bool user)
    {
        size_t currentLength = strlen(text[line]);
        size_t newTextLength = strlen(newText);

        if (index + newTextLength > currentLength)
        {
            printf("You cannot replace more symbols than present!\n");
            return;
        }
        char* previousText = SavePreviousText(line, index, newTextLength);       
        for (int i = 0; i < newTextLength; i++) {
            text[line][index + i] = newText[i];
        }
        if (user)
        {
            CommandHistory commandInfo(14, line, index, newTextLength, newText, previousText);
            stackUndo.PushToStack(&commandInfo);
        }
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
        InsertAtIndex(line, index, node->text, true);
       
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
        std::cout << "There are " << lines << " lines and, on your chosen line there are " << *currentLength << " symbols" << std::endl;

        if (*index >= *currentLength || *number > *currentLength - *index || *currentLength == 0 || *line >= lines) {
            std::cout << "Something is wrong in your numbers! Look higher!" << std::endl;
            return 0;
        }
        return 1;
    }
    int ChooseLineIndex(int* line, int* index) {
        std::cout << "Choose line and index:" << std::endl;
        if (scanf("%d %d", line, index) != 2 || *line < 0 || *index < 0) {
            std::cout << "Invalid input" << std::endl;

            while (getchar() != '\n');
            return 0;
        }
        while (getchar() != '\n');
        return 1;
    }
    void Clear() {
        lines, symbolsPerLine = INITIAL_SIZE; 
        text = createArray(INITIAL_SIZE, INITIAL_SIZE);
        currentLine = 0;       
    }
};

class UserInput
{
    void MakeUserArrayLonger() {
        size_t newCapacity = capacity * 2;
        char* temp = (char*)realloc(text, newCapacity * sizeof(char));
        if (temp == NULL)
        {
            std::cerr << "Memory allocation failed" << std::endl;
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
            std::cerr << "Memory allocation failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        text = temp;
        capacity = realLength + 1;
    }
public:
    char* text;
    size_t capacity;
    char* CreateArrayForUserInput() {
        char* text = new char[INITIAL_SIZE];
        if (text == NULL)
        {
            std::cerr << "Memory allocation failed" << std::endl;
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
           delete[] text;
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
            std::cout << "This file cannot be opened or something bad happened to it!" << std::endl;
            return;
        }
        for (int i = 0; i < editor->lines; i++)
        {
            fputs(editor->text[i], file);
            fputc('\n', file);
        }
        fclose(file);
        std::cout << "Text has been saved successfully!" << std::endl;

    }
    static void LoadFromFile(TextEditor* editor, char fileName[]) {
        FILE* file = fopen(fileName, "r");
        if (file == NULL) {
            std::cout << "Error opening file. It looks like it does not exist!" << std::endl;
            return;
        }

        size_t bufferCapacity = INITIAL_SIZE;
        char* buffer = (char*)malloc(sizeof(char)* INITIAL_SIZE);
        if (buffer == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        
        editor->Clear(); 
        while (fgets(buffer, bufferCapacity, file) != NULL) {
            while (buffer[strlen(buffer) - 1] != '\n' && !feof(file)) {
                bufferCapacity += bufferCapacity;
                char* temp = (char*)realloc(buffer, bufferCapacity * sizeof(char));
                if (temp == NULL) {
                    std::cerr << "Memory allocation failed" << std::endl;
                    exit(EXIT_FAILURE);
                }
                buffer = temp;
                fgets(buffer + strlen(buffer), bufferCapacity - strlen(buffer), file);
            }
            buffer[strcspn(buffer, "\n")] = '\0';

            if (editor->currentLine >= editor->lines) {
                editor->MakeMoreLines(editor->currentLine + 1);
            }
            editor->AppendToEnd(buffer, false);
            editor->currentLine++;
        }
        free(buffer);
        fclose(file);
        std::cout << "File has been loaded successfully!" << std::endl;

    }
    static void LineToModify(TextEditor* editor)
    {
        int line;
        std::cout << "Your current line is: " << editor->currentLine << ". Enter a line you want to modify:" << std::endl;
        if (scanf("%d", &line) != 1) {
            std::cout << "Invalid input" << std::endl;
            while (getchar() != '\n');
            return;
        };

        while (getchar() != '\n');
        if (line >= editor->lines)
        {
            editor->MakeMoreLines(line);
        }
        CommandHistory commandInfo(8, editor->currentLine, 0, 0, NULL, NULL);
        editor->stackUndo.PushToStack(&commandInfo);
        editor->currentLine = line;
        std::cout << "Your line: " << editor->currentLine << std::endl;

    }
    static void help()
    {
        std::cout << "There is an explanation for every command:\n"
            "0 - exit\n"
            "1 - append text symbols to the end\n"
            "2 - start the new line\n"
            "3 - use files to save the information\n"
            "4 - print the current text to console\n"
            "5 - insert the text by line and symbol index\n"
            "6 - search\n"
            "7 - use files to loading the information\n"
            "8 - switch line\n"
            "9 - explain every command\n"
            "10 - clear editor\n"
            "11 - clear console\n"
            "12 - delete some data\n"
            "13 - insert with replacement\n"
            "14 - cut\n"
            "15 - copy\n"
            "16 - paste\n"
            "17 - undo\n"
            "18 - redo\n"
            "20/21 - display history of text improvement\n";


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
        int* lps = new int[M];
        if (lps == NULL) {
            std::cerr << "Memory allocation failed" << std::endl;
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
                    std::cout << "Your pattern is found at location (line, place): " << k << " " << i - j << std::endl;
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
            std::cout << "No pattern like yours can be found!" << std::endl;
        }
        delete[] lps;
    }
    static void DoCommand1(TextEditor* editor, UserInput* userInput)
    {
        std::cout << "Enter text to append:" << std::endl;
        userInput->TakeUserInput();
        editor->AppendToEnd(userInput->text, true);
    }

    static void DoCommand3(TextEditor* editor, UserInput* userInput)
    {
        do {
            std::cout << "Enter a filename in which you want to store the text:" << std::endl;
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
        std::cout << "Enter text to insert:" << std::endl;
        userInput->TakeUserInput();
        editor->InsertAtIndex(line, index, userInput->text, true);
    }

    static void DoCommand6(TextEditor* editor, UserInput* userInput)
    {
        std::cout << "Enter a pattern:" << std::endl;
        userInput->TakeUserInput();
        KMPSearch(editor, userInput->text);
    }
    static void DoCommand7(TextEditor* editor, UserInput* userInput)
    {
        std::cout << "Enter a filename from which you want to load data into the text editor:" << std::endl;
        userInput->TakeUserInput();
        LoadFromFile(editor, userInput->text);
    }

    static void DoCommand12(TextEditor* editor) {
        int line, index, number;
        size_t currentLength;

        if (!editor->ChooseLineIndexNumber(&line, &index, &number, &currentLength)) {
            return;
        }

        editor->DeleteSymbols(line, index, number, currentLength, true);
    }
    static void DoCommand14or15(TextEditor* editor, Clipboard* clipboard, bool needCut)

    {
        int line, index, number;
        size_t currentLength;
        if (!editor->ChooseLineIndexNumber(&line, &index, &number, &currentLength)) {
            return;
        }
        editor->CutOrCopy(clipboard, needCut, true, line, number, index, currentLength);
    }

    static void DoCommand13(TextEditor* editor, UserInput* userInput)
    {
        int line, index;
        if (!editor->ChooseLineIndex(&line, &index))
        {
            return;
        }
        size_t currentLength = strlen(editor->text[line]);
        if (line > editor->lines || index > currentLength || currentLength == 0)
        {
            std::cout << "Something is wrong in your numbers!" << std::endl;
            return;
        }
        std::cout << "Enter text to insert:" << std::endl;
        userInput->TakeUserInput();
        editor->InsertWithReplacement(line, index, userInput->text, true);
    }
    static void ChooseAbortActionForUndo(NodeHistory* node, TextEditor* editor)
    {
        switch (node->commandInfo.command)
        {
        case 1:
            editor->DeleteSymbols(node->commandInfo.line, node->commandInfo.index, strlen(node->commandInfo.text), strlen(editor->text[node->commandInfo.line]), false);
            break;
        case 2:
            editor->DeleteLine(node->commandInfo.line);
            break;
        case 5:
            editor->DeleteSymbols(node->commandInfo.line, node->commandInfo.index, strlen(node->commandInfo.text), strlen(editor->text[node->commandInfo.line]), false);
            break;
        case 8:
            editor->currentLine = node->commandInfo.line;
            break;
        case 12:
            editor->InsertAtIndex(node->commandInfo.line, node->commandInfo.index, node->commandInfo.text, false);
            break;
        case 13:
            editor->InsertWithReplacement(node->commandInfo.line, node->commandInfo.index, node->commandInfo.pText, false);
            break;
        case 14:
            editor->InsertAtIndex(node->commandInfo.line, node->commandInfo.index, node->commandInfo.text, false);
            break;
        case 16:
            editor->DeleteSymbols(node->commandInfo.line, node->commandInfo.index, strlen(node->commandInfo.text), strlen(editor->text[node->commandInfo.line]), false);
            break;
        default:
            std::cout << "I can't return your previous text!" << std::endl;
            return;
        }
        
        
    }
    static void ChooseAbortActionForRedo(NodeHistory* node, TextEditor* editor, Clipboard* clipboard)
    {
        switch (node->commandInfo.command)
        {
        case 1:
            editor->AppendToEnd(node->commandInfo.text, false);
            break;
        case 2:
            editor->StartNewLine(false);
            break;
        case 5:
            editor->InsertAtIndex(node->commandInfo.line, node->commandInfo.index, node->commandInfo.text, false);
            break;
        case 8:
            editor->currentLine = node->commandInfo.line;
            break;
        case 12:
            editor->DeleteSymbols(node->commandInfo.line, node->commandInfo.index, node->commandInfo.number, strlen(editor->text[node->commandInfo.line]), false);
            break;
        case 13:
            editor->InsertWithReplacement(node->commandInfo.line, node->commandInfo.index, node->commandInfo.text, false);
            break;
        case 14:
            editor->CutOrCopy(clipboard,true, false, node->commandInfo.line, node->commandInfo.number, node->commandInfo.index, strlen(editor->text[node->commandInfo.line]));
            break;
        case 16:
            editor->DeleteSymbols(node->commandInfo.line, node->commandInfo.index, strlen(node->commandInfo.text), strlen(editor->text[node->commandInfo.line]), false);
            break;
        default:
            std::cout << "I can't return your previous text!" << std::endl;
            return;
        }
    }

    static void Undo(TextEditor* editor) {
        if (editor->stackUndo.size > 0) {
            NodeHistory* undoNode = editor->stackUndo.top;
            editor->stackUndo.PopFromStack();
            editor->stackRedo.PushToStack(&undoNode->commandInfo);
            ChooseAbortActionForUndo(undoNode, editor);
        }
        else {
            std::cout << "Nothing more to undo." << std::endl;

          
        }
    }

    static void Redo(TextEditor* editor, Clipboard* clipboard) {
        if (editor->stackRedo.size > 0) {
            NodeHistory* redoNode = editor->stackRedo.top;
            editor->stackRedo.PopFromStack();
           ChooseAbortActionForRedo(redoNode, editor, clipboard); 
            editor->stackUndo.PushToStack(&redoNode->commandInfo);
            delete redoNode;
        }
        else {
            std::cout << "Nothing more to undo." << std::endl;
        }
    }


public:
    int command;

    Command(int newCommand)
        :command(newCommand) {}

    static void ProcessCommand(int command, TextEditor* editor, Clipboard* clipboard) {
        UserInput userInput;
        bool needCut = false;

        switch (command) {
        
        case 0:
            std::cout << "Exiting text editor. Bye!" << std::endl;
            break;
        case 1:
            DoCommand1(editor, &userInput);
            break;
        case 2:
            editor->StartNewLine(true);
            break;
        case 3:
            DoCommand3(editor, &userInput);
            break;
        case 4:
            editor->Print();
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
        case 9:
            help();
            break;
        case 10:
            editor->Clear();
            std::cout << "Editor has been cleand!" << std::endl;
            break;
        case 11:
            system("cls");
            break;
        case 12:
            DoCommand12(editor);
            break;        
        case 13:
            DoCommand13(editor, &userInput);
            break;
        case 14:
            needCut = true;
            DoCommand14or15(editor, clipboard, needCut);
            break;
        case 15:
            DoCommand14or15(editor, clipboard, needCut);
            break;
        case 16:
            editor->Paste(clipboard);
            break;
        case 17:
            Undo(editor);
            break;
        case 18:
            Redo(editor, clipboard);
            break;
        case 19:
            clipboard->DisplayContentsOfClipboard();
            break;
        case 20:
            editor->stackRedo.DisplayContentsOfStack();
            break;
        case 21:
            editor->stackUndo.DisplayContentsOfStack();
            break;
        default:
            std::cout << "The command is not implemented. Type '9' for help." << std::endl;
        }
    }
};

int main() {
    std::cout << "Hello! Welcome to the Text Editor! Enter '9' to see the available list of commands :)" << std::endl;
    Clipboard clipboard;   
    TextEditor editor;
    int command;
    do {
        std::cout << "Enter command: " ;
        if (scanf("%d", &command) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        Command commandUser(command);
        commandUser.ProcessCommand(commandUser.command, &editor, &clipboard);
    } while (command != 0);
    return 0;
}