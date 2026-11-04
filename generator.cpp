#include "wordsearch.h"
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <format>
#include <random>


std::random_device rd;
std::mt19937 gen(rd());

Wordsearch generateWS(int w, int h, std::vector<std::string> Words = {}, int numberOfWords = 0){
    int biggestPossible;
    if (w > h){
        biggestPossible = w;
    }
    else{
        biggestPossible = h;
    }
    std::vector<std::string>dictionary = createDictionary(biggestPossible); //creates vector of all of the possibe words > length 3 < length biggestPossible
    if (Words.empty()){
        if (numberOfWords == 0){
            throw std::invalid_argument("You have to supply a list of words or a number of words");
        }
        else{
            Words = generateWords(biggestPossible, dictionary, numberOfWords);
        }
    }
    else{
        for (std::string word : Words){
            if (word.length() > biggestPossible){
                throw std::invalid_argument(std::format("A word ({}) is bigger than the biggest possible length {}", word, biggestPossible));
            }
        }
    }
    
    createLetterList(h, w, Words);
}
std::vector<std::vector<char>> createLetterList(int h, int w, std::vector<std::string>Words){
    std::vector<std::string> Temp = Words;
    while (!Temp.empty()){
        int biggestWord = 0;
        int biggestIndex = 0;

        for (int i=0; i < Temp.size(); i++){
            if (Temp[i].length() > biggestWord){
                biggestWord = Temp[i].length();
                biggestIndex = i;
            }
        }
        std::string bigWord = Temp[biggestIndex];
        Temp.erase(Temp.begin() + biggestIndex);
        std::vector<std::vector<char>> grid(h, std::vector<char>(w, ' '));
        std::uniform_int_distribution<int> dist(0, h-1);
        int rH = dist(gen);
        std::uniform_int_distribution<int> dist(0, w-1);
        int rW = dist(gen);
        int directions[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                { 0, -1},           { 0, 1},
                { 1, -1}, { 1, 0}, { 1, 1}
            };
        works
        while(bool)
        std::uniform_int_distribution<int> dist(0, 7);
        int ranDir = directions[]


    }
}

std::vector<std::string> generateWords(int biggestPossible, const std::vector<std::string>& dictionary, int numberOfWords){
    std::vector<std::string> words;
    std::unordered_set<std::string> selected;
    

    std::uniform_int_distribution<int> dist(
        0, dictionary.size() - 1
    );
    while(words.size() < numberOfWords){
        std::string word = dictionary[dist(gen)];

        if (selected.insert(word).second){
            words.push_back(word);
        }
    }
    return words;
}

std::vector<std::string> createDictionary(int biggestPossible){
    std::string line;
    std::ifstream file("data/words_alpha.txt");
    std::vector<std::string> data;
    while (std::getline(file, line)){
        if (line.length() <=biggestPossible){
            if (line.length() > 3){
                data.push_back(line);
            }
        }
    }
    return data;
}