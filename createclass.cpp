#include "wordsearch.h"
#include <string>
#include <vector>
#include <cctype>
Wordsearch createClass(auto data) {
    int width;
    int height;

    try {
        width = std::stoi(data[0][0]);
        height = std::stoi(data[0][1]);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        throw;
    }

    std::vector<std::string> words;

    for (const std::string& word : data[1]) {
        std::string lowerWord = word;

        for (char& letter : lowerWord) {
            letter = std::tolower(static_cast<unsigned char>(letter));
        }

        words.push_back(lowerWord);
    }

    std::vector<std::vector<char>> letters;

    for (int i = 0; i < height; i++) {
        std::vector<char> row;

        for (int j = 0; j < width; j++) {
            row.push_back(
                std::tolower(static_cast<unsigned char>(
                    data[2][0][i * width + j]
                ))
            );
        }

        letters.push_back(row);
    }

    Wordsearch ws(width, height, words, letters);
    return ws;
}