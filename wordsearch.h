#ifndef WORDSEARCH_H
#define WORDSEARCH_H
#include <vector>
#include <utility>
#include <string>
class Wordsearch{
    public:
        int width;
        int height;
        std::vector<std::string> words;
        std::vector<std::vector<char>> letters;

        Wordsearch(int w, int h, std::vector<std::string> a, std::vector<std::vector<char>> l ){
            width = w;
            height = h;
            words = a;
            letters = l;
        }
        friend std::ostream& operator<<(std::ostream& os, const Wordsearch& ws) {
            os << ws.width << " x " << ws.height << "\nwords: ";

            for (int i = 0; i < ws.words.size(); i++) {
                os << ws.words[i] << " ";
            }

            os << "\nletters:\n";

            for (int i = 0; i < ws.letters.size(); i++) {
                for (int j = 0; j < ws.letters[i].size(); j++) {
                    os << ws.letters[i][j] << " ";
                }
                os << "\n";
            }

            return os;
        }
        bool checkLetterLength() {
            if (letters.size() != height) {
                return false;
            }

            for (const auto& row : letters) {
                if (row.size() != width) {
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

                        while (k < word.size()) {
                            int row = i + k * rowStep;
                            int col = j + k * colStep;

                            if (row < 0 || row >= height ||
                                col < 0 || col >= width ||
                                letters[row][col] != word[k]) {
                                break;
                            }

                            k++;
                        }

                        if (k == word.size()) {
                            for (int n = 0; n < word.size(); n++) {
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
};


#endif