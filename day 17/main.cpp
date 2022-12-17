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
static const int32_t APPEAR_UNITS_LEFT = 2;
static const int32_t APPEAR_UNITS_DOWN = 3;

struct Position{
    int32_t x;
    int32_t y;
    
    void print() const{
        std::cout << "X: " << x << ", Y: " << y;
    }
};

static const std::vector<Position> FIGURES[] = {
    {{0,0}, {1,0}, {2,0}, {3,0}}, // -
    {{1,2}, {0,1}, {1,1}, {2,1}, {1, 0}}, // +
    {{2,2}, {2, 1}, {0,0}, {1, 0}, {2,0}}, // L
    {{0,3}, {0,2}, {0,1}, {0,0}}, // |
    {{0,1}, {1,1}, {0,0}, {1,0}} // []
};

// Wind
static const Position* left = new Position{-1, 0};
static const Position* right = new Position{1, 0};
static const Position* down = new Position{0, -1};

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



int32_t getTallestPosition(const char* map, int32_t width, int32_t height) {
    int32_t tallest = -1;
    for(int32_t y = 0; y < height; y++) {
        for(int32_t x = 0; x < width; x++) {
            if(map[y*width+x] == '#' && y > tallest)tallest = y;
        }
    }
    
    return tallest;
}

bool isInOfBounds(int32_t width, int32_t height, const Position &pos) {
    if(pos.x >= 0 && pos.x < width && pos.y >= 0 && pos.y < height) {
        return true;
    }else{
        return false;
    }
}

bool collidesWithCell(const char* map, int32_t width, const Position &pos) {
    char cellInMap = map[pos.y*width+pos.x];
    if(cellInMap == '#'){
        return true;
    }else{
        return false;
    }
}

std::vector<Position> transformToValidMapCoordinates(char* map, int32_t width, int32_t height, const std::vector<Position> figure) {
    // Get tallest position
    int32_t tallest = getTallestPosition(map, width, height);

    // Add to board
    std::vector<Position> figToAdd;
    for(const Position &pos: figure) {
        Position posToMap = {pos.x, pos.y};
        posToMap.x += APPEAR_UNITS_LEFT;
        posToMap.y += tallest + APPEAR_UNITS_DOWN + 1;
        figToAdd.push_back(posToMap);
    }
    return figToAdd;
}

bool isCollidingAfterDirection(char* map, int32_t width, int32_t height, std::vector<Position> figure, const Position direction) {
    bool isColliding = false;
    for(Position &p: figure) {
        Position newPos = {p.x + direction.x, p.y + direction.y};
        if(!isInOfBounds(width, height, newPos) || collidesWithCell(map, width, newPos)){
            isColliding = true;
            break;
        }
    }
    return isColliding;
}

void putFigureInMap(char* map, int32_t width, std::vector<Position> &figure) {
    for(Position &p: figure) {
        map[p.y*width + p.x] = '#';
    }
}

void printMap(const char* map, int32_t width, int32_t height, const std::vector<Position> &figure) {
     for(int32_t y = height; y >= 0; y--) {
        for(int32_t x = 0; x < width; x++) {
            bool isIN = false;
            for(const Position &p: figure){
                if(p.x == x && p.y == y){
                    isIN = true;
                    break;
                }
            }
            if(isIN){
                std::cout << "@";
            }else{
                std::cout << map[y*width+x];
            }
            
        }
        std::cout << "\n";
    }
}

void printMap(const char* map, int32_t width, int32_t height) {
     for(int32_t y = height-1; y >= 0; y--) {
        for(int32_t x = 0; x < width; x++) {
            std::cout << map[y*width+x];
        }
        std::cout << "\n";
    }
}

void printFigure(const std::vector<Position> figure) {
    int32_t width = 0;
    int32_t height = 0;
    for(const Position &p: figure) {
        p.print();
        std::cout << "\n";
        if(width < p.x)width = p.x;
        if(height < p.y)height = p.y;
    }
    width += 1;
    height += 1;

    char* figureMap = new char[width*height];
    for(int64_t i = 0; i < width*height; i++)figureMap[i] = '.';
    for(const Position &p: figure)figureMap[p.y*width+p.x] = '#';

    printMap(figureMap, width, height);

    delete[] figureMap;
}

