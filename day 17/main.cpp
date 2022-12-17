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
        if(map[p.y*width + p.x] == '#')std::cout << "ERROR " << "\n";
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
     for(int32_t y = height; y >= 0; y--) {
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

uint64_t getMaxHeight(uint64_t rocksToSpawn, const std::vector<Position*> &wind) {
    int32_t width = 7;
    int32_t height = 60;

    char* map = new char[width*height];
    for(int i = 0; i < width*height; i++)map[i] = '.';

    uint64_t rocksSpawned = 0;
    int32_t step = 0;
    int32_t figureIndex = 0;
    int32_t windIndex = 0;
    std::vector<Position> currentFigure;

    Position direction = {0, 0};

    while(rocksSpawned < rocksToSpawn) {
        if(step == 0){
            // check height

            // create figure
            currentFigure = transformToValidMapCoordinates(map, width, height, FIGURES[figureIndex]);

            figureIndex = (figureIndex+1) % 5;
            std::cout << "Next figure " << figureIndex << "\n";
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
        //printMap(tetrisMap, WIDTH, 40, currentFigure);
        //std::cout << "\n";
    }
}

int main() {
    std::fstream inputFile = std::fstream(INPUT_FILE);

    if(!inputFile.is_open()) {
        std::cout << "Can't open " << INPUT_FILE << std::endl;
        exit(0);
    }

    int32_t WIDTH = 7;
    int32_t HEIGHT = 5000;

    char* tetrisMap = new char[WIDTH*HEIGHT];
    for(int64_t i = 0; i < WIDTH*HEIGHT; i++)tetrisMap[i] = '.';

    std::vector<const Position*> wind;

    int32_t figureIndex = 0;
    std::vector<Position> currentFigure;

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
    
    

    int32_t step = 0;
    Position direction = {0, 0};
    int32_t rocksStopped = 0;
    int32_t windIndex = 0;

    while(rocksStopped < 2022) {
        if(step == 0){
            // create figure
            currentFigure = transformToValidMapCoordinates(tetrisMap, WIDTH, HEIGHT, FIGURES[figureIndex]);

            figureIndex = (figureIndex+1) % 5;
        }

        bool collidesWithGround = false;
        bool collidesLateral = false;
        if(step % 2 == 1){
            direction = *down;
            collidesWithGround = isCollidingAfterDirection(tetrisMap, WIDTH, HEIGHT, currentFigure, direction);
        }else if(step % 2 == 0){
            if(wind.size() > 0) {
                direction = *wind[windIndex];
                windIndex = (windIndex+1) % wind.size();

                collidesLateral = isCollidingAfterDirection(tetrisMap, WIDTH, HEIGHT, currentFigure, direction);
            }
        }

        // Update figure if it doesn't collide
        if(!collidesWithGround && !collidesLateral) {
            for(Position &p: currentFigure) {
                p.x += direction.x;
                p.y += direction.y;
            }
        }

        if(collidesWithGround) {
            // Put figure in map
            putFigureInMap(tetrisMap, WIDTH, currentFigure);
            step = 0;
            rocksStopped++;
        }else{
            step++;
        }
    }

    int32_t silver = getTallestPosition(tetrisMap, WIDTH, HEIGHT);



    std::cout << "\n\n";
    std::cout << "Silver: " << silver+1 <<   "\n";
    std::cout << "Gold: " << "\n";


    return 0;
}