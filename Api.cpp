#include "wordsearch.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


Wordsearch generateWS(int width, int height, std::vector<std::string> words, int numberOfWords);

//Trims given string returning string without any whitespace
static std::string trim(std::string value){
    const auto first = value.find_first_not_of(" \t\r\n\"");
    if (first == std::string::npos) return {};
    const auto last = value.find_last_not_of(" \t\r\n\"");
    return value.substr(first, last-first +1);
}

//Splits a line on "," then sends through trim
static std::vector<std::string> split(const std::string& line){
    std::vector<std::string> values;
    std::stringstream stream(line);
    std::string value;
    while (std::getline(stream, value, ',')) values.push_back(trim(value));
    return values;
}

static std::string jsonEscape(const std::string& value){
    std::string result;
    for (unsigned char c: value){
        switch (c){
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: if (c>= 32) result += static_cast<char>(c);
        }
    }
    return result;
}

//takes a wordsearch and formats it to json in this order: width, height, words (arr), letters(arr), solution(arr)
static std::string toJson(const Wordsearch& puzzle){
    std::ostringstream out;
    out << "{\"width\":" << puzzle.width << ", \"height\":" << puzzle.height <<",\"words\":[";
    for (size_t i = 0; i <puzzle.words.size(); ++i){
        if (i) out << ',';
        out << '\"' << jsonEscape(puzzle.words[i]) << '\"';
    }
    out << "], \"letters\":[";
    for (size_t row = 0; row < puzzle.letters.size(); ++row){
        if (row) out <<',';
        out <<'\"';
        for (char letter : puzzle.letters[row]) out << jsonEscape(std::string(1, letter));
        out << '\"';
    }
    out << "],\"solution\":[";
    for (size_t i = 0; i < puzzle.solution.size(); ++i){
        if (i) out << ',';
        out <<'[';
        for (size_t j = 0; j < puzzle.solution[i].size(); ++j){
            if (j) out << ',';
            out << '[' << puzzle.solution[i][j].first << ',' << puzzle.solution[i][j].second << ']';
        }
        out << ']';
    }
    out <<"]}";
    return out.str();
}

//Loads the csv given from front end validates and then converts to a wordsearch and then returns
static Wordsearch loadCsv(const std::string& csv){
    std::istringstream input(csv);
    std::string line;
    std::vector<std::vector<std::string>> rows;
    while (std::getline(input, line)){
        if (!line.empty() && line.back() == '\r') line.pop_back();
        rows.push_back(split(line));
    }
    if (rows.size() < 3 || rows[0].size() < 2 || rows[2].empty()){
        throw std::runtime_error("CSV must contain dimensions, a word row, and a letters row.");
    }
    size_t usedWidth = 0, usedHeight = 0;
    int width, height;
    try {
        width = std::stoi(rows[0][0], &usedWidth);
        height = std::stoi(rows[0][1], &usedHeight);
        if (usedWidth != rows[0][0].size() || usedHeight != rows[0][1].size()){
            throw std::runtime_error("Dimensions must be whole numbers.");
        } 
    }catch (const std::exception&) {
        throw std::runtime_error("CSV dimensions must be valid whole numbers.");
    }
    if (width < 1 || height < 1 || width > 50 || height > 50){
        throw std::runtime_error("Puzzle demensions must be between 1 and 50.");
        }

    std::vector<std::string> words;
    for (auto word: rows[1]){
        word = trim(word);
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ return static_cast<char>(std::tolower(c));});
        if (!word.empty()) words.push_back(word);
    }
    if (words.empty()) throw std::runtime_error("No words were found in the CSV.");
    std::string flatLetters;
    for (const auto& item: rows[2]){
        for (unsigned char c : item) if (!std::isspace(c)) flatLetters += static_cast<char>(std::tolower(c));
    }
    if (flatLetters.size() != static_cast<size_t>(width) * height){
        throw std::runtime_error("Expected " + std::to_string(width * height) + " letters, but found " + std::to_string(flatLetters.size()) +'.');
    }
    std::vector<std::vector<char>> letters(height, std::vector<char>(width));
    for (int row = 0; row < height; ++row){
        for (int col = 0; col < width; ++col){
            letters[row][col] = flatLetters[row*width + col];
        }
    }
    return Wordsearch(width, height, words, letters);
}


int main(){
    try{
        std::string mode;
        std::getline(std::cin, mode);
        Wordsearch puzzle(1, 1, {""}, {{' '}});
        if (mode == "csv"){
            std::ostringstream contents;
            contents << std::cin.rdbuf();
            puzzle = loadCsv(contents.str());
        } else if (mode == "random"){
            int width, height, count;
            if (!(std::cin >> width >> height >> count)) throw std::runtime_error("Random puzzle settings are incomplete.");
            if (width < 5 || width > 50 || height < 5 || height > 50|| count < 1 || count > 100){
                throw std::runtime_error("Width and height must be 5-50, and word count must be 1-100.");
            }
            puzzle = generateWS(width, height, {}, count);
        } else{
            throw std::runtime_error("Unknown request type.");
        }
        std::cout << toJson(puzzle);
    } catch (const std::exception& error){
        std::cout << "{\"error\":\"" << jsonEscape(error.what())<< "\"}";
    }
}