uint64_t getMaxHeight(char* map, int32_t width, int32_t height, uint64_t rocksToSpawn, const std::vector<const Position*> &wind) {
    uint64_t rocksSpawned = 0;
    int32_t step = 0;
    int32_t figureIndex = 0;
    int32_t windIndex = 0;
    std::vector<Position> currentFigure;

    Position direction = {0, 0};

    while(rocksSpawned < rocksToSpawn) {
        if(step == 0){
            // create figure
            currentFigure = transformToValidMapCoordinates(map, width, height, FIGURES[figureIndex]);

            figureIndex = (figureIndex+1) % 5;
            //std::cout << "Next figure " << figureIndex << "\n";
        }

        bool collidesWithGround = false;
        bool collidesLateral = false;
        if(step % 2 == 1){
            //std::cout << "Rock falls 1 unit";
            direction = *down;
            collidesWithGround = isCollidingAfterDirection(map, width, height, currentFigure, direction);
        }else if(step % 2 == 0){
            if(wind.size() > 0) {
                //std::cout << "Jet of gas pushes rock";
                direction = *wind[windIndex];
                windIndex = (windIndex+1) % wind.size();
                //if(direction.x  == -1) std::cout << " left";
                //if(direction.x  == 1) std::cout << " right";

                collidesLateral = isCollidingAfterDirection(map, width, height, currentFigure, direction);
            }
        }

        // Update figure if it doesn't collide
        if(!collidesWithGround && !collidesLateral) {
            for(Position &p: currentFigure) {
                p.x += direction.x;
                p.y += direction.y;
            }
        }

        //if(collidesLateral)std::cout << ", but nothing happens";

        if(collidesWithGround) {
            //std::cout << ", causing it to come to rest";
            // Put figure in map
            putFigureInMap(map, width, currentFigure);
            step = 0;
            rocksSpawned++;
        }else{
            step++;
        }
        //std::cout << "\n";
        //printMap(map, width, 40, currentFigure);
        //std::cout << "\n";
    }

    return getTallestPosition(map, width, height);
}

std::vector<int32_t> repeatUpdates(char* map, int32_t width, int32_t height, uint64_t loopsthroughData, const std::vector<const Position*> &wind) {
    size_t loopsDone = 0;
    int32_t step = 0;
    int32_t figureIndex = 0;
    int32_t windIndex = 0;
    std::vector<Position> currentFigure;

    uint64_t rocksFall = 0;
    Position direction = {0, 0};

    std::vector<int32_t> tallestPointEach;

    while(loopsDone < loopsthroughData) {
    //while(rocksFall < 2022){
        if(step == 0){
            // create figure
            currentFigure = transformToValidMapCoordinates(map, width, height, FIGURES[figureIndex]);

            figureIndex = (figureIndex+1) % 5;
            //std::cout << "Next figure " << figureIndex << "\n";
        }

        bool collidesWithGround = false;
        bool collidesLateral = false;
        if(step % 2 == 1){
            //std::cout << "Rock falls 1 unit";
            direction = *down;
            collidesWithGround = isCollidingAfterDirection(map, width, height, currentFigure, direction);
        }else if(step % 2 == 0){
            if(wind.size() > 0) {
                //std::cout << "Jet of gas pushes rock";
                direction = *wind[windIndex];
               
                if(windIndex % wind.size() == 0){
                    //std::cout << windIndex << "\n";
                    loopsDone++;
                }
                windIndex = (windIndex+1) % wind.size();
                //if(direction.x  == -1) std::cout << " left";
                //if(direction.x  == 1) std::cout << " right";

                collidesLateral = isCollidingAfterDirection(map, width, height, currentFigure, direction);
            }
        }

        // Update figure if it doesn't collide
        if(!collidesWithGround && !collidesLateral) {
            for(Position &p: currentFigure) {
                p.x += direction.x;
                p.y += direction.y;
            }
        }

        //if(collidesLateral)std::cout << ", but nothing happens";

        if(collidesWithGround) {
            //std::cout << ", causing it to come to rest";
            // Put figure in map
            putFigureInMap(map, width, currentFigure);
            step = 0;
            rocksFall++;
            tallestPointEach.push_back(getTallestPosition(map, width, height));
        }else{
            step++;
        }
        //std::cout << "\n";
        //printMap(map, width, 40, currentFigure);
        //std::cout << "\n";
    }

    return tallestPointEach;
}

