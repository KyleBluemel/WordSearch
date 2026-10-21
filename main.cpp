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
    bool length = ws.checkLetterLength();

    std::vector<std::vector<std::pair<int, int>>> locations = ws.solution;
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

