#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <set>

const char* INPUT_FILE = "input.txt";





size_t getIndexFirstFourRepeated(std::string line) {
    size_t indexCheck = 0;
    uint8_t checksInserted = 0;
    std::set<char> charExisting;

    while(indexCheck < line.size()-4) {
        charExisting.insert(line[indexCheck]);
        checksInserted += charExisting.insert(line[indexCheck+1]).second;
        checksInserted += charExisting.insert(line[indexCheck+2]).second;
        checksInserted += charExisting.insert(line[indexCheck+3]).second;

        if(checksInserted == 3){
            for(char a: charExisting){
                std::cout << (char)a;
            }
            std::cout << std::endl;
            return indexCheck;
        }

        charExisting.clear();
        checksInserted = 0;
        indexCheck++;
    }
    return 0;
}

size_t getIndexFirstFourteeeeenRepeated(std::string line) {
    size_t indexCheck = 0;
    uint8_t checksInserted = 0;
    std::set<char> charExisting;

    while(indexCheck < line.size()-4) {
        charExisting.insert(line[indexCheck]);
        checksInserted += charExisting.insert(line[indexCheck+1]).second;
        checksInserted += charExisting.insert(line[indexCheck+2]).second;
        checksInserted += charExisting.insert(line[indexCheck+3]).second;
        checksInserted += charExisting.insert(line[indexCheck+4]).second;
        checksInserted += charExisting.insert(line[indexCheck+5]).second;
        checksInserted += charExisting.insert(line[indexCheck+6]).second;
        checksInserted += charExisting.insert(line[indexCheck+7]).second;
        checksInserted += charExisting.insert(line[indexCheck+8]).second;
        checksInserted += charExisting.insert(line[indexCheck+9]).second;
        checksInserted += charExisting.insert(line[indexCheck+10]).second;
        checksInserted += charExisting.insert(line[indexCheck+11]).second;
        checksInserted += charExisting.insert(line[indexCheck+12]).second;
        checksInserted += charExisting.insert(line[indexCheck+13]).second;

        if(checksInserted == 13){
            for(char a: charExisting){
                std::cout << (char)a;
            }
            std::cout << std::endl;
            return indexCheck;
        }

        charExisting.clear();
        checksInserted = 0;
        indexCheck++;
    }
    return 0;
}

/*
static const uint32_t NUM_COMPARE_CHARS = 256;

uint8_t arrCompareChars[NUM_COMPARE_CHARS] = {0};

void clearArrCompare() {
    for(uint32_t i = 0; i < NUM_COMPARE_CHARS; i++) {
        arrCompareChars[i] += 1;
    }
}

uint32_t getIndexMostRepetition() {
    uint32_t index = 0;
    uint32_t mostRepeated = 0;
    for(uint32_t i = 0; i < NUM_COMPARE_CHARS; i++) {
       if(arrCompareChars[i] > mostRepeated){
            mostRepeated = arrCompareChars[i];
            index = i;
       }
    }
    return index;
}

size_t getIndexFirstFourRepeated(std::string input) {
    size_t indexCheck = 0;
    while(indexCheck < input.size()-4) {
        arrCompareChars[(uint8_t)input[indexCheck]] += 1;
        arrCompareChars[(uint8_t)input[indexCheck+1]] += 1;
        arrCompareChars[(uint8_t)input[indexCheck+2]] += 1;
        arrCompareChars[(uint8_t)input[indexCheck+3]] += 1;

        uint32_t mostRepeatedCharIndex = getIndexMostRepetition();
        if(arrCompareChars[mostRepeatedCharIndex] == 1) {
            return indexCheck;
        }

        clearArrCompare();
        indexCheck++;
    }
    return 0;
}

*/

int main() {
	std::fstream inputFile(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}
    
    std::string line;
    while(std::getline(inputFile, line)) {
        size_t indexFourCharsNonRepeating = getIndexFirstFourRepeated(line);
        std::cout << "Index first char non repeated: " << indexFourCharsNonRepeating << std::endl;
        std::cout << "Chars: " << line.substr(indexFourCharsNonRepeating, 4) << std::endl;

        size_t indexFourteen = getIndexFirstFourteeeeenRepeated(line);
        std::cout << "Index first char non repeated 14: " << indexFourteen << std::endl;
    }
   
	return 0;
}