
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define PRINTBASIC 1
#define PRINTVERBOSE 0

bool analyzeResult(std::string &result, std::vector<char> &guess, std::vector<bool> &correct, std::vector<std::vector<char>> &maybeChars, std::vector<std::vector<char>> &wrongChars)
{
    for (char c : guess)
    {
        std::vector<char>::iterator index;
        for (int i = 0; i < 5; i++)
        {
            index = std::find(maybeChars[i].begin(), maybeChars[i].end(), c);
            while (index != maybeChars[i].end())
            {
                maybeChars[i].erase(index);
                index = std::find(maybeChars[i].begin(), maybeChars[i].end(), c);
            }
        }
    }
    bool inputValid = true;
    for (int i = 0; i < result.length(); i++)
    {
        char c = result.at(i);
        char g = guess.at(i);
        switch (c)
        {
            // if we see a green, indicate that this index in the guess is correct
        case 'g':
        case 'G':
            correct[i] = true;
            break;

            // if we see a yellow
        case 'y':
        case 'Y':
            // iterate and add this char to all "maybe" lists except the current index (the letter appears somewhere other than its current position)
            for (int j = 0; j < 5; j++)
                if (j != i && (std::find(wrongChars[i].begin(), wrongChars[i].end(), g) == wrongChars[i].end()))
                    maybeChars[j].push_back(g);

            // Add this char to the incorrect list for this position if it's not already in there
            if (std::find(wrongChars[i].begin(), wrongChars[i].end(), g) == wrongChars[i].end())
                wrongChars[i].push_back(g);

            break;

        case 'x':
        case 'X':
        {
            for (int j = 0; j < 5; j++)
            {
                std::vector<char>::iterator index = std::find(maybeChars[j].begin(), maybeChars[j].end(), g);
                if (index == maybeChars[j].end())
                {
                    if (std::find(wrongChars[j].begin(), wrongChars[j].end(), g) == wrongChars[j].end())
                    {
                        wrongChars[j].push_back(g);
                    }
                    //maybeChars[j].erase(index);
                }
            }
        }
        break;

        default:
            std::cout << "Please enter only g, y, or x!" << std::endl;
            inputValid = false;
            break;
        }
    }
    return inputValid;
}

void playerDrivenAnswer(std::vector<char> &guess, std::vector<bool> &correct, std::vector<std::vector<char>> &maybeChars, std::vector<std::vector<char>> &wrongChars)
{
    bool inputValid = false;
    while (!inputValid)
    {
        inputValid = true;
        std::cout << "\nEnter result (g = green, y = yellow, x = nothing)" << std::endl;
        std::string result;
        std::cin >> result;

        inputValid = analyzeResult(result, guess, correct, maybeChars, wrongChars);
    }
}

