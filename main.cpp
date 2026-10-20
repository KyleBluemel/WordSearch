#include <iostream>
#include <string>
#include "reader.cpp"
#include "testprint.cpp"
#include "wordsearch.h"

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr <<"Usage: program <filename.csv>\n";
        return 1;
    }
    std::string filename = argv[1];
    auto data = reader(filename);
    testprint(data);
    return 0;

}

Wordsearch createClass(auto data){
    for (int i = 0; i < data.length(); i++){
        try{
            int width = std::stoi(data[0][0]);
            int height = std::stoi(data[0][1]);
        }
        catch(const std::exception& e){
            std::cout << e.what() << '\n';
        }
        
    }
}