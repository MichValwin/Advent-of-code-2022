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

struct Position{
    int32_t x;
    int32_t y;
    int32_t z;

    bool operator<(const Position &p2) const {
        return x < p2.x || y < p2.y || z < p2.z; 
    }

    std::string toString() const{
        return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
    } 

    void print() const {
        std::cout << "x: " << x << ", y: " << y << ", z: " << z;
    }
};

// Helpers
std::vector<std::string> splitString(const std::string &str, const std::string &delimiter) {
    std::vector<std::string> strings;

    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        strings.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    strings.push_back(str.substr(start, end - start));

    return strings;
}

static const Position POSITIONS_SIDE[] {
    {1,0,0},    // +x
    {-1,0,0},   // -x
    {0,1,0},    // +y
    {0,-1,0},   // -y
    {0,0,1},    // +z
    {0,0,-1},   // -z
};

bool positionIsInMap(int32_t width, int32_t height, int32_t depth, const Position* posToCheck) {
    if(posToCheck->x >= 0 && posToCheck->x < width &&
    posToCheck->y >= 0 && posToCheck->y < height &&
    posToCheck->z >= 0 && posToCheck->z < depth) {
        return true;
    }
    return false;
}

bool positionIsCoveredByCube(const bool* mapPositions, int32_t width, int32_t height, int32_t depth, const Position* posToCheck) {
    if(positionIsInMap(width, height, depth, posToCheck)) {
        bool posIsOccupied = mapPositions[posToCheck->x + (posToCheck->y * width) + (posToCheck->z * width * height)];
        if(posIsOccupied == 1)return true;
    }
    return false;
}

int32_t countPositionsConnected(const bool* mapPositions, int32_t width, int32_t height, int32_t depth, const Position* posCubeToCheck) {
    int32_t sidesConnected = 0;
    for(int i = 0; i < 6; i++) {
        // Check each side
        Position side = POSITIONS_SIDE[i];
        Position toCheck = {posCubeToCheck->x + side.x, posCubeToCheck->y + side.y, posCubeToCheck->z + side.z};
        if(positionIsCoveredByCube(mapPositions, width, height, depth, &toCheck)){
            sidesConnected++;
        }
    }
    return sidesConnected;
}

int32_t getAllPositionsOuter(const bool* mapPositions, int32_t width, int32_t height, int32_t depth) {
    Position firstPosition = {0,0,0};

    std::unordered_set<std::string> unq;
    std::queue<Position> queue;

    queue.push(firstPosition);
    unq.insert(firstPosition.toString());

    int32_t wallsCoveredByWater = 0;

    while(!queue.empty()) {
        Position currentPos = queue.front();
        queue.pop();

        // Get adjacents
        for(int i = 0; i < 6; i++) {
            Position nextPos = {currentPos.x + POSITIONS_SIDE[i].x,
                                currentPos.y + POSITIONS_SIDE[i].y,
                                currentPos.z + POSITIONS_SIDE[i].z};
           
            if(positionIsInMap(width, height, depth, &nextPos)){
                if(!positionIsCoveredByCube(mapPositions, width, height, depth, &nextPos)){
                    if(unq.find(nextPos.toString()) == unq.end()){ // if it doesn't exist
                        queue.push(nextPos);
                        unq.insert(nextPos.toString());
                    }
                }else{
                    wallsCoveredByWater++;
                }
            }
        }
    }

    return wallsCoveredByWater;
}

int main() {
    std::fstream inputFile = std::fstream(INPUT_FILE);

    if(!inputFile.is_open()) {
        std::cout << "Can't open " << INPUT_FILE << std::endl;
        exit(0);
    }

    std::vector<Position> positionsCubes;
    std::vector<int32_t> sidesOccupied;

    int32_t width = 0;
    int32_t height = 0;
    int32_t depth = 0;
    bool* mapPositions;


    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        std::vector<std::string> cubePosStr = splitString(line, ",");
        Position cubePos = {std::atoi(cubePosStr[0].c_str()), std::atoi(cubePosStr[1].c_str()), std::atoi(cubePosStr[2].c_str())};
        
        //offset by 2 (there are positions that start by 0, not in example)
        cubePos.x += 2;
        cubePos.y += 2;
        cubePos.z += 2;

        if(cubePos.x > width)width = cubePos.x;
        if(cubePos.y > height)height = cubePos.y;
        if(cubePos.z > depth)depth = cubePos.z;
        positionsCubes.push_back(cubePos);
    }

    width += 4;
    height += 4;
    depth += 4;
    std::cout << "Width: " << width << ", height: " << height << ", depth: " << depth << "\n";
    mapPositions = new bool[width*height*depth];
    for(int32_t i = 0; i < width*height*depth; i++)mapPositions[i] = 0;
    std::cout << "Total size of map: " << width*height*depth << "\n";

    // Put on map
    for(Position &pos: positionsCubes) {
        mapPositions[pos.x + (pos.y * width) + (pos.z * width * height)] = 1;
    }

    // Check sides
    for(Position &pos: positionsCubes) {
        int32_t sidesOcc = countPositionsConnected(mapPositions, width, height, depth, &pos);
        sidesOccupied.push_back(sidesOcc);
    }

    // Print
    for(size_t i = 0; i < positionsCubes.size(); i++) {
        //positionsCubes[i].print();
        //std::cout << " Sides occ: " << sidesOccupied[i];
        //std::cout << "\n";
    }

    // Surface area
    int32_t surfaceArea = positionsCubes.size() * 6;
    for(size_t i = 0; i < sidesOccupied.size(); i++) {
        surfaceArea -= sidesOccupied[i];
    }


    std::cout << "Total surface area: " << surfaceArea << "\n";
    std::cout << "All sides touching water: " << getAllPositionsOuter(mapPositions, width,height, depth)  << "\n";

    return 0;
}