void autoAnswer(std::vector<char> &guess, std::vector<bool> &correct, std::vector<std::vector<char>> &maybeChars, std::vector<std::vector<char>> &wrongChars, std::string correctAnswer)
{
    std::string result = "     ";
    std::vector<char> charCounts(26);
    for (char &c : correctAnswer)
        charCounts[c - 97]++;

    for (int i = 0; i < 5; i++)
    {
        if (guess[i] == correctAnswer[i])
        {
            correct[i] = true;
            result[i] = 'g';
            charCounts[guess[i] - 97]--;
        }
        else
        {
            if (charCounts[guess[i] - 97] == 0)
            {
                result[i] = 'x';
            }
            else
            {
                result[i] = 'y';
                charCounts[guess[i] - 97]--;
            }
        }
    }
    analyzeResult(result, guess, correct, maybeChars, wrongChars);

    if (PRINTBASIC)
    {
        for (char &c : result)
        {
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int guessWord(std::vector<std::string> &dictionary, std::string correctAnswer)
{
    std::vector<bool> correct = {false, false, false, false, false};
    std::vector<std::vector<char>> wrongChars(5);
    std::vector<std::vector<char>> maybeChars(5);

    std::vector<char> guess = {'m', 'e', 'a', 't', 'y'};
    //std::vector<char> guess = {'r', 'o', 'a', 't', 'e'};

    int guessCount = 0;
    bool stillGuessing = true;
    while (stillGuessing)
    {
        guessCount++;
        if (PRINTVERBOSE)
        {
            std::cout << "Size of dictionary = %d\n"
                      << dictionary.size()
                      << "\n";
        }
        if (PRINTBASIC)
        {
            std::cout << "Guess " << ++guessCount << ":";
            for (char c : guess)
            {
                std::cout << c;
            }
            std::cout << "\n";
        }

        if (correctAnswer.length() == 0)
        {
            playerDrivenAnswer(guess, correct, maybeChars, wrongChars);
        }
        else
        {
            autoAnswer(guess, correct, maybeChars, wrongChars, correctAnswer);
        }

        if (PRINTVERBOSE)
        {
            for (int i = 0; i < 5; i++)
            {
                if (!correct[i])
                {
                    std::cout << "MAYBE:";
                    for (char c : maybeChars[i])
                    {
                        std::cout << c;
                    }
                    std::cout << "|NOT:";
                    for (char c : wrongChars[i])
                    {
                        std::cout << c;
                    }
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << guess[i] << std::endl;
                }
            }
        }

        for (int i = 0; i < 5; i++)
        {
            if (correct[i])
            {
                for (int j = 0; j < dictionary.size(); j++)
                    if (dictionary[j][i] != guess[i])
                        dictionary.erase(dictionary.begin() + j--);
            }
            else
            {
                // remove any words containing grey letters
                for (int j = 0; j < dictionary.size(); j++)
                {
                    for (char &c : wrongChars[i])
                    {

                        if (dictionary[j].at(i) == c)
                        {
                            dictionary.erase(dictionary.begin() + j);
                            if (j > 0)
                                j--;
                        }
                    }
                }
                for (int j = 0; j < dictionary.size(); j++)
                {

                    if (maybeChars[i].size() > 0)
                    {

                        bool wordValid = false;
                        for (char &c : dictionary[j])
                        {
                            if (std::find(maybeChars[i].begin(), maybeChars[i].end(), c) != maybeChars[i].end())
                            {
                                wordValid = true;
                                break;
                            }
                        }

                        if (!wordValid)
                            dictionary.erase(dictionary.begin() + j--);
                    }
                }
            }
        }
        bool allRight = true;
        for (int i = 0; i < 5; i++)
        {
            guess[i] = dictionary[dictionary.size() - 1][i];
            allRight &= correct[i];
        }
        if (allRight)
        {
            stillGuessing = false;
        }
    }
    if (PRINTBASIC)
    {
        std::cout << "Found the word!" << std::endl;
    }
    return guessCount;
}

int main(int argc, char *argv[])
{
    std::ifstream inFile;
    inFile.open("./words.txt", std::ifstream::in);
    std::vector<std::string> dictionary;

    for (std::string word; std::getline(inFile, word);)
        dictionary.push_back(word);

    std::vector<std::string> fullDictionary = dictionary;
    /*
    int wordCount = 0;
    int totalGuesses = 0;
    for (int i = 1; i < 26; i++)
    {
        std::cout << "Examining word " << wordCount++ << ":" << fullDictionary[(fullDictionary.size() - 1) / i] << std::endl;
        totalGuesses += guessWord(dictionary, fullDictionary[(fullDictionary.size() - 1) / i]);
        dictionary = fullDictionary;
    }
    printf("%d\n", totalGuesses);
    //printf("%d\n", dictionary.size());
    */

    if (argc == 1)
    {
        guessWord(dictionary, std::string());
    }
    else
    {
        guessWord(dictionary, std::string(argv[1]));
    }
}

// 25 evenly distributed words:
// combine loop for yellow letters and wrong letters - 9.48s