int64_t repeatingPattern(const char* map, int64_t size) {
    int64_t sizeRepeatedArea = size / 2.0;
    
    std::string toSearch = map;

    bool repeats = false;
    while(!repeats && sizeRepeatedArea > 10) {
        repeats = false;

        for(int64_t i = 0; i < size-sizeRepeatedArea; i++) {
            std::string substrToSearchFor = toSearch.substr(i, sizeRepeatedArea);
            if(toSearch.find(substrToSearchFor, i+1) != std::string::npos){
                repeats = true;
                std::cout << "From " << i << " to " << sizeRepeatedArea <<  "\n"; 
                break;
            }
        }
        std::cout << "size repeated area: " << sizeRepeatedArea << "\n";
        sizeRepeatedArea--;
    }
    
    return sizeRepeatedArea;
}

int main() {
    std::fstream inputFile = std::fstream(INPUT_FILE);

    if(!inputFile.is_open()) {
        std::cout << "Can't open " << INPUT_FILE << std::endl;
        exit(0);
    }

    int32_t WIDTH = 7;
    int32_t HEIGHT = 800000;

    char* tetrisMap = new char[WIDTH*HEIGHT];
    for(int64_t i = 0; i < WIDTH*HEIGHT; i++)tetrisMap[i] = '.';

    std::vector<const Position*> wind;

    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        for(size_t i = 0; i < line.size(); i++){
            char windChar = line[i];
            switch(windChar){
                case '<':
                    wind.push_back(left);
                    break;
                case '>':
                     wind.push_back(right);
                    break;
            }
        }
    }

    /*
    //int32_t silver = getMaxHeight(tetrisMap, WIDTH, HEIGHT, 2022, wind);
    std::vector<int32_t> tallestPointsEachblock = repeatUpdates(tetrisMap, WIDTH, HEIGHT, 1, wind);
    //printMap(tetrisMap, WIDTH, HEIGHT);
    std::cout << "Done \n";
    std::vector<int32_t> diffs;
    for(size_t i = 0; i < tallestPointsEachblock.size()-1; i++){
        diffs.push_back(tallestPointsEachblock[i+1] - tallestPointsEachblock[i]);
    }
    for(int32_t diff: diffs){
        std::cout << diff;
    }
    std::cout << "\n";
    */

    std::string sumcadena = "33201330203030003101330013300121301030013240130020222213320133401303213320003401230013030132100322012220133001303012130132";
    int32_t total = 0;
    for(size_t i = 0; i < sumcadena.size(); i++){
        total += (sumcadena[i] - '0');
    }
    std::cout << "total : " << total << "\n";

    std::cout << "Mod: " << 1514285714288 / 53  << "\n";

    int64_t rockInit = 1722;
    int64_t heightInit = 2720;

    int64_t repetHeight = 2709;
    int64_t repetRocks = 1725;
    int64_t height = heightInit;

    int64_t totalRocks = rockInit;

    for(; totalRocks < 1000000000000; totalRocks+= repetRocks) {
        height += repetHeight;
    }  
    std::cout << "Final height: " << height << "\n";
    std::cout << "Rocks: " << totalRocks << "\n";
    // check repetition of blocks
    /*
    int64_t sizeRepeatedAreaFirst = repeatingPattern(tetrisMap, WIDTH*HEIGHT);
    std::cout << "sizeRepeatedArea: " << sizeRepeatedAreaFirst << "\n";
    */

    std::cout << int64_t(height - 182) << "\n";
  

    std::cout << "\n\n";
    //std::cout << "Silver: " << silver+1 <<   "\n";
    std::cout << "Gold: " << "\n";

    delete[] tetrisMap;

    return 0;
}