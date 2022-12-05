#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

const char* INPUT_FILE = "bigboy.txt";

std::vector<char>* cratesVector;
std::stack<char>* cratesStack1;
std::stack<char>* cratesStack2;
size_t numberOfStacks = 0;

void moveSupply(std::stack<char>* crates, size_t numberToMove, uint32_t supplyIndexStart, uint32_t supplyIndexEnd) {
    for(size_t i = 0; i < numberToMove; i++) {
        char movingBox = crates[supplyIndexStart].top();
        crates[supplyIndexStart].pop();
        crates[supplyIndexEnd].push(movingBox);
    }
}

void moveSupply9001(std::stack<char>* crates, size_t numberToMove, uint32_t supplyIndexStart, uint32_t supplyIndexEnd) {
    std::vector<char> cache;
    for(size_t i = 0; i < numberToMove; i++) {
        char movingBox = crates[supplyIndexStart].top();
        crates[supplyIndexStart].pop();
        cache.push_back(movingBox);
    }

    while(!cache.empty()){
        crates[supplyIndexEnd].push(cache.back());
        cache.pop_back();
    }
}

void initStacks(std::stack<char>* crates, std::vector<char>* cratesVector) {
    for(size_t i = 0; i < numberOfStacks; i++) {
        for(int64_t j = cratesVector[i].size()-1; j >= 0; j--){
            crates[i].push(cratesVector[i][j]); 
        }
    }
}

std::string getTopBoxes(std::stack<char>* crates) {
    std::string topBoxes = "";
    for(size_t i = 0; i < numberOfStacks; i++) {
        topBoxes.push_back(crates[i].top());
    }
    return topBoxes;
}

int main() {
	std::fstream inputFile(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}
    
    std::string line;

    // Get line index
    size_t indexFinishCrates = 0;
    size_t indexStartCommands = 0;
    size_t lineIndex = 0;
    
    while(std::getline(inputFile, line)) {
        if(line.size() > 2 && std::isdigit(line.c_str()[1])) {
            indexFinishCrates = lineIndex;
            numberOfStacks = (size_t)std::ceil(line.size() / 4.0);
        }

        if(line.size() > 2 && line.substr(0, 4).compare("move") == 0) {
            indexStartCommands = inputFile.tellg();
            indexStartCommands -= line.size();
            break;
        }
        lineIndex++;
    }
    std::cout << "Number of stacks " << numberOfStacks << std::endl;
    cratesVector = new std::vector<char>[numberOfStacks];

    inputFile.clear();
    inputFile.seekg(0);
    lineIndex = 0;
    // Parse crates
    while(std::getline(inputFile, line)) {
        for(size_t i = 0; i < numberOfStacks; i++) {
            if(line.c_str()[i*4+1] != ' ')cratesVector[i].push_back(line.c_str()[i*4+1]);
        }

        if(lineIndex == indexFinishCrates-1)break;
        lineIndex++;
    }

    cratesStack1 = new std::stack<char>[numberOfStacks];
    cratesStack2 = new std::stack<char>[numberOfStacks];
    initStacks(cratesStack1, cratesVector);
    initStacks(cratesStack2, cratesVector);

    std::cout << "Top boxes before rearrange: " << getTopBoxes(cratesStack1) << std::endl;

    inputFile.clear();
    inputFile.seekg(indexStartCommands, std::ios::beg);
    while(std::getline(inputFile, line)) {
        if(line.empty())continue;

        size_t numberToMove;
        uint32_t supplyIndexStart;
        uint32_t supplyIndexEnd;

        int endStrIndex = line.find("move ") + 5;
        int initfromStrIndex = line.find(" from ");
        int inittoStrIndex = line.find(" to ");

        numberToMove = std::atoi(line.substr(endStrIndex, initfromStrIndex - endStrIndex).c_str());
        supplyIndexStart =  std::atoi(line.substr(initfromStrIndex+6, inittoStrIndex - (initfromStrIndex+6)).c_str());
        supplyIndexEnd =  std::atoi(line.substr(inittoStrIndex+4, line.size()-1).c_str());
      
        //std::cout << numberToMove << " " << supplyIndexStart << " " << supplyIndexEnd << std::endl;

        moveSupply(cratesStack1, numberToMove, supplyIndexStart-1, supplyIndexEnd-1);
        moveSupply9001(cratesStack2, numberToMove, supplyIndexStart-1, supplyIndexEnd-1);
    }

    std::cout << "Top boxes after rearrange SILVER: " << getTopBoxes(cratesStack1) << std::endl;
    std::cout << "Top boxes after rearrange GOLD: " << getTopBoxes(cratesStack2) << std::endl;

    delete[] cratesStack1;
    delete[] cratesStack2;
    delete[] cratesVector;

	return 0;
}