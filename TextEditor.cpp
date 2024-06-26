#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#define INITIAL_SIZE_OF_ROW 100
#define INITIAL_SIZE_OF_LINES 20
#include <string> 
#include <windows.h>
using namespace std;
#undef max
#define CHUNK_SIZE 128
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
        : command(userCommand), line(userLine), index(userIndex), number(userNumber), pText(nullptr) {
        if (userText != nullptr) {
            text = new char[strlen(userText) + 1];
            strcpy(text, userText);
        }
        else {
            text = nullptr;
        }
        if (previousText != nullptr) {
            pText = new char[strlen(previousText) + 1];
            strcpy(pText, previousText);
        }
        else {
            pText = nullptr;
        }
    }
    void Print()
    {
        cout << "command: " << command
            << ", line: " << line
            << ", index: " << index
            << ", number of symbols: " << number
            << ", new text: " << (text ? text : "-")
            << ", previous text: " << (pText ? pText : "-") << endl;
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
        : next(nullptr), commandInfo(userCommand, userLine, userIndex, userNumber, userText, previousText) {};

};

class HistoryStack
{
public:
    NodeHistory* top;
    size_t size;
    HistoryStack()
        : size(0), top(nullptr) {}

    void PushToStack(CommandHistory* commandInfo) {
        NodeHistory* node = new NodeHistory(commandInfo->command, commandInfo->line, commandInfo->index, commandInfo->number, commandInfo->text, commandInfo->pText);
        node->next = top;
        top = node;
        size++;

    }
    void PopFromStack() {
        if (top == nullptr) {
            return;
        }
        NodeHistory* temp = top;
        top = top->next;
        size--;
    }
    void DisplayContentsOfStack() {
        NodeHistory* current = top;
        if (current == nullptr) {
            cout << "The stack is empty!" << endl;
            return;
        }
        cout << "Contents of the stack:" << endl;
        while (current != nullptr) {
            current->commandInfo.Print();
            current = current->next;
        }
    }
    ~HistoryStack() {
        NodeHistory* current = top;
        while (current != nullptr) {
            NodeHistory* next = current->next;
            delete current;
            current = next;

        }
        top = nullptr;
        size = 0;
    }

};
class NodeForClipboard
{
public:
    char* text;
    NodeForClipboard* next;
    NodeForClipboard() : text(nullptr), next(nullptr) {}

    NodeForClipboard(char* newText)
        : text(newText), next(nullptr) {}

};

class Clipboard
{
    size_t size;
public:
    NodeForClipboard* top;
    Clipboard()
        : size(0), top(nullptr) {}

