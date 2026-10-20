#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

//takes in a csv, splits file into items and returns items
std::vector<std::vector<std::string>> reader(const std::string& filename){
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;
    std::string line;
    
    if (!filename.ends_with(".csv")){
        std::cerr<<"Error: File must be a csv\n";
    }
    while (std::getline(file, line)){
        std::stringstream ss(line);
        std::vector<std::string> row;
        std::string item;

        while (std::getline(ss, item, ',')){
            row.push_back(item);
        }
        data.push_back(row);
    }
    return data;
}