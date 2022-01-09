
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::ifstream inFile;
    inFile.open("./words.txt", std::ifstream::in);
    std::vector<std::string> dictionary;
    std::vector<bool> correct = {false, false, false, false, false};
    std::vector<std::vector<char>> wrongChars(5);
    std::vector<std::vector<char>> maybeChars(5);

    std::vector<char> guess = {'m', 'e', 'a', 't', 'y'};

    for (std::string word; std::getline(inFile, word);)
        dictionary.push_back(word);

    while (true)
    {
        printf("Size of dictionary = %d\n", dictionary.size());
        std::cout << "Guess:";
        for (char c : guess)
        {
            std::cout << c;
        }

        bool inputValid = false;
        while (!inputValid)
        {
            inputValid = true;
            std::cout << "\nEnter result (g = green, y = yellow, x = nothing)" << std::endl;
            std::string result;
            std::cin >> result;
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
                        if (j != i && !std::count(maybeChars[i].begin(), maybeChars[i].end(), c))
                            maybeChars[j].push_back(g);

                    // this position for this character is wrong
                    wrongChars[i].push_back(g);

                    break;

                case 'x':
                case 'X':
                {
                    for (int j = 0; j < 5; j++)
                        wrongChars[j].push_back(g);

                    std::vector<char>::iterator index = std::find(maybeChars[i].begin(), maybeChars[i].end(), g);
                    if (index != maybeChars[i].end())
                    {
                        maybeChars[i].erase(index);
                    }
                }
                break;

                default:
                    std::cout << "Please enter only g, y, or x!" << std::endl;
                    inputValid = false;
                    break;
                }
            }
        }
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

        for (int i = 0; i < 5; i++)
        {
            if (correct[i])
            {
                for (int j = 0; j < dictionary.size(); j++)
                    if (dictionary.at(j).at(i) != guess[i])
                        dictionary.erase(dictionary.begin() + j--);
            }
            else
            {
                for (char &c : wrongChars[i])
                {
                    for (int j = 0; j < dictionary.size(); j++)
                    {
                        if (dictionary[j].at(i) == c)
                        {
                            dictionary.erase(dictionary.begin() + j--);
                        }
                    }
                }
                if (maybeChars[i].size() > 0)
                {
                    for (int j = 0; j < dictionary.size(); j++)
                    {
                        bool wordValid = false;
                        for (char &c : dictionary[j])
                        {
                            if (std::count(maybeChars[i].begin(), maybeChars[i].end(), c))
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

        /*for (std::string s : dictionary)
        {
            std::cout << s << std::endl;
        }*/

        for (int i = 0; i < 5; i++)
        {
            guess[i] = dictionary[0][i];
        }
    }
    printf("\n\n");
}