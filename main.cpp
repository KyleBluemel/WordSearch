#include <iostream>
#include <string>
#include "reader.cpp"
#include "testprint.cpp"
#include "wordsearch.h"
#include "createclass.cpp"
#include "generator.cpp"



int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr <<"Usage: program <filename.csv>\n";
        return 1;
    }
    std::string filename = argv[1];
    auto data = reader(filename);
    Wordsearch ws = createClass(data);

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
    int width = 20;
    int height = 40;
    Wordsearch genWS = generateWS(width, height, {"Animals", "Dogs", "Cats", "Orange"});
    std::cout<< "Generated Wordsearch: " << "\n" << "\n";
    locations = genWS.solution;
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
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            std::cout << genWS.letters[i][j]<< " ";
        }
        std::cout << "\n";
    }
    return 0;

}