    void DisplayContentsOfClipboard()
    {
        NodeForClipboard* current = nullptr;
        current = top;
        if (current == nullptr)
        {
            cout << "Your clipboard is empty!" << endl;
            return;
        }
       cout << "Contents of your clipboard:" << endl;
        while (current != nullptr)
        {
            cout << current->text << endl;
            current = current->next;
        }

    }
    ~Clipboard()
    {
        NodeForClipboard* current = top;
        while (current != nullptr)
        {
            NodeForClipboard* next = current->next;
            delete[] current->text;
            delete current;
            current = next;
        }
        top = nullptr;
    }
    void PushToStack(char* text) {
        NodeForClipboard* node = new NodeForClipboard;
        if (node == nullptr) {
            cerr << "Memory allocation failed" << endl;
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
        if (temp == nullptr)
        {
            return nullptr;
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
        if (array == nullptr)
        {
            cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < rows; i++)
        {
            array[i] = new char[columns];
            if (array[i] == nullptr)
            {
                cerr << "Memory allocation failed" << endl;
                exit(EXIT_FAILURE);
            }
            array[i][0] = '\0';
        }
        return array;
    }

    void AdjustSizeOfLine(int line, int length) {
        char* temp = (char*)realloc(text[line], (length + 1) * sizeof(char));
        if (temp == nullptr) {
           cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }
        text[line] = temp;
        text[line][length] = '\0';

    }

    void MakeSizeOfEditorSmaller(int line) {
        if (line < 0 || line >= lines) {
           cout << "Invalid line number" << endl;
            return;
        }
        size_t newLinesCapacity = lines - line;
        if (newLinesCapacity > lines) {
            cout << "Invalid newLinesCapacity calculated" << endl;
            return;
        }
        for (size_t i = lines; i > newLinesCapacity; i--) {
            if (text[i - 1] != nullptr) {
                delete[] text[i - 1];
                text[i - 1] = nullptr;
            }
        }
        char** temp = (char**)realloc(text, newLinesCapacity * sizeof(char*));
        if (temp == nullptr) {
            cerr << "Memory allocation failed" << endl;
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
        lines = INITIAL_SIZE_OF_LINES;
        currentLine = 0;
        symbolsPerLine = INITIAL_SIZE_OF_ROW;
        text = createArray(INITIAL_SIZE_OF_LINES, INITIAL_SIZE_OF_ROW);
    }

    void Print()
    {
        for (int i = 0; i < lines; i++) {
          cout << text[i] << endl;
        }
    }
    void MakeLineLonger(size_t currentLength, size_t newLength)
    {
        size_t newCapacity = currentLength + newLength + 1;
        char* temp = new char[newCapacity];
        if (temp == nullptr) {
            cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }
        strcpy(temp, text[currentLine]);
        delete[] text[currentLine];       
        text[currentLine] = temp;
        symbolsPerLine = newCapacity;
    }
    void MakeMoreLines(int line) {
        size_t newLinesCapacity = line + 1;
        char** temp = new char* [newLinesCapacity];
        if (temp == nullptr)
        {
            cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }
        copy(text, text + lines, temp);
        
        delete[] text;
        text = temp;
        for (size_t i = lines; i < newLinesCapacity; i++) {
            text[i] = new char[symbolsPerLine]; 
            if (text[i] == nullptr)
            {
                cerr << "Memory allocation failed" <<endl;
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
            CommandHistory commandInfo(12, line, index, number, previousText, nullptr);
            stackUndo.PushToStack(&commandInfo);
        }
    }
    void StartNewLine(bool user)
    {
       cout << "Your current line: " << currentLine << ". Starting new line..." << endl;
        if (currentLine + 1 > lines)
        {
            MakeMoreLines(currentLine);
        }
        currentLine++;
        cout << "Your new line: " << currentLine << endl;
        if (user)
        {
            CommandHistory commandInfo(2, currentLine, 0, 0, nullptr, nullptr);
            stackUndo.PushToStack(&commandInfo);
        }
    }
    void DeleteLine(int line)
    {
        if (currentLine == 0)
        {
            cout << "nothing to delete!" <<endl;
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
                CommandHistory commandInfo(14, line, index, number, textToBuffer, nullptr);
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
            cout << "You cannot replace more symbols than present!" << endl;
            return;
        }
        char* previousText = SavePreviousText(line, index, newTextLength);       
        for (int i = 0; i < newTextLength; i++) {
            text[line][index + i] = newText[i];
        }
        if (user)
        {
            CommandHistory commandInfo(13, line, index, newTextLength, newText, previousText);
            stackUndo.PushToStack(&commandInfo);
        }
    }
  
    void Paste(Clipboard* clipboard)
    {
        if (clipboard->top == nullptr)
        {
            cout << "There is nothing to paste, your clipboard is empty!" << endl;
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
            delete[] text[i];
            text[i] = nullptr;
        }
        delete text;
        text = nullptr;
    }

    int ChooseLineIndexNumber(int* line, int* index, int* number, size_t* currentLength) {
    cout << "Choose line, index and number of symbols:" << endl;
    if (!(cin >> *line >> *index >> *number) || *line < 0 || *index < 0 || *number < 0) {
        cout << "Invalid input!" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 0;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    *currentLength = strlen(text[*line]);
    cout << "There are " << lines << " lines and, on your chosen line there are " << *currentLength << " symbols" << endl;

    if (*index >= *currentLength || *number > *currentLength - *index || *currentLength == 0 || *line >= lines) {
        cout << "Something is wrong in your numbers! Look higher!" << endl;
        return 0;
    }
    return 1;
}
    int ChooseLineIndex(int* line, int* index) {
        cout << "Choose line and index:" << endl;
        if (!(cin >> *line >> *index) != 2 || *line < 0 || *index < 0) {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return 0;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return 1;
    }
    void Clear(bool user) {
        lines = INITIAL_SIZE_OF_LINES;
        symbolsPerLine = INITIAL_SIZE_OF_ROW;
        text = createArray(INITIAL_SIZE_OF_LINES, INITIAL_SIZE_OF_ROW);
        currentLine = 0;
        if (user)
        {
            CommandHistory commandInfo(10, 0, 0, 0, nullptr, nullptr);
            stackUndo.PushToStack(&commandInfo);
        }
    }
};
class CaesarCipher
{
    HINSTANCE handle;
    typedef char* (*encrypt_ptr_t)(char*, int);
    typedef char* (*decrypt_ptr_t)(char*, int);
    
    bool LoadLibrary()
    {
        handle = ::LoadLibrary(TEXT("C:\\Margo\\CaesarCodeAndLibrary\\Library\\Caesar.dll"));
        if (handle == nullptr || handle == INVALID_HANDLE_VALUE)
        {
            cout << "Lib not found" << endl;
            return false;
        }
        return true;
    }
    bool LoadFunctions() {
        encrypt_ptr = (encrypt_ptr_t)GetProcAddress(handle, "Encrypt");
        if (encrypt_ptr == nullptr)
        {
            cout << "Encrypt function not found." << endl;
            FreeLibrary(handle);
            return false;
        }
        decrypt_ptr = (encrypt_ptr_t)GetProcAddress(handle, "Decrypt");
        if (decrypt_ptr == nullptr)
        {
            cout << "Encrypt function not found." << endl;
            FreeLibrary(handle);
            return false;
        }
        return true;
    }
public:
    encrypt_ptr_t encrypt_ptr;
    decrypt_ptr_t decrypt_ptr;
   

    CaesarCipher() : handle(nullptr), encrypt_ptr(nullptr), decrypt_ptr(nullptr)
    {
       
        if (!(LoadLibrary() && LoadFunctions()))
        {
            cout << "Failed to load library or functions." << endl;
        }
    }
    ~CaesarCipher()
    {
        if (handle != nullptr) {
            FreeLibrary(handle);
        }
    }
};

class UserInput
{
    void MakeUserArrayLonger() {
        size_t newCapacity = capacity * 2;
        char* temp = new char [newCapacity];
        if (temp == NULL)
        {
            cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }
        strcpy(temp, text);
        delete[] text;
        text = temp;
        capacity = newCapacity;
    }
    void AdjustNeededSizeForUserArray(size_t realLength)
    {
        char* temp = new char[realLength + 1];
        if (temp == nullptr)
        {
            cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }
        strcpy(temp, text);      
        delete[] text;
        text = temp;
        capacity = realLength + 1;
    }
public:
    char* text;
    size_t capacity;
    char* CreateArrayForUserInput() {
        char* text = new char[INITIAL_SIZE_OF_ROW];
        if (text == nullptr)
        {
            cerr << "Memory allocation failed" << endl;
            exit(EXIT_FAILURE);
        }
        text[0] = '\0';
        return text;
    }
    UserInput()
    {
        text = CreateArrayForUserInput();
        capacity = INITIAL_SIZE_OF_ROW;
    }
    ~UserInput()
    {
        if (text != nullptr)
        {
           delete[] text;
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
    static void SaveToFile(TextEditor* editor, const char* fileName, bool user)
    {
        ofstream file(fileName);
        if (!file.is_open())
        {
            cout << "This file cannot be opened or something bad happened to it!" << endl;
            return;
        }

        for (int i = 0; i < editor->lines; i++)
        {
            if (editor->text[i] != nullptr)
            {
                file << editor->text[i] << '\n';
            }
        }

        file.close();
        cout << "Text has been saved successfully!" << endl;
        if (user)
        {
            CommandHistory commandInfo(3, 0, 0, 0, nullptr, nullptr);
            editor->stackUndo.PushToStack(&commandInfo);
        }
    }
    static void LoadFromFile(TextEditor* editor, const char* fileName, bool user) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cout << "Error opening file. It looks like it does not exist!" << endl;
            return;
        }
        editor->Clear(false);
        string line;
        while (getline(file, line)) {
            if (editor->currentLine >= editor->lines) {
                editor->MakeMoreLines(editor->currentLine + 1);
            }
            char* mline = new char[line.size() + 1];
            strcpy(mline, line.c_str());
            editor->AppendToEnd(mline, false);
            delete[] mline;
            editor->currentLine++;
        }

        file.close();
        cout << "File has been loaded successfully!" << endl;

        if (user) {
            CommandHistory commandInfo(7, 0, 0, 0, nullptr, nullptr);
            editor->stackUndo.PushToStack(&commandInfo);
        }
    }
    static void LineToModify(TextEditor* editor)
    {
        int line;
        cout << "Your current line is: " << editor->currentLine << ". Enter a line you want to modify:" << endl;
        if (!(cin >> line)) {
            cout << "Invalid input" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        };

        cin.ignore(numeric_limits<streamsize>::max() , '\n');
        if (line >= editor->lines)
        {
            editor->MakeMoreLines(line);
        }
        CommandHistory commandInfo(8, editor->currentLine, 0, 0, nullptr, nullptr);
        editor->stackUndo.PushToStack(&commandInfo);
        editor->currentLine = line;
       cout << "Your line: " << editor->currentLine << endl;

    }
    static void help()
    {
        cout << "There is an explanation for every command:\n"
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
            "20/21 - display history of text improvement\n"
            "22 - enter some text to encrypt\n"
            "23 - enter some text to decrypt\n"
            "24 - encrypt a file\n"
            "25 - decrypt a file\n";


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
        if (lps == nullptr) {
            cerr << "Memory allocation failed" << endl;
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
                    cout << "Your pattern is found at location (line, place): " << k << " " << i - j << endl;
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
            cout << "No pattern like yours can be found!" << endl;
        }
        delete[] lps;
    }
    static void DoCommand1(TextEditor* editor, UserInput* userInput)
    {
       cout << "Enter text to append:" << endl;
        userInput->TakeUserInput();
        editor->AppendToEnd(userInput->text, true);
    }

    static void DoCommand3(TextEditor* editor, UserInput* userInput)
    {
        do {
            cout << "Enter a filename in which you want to store the text:" << endl;
            userInput->TakeUserInput();
        } while (strlen(userInput->text) == 0);
        SaveToFile(editor, userInput->text, true);
    }

    static void DoCommand5(TextEditor* editor, UserInput* userInput)
    {
        int line, index;
        if (!editor->ChooseLineIndex(&line, &index)) {
            return;
        }
        cout << "Enter text to insert:" << endl;
        userInput->TakeUserInput();
        editor->InsertAtIndex(line, index, userInput->text, true);
    }

    static void DoCommand6(TextEditor* editor, UserInput* userInput)
    {
       cout << "Enter a pattern:" << endl;
        userInput->TakeUserInput();
        KMPSearch(editor, userInput->text);
    }
    static void DoCommand7(TextEditor* editor, UserInput* userInput)
    {
        cout << "Enter a filename from which you want to load data into the text editor:" << endl;
        userInput->TakeUserInput();
        LoadFromFile(editor, userInput->text, true);
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
            cout << "Something is wrong in your numbers!" << endl;
            return;
        }
       cout << "Enter text to insert:" << endl;
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
           cout << "I can't return your previous text!" << endl;
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
            cout << "I can't return your previous text!" << endl;
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
            cout << "Nothing more to undo." << endl;         
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
            cout << "Nothing more to undo." <<endl;
        }
    }
    static char* Encrypt(char* text, int key)
    {
        CaesarCipher cipher;
        return cipher.encrypt_ptr(text, key);
    }
    static char* Decrypt( char* text, int key)
    {
        CaesarCipher cipher;
        return cipher.decrypt_ptr(text, key);
    }
    static bool AskUserToEnterKey(int* key) {
        cout << "Enter a key (0 to 25): " << endl;
        if (!(cin >> *key)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input!" << endl;
            return false;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (*key < 0 || *key > 25) {
            cout << "Invalid range!" << endl;
            return false;
        }
        return true;
    }
    static void DoCommand22or23(UserInput* userInput, bool encrypt)
    {
        int key;
        cout << "Enter text: " << endl;
        userInput->TakeUserInput();
        if (strlen(userInput->text) == 0)
        {
            cout << "You have not input anything!!!" << endl;
            return;
        }
        if (!AskUserToEnterKey(&key))
        {
            return;
        }
        if (encrypt) {
            cout << "Encrypted: " << Encrypt(userInput->text, key) << endl;

        }
        else
        {
            cout << "Decrypted:" <<  Decrypt(userInput->text, key) << endl;
        }

    }
    static void ReadAndWriteChunks(ofstream &saveFile, ifstream &loadFile, int* key, bool encrypt)
    {
        char* chunk = new char[CHUNK_SIZE + 1];
        while (loadFile.read(chunk, CHUNK_SIZE) || loadFile.gcount() > 0) {
            streamsize bytesRead = loadFile.gcount();
            chunk[bytesRead] = '\0';
            if (encrypt) {
                saveFile.write(Encrypt(chunk, *key), bytesRead);
            }
            else {
                saveFile.write(Decrypt(chunk, *key), bytesRead);
            }
        }
        delete[] chunk;
    }
    static void CloseFiles(ofstream& saveFile, ifstream& loadFile)
    {
        if (loadFile.bad()) {
            cout << "Error reading from file!" << endl;
        }
        if (saveFile.bad()) {
            cout << "Error writing to file!" << endl;
        }

        loadFile.close();
        saveFile.close();
   }

    static void DoCommand24or25(UserInput* userInput, bool encrypt, int* key) {
        cout << "Enter a path of a file, from which you want to load data: " << endl;
        userInput->TakeUserInput();
        ifstream loadFile(userInput->text, ios::binary);
        if (!loadFile.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        if (!AskUserToEnterKey(key)) {
            return;
        }
        userInput->text[0] = '\0';
        cout << "Enter a file path, where you want to save new text: " << endl;
        userInput->TakeUserInput();
        ofstream saveFile(userInput->text, ios::binary);
        if (!saveFile.is_open()) {
            cout << "Error opening file!" << endl;
            return;
        }

        ReadAndWriteChunks(saveFile, loadFile, key, encrypt);
        CloseFiles(saveFile, loadFile);
    }
  
    

public:
    int command;

    Command(int newCommand)
        :command(newCommand) {}

    static void ProcessCommand(int command, TextEditor* editor, Clipboard* clipboard) {
        UserInput userInput;
        bool needCut = false;
        bool encrypt = true; 
        int key;
        switch (command) {
        
        case 0:
            cout << "Exiting text editor. Bye!" << endl;
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
            editor->Clear(true);
            cout << "Editor has been cleand!" << endl;
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
        case 22:
            DoCommand22or23(&userInput, encrypt);
            break;
        case 23:
            encrypt = false;
            DoCommand22or23(&userInput, encrypt);
            break;
        case 24:
            DoCommand24or25(&userInput, encrypt, &key);
            break;
        case 25:
            encrypt = false;
            DoCommand24or25( &userInput, encrypt, &key);
            break;
        default:
            cout << "The command is not implemented. Type '9' for help." << endl;
        }
    }
};

int main() {
    cout << "Hello! Welcome to the Text Editor! Enter '9' to see the available list of commands :)" << endl;
    Clipboard clipboard;   
    TextEditor editor;
    int command;
    do {
        cout << "Enter command: ";
        if (!(cin >> command)) {
           cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid command." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (command == 0) {
            break;
        }
        Command commandUser(command);
        commandUser.ProcessCommand(commandUser.command, &editor, &clipboard);
    } while (true);
    return 0;
}