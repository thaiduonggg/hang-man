#pragma once
#include <string>
#include <set>
#include <map>
using namespace std;

vector<string> readWordListFromFile(const string& filePath)
{
    vector<string> wordList;
    ifstream file(filePath);
    string word;
    while (getline(file, word))
    {
        wordList.push_back(word);
    }
    file.close();
    return wordList;
}

set<char> getRemainingChars(const set<char> &previousGuesses)
{
    set<char> remainingChars;
    for(char c = 'a'; c <= 'z'; c++)
    {
        remainingChars.insert(c);
    }
    for(char c: previousGuesses)
    {
        remainingChars.erase(c);
    }
    return remainingChars;
}

char selectRandomChar(const set<char> &s)
{
    int r = rand() % s.size();
    for(char c: s)
    {
        if(r-- == 0)
        {
            return c;
        }
    }
    return 0;
}

char getVowelGuess(const set<char> &remainingChars)
{
    char vowel[] = {'e', 'a', 'o', 'i', 'u'};
    for(char c: vowel)
    {
        if(remainingChars.find(c) != remainingChars.end())
        {
            return c;
        }
    }
    return 0;
}

bool isSuitableWord(const string &word, const string &secretWord, const set<char> &remainingChars)
{
    if(word.size() != secretWord.size())
    {
        return false;
    }
    for(unsigned int i = 0; i < word.size(); i++)
    {
        if(secretWord[i] != '-')
        {
            if(tolower(word[i]) != tolower(secretWord[i]))
            {
                return false;
            }
        }
        else if(remainingChars.find(word[i]) == remainingChars.end())
        {
            return false;
        }
    }
    return true;
}

vector<string> getSuitableWords(const vector<string> &wordList, const string &secretWord, const set<char> &remainingChars)
{
    vector<string> result;
    for(const string &word: wordList)
    {
        if(isSuitableWord(word, secretWord, remainingChars))
        {
            result.push_back(word);
        }
    }
    return result;
}

map<char, int> getOccurenceCount(const set<char> &remainingChars, const vector<string> &wordList)
{
    map<char, int> count;
    for(char c: remainingChars)
    {
        count[c] = 0;
    }
    for(const string &word: wordList)
    {
        for(char c: word)
        {
            if(count.find(c) != count.end())
            {
                count[c]++;
            }
        }
    }
    return count;
}

char getMaxOccurenceChar(const set<char> &remainingChars, const map<char, int> &count)
{
    char best = 0;
    int best_count = 0;
    for(auto p: count)
    {
        if(p.second > best_count)
        {
            best = p.first;
            best_count = p.second;
        }
    }
    return best;
}
