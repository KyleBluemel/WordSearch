#ifndef WORDSEARCH_H
#define WORDSEARCH_H
#include <vector>
#include <utility>
#include <iostream>
#include <string>
#include <cstddef>


std::vector<std::vector<char>> createLetterList(int h, int w, std::vector<std::string> Words, const std::vector<std::string>& dictionary);

std::vector<std::string> generateWords(const std::vector<std::string>& dictionary, int numberOfWords);

std::vector<std::string> createDictionary(int biggestPossible);
class Wordsearch{
    public:
        int width;
        int height;
        std::vector<std::string> words;
        std::vector<std::vector<char>> letters;
        std::vector<std::vector<std::pair<int, int>>> solution;
        

        Wordsearch(int w, int h, std::vector<std::string> a, std::vector<std::vector<char>> l ){
            width = w;
            height = h;
            words = a;
            letters = l;

            SolvePuzzle();
        }
        
        friend std::ostream& operator<<(std::ostream& os, const Wordsearch& ws) {
            os << ws.width << " x " << ws.height << "\nwords: ";

            for (size_t i = 0; i < ws.words.size(); i++) {
                os << ws.words[i] << " ";
            }

            os << "\nletters:\n";

            for (size_t i = 0; i < ws.letters.size(); i++) {
                for (size_t j = 0; j < ws.letters[i].size(); j++) {
                    os << ws.letters[i][j] << " ";
                }
                os << "\n";
            }

            return os;
        }
        
        bool checkLetterLength() {
            if (letters.size() != static_cast<size_t>(height)) {
                return false;
            }

            for (const auto& row : letters) {
                if (row.size() != static_cast<size_t>(width)) {
                    return false;
                }
            }

            return true;
        }
        
        std::vector<std::pair<int, int>> checkSearch(std::string word) {
            std::vector<std::pair<int, int>> positions;

            if (!checkLetterLength() || word.empty()) {
                return positions;
            }

            int directions[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                { 0, -1},           { 0, 1},
                { 1, -1}, { 1, 0}, { 1, 1}
            };

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    if (letters[i][j] != word[0]) {
                        continue;
                    }

                    for (auto& direction : directions) {
                        int rowStep = direction[0];
                        int colStep = direction[1];

                        int k = 0;

                        while (static_cast<size_t>(k) < word.size()) {
                            int row = i + k * rowStep;
                            int col = j + k * colStep;

                            if (row < 0 || row >= height ||
                                col < 0 || col >= width ||
                                letters[row][col] != word[k]) {
                                break;
                            }

                            k++;
                        }

                        if (static_cast<size_t>(k) == word.size()) {
                            for (size_t n = 0; n < word.size(); n++) {
                                positions.push_back({
                                    i + n * rowStep,
                                    j + n * colStep
                                });
                            }

                            return positions;
                        }
                    }
                }
            }

            return positions;
        }
        void SolvePuzzle(){
            for (size_t i = 0; i < words.size(); i++){
                solution.push_back(checkSearch(words[i]));
            }
        }

    };


#endif