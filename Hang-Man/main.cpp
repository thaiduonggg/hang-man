#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "draw.h"
#include "guesser.h"
#include "util.h"
using namespace std;
const int MAX_BAD_GUESSES = 8;

vector<string> loadWordsFromFile(const string &filename)
{
    vector<string> words;
    ifstream file(filename);
    string word;
    while (file >> word)
    {
        words.push_back(word);
    }
    return words;
}

string chooseWord(const vector<string> &WORD_LIST)
{
    int randomIndex = rand() % WORD_LIST.size();
    return WORD_LIST[randomIndex];
}

void update(string &guessedWord, const string &secretWord, char guess)
{
    for (int i = 0; i < secretWord.length(); i++)
    {
        if (secretWord[i] == guess)
        {
            guessedWord[i] = guess;
        }
    }
}

bool contains(const string &secretWord, char guess)
{
    return secretWord.find(guess) != string::npos;
}

char readAGuess()
{
    char guess;
    cout << "Your guess: ";
    cin >> guess;
    return guess;
}

int getUserWordLength()
{
    int wordLength;
    cout << endl << "Enter your word length: ";
    cin >> wordLength;
    return wordLength;
}

string getUserAnswer(char guess)
{
    string answer;
    cout << endl << "I guess " << guess << ", please enter your mask: ";
    cin >> answer;
    return answer;
}

void initialize(int &wordLength, string &secretWord, int &incorrectGuess, set<char> &previousGuesses, bool &stop)
{
    wordLength = getUserWordLength();
    secretWord = string(wordLength, '-');
    incorrectGuess = 0;
    previousGuesses = set<char>();
    stop = false;
}

void update(char guess, const string &mask, int &incorrectGuess, set<char> &previousGuesses, string &secretWord, bool &stop)
{
    if(!isGoodMask(guess, mask, secretWord))
    {
        throw invalid_argument("mistake entering answer");
    }
    previousGuesses.insert(guess);
    if(isAllDash(mask))
    {
        incorrectGuess++;
        if(incorrectGuess == MAX_BAD_GUESSES)
        {
            stop = true;
        }
    }
    else
    {
        updateSecretWord(mask, secretWord);
        if(isAllNotDash(secretWord))
        {
            stop = true;
        }
    }
}

char getNextGuess(const set<char> &previousGuesses, const string& secretWord)
{
    static vector<string> wordList = readWordListFromFile("words.txt");
    set<char> remainingChars = getRemainingChars(previousGuesses);
    if(remainingChars.size() == 0)
    {
        return 0;
    }
    if(isAllDash(secretWord))
    {
        return getVowelGuess(remainingChars);
    }
    vector<string> filteredWordList = getSuitableWords(wordList, secretWord, remainingChars);
    map<char, int> occurenceCount = getOccurenceCount(remainingChars, filteredWordList);
    return getMaxOccurenceChar(remainingChars, occurenceCount);
}

void runGame1()
{
    srand(time(0));
    vector<string> WORD_LIST = loadWordsFromFile("words.txt");
    string wrongGuesses;
    string secretWord = chooseWord(WORD_LIST);
    string guessedWord(secretWord.length(), '-');
    int badGuessCount = 0;
    do
    {
        renderGame(guessedWord, badGuessCount, wrongGuesses);
        char guess = readAGuess();
        if (contains(secretWord, guess))
        {
            update(guessedWord, secretWord, guess);
        }
        else
        {
            badGuessCount++;
            wrongGuesses.push_back(guess);
        }
    }
    while (badGuessCount < MAX_BAD_GUESSES && secretWord != guessedWord);
    displayFinalResult(badGuessCount < MAX_BAD_GUESSES, secretWord);
}

void runGame2()
{
    int wordLength;
    string secretWord;
    int incorrectGuess;
    set<char> previousGuesses;
    bool stop;

    initialize(wordLength, secretWord, incorrectGuess, previousGuesses, stop);

    render(incorrectGuess, previousGuesses, secretWord);
    do
    {
        char guess = getNextGuess(previousGuesses, secretWord);
        if(guess == 0)
        {
            clearScreen();
            auto start_time = chrono::steady_clock::now();
            while (chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start_time).count() < 3)
            {
                cout << "I give up! What is your word? :3" << endl;
                cout << getNextHangMan();
                this_thread::sleep_for(chrono::milliseconds(300));
                clearScreen();
            }
            string newWord;
            cin >> newWord;
            ofstream file("words.txt", ios::app);
            if(file.is_open())
            {
                file << endl << newWord;
                file.close();
                cout << "Your word has been updated to the file.";
            }
        }
        do
        {
            try
            {
                string mask = getUserAnswer(guess);
                update(guess, mask, incorrectGuess, previousGuesses, secretWord, stop);
                break;
            }
            catch(invalid_argument e)
            {
                cout << "Invalid mask, please try again." << endl;
            }
        }
        while(true);
        render(incorrectGuess, previousGuesses, secretWord);
    }
    while(!stop);
    playAnimation(incorrectGuess == MAX_BAD_GUESSES, secretWord);
}

int main()
{
    cout << "Choose your role:" << endl << "1. Guesser" << endl << "2. Chooser" << endl;
    int chooseRole;
    cin >> chooseRole;
    if(chooseRole == 1)
    {
        runGame1();
    }
    else
    {
        runGame2();
    }
}
