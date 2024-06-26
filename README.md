# TextEditor

# Assignment 1

## Available Commands

There are a total of 12 commands. Below are descriptions and screenshots illustrating their usage:

### 0: Exit the Text Editor

Closes the program.

![Command 0](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%200.png)

### 1: Append Text

Appends the text entered by the user to the end of the current line.

![Command 1](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%201.png)

### 2: Start a New Line

Begins a new line in the text editor.

![Command 2](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%202.png)

### 3: Save Text to File

Saves the current text to a specified file.

![Command 3](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%203.png)
![Command 3 - File](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%203(file).png)

### 4: Print Text

Prints the entire text to the console.

![Command 4](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%204.png)

### 5: Insert Text at Index

Inserts text at specified line and position provided by the user.

![Command 5](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%205.png)

### 6: Search for Pattern

Searches for a specific pattern in the text.

![Command 6](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%206.png)

### 7: Load Data from File

Loads data from a specified file into the text editor. Example uses `try.txt` and `table.txt`.

![Command 7](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%207.png)

### 8: Switch Line

Changes the current line to the line number specified by the user.

![Command 8](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%208.png)

### 9: Print Help

Displays a list of available commands.

![Command 9](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%209.png)

### 10: Clear Editor Content

Clears all content from the text editor.

![Command 10](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Screenshots%20of%20commands%20work/Command%2010.png)

### 11: Clear Console

Clears the console output. This is better to demostrate in a classroom.

## Combined Task Examples

### First Combined Task

**Steps:**
1. Append text
2. Save to file
3. Print text
4. Append more text
5. Load from file `table.txt`
6. Print text

![Test 1 - Step 1](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/Test1(1%20picture).png)
![Test 1 - Step 2](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test1(2%20picture).png)

### Second Combined Task

**Steps:**
1. Append text
2. Start a new line
3. Append more text
4. Insert at index
5. Print text

![Test 2](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test2.png)

### Third Combined Task

**Steps:**
1. Load data from file `table.txt`
2. Search for a specific pattern twice
3. Insert text at the beginning
4. Print text

![Test 3](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test3(1%20picture).png)
![Test 3](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test%203(2%20ptcture).png)

### Fourth Combined Task

**Steps:**
1. Append text
2. Insert text at line 5
3. Switch to the second line
4. Append more text
5. Save to file
6. Print text

![Test 4](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test4.png)

### Fifth Combined Task

**Steps:**
1. Start a new line
2. Append the words "hello my friends"
3. Print text
4. Insert "dear " between "my" and "friends" (index 1 9)
5. Print text
6. Clear editor
7. Print text
8. Exit

![Test 5](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test%205(picture1).png)
![Test 5](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test5(picture%202).png)

### Sixth Combined Task

**Steps**
1. Append to end
2. Print
3. Insert in the middle something
4. Print
5. Save to a file 
6. Clear the text editor
7. Print
8. Load from the same file, used for saving, data
9. Print
10. Exit

