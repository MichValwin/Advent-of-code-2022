#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_set>
#include <vector>
#include <deque>
#include <algorithm>
#include <queue>
#include <set>

#include <cstdlib>

static const char* INPUT_FILE = "input.txt";

// Helpers
struct SplitString{
    std::string delimiter;
    std::vector<std::string> strings;
};

SplitString splitString(std::string str, std::string delimiter) {
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

struct Position {
	int32_t x;
	int32_t y;
};

uint64_t toU64ForIndexing(const Position &p) {
	return ((uint64_t)p.x << 32) + (uint64_t)p.y;
}

void fromU64(uint64_t index, Position &p) {
    p.x = index >> 32;
    p.y = index;
}

void printMap(char* elevationMap, char* traversedMap, int32_t width, int32_t height, Position player, Position goal) {
    for(int32_t y = 0; y < height; y++) {
        for(int32_t x = 0; x < width; x++) {
            if(player.x == x && player.y == y) {
                std::cout << 'S';
            }else if(goal.x == x && goal.y == y) {
                std::cout << 'E';
            }else{
                if(traversedMap[y*width + x] == '#'){
                    // RED traversed
                    std::cout << "\x1B[31m" << elevationMap[y*width + x]  << "\033[0m";
                }else{
                    std::cout << elevationMap[y*width + x];
                }
            }
        }
        std::cout << std::endl;
    }
}

int32_t getElevation(const Position &p1, uint32_t width, const char* map) {
    return (int32_t)map[p1.y*width + p1.x];
}

int32_t distanceTo(const Position &p1, const Position &p2) {
    int32_t distance = std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
    return distance;
}


struct PositionTreeNode{
    Position currentPosition;
    Position parentNode;
};

uint32_t getShortestPathBFS(char* elevationMap, int32_t width, int32_t height, Position &player, Position &goal) {
    uint64_t* visitedChain = new uint64_t[width*height];
    for(int i = 0; i < width*height; i++)visitedChain[i] = 9999999999999999999U;

    uint32_t numSteps = 99999999;

    //std::cout << "Origin: " << player.x << ", " << player.y << std::endl;
    //std::cout << "End: " << goal.x << ", " << goal.y << std::endl;

    std::queue<Position> searchPositions;
    std::set<std::pair<int32_t, int32_t>> positionsVisited;
    
    char* mapVisited = new char[width*height];
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            mapVisited[i*width + j] = elevationMap[i*width + j];
        }
    }

    searchPositions.push(player);
    positionsVisited.insert(std::pair<int32_t, int32_t>(player.x, player.y));

    Position toMove[4];
    toMove[0] = Position{0, 1}; // UP
    toMove[1] = Position{1, 0}; // RIGHT
    toMove[2] = Position{0, -1}; // DOWN
    toMove[3] = Position{-1, 0}; // LEFT

    bool found = false;
    
    while(!searchPositions.empty()) {
        Position currentPos = searchPositions.front();
        searchPositions.pop();

        //std::cout << "Checking adjacents to: " << currentPos.x << ", " << currentPos.y << std::endl;
        //std::cout << "Size: " << positionsVisited.size() << std::endl;
        //std::cout << "Pos size to check: " << searchPositions.size() << ", num steps: " << numSteps<<   std::endl;

        if(currentPos.x == goal.x && currentPos.y == goal.y){
            //std::cout << "FOUND!" << std::endl;
            found = true;
            break;
        }

        // Check adjacent nodes
        for(int32_t i = 0; i < 4; i++) {
            Position nextPosition = {currentPos.x + toMove[i].x, currentPos.y + toMove[i].y};

            //std::cout << "NextPos to check: " << nextPosition.x << ", " << nextPosition.y<<   std::endl;
            if(positionsVisited.find(std::pair<int32_t, int32_t>(nextPosition.x, nextPosition.y)) == positionsVisited.end()) {
                // Bounds checking
                if(nextPosition.x >= 0 && nextPosition.x < width && nextPosition.y >= 0 && nextPosition.y < height) {
                    int32_t currentElevation = getElevation(currentPos, width, elevationMap);
                    int32_t nextElevation = getElevation(nextPosition, width, elevationMap);
                    int32_t diffElevation = std::abs(nextElevation-currentElevation);

                    if(diffElevation <= 1 || currentElevation > nextElevation) {
                        searchPositions.push(nextPosition);
                        positionsVisited.insert(std::pair<int32_t, int32_t>(nextPosition.x, nextPosition.y));
                        visitedChain[nextPosition.y*width + nextPosition.x] = toU64ForIndexing(currentPos);
                    }
                }
            }
        }

        mapVisited[currentPos.y*width + currentPos.x] = '#';
    }

    //printMap(elevationMap, mapVisited, width, height, player, goal);

    if(found){
        numSteps = 0;
        std::cout << " Llega aqui" << std::endl;
        Position nextPost = goal;
        while(nextPost.x != player.x || nextPost.y != player.y) {
            Position pos;
            fromU64(visitedChain[nextPost.y*width + nextPost.x], pos);
            nextPost = pos;
            numSteps++;
        }
    }
    
   
    delete[] mapVisited;
    delete[] visitedChain;
    

    return numSteps;
}


int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

    uint32_t lineWidth = 0;
    uint32_t lineHeight = 0;

    Position playerPosition;
    Position goalPosition;

    char* elevationMap;

    // Get line width
    std::string line;
    std::getline(inputFile, line);
    lineWidth = line.size();
    lineHeight++;
    // Get line height
    while(std::getline(inputFile, line)) {
        lineHeight++;
    }

    elevationMap = new char[lineWidth*lineHeight];

    // Load tree matrix
    inputFile.clear();
    inputFile.seekg(0, std::ios::beg);
    uint32_t lineProccessed = 0;
    while(std::getline(inputFile, line)) {
        for(size_t i = 0; i < line.size(); i++) {
            if(line[i] == 'S'){
                elevationMap[lineWidth * lineProccessed + i] = 'a';
                playerPosition = {(int32_t)i, (int32_t)lineProccessed};
            }else if(line[i] == 'E') {
                elevationMap[lineWidth * lineProccessed + i] = 'z';
                goalPosition = {(int32_t)i, (int32_t)lineProccessed};
            }else{
                elevationMap[lineWidth * lineProccessed + i] = line[i];
            }
        }
        lineProccessed++;
    }


    uint32_t shortestPathSteps = getShortestPathBFS(elevationMap, lineWidth, lineHeight, playerPosition, goalPosition);
    std::cout << "Silver: " << shortestPathSteps << std::endl;
    
    uint32_t shortestFromAnyA = 999999;
    for(int y = 0; y < (int32_t)lineHeight; y++) {
        for(int x = 0; x < (int32_t)lineWidth; x++) {
            if(elevationMap[y*lineWidth+x] == 'a'){
                Position start = {x, y};
                uint32_t pathTimes = getShortestPathBFS(elevationMap, lineWidth, lineHeight, start, goalPosition);
                std::cout << "Times: " << pathTimes << std::endl;
                if(pathTimes < shortestFromAnyA) shortestFromAnyA = pathTimes;
            }
        }
    }
    std::cout << shortestFromAnyA << std::endl;
    

    return 0;
}