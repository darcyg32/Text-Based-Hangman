#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

// Global variables
string correctWord;
string currentWord;
string incorrectGuesses;
int numIncorrectGuesses;
int numGuessesAllowed = 7;
char currentLetterGuess;

void initialize() {
    incorrectGuesses = "";
    numIncorrectGuesses = 0;
    
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "Welcome to Hangman! Guess lowercase letters and try to solve the word in 7 guesses or less!\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void setCorrectWord() {
    // Initialize vector of words
    vector<string> words;
    
    // Open words.txt file
    ifstream file("words.txt");

    // If file is unable to be opened
    if (!file.is_open()) {
        cerr << "Error: Unable to open 'words.txt'.\n";
        return; // Exit function
    }

    // Else, add each line of words.txt as an element to vector words
    string line;
    while (getline(file, line)) {
        words.push_back(line);
    }
    
    // If vector words is not empty
    if (!words.empty()) {
        // Seed the random number generator
        srand(static_cast<unsigned int>(std::time(nullptr)));
        // Set correctWord as a random word in the vector
        correctWord = words[rand() % words.size()];
    } else {
        // Else, exit function
        cerr << "Error: 'words.txt' is empty.\n";
    }
}

void displayCurrentState() {
    std::cout << currentWord; 
    if (numIncorrectGuesses > 0) {
        std::cout << " | Incorrect Guesses: ";
        for (int i = 0 ; i < numIncorrectGuesses - 1 ; i++) {
            std::cout << incorrectGuesses[i] << ", ";
        }
        std::cout << incorrectGuesses[numIncorrectGuesses - 1];
    }
    std::cout << " | Allowed Number of Incorrect Guesses Left: " << numGuessesAllowed - numIncorrectGuesses << "\n";
}

void getUserGuess() {
    std::cout << "Enter your guess: ";
    cin >> currentLetterGuess;
    if (incorrectGuesses.find(currentLetterGuess) != std::string::npos || currentWord.find(currentLetterGuess) != std::string::npos) {
        std::cout << "~~ You've already guessed that! ~~\n";
        displayCurrentState();
        getUserGuess();
    }

    if (!std::islower(currentLetterGuess)) {
        std::cout << "~~ Please enter a lowercase letter! ~~\n";
        displayCurrentState();
        getUserGuess();
    }
}

bool isGuessInWord() {
    for (int i = 0 ; i < correctWord.size() ; i++) {
        if (currentLetterGuess == correctWord[i]) {
            return true;
        }
    }
    return false;
}

void updateCurrentWord() {
    for (int i = 0 ; i < correctWord.size() ; i++) {
        if (correctWord[i] == currentLetterGuess) {
            currentWord[i] = currentLetterGuess;
        }
    }
}

void updateIncorrectGuesses() {
    incorrectGuesses += currentLetterGuess;
    numIncorrectGuesses++;
}

bool isWordGuessed() {
    return (currentWord == correctWord);
}

bool isTooManyGuesses() {
    if (numIncorrectGuesses >= numGuessesAllowed) {
        return true;
    }
    return false;
}

void displayWin() {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "You guessed the word! It was " << correctWord << "! You win!\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

void displayLose() {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    std::cout << "You ran out of guesses! It was " << correctWord << "! You lose!\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

int main() {

    initialize();

    int gameOver = false;
    setCorrectWord();
    currentWord = string(correctWord.size(), '_');
    while (!gameOver) {
        // Display current state
        displayCurrentState();
        // Get user guess
        getUserGuess();
        // If user guess is in word
        if (isGuessInWord()) {
            // Add it to the display word
            updateCurrentWord();
        } else {
            // Else
            // Add it to the incorrect guesses
            // numIncorrectGuesses++
            updateIncorrectGuesses();
        }

        // Check if word is complete
        if (isWordGuessed()) {
            displayWin();
            gameOver = true;
            break;
        }

        // Check if too many guesses
        if (isTooManyGuesses()) {
            displayLose();
            gameOver = true;
            break;
        }
    }

    char answer;
    std::cout << "Would You Like To Play Again? (y/n)\n";
    cin >> answer;
    if (answer == 'y') {
        main();
    } else {
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        std::cout << "Thanks for Playing!\n";
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    }

    return 0;
}