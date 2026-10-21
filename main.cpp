#include <iostream>
#include <string>
#include "reader.cpp"
#include "testprint.cpp"
#include "wordsearch.h"
#include "createclass.cpp"



int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr <<"Usage: program <filename.csv>\n";
        return 1;
    }
    std::string filename = argv[1];
    auto data = reader(filename);
    Wordsearch ws = createClass(data);
    std::cout << ws << "\n";
    bool length = ws.checkLetterLength();
    std::cout << length <<"\n";
    auto positions = ws.checkSearch("class");

    if (positions.empty()) {
        std::cout << "Word not found\n";
    } else {
        for (const auto& [row, col] : positions) {
            std::cout << "(" << row << ", " << col << ")\n";
        }
    }

    std::cout << "word: " << ws.checkSearch("word").size() << '\n';
    std::cout << "search: " << ws.checkSearch("search").size() << '\n';
    std::cout << "code: " << ws.checkSearch("code").size() << '\n';
    std::cout << "class: " << ws.checkSearch("class").size() << '\n';
    std::cout << "cplusplus: " << ws.checkSearch("cplusplus").size() << '\n';
    std::cout << "vector: " << ws.checkSearch("vector").size() << '\n';
    std::vector<std::vector<std::pair<int, int>>> locations = ws.SolvePuzzle();
    for (const auto& word: locations){
        if (word.empty()){
            std::cout << "Word not found" << "\n";
        }
        else{
            for (const auto& address : word){
                std::cout <<"( " << address.first <<", " << address.second <<" ), ";
            }
            std::cout << "\n";
        }
    }
    return 0;

}

