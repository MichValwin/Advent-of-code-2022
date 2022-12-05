#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

const char* INPUT_FILE = "input.txt";

struct SplitString{
    char delimiter;
    std::vector<std::string> strings;
};

SplitString splitString(std::string str, char delimiter) {
    SplitString strings;
    strings.delimiter = delimiter;

    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        strings.strings.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    strings.strings.push_back(str.substr(start, end - start));

    return strings;
}


// Stacks from input
std::stack<char> supplyStacks[9] = {
    std::stack<char>({'W', 'R', 'F'}),
    std::stack<char>({'T', 'H', 'M', 'C', 'D', 'V', 'W', 'P'}),
    std::stack<char>({'P', 'M', 'Z', 'N', 'L'}),
    std::stack<char>({'J', 'C', 'H', 'R'}),
    std::stack<char>({'C', 'P', 'G', 'H', 'Q', 'T', 'B'}),
    std::stack<char>({'G', 'C', 'W', 'L', 'F', 'Z'}),
    std::stack<char>({'W', 'V', 'L', 'Q', 'Z', 'J', 'G', 'C'}),
    std::stack<char>({'P', 'N', 'R', 'F', 'W', 'T', 'V', 'C'}),
    std::stack<char>({'J', 'W', 'H', 'G', 'R', 'S', 'V'})
};

void moveSupply(size_t numberToMove, uint32_t supplyIndexStart, uint32_t supplyIndexEnd) {
    for(size_t i = 0; i < numberToMove; i++) {
        char movingBox = supplyStacks[supplyIndexStart].top();
        supplyStacks[supplyIndexStart].pop();
        supplyStacks[supplyIndexEnd].push(movingBox);
    }
}

void moveSupply9001(size_t numberToMove, uint32_t supplyIndexStart, uint32_t supplyIndexEnd) {
    std::vector<char> cache;
    for(size_t i = 0; i < numberToMove; i++) {
        char movingBox = supplyStacks[supplyIndexStart].top();
        supplyStacks[supplyIndexStart].pop();
        cache.push_back(movingBox);
    }

    while(!cache.empty()){
        supplyStacks[supplyIndexEnd].push(cache.back());
        cache.pop_back();
    }
}

std::string getTopBoxes() {
    std::string topBoxes = "";
    for(int i = 0; i < 9; i++) {
        topBoxes.push_back(supplyStacks[i].top());
    }
    return topBoxes;
}

int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}
    /*
    for(int i = 0; i < 9; i++) {
        std::cout << "NUmber: " << (int)(i+1) << std::endl;
        while(!supplyStacks[i].empty()) { 
            std::cout << supplyStacks[i].top() << std::endl;
            supplyStacks[i].pop();
        }
        std::cout << "-------------------" << std::endl;
    }
    */
    
    std::string line;
    while(std::getline(inputFile, line)) {
        size_t numberToMove;
        uint32_t supplyIndexStart;
        uint32_t supplyIndexEnd;

        int endStrIndex = line.find("move ") + 5;
        int initfromStrIndex = line.find(" from ");
        int inittoStrIndex = line.find(" to ");

        numberToMove = std::atoi(line.substr(endStrIndex, initfromStrIndex - endStrIndex).c_str());
        supplyIndexStart =  std::atoi(line.substr(initfromStrIndex+6, inittoStrIndex - (initfromStrIndex+6)).c_str());
        supplyIndexEnd =  std::atoi(line.substr(inittoStrIndex+4, line.size()-1).c_str());
      
        std::cout << numberToMove << " " << supplyIndexStart << " " << supplyIndexEnd << std::endl;

        //moveSupply(numberToMove, supplyIndexStart-1, supplyIndexEnd-1);
        moveSupply9001(numberToMove, supplyIndexStart-1, supplyIndexEnd-1);
    }

    std::cout << "Top boxes after rearrange: " << getTopBoxes() << std::endl;

	return 0;
}