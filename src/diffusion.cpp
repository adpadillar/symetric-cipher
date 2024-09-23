#include "../include/diffusion.h"
#include <bitset>
#include <string>
#include <vector>

using namespace std;

size_t hexCharToValue(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9')
        return hexChar - '0';
    if (hexChar >= 'a' && hexChar <= 'f')
        return 10 + (hexChar - 'a');
    if (hexChar >= 'A' && hexChar <= 'F')
        return 10 + (hexChar - 'A');
    return 0;
}

std::string shuffleText(const std::string& text, const std::vector<size_t>& pattern) {
    std::string shuffled(text.size(), ' ');
    for (size_t i = 0; i < text.size(); ++i) {
        shuffled[pattern[i]] = text[i];
    }
    return shuffled;
}

std::string restoreText(const std::string& shuffledText, const std::vector<size_t>& pattern) {
    std::vector<size_t> reversePattern(pattern.size());
    for (size_t i = 0; i < pattern.size(); ++i) {
        reversePattern[pattern[i]] = i;
    }
    return shuffleText(shuffledText, reversePattern);
}

std::vector<size_t> generatePatternFromKey(const std::string& hexKey, size_t textSize) {
    std::vector<size_t> pattern(textSize);

    for (size_t i = 0; i < textSize; ++i) {
        pattern[i] = i;
    }

    size_t keyLength = hexKey.size();
    for (size_t i = 0; i < textSize; ++i) {
        size_t keyIndex = i % keyLength;
        size_t shift = hexCharToValue(hexKey[keyIndex]);
        std::swap(pattern[i], pattern[(i + shift) % textSize]); 
    }

    return pattern;
}

void diffuse_1(char message[], char key[], char output[]) {
    std::bitset<128> bloque;
    std::bitset<128> keyBits;

    // Convert message and key to bitsets
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            bloque[i*8 + j] = (message[i] >> j) & 1;
            keyBits[i*8 + j] = (key[i] >> j) & 1;
        }
    }

    std::bitset<128> bloque_con_key = bloque ^ keyBits;

    int perm_map[128] = {
        127, 120, 103, 115, 87, 45, 23, 91, 
        78, 22, 111, 65, 39, 72, 84, 9,
        101, 52, 13, 67, 96, 32, 14, 58,
        5, 11, 123, 20, 3, 90, 61, 79,
        106, 18, 76, 100, 46, 1, 55, 92,
        125, 6, 27, 44, 7, 81, 116, 35,
        10, 64, 68, 29, 19, 112, 28, 105,
        12, 34, 25, 53, 30, 16, 50, 66,
        57, 108, 97, 121, 15, 119, 40, 102,
        21, 98, 74, 37, 109, 54, 4, 26,
        24, 107, 2, 95, 93, 73, 0, 89,
        122, 36, 114, 49, 62, 33, 113, 88,
        80, 99, 42, 43, 38, 60, 83, 77,
        17, 85, 71, 59, 48, 47, 41, 56,
        124, 75, 118, 104, 31, 51, 70, 94,
        8, 82, 63, 117, 110, 86, 69, 126
    };

    std::bitset<128> permutado;
    for (int i = 0; i < 128; ++i) {
        permutado[i] = bloque_con_key[perm_map[i]];
    }

    // Convert permutado back to char array
    for (int i = 0; i < 16; ++i) {
        output[i] = 0;
        for (int j = 0; j < 8; ++j) {
            output[i] |= permutado[i*8 + j] << j;
        }
    }
}

void dediffuse_1(char cypher[], char key[], char output[]) {
    std::bitset<128> bloque;
    std::bitset<128> keyBits;

    // Convert cypher and key to bitsets
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 8; ++j) {
            bloque[i*8 + j] = (cypher[i] >> j) & 1;
            keyBits[i*8 + j] = (key[i] >> j) & 1;
        }
    }

    int perm_map[128] = {
        127, 120, 103, 115, 87, 45, 23, 91, 
        78, 22, 111, 65, 39, 72, 84, 9,
        101, 52, 13, 67, 96, 32, 14, 58,
        5, 11, 123, 20, 3, 90, 61, 79,
        106, 18, 76, 100, 46, 1, 55, 92,
        125, 6, 27, 44, 7, 81, 116, 35,
        10, 64, 68, 29, 19, 112, 28, 105,
        12, 34, 25, 53, 30, 16, 50, 66,
        57, 108, 97, 121, 15, 119, 40, 102,
        21, 98, 74, 37, 109, 54, 4, 26,
        24, 107, 2, 95, 93, 73, 0, 89,
        122, 36, 114, 49, 62, 33, 113, 88,
        80, 99, 42, 43, 38, 60, 83, 77,
        17, 85, 71, 59, 48, 47, 41, 56,
        124, 75, 118, 104, 31, 51, 70, 94,
        8, 82, 63, 117, 110, 86, 69, 126
    };

    std::bitset<128> unpermuted;
    for (int i = 0; i < 128; ++i) {
        unpermuted[perm_map[i]] = bloque[i];
    }

    std::bitset<128> original = unpermuted ^ keyBits;

    // Convert original back to char array
    for (int i = 0; i < 16; ++i) {
        output[i] = 0;
        for (int j = 0; j < 8; ++j) {
            output[i] |= original[i*8 + j] << j;
        }
    }
}




// Implementation of diffuse_2 using the algorithm from difusion_2.cpp
void diffuse_2(char message[], char key[], char output[]) {
    std::string messageStr(message, 16);
    std::string keyStr(key, 16);

    std::vector<size_t> pattern = generatePatternFromKey(keyStr, 16);
    std::string shuffled = shuffleText(messageStr, pattern);

    for (int i = 0; i < 16; ++i) {
        output[i] = shuffled[i];
    }
}

// Implementation of dediffuse_2 (reverse of diffuse_2)
void dediffuse_2(char cypher[], char key[], char output[]) {
    std::string cypherStr(cypher, 16);
    std::string keyStr(key, 16);

    std::vector<size_t> pattern = generatePatternFromKey(keyStr, 16);
    std::string restored = restoreText(cypherStr, pattern);

    for (int i = 0; i < 16; ++i) {
        output[i] = restored[i];
    }
}
