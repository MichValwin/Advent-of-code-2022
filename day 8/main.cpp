#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_set>


static const char* INPUT_FILE = "odu_.txt";


void printTreeMatrix(uint8_t* treeMatrix, int32_t width, int32_t height) {
    for(int32_t i = 0; i < height; i++) {
        for(int32_t j = 0; j < width; j++) {
            std::cout << (int32_t)treeMatrix[i*width + j];
        }
        std::cout << std::endl;
    }
}

bool isTreeVisible(uint8_t* treeMatrix, int32_t x, int32_t y, int32_t width, int32_t height) {
    uint8_t treeValue = treeMatrix[y*width + x];

    bool visible = true;
    // Left
    for(int i = x-1; i >= 0; i--) {
        if(treeMatrix[y*width + i] >= treeValue){
            visible = false;
            break;
        }
    }
    if(visible) return true;
    
    visible = true;
    // Right
    for(int i = x+1; i < width; i++) {
        if(treeMatrix[y*width + i] >= treeValue){
            visible = false;
            break;
        }
    }
    if(visible) return true;

    visible = true;
    // Top
    for(int i = y-1; i >= 0; i--) {
        if(treeMatrix[i*width +x] >= treeValue){
            visible = false;
            break;
        }
    }
    if(visible) return true;

    visible = true;
    // Bottom
    for(int i = y+1; i < height; i++) {
        if(treeMatrix[i*width +x] >= treeValue){
            visible = false;
            break;
        }
    }
    if(visible) return true;

    return false;
}

uint32_t getTreesVisibles(uint8_t* treeMatrix, int32_t width, int32_t height) {
    uint32_t treesVisible = width*2 + height*2 -4;

    // Only check for middle trees
    for(int32_t i = 1; i < height - 1; i++) {
        for(int32_t j = 1; j < width - 1; j++) {
            if(isTreeVisible(treeMatrix, j, i, width, height)){
                //std::cout << "Tree visible: " << (int32_t)treeMatrix[i*width + j] << std::endl;
                treesVisible++;
            }
        }
    }

    return treesVisible;
}

uint64_t getScenicScore(uint8_t* treeMatrix, int32_t x, int32_t y, int32_t width, int32_t height) {
    uint8_t treeValue = treeMatrix[y*width + x];
    uint64_t scenicScore = 1;

    int treesVisible = 0;

    // Left
    for(int i = 1; x-i >= 0; i++) {
        treesVisible++;
        if(treeMatrix[y*width + x-i] >= treeValue){
            break;
        }
    }
    scenicScore *= treesVisible;

    treesVisible = 0;
    // Right
    for(int i = 1; i+x < width; i++) {
        treesVisible++;
        if(treeMatrix[y*width + x+i] >= treeValue){
            break;
        }
    }
    scenicScore *= treesVisible;

    treesVisible = 0;
    // Top
    for(int i = 1; y-i >= 0; i++) {
        treesVisible++;
        if(treeMatrix[(y-i)*width + x] >= treeValue){
            break;
        }
    }
    scenicScore *= treesVisible;

    treesVisible = 0;
    // Bottom
    for(int i = 1; i+y < height; i++) {
        treesVisible++;
        if(treeMatrix[(y+i)*width + x] >= treeValue){
            break;
        }
    }
    scenicScore *= treesVisible;
    
   
    return scenicScore;
}

uint64_t sumScenicScore(uint8_t* treeMatrix, int32_t width, int32_t height) {
    uint64_t scenicScoreMax = 0;
    
    // Only check for middle trees
    for(int32_t i = 1; i < height - 1; i++) {
        for(int32_t j = 1; j < width - 1; j++) {
            //std::cout << "num check: " << (int32_t)treeMatrix[i*width + j] << ", on X: " << j << ", Y: " << i <<  std::endl;
            uint64_t scenicScore = getScenicScore(treeMatrix, j, i, width, height);
            if(scenicScore > scenicScoreMax)scenicScoreMax = scenicScore;
        }
    }

    return scenicScoreMax;
}

int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

    int32_t lineWidth = 0;
    int32_t lineHeight = 0;

    // Get line width
    std::string line;
    std::getline(inputFile, line);
    lineWidth = line.size();
    lineHeight++;

    // Get line height
    while(std::getline(inputFile, line)) {
        lineHeight++;
    }
    
    uint8_t* treeMatrix = new uint8_t[lineWidth * lineHeight];

    std::cout << "Width: " << lineWidth << ", height: " << lineHeight << std::endl;

    // Load tree matrix
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    uint32_t lineProccessed = 0;
    while(std::getline(inputFile, line)) {
        for(size_t i = 0; i < line.size(); i++) {
            treeMatrix[lineWidth * lineProccessed + i] = line[i] - '0';
        }
       
        lineProccessed++;
    }

   

    // print
    //printTreeMatrix(treeMatrix, lineWidth, lineHeight);
    
    std::cout << "Silver: " << getTreesVisibles(treeMatrix, lineWidth, lineHeight) << std::endl;
    std::cout << "Gold: " << sumScenicScore(treeMatrix, lineWidth, lineHeight) << std::endl;
   
    //uint64_t scenicScore = getScenicScore(treeMatrix, 2, 1, lineWidth, lineHeight);

    delete[] treeMatrix;

	return 0;
}