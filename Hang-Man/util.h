#include <iostream>
using namespace std;

bool isAllDash(const string &s)
{
    for(char c: s)
    {
        if(c != '-')
        {
            return false;
        }
    }
    return true;
}

bool isAllNotDash(const string &s)
{
    for(char c: s)
    {
        if(c == '-')
        {
            return false;
        }
    }
    return true;
}

void updateSecretWord(const string &mask, string &secretWord)
{
    for (unsigned int i = 0; i < secretWord.size(); i++)
    {
        if(mask[i] != '-')
        {
            secretWord[i] = mask[i];
        }
    }
}

bool isGoodMask(char guess, const string &mask, const string &secretWord)
{
    if(mask.size() != secretWord.size())
    {
        return false;
    }
    for(unsigned int i = 0; i < secretWord.size(); i++)
    {
        if(mask[i] != '-')
        {
            if(mask[i] != guess)
            {
                return false;
            }
            if(secretWord[i] != '-' && secretWord[i] != mask[i])
            {
                return false;
            }
        }
    }
    return true;
}
