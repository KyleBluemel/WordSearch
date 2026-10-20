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
    auto positions = ws.checkSearch("code");

    if (positions.empty()) {
        std::cout << "Word not found\n";
    } else {
        for (const auto& [row, col] : positions) {
            std::cout << "(" << row << ", " << col << ")\n";
        }
    }
    return 0;

}

