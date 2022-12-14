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

SplitString splitString(const std::string &str, const std::string &delimiter) {
    SplitString strings;
    strings.delimiter = delimiter;

    int start = 0;
    int end = str.find(delimiter);
    while (end != -1) {
        strings.strings.push_back(str.substr(start, end - start));
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    strings.strings.push_back(str.substr(start, end - start));

    return strings;
}

void testSplit(const std::string &str, const std::string &delimiter, const std::string &numTest) {
    SplitString split = splitString(str, delimiter);
    std::cout << numTest << " ";
    for(size_t i = 0; i < split.strings.size(); i++){
       std::cout << i << ": #" << split.strings[i] << "#  ";

    }
    std::cout << "\n";
}

struct Position{
    int32_t x;
    int32_t y;
};

std::vector<Position> getPointsWall(const std::string &lineInput) {
    std::vector<Position> pointsWall;

    SplitString wallPoints = splitString(lineInput, " -> ");
    for(std::string &s: wallPoints.strings) {
        SplitString point =  splitString(s, ",");
        Position p = {std::atoi(point.strings[0].c_str()), std::atoi(point.strings[1].c_str())};
        pointsWall.push_back(p);
    }

    return pointsWall;
}

void putWall(char* map, int32_t width, const std::vector<Position> &pointsFormingWall) {
    //std::cout << "Putting points line" << "\n";

    // Put first position
    map[pointsFormingWall[0].y*width + pointsFormingWall[0].x] = '#';

    size_t i = 0;
    while(i < pointsFormingWall.size()-1) {
        // Get a pair
        Position p1 = pointsFormingWall[i];
        Position p2 = pointsFormingWall[i+1];

        int32_t xMin = std::min(p1.x, p2.x);
        int32_t xMax = std::max(p1.x, p2.x);
        int32_t yMin = std::min(p1.y, p2.y);
        int32_t yMax = std::max(p1.y, p2.y);

        for(int32_t y = yMin; y <= yMax; y++) {
            for(int32_t x = xMin; x <= xMax; x++) {
                map[y*width + x] = '#';
            }
        }
        i++;
    }
}



void printMap(char* map, int32_t width, int32_t height) {
    for(int32_t y = 0; y < height; y++) {
        for(int32_t x = 0; x < width; x++) {
            uint8_t posValue = map[y*width + x];
            std::cout << posValue;
        }
        std::cout << "\n";
    }
}

void printMap(char* map, Position minValues, Position maxValues, int32_t width) {
    for(int32_t y = minValues.y; y <= maxValues.y; y++) {
        for(int32_t x = minValues.x; x <= maxValues.x; x++) {
            uint8_t posValue = map[y*width + x];
            std::cout << posValue;
        }
        std::cout << "\n";
    }
}

std::pair<Position, Position> getMinMaxWallPoints(char* map, int32_t width, int32_t height) {
    Position pMin = {999999, 999999};
    Position pMax = {0, 0};

    for(int32_t y = 0; y < height; y++) {
        for(int32_t x = 0; x < width; x++) {
            uint8_t posValue = map[y*width + x];
            if(posValue != '.'){
                if(pMin.x > x) pMin.x = x;
                if(pMin.y > y) pMin.y = y;
                if(pMax.x < x) pMax.x = x;
                if(pMax.y < y) pMax.y = y;
            }
        }
    }
    return std::pair<Position, Position> {pMin, pMax};
}


char getCharAtPositon(const char* map, int32_t x, int32_t y, int32_t width, int32_t height) {
    if(x >= 0 && x < width && y >= 0 && y < height) {
        return map[y*width + x];
    } else {
        return 0;
    }
}

Position spawnSand(char* map, const Position &spawner, int32_t width, int32_t height) {
    Position sand = {spawner.x, spawner.y};
     // Y+ is down

    bool canMove = true;
    while(canMove) {
        char checkDown = getCharAtPositon(map, sand.x, sand.y + 1, width, height);
        if(checkDown == '.') {
            sand.y++;
        }else{
            char checkDiagLeft = getCharAtPositon(map, sand.x - 1, sand.y + 1, width, height);
            if(checkDiagLeft == '.') {
                sand.x--;
                sand.y++;
            }else{
                char checkDiagRight = getCharAtPositon(map, sand.x + 1, sand.y + 1, width, height);
                if(checkDiagRight == '.') {
                    sand.x++;
                    sand.y++;
                }else{
                    canMove = false;
                }
            }
        }
    }

    char lastPosition = getCharAtPositon(map, sand.x, sand.y, width, height);
    if(lastPosition == 0 || (sand.x == spawner.x && sand.y == spawner.y )){
        sand.x = -1;
        sand.y = -1;
    }

    return sand;
}

int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

    static const int32_t WIDTH = 1024;
    static const int32_t HEIGHT = 1024;

    char* map = new char[WIDTH*HEIGHT];
    for(size_t i = 0; i < WIDTH*HEIGHT; i++)map[i] = '.';

    // Get line width
    std::string line;
    while(std::getline(inputFile, line)) {
        std::vector<Position> pointsWall = getPointsWall(line);
        putWall(map, WIDTH, pointsWall);
    }

    // Sand spawner (500,0)
    map[500] = '+';

    std::pair<Position, Position> boundsValues = getMinMaxWallPoints(map, WIDTH, HEIGHT);
    std::cout << "MinValues: " << boundsValues.first.x << "," << boundsValues.first.y << ". MaxValues: " << boundsValues.second.x << "," << boundsValues.second.y << "\n";
   

    // Put ground
    for(int x = 0; x < 1024; x++) {
        map[(boundsValues.second.y+2)*WIDTH+x] = '#';
    }

    boundsValues = getMinMaxWallPoints(map, WIDTH, HEIGHT);
    
    int numSandUntil = 0;
    bool lastFall = false;
    while(!lastFall) {
        Position sand = spawnSand(map, Position{500,0}, WIDTH, HEIGHT);
        if(sand.x != -1){
            //std::cout << "Creating sand at: " << sand.x << "," << sand.y << "\n";
            map[sand.y*WIDTH+sand.x] = 'o';
        }else{
            std::cout << "SAND DID FALL FOR EVER" << "\n";
            lastFall = true;
        }
        numSandUntil++;
    }  
    //printMap(map, boundsValues.first, boundsValues.second, WIDTH); 


    std::cout << "Gold: " << numSandUntil << "\n";
   


    delete[] map;

    return 0;
}