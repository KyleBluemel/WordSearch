#include <iostream>
#include <vector>
#include <string>

//testprint data
void testprint(std::vector<std::vector<std::string>>& data){
    for (const auto& row : data){
        for (const auto& item : row){
            std::cout << item <<" ";
        }
        std::cout <<"\n";
    }
}