#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <set>

const char* INPUT_FILE = "input.txt";

size_t getIndexCharRepeated(std::string line, uint32_t numCheckForChars) {
    size_t indexCheck = 0;
    uint8_t checksInserted = 0;
    std::set<char> charExisting;

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

int main() {
	std::fstream inputFile(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}
    
    std::string line;
    while(std::getline(inputFile, line)) {
        size_t indexFourCharsNonRepeating = getIndexCharRepeated(line, 4) +4;
        std::cout << "Index first char non repeated: " << indexFourCharsNonRepeating << std::endl;

        size_t indexFourteen = getIndexCharRepeated(line, 14) +14;
        std::cout << "Index first char non repeated 14: " << indexFourteen << std::endl;
    }
   
	return 0;
}