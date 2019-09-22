//
// Created by toozig on 9/22/19.
//

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "HashMap.hpp"


static const int DATABASE_FILE = 1;

static const int NUM_OF_ARGS = 4;

static const int EMAIL = 2;

static const int THRESHOLD = 3;


static const int TWO_IN_ROW = 2;
enum cell_type {phrase = 0 , score = 1};

/**
 * object that parse and vble(HashMap alidate the correctness of the string from a file
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
     * Ctor of the parser class is delted since this is a static class
     */
    Parser()= delete;


    /**
     * checks if a given string is a valid number (positive)
     */
    static bool isValidNumber(const std::string &value)
    {
        size_t idx = 0;
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

        int counter = TWO_IN_ROW;
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
        return !counter;
    }

};


/**
 * This function counts how many times an expression appears in a given mail
 * @return  the amount of times the phrase appears
 */
size_t phraseCounter(const std::string& mail,
                   const std::string& phrase)  {

    const size_t size = phrase.size();

    size_t count = 0 ;
    size_t pos = 0 ;

    while( (pos=mail.find(phrase, pos)) !=std::string::npos) {
        pos +=size;
        ++count ;
    }

    return count;

}

int main(int argc, char *argv[])
{

    if (argc != NUM_OF_ARGS)
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>\n";
        return EXIT_FAILURE;
    }
    std::fstream fstream(argv[DATABASE_FILE]);
    std::ifstream MAILstream(argv[EMAIL]);
    int threshold = 0;
    if (Parser::isValidNumber(argv[THRESHOLD]))
    {
        threshold = std::stoi(argv[THRESHOLD]);
    }
    // arg check
    if (fstream.fail() || MAILstream.fail() || !threshold)
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
    while(fstream.good())
    {
        std::getline(fstream, line);
        if(!Parser::parseLine(line, pair) )
        {
            if(fstream.fail() && line.empty()) { break; }
            std::cerr << "Invalid input\n";
            return EXIT_FAILURE;
        }
        std::for_each(pair[phrase].begin(), pair[phrase].end(), [](char & c) {  c = std::tolower(c); });
        stringVec.push_back(pair[phrase]),
        intVec.push_back(std::stoi(pair[score]));
    }
    auto map = HashMap<std::string, int> (stringVec, intVec);
    std::string mail( (std::istreambuf_iterator<char>(MAILstream) ), (std::istreambuf_iterator<char>()));
    std::for_each(mail.begin(), mail.end(), [](char & c) {  c = std::tolower(c); });
    int mailScore = 0;
    for (const auto &i : map)
    {
        mailScore += phraseCounter(mail, i.first) * i.second;
    }
    std::string res = mailScore < threshold ? "NOT_SPAM" : "SPAM";
    std::cout << res << std::endl;
    return 0;
}

