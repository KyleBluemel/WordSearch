#include "wordsearch.h"
#include <fstream>
#include <unordered_set>
#include <sstream>
#include <string>
#include <algorithm>
#include <format>
#include <random>
#include <cstddef>


std::random_device rd;
std::mt19937 gen(rd());
std::vector<char> alpha = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

bool makesUnwantedWord( const std::vector<std::vector<char>>& letters, int row, int col, const std::unordered_set<std::string>& dictionary, const std::unordered_set<std::string>& allowedWords);
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
            Words = generateWords(dictionary, numberOfWords);
        }
    }
    else{
        for (std::string word : Words){
            if (word.length() > static_cast<size_t>(biggestPossible)){
                throw std::invalid_argument(std::format("A word ({}) is bigger than the biggest possible length {}", word, biggestPossible));
            }
        }
    }
    
    std::vector<std::vector<char>> letters = createLetterList(h, w, Words, dictionary);

    return (Wordsearch(w, h, Words, letters));
}
std::vector<std::vector<char>> createLetterList(int h, int w, std::vector<std::string>Words, const std::vector<std::string>& dictionary){
    std::sort(Words.begin(), Words.end(),
        [](const std::string& a, const std::string& b) {
            return b.length() < a.length();
        }
    );// sort words biggest to smallest
    std::vector<std::vector<char>> letterList(h, std::vector<char>(w, ' ')); //making the grid with only spaces
    int directions[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                { 0, -1},           { 0, 1},
                { 1, -1}, { 1, 0}, { 1, 1}
            };
    for (const auto& word : Words){
        bool wordInserted = false;
        while(!wordInserted){
            std::uniform_int_distribution<int> distH(0, h-1);
            std::uniform_int_distribution<int> distW(0, w-1);
            int posH = distH(gen);
            int posW = distW(gen);
            std::uniform_int_distribution<int> dist(0, 7);
            int ranDir = dist(gen);
            int randX = directions[ranDir][0];
            int randY = directions[ranDir][1];
            bool fits = true;
            for (size_t i = 0; i <word.length(); i ++){
                int checkH = posH + (i*randY);
                int checkW = posW + (i*randX);
                if (checkH < 0 || checkH >=h || checkW < 0 || checkW >= w){
                    fits = false;
                    break;
                }
                if (letterList[checkH][checkW] != ' ' && letterList[checkH][checkW] != word[i]){
                    fits = false;
                    break;
                }
            }
            if (fits){
                for (size_t i = 0; i < word.length(); i++){
                    int insH = posH +(i*randY);
                    int insW = posW + (i*randX);
                    letterList[insH][insW] = word[i];
                }
                wordInserted = true;
            }
        }
    }
    std::unordered_set<std::string> dictionarySet(dictionary.begin(), dictionary.end());
    std::unordered_set<std::string> allowedWords(Words.begin(), Words.end());

    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            if (letterList[i][j] == ' '){
                std::vector<char> candidates = alpha;
                std::shuffle(candidates.begin(), candidates.end(), gen);

                bool letterAccepted = false;
                for (char candidate : candidates){
                    letterList[i][j] = candidate;

                    if (!makesUnwantedWord(letterList, i, j, dictionarySet, allowedWords)){
                        letterAccepted = true;
                        break;
                    }
                }
                if (!letterAccepted){
                    throw std::runtime_error("Unable to fill puzzle without making an unwanted word");
                }
            }
        }
    }
    return letterList;
}

std::vector<std::string> generateWords(const std::vector<std::string>& dictionary, int numberOfWords){
    std::vector<std::string> words;
    std::unordered_set<std::string> selected;
    

    std::uniform_int_distribution<int> dist(
        0, dictionary.size() - 1
    );
    while(words.size() < static_cast<size_t>(numberOfWords)){
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
        if (line.length() <= static_cast<size_t>(biggestPossible)){
            if (line.length() > 3){
                data.push_back(line);
            }
        }
    }
    return data;
}


bool makesUnwantedWord(const std::vector<std::vector<char>>& letters, int row, int col, const std::unordered_set<std::string>& dictionary, const std::unordered_set<std::string>& allowedWords){
    int directions[4][2] = {
        {0, 1}, {1, 0}, {1, 1}, {1, -1}
    };
    int height = letters.size();
    int width = letters[0].size();

    for (auto& direction : directions){
        int dr = direction[0];
        int dc = direction[1];

        int r = row;
        int c = col;
        while (r >= 0 && r < height && c>= 0 && c < width && letters[r][c] != ' '){
            r -= dr;
            c -= dc;
        }

        r += dr;
        c += dc;

        std::string line;
        size_t center = 0;

        while (r >= 0 && r < height && c >= 0 && c < width && letters[r][c] != ' '){
            if (r == row && c == col){
                center = line.size();
            }
            line += letters[r][c];

            r += dr;
            c += dc;
        }

        for (size_t start = 0; start <= center; start++){
            for (size_t end = center; end < line.size(); end++){
                if (end-start + 1 < 4){
                    continue;
                }

                std::string word = line.substr(start, end - start + 1);

                if (dictionary.count(word) && !allowedWords.count(word)){
                    return true;
                }

                std::reverse(word.begin(), word.end());

                if (dictionary.count(word) && !allowedWords.count(word)){
                    return true;
                }
            }
        }
    }
    return false;
}