![Test 6](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test%206%20(picture%201).png)
![Test 6](https://github.com/MarharytaFilipovych/TextEditor/blob/master/Tests/test%206(picture%202).png)


# Assignment 2

Overall, this text editor implements 21 commands. In this assignment, I added 7 main commands and additional commands for enhanced functionality. The editor is designed with an object-oriented approach and uses a stack data structure to manage clipboard operations and undo/redo functionality.

## Main Commands

### 13. Insert with Replacement 
Inserts text at a specified position, replacing any existing text.

![Insert with Replacement](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Inseert%20with%20replacement.png)

### 12. Delete 
Deletes specified text or a portion of the text.

![Delete](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Delete.png)

### 14. Cut 
Cuts a portion of the text and places it in the clipboard.

![Cut](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Cut.png)

### 15. Copy 
Copies a portion of the text to the clipboard without removing it from the document.

![Copy](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Copy.png)

### 16. Paste 
Pastes the text from the clipboard into the document at the specified position.

![Paste](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Paste.png)

### 17. Undo 
Reverts the last change made to the document.
### 18. Redo 
Reapplies a change that was undone.

![Undo/Redo](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Undoredo%20(part1).png)
![Undo/Redo](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/Undoredo(part2).png)

## Additional Commands

### 19. Display Clipboard Content
Displays the current contents of the clipboard.

![Display Clipboard Content](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/clipboard.png)

### 20/21. Display Redo/Undo Stack 
Shows the history of changes that can be undone.

![Display Undo/Redo Stack](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Screenshots%20of%20commands%20work/undoredo(stack).png)


## Combined Task Examples

### First Combined Task

**Steps:**
1. Append text "Combined Task Examples"
2. Delete symbols first 9 symbols
3. Print text
4. Insert with replacement - replace word "Task" with "code"
5. Print text
6. Copy word "code"
7. Paste it in the end of the sentence

![Test 1 - Step 1](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/1%20task%20(1).png)
![Test 1 - Step 2](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/1%20task%20(2).png)

### Second Combined Task

**Steps:**
1. Append text 
2. Start a new line
3. Append more text
4. Insert with replacement
5. Print text
6. Undo 2 times
7. Redo 1 time
8. Print text

![Test 2 - Step 1](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/2%20task%20(1).png)![Test 2 - Step 1](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/2%20task%20(1).png)
![Test 2 - Step 2](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/2%20task%20(2).png)

### Third Combined Task

**Steps:**
1. Append text "Today is a good day"
2. Cut word "Today"
3. Print
4. Display content of the clipboard
5. Paste in the end
7. Copy word "day"
8. Paste it somewhere
9. Print

![Test 3](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%203%20(1).png)
![Test 3](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%203%20(2).png)

### Fourth Combined Task

**Steps:**
1. Append text
2. Insert text at line 5
3. Switch to the second line
4. Append more text
5. Print text
6. Undo 1 time
7. Print text
8. Redo 1 time
9. Print text

![Test 4](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/4%20task%20(1).png)
![Test 4](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/4%20task%20(2).png)

### Fifth Combined Task

**Steps:**
1. Start a new line
2. Append the words "I have a lab to do this week"
3. Cut word "lab"
4. Insert "test " between "a" and "to" (index 1 9)
5. Delete last word
6. Append "day"
7. Print text
8. Replace "have" with 'need'
9. Print text
10. Undo
11. Print

![Test 5](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%205%20(1).png)
![Test 5](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%205%20(2).png)
![Test 5](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%205%20(3).png)

### Sixth Combined Task

**Steps**
1. Append to end
2. Print
3. Replace in the middle something
4. Print
5. Delete 
6. Print
7. Copy as much as you can
8. Paste it
9. Print


![Test 6](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%206%20(1).png)
![Test 6](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-2/Tests/task%206%20(2).png)

# Assignment 3

Now the text editor has 25 commands in its arsenal. Hence, I added 4 new commands:

### 22. Enter some random text with a key (from 0 to 25) to encrypt it:
![Command 22](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Screenshots%20of%20commands%20work/Command%2022.png)

### 23. Enter some random encrypted text with a key (from 0 to 25) to decrypt it:
![Command 23](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Screenshots%20of%20commands%20work/Command%2023.png)

### 24. Input a path of a file, whose content you wish to encrypt and save this work in a separate file by entering its path:
![Command 24](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Screenshots%20of%20commands%20work/Command%2024.png)

### 25. Input a path of a file, whose content you wish to decrypt and save this work in a separate file by entering its path:
![Command 25](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Screenshots%20of%20commands%20work/Command%2025.png)

These commands were implemented using my own dynamic library, where I developed procedures for encryption and decryption functionalities.

## Combined Task Examples

### First Combined Task

**Steps:**
1. Use **Command 22 (encryption)** to write `"I hate sitting on my couch because it is uncomfortable"`.
2. Try to use **key 67**.
3. Try again to use the same command with the same text and **key 13**.
4. Now, when there is an encrypted version, use **Command 23 (decryption)** and enter this encrypted text with the same **key 13**.

![Test 1](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Tests/Test%203%20(1))%20-%203.png)

### Second Combined Task

**Steps:**
1. Use **Command 24**:
   - Load file: `heart.txt`
   - Key: 17
   - Save file: `encryptedheart.txt`
2. Use **Command 25**:
   - Load file: `encryptedheart.txt`
   - Key: 17
   - Save file: `decryptedheart.txt`

![Test 2](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Tests/Test%202%20-%203.png)

### Third Combined Task

**Steps:**
1. Append text: `"Elephants are the largest living land animals."`
2. Start a new line.
3. Append text: `"Elephants are scattered throughout sub-Saharan Africa, South Asia, and Southeast Asia and are found in different habitats, including savannahs, forests, deserts, and marshes. They are herbivorous, and they stay near water when it is accessible. They are considered to be keystone species, due to their impact on their environments."`
4. Start a new line.
5. Append text: `"Elephants have a fission–fusion society, in which multiple family groups come together to socialise."`
6. Save to file: `elephant.txt`
7. Use **Command 24**:
   - Load file: `elephant.txt`
   - Key: 8
   - Save file: `encryptedelephant.txt`
8. Load from file `encryptedelephant.txt` to ensure that the content was encrypted.
9. Print text
10. Use **Command 25**:
   - Load file: `encryptedelephant.txt`
   - Key: 8
   - Save file: `decryptedelephant.txt`
11. Load from file `decryptedelephant.txt` to ensure that the content was decrypted.
12. Print text

![Test 3](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Tests/Test%203%20-%201.png)
![Test 3](https://github.com/MarharytaFilipovych/TextEditor/blob/assignment-4/Tests/Test%203%20(2)%20-%203.png)
