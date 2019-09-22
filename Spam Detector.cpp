//
// Created by toozig on 9/22/19.
//

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "HashMap.hpp"


static const int DATABASE_FILE = 0;

static const int NUM_OF_ARGS = 3;

static const int EMAIL = 1;

static const int THRESHOLD = 2;


enum cell_type {phrase = 0 , score = 1};

/**
 * object that parse and validate the correctness of the string from a file
 */
class Parser
{


    /**
     * checks if a given argument is valid
     */
    static bool isValid(const std::string &value, const cell_type cell)
    {
        if (value.empty())
        {
            return false;
        }
        switch (cell)
        {
            case phrase:
                return true;
            case score:
                return isValidNumber(value);
            default:
                return false;
        }
    }




public:
    /**
     * Ctor of the parser class
     */
    Parser()= default;


    /**
     * checks if a given string is a valid number (positive)
     */
    static bool isValidNumber(const std::string &value)
    {
        int res;
        char cur = value[0];
        int idx = 0;
        bool isValid = true;
        while(idx < value.size() && isValid)
        {
            isValid = std::isdigit(value[idx]);
            ++idx;
        }
        return isValid;
    }

    /**
     *
     * @return true if line is valid and false otherwise, also fill the array with it's values
     */
    static bool parseLine(const std::string & string, std::string arr[NUM_OF_ARGS])
    {

        int counter = NUM_OF_ARGS;
        std::string value;
        std::stringstream stringstream(string);
        cell_type type = phrase;
        while (std::getline(stringstream, value, ','))
        {

            // cheks if there are more than 2 ',' and if vaild
            if (counter == 0 || !isValid(value,type))
            {
                return false;
            }
            arr[type] = value;
            --counter;
            type = type == phrase ? score : phrase;
        }
        return true;
    }

};

int main(int argc, char *argv[])
{

    if (argc != NUM_OF_ARGS)
    {
        std::cerr << "Useage: SpamDetector <database Path> <message path> <threshold>\n";
        return EXIT_FAILURE;
    }
    std::fstream DBfstream(argv[DATABASE_FILE]);
    std::ifstream MAILfstream(argv[EMAIL]);
    int threshold = 0;
    if (Parser::isValidNumber(argv[THRESHOLD]))
    {
        threshold = std::stoi(argv[THRESHOLD]);
    }
    // arg check
    if (DBfstream.fail() || MAILfstream.fail(), !threshold)
    {
        std::cerr << "Invalid input\n";
        return EXIT_FAILURE;
    }

    // parse database
    std::vector<std::string> stringVec;
    std::vector<int> intVec;
    std::string pair[2];
    std::string line;
    //creates the vectore of phrase and scores;
    while(DBfstream.good())
    {
        std::getline(DBfstream, line);
        if(!Parser::parseLine(line, pair))
        {
            std::cerr << "Invalid input\n";
            return EXIT_FAILURE;
        }
        stringVec.push_back(pair[phrase]),
        intVec.push_back(std::stoi(pair[score]));
    }
    auto map = HashMap<std::string, int> (stringVec, intVec);

}

