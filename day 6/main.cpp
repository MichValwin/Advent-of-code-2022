#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_set>

const char* INPUT_FILE = "bigboy.txt";
const static uint8_t ARRAY_CHECK_MAX = 128;

size_t getIndexCharRepeated(std::string line, uint32_t numCheckForChars) {
    size_t indexCheck = 0;
    uint8_t checksInserted = 0;
    std::unordered_set<char> charExisting;

    while(indexCheck < line.size()-numCheckForChars) {
        for(uint8_t i = 0; i < numCheckForChars; i++) {
            checksInserted += charExisting.insert(line[indexCheck + i]).second;
        }

        if(checksInserted == numCheckForChars)return indexCheck;

        charExisting.clear();
        checksInserted = 0;
        indexCheck++;
    }
    return 0;
}



std::string getCharsOnCount(uint8_t arrCharCounter[]) {
    std::string charsIn = "";
    for(int i = 0; i < ARRAY_CHECK_MAX; i++) {
        int numChars = arrCharCounter[i];
        for(int j = 0; j < numChars; j++) {
            charsIn += (char)i;
        }
    }
    return charsIn;
}

bool checkRepeatingCharsCount(uint8_t arrCharCounter[]) {
    for(int i = 0; i < ARRAY_CHECK_MAX; i++) {
        if(arrCharCounter[i] > 1)return true;
    }
    return false;
}

size_t getIndexCharRepeatedArray(std::string line, uint32_t numCheckForChars) {
    size_t indexCheck = 0;
    uint8_t arrCheck[ARRAY_CHECK_MAX] = {0};

    // Insert first
    for(; indexCheck < numCheckForChars; indexCheck++) {
        arrCheck[(uint8_t)line[indexCheck]] += 1;
    }
    if(!checkRepeatingCharsCount(arrCheck))return indexCheck;

    while(indexCheck < line.size()-numCheckForChars) {
        uint8_t charLeaving = (uint8_t)line[indexCheck-numCheckForChars];
        uint8_t charEntering = (uint8_t)line[indexCheck];
        arrCheck[charEntering] += 1;
        arrCheck[charLeaving] -= 1;

        if(!checkRepeatingCharsCount(arrCheck)) {
            return indexCheck;
        }

        indexCheck++;
    }

    return 0;
}

int main() {
	std::fstream inputFile(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}
    
    std::string line;
    while(std::getline(inputFile, line)) {
        size_t indexFourCharsNonRepeating = getIndexCharRepeatedArray(line, 4);
        std::cout << "Index first char non repeated: " << indexFourCharsNonRepeating << ", answer: " << indexFourCharsNonRepeating+1 << std::endl;

        size_t indexFourteen = getIndexCharRepeatedArray(line, 14);
        std::cout << "Index first char non repeated 14: " << indexFourteen << ", answer: " << indexFourteen+1  << std::endl;
    }
   
	return 0;
}