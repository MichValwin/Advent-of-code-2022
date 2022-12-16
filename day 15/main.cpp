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

struct Position{
    int32_t x;
    int32_t y;

    void print() {
        std::cout << x << "," << y << "\n"; 
    } 
};

struct SensorClosestBeacon{
    Position sensor;
    Position closestBeacon;
    int32_t manhattanDistance;
};

void getMinMaxNumbers(const std::vector<SensorClosestBeacon> &sensorBeacons, int32_t &minX, int32_t &minY, int32_t &maxX, int32_t &maxY) {
    minX = 999999999;
    maxX = -999999999;
    minY = 999999999;
    maxY = -999999999;
    for(const SensorClosestBeacon &s: sensorBeacons){
        if(minX > s.sensor.x)minX = s.sensor.x;
        if(minY > s.sensor.y)minY = s.sensor.y;
        if(minX > s.closestBeacon.x)minX = s.closestBeacon.x;
        if(minY > s.closestBeacon.y)minY = s.closestBeacon.y;

        if(maxX < s.sensor.x)maxX = s.sensor.x;
        if(maxY < s.sensor.y)maxY = s.sensor.y;
        if(maxX < s.closestBeacon.x)maxX = s.closestBeacon.x;
        if(maxY < s.closestBeacon.y)maxY = s.closestBeacon.y;
    }
}

int32_t manhattanDistanceTwoPoints(const Position &p1, const Position &p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}


struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1, T2> const &pair) const {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);
 
        return h1 ^ h2;
    }
};


bool isPositionCovered(std::vector<SensorClosestBeacon> &sensors, Position &p) {
    for(SensorClosestBeacon &s: sensors) {
        int32_t distance = manhattanDistanceTwoPoints(s.sensor, p);
        if(distance <= s.manhattanDistance)return true;
    }
    return false;
}

Position getPositionNotCovered(std::vector<SensorClosestBeacon> &sensors, int32_t maxToCheck) {
    Position pos[4] = {
        {-1,1},
        {-1,-1},
        {1,-1},
        {1,1}
    };

    Position posNotCovered = {-1, -1};

    for(SensorClosestBeacon &s: sensors){
        // Calculate all positions by perimeter
        int32_t perimeterSize = s.manhattanDistance + 2;
        Position toTest = {s.sensor.x + s.manhattanDistance + 1, s.sensor.y};
        std::cout << "Distance: " << s.manhattanDistance << "\n";
        std::cout << "First with: ";
        toTest.print();
        for(int i = 0; i < 4; i++) {
            std::cout << i << " Starts: ";
            toTest.print();
            for(int32_t j = 0; j < perimeterSize; j++) {
                toTest.x += pos[i].x;
                toTest.y += pos[i].y;
                if(toTest.x >= 0 && toTest.x <= maxToCheck && toTest.y >= 0 && toTest.y <= maxToCheck){
                    if(!isPositionCovered(sensors, toTest)){
                        posNotCovered = toTest;
                        return posNotCovered;
                    }
                }
            }
            std::cout << i << " Finish: ";
            toTest.print();

        }
    }

    return posNotCovered;
}

int main() {
    std::fstream inputFile = std::fstream(INPUT_FILE);

    if(!inputFile.is_open()) {
        std::cout << "Can't open " << INPUT_FILE << std::endl;
        exit(0);
    }

    std::vector<SensorClosestBeacon> sensorBeacons;



    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        //Sensor at 
        size_t endSensor = line.find(":");
        std::string sensorPos = line.substr(10, endSensor-10);
        SplitString sensors = splitString(sensorPos, ", ");

        Position sensor;
        sensor.x = std::atoi(sensors.strings[0].substr(2, -1).c_str());
        sensor.y = std::atoi(sensors.strings[1].substr(2, -1).c_str());

        //Beacon at
        const char* second = "closest beacon is at ";
        std::string beaconPos = line.substr(line.find(second)+21, -1);
        SplitString beaconsSplitStr = splitString(beaconPos, ", ");

        Position beacon;
        beacon.x = std::atoi(beaconsSplitStr.strings[0].substr(2, -1).c_str());
        beacon.y = std::atoi(beaconsSplitStr.strings[1].substr(2, -1).c_str());

        sensorBeacons.push_back({sensor, beacon, manhattanDistanceTwoPoints(sensor, beacon)});
    }

    for(SensorClosestBeacon &s: sensorBeacons){
        std::cout << "Sensor: " << s.sensor.x << ", " << s.sensor.y << ". Beacon: " << s.closestBeacon.x << ", " << s.closestBeacon.y << ". Manhattan: " << s.manhattanDistance <<  "\n";
    }

    int32_t minX;
    int32_t maxX;
    int32_t minY;
    int32_t maxY;
    getMinMaxNumbers(sensorBeacons, minX, minY, maxX, maxY);
    int32_t maxManhattanDistance = 0;
    for(SensorClosestBeacon &s: sensorBeacons){
        if(s.manhattanDistance > maxManhattanDistance)maxManhattanDistance = s.manhattanDistance;
    }

    std::cout << "MinX: " << minX << ", MaxX: " << maxX << "\n";
    std::cout << "MinY: " << minY << ", MaxY: " << maxY << "\n";
    std::cout << "After applying max distance: " << "\n";
    minX = minX - maxManhattanDistance;
    maxX = maxX + maxManhattanDistance;

    std::cout << "MinX: " << minX << ", MaxX: " << maxX << "\n";
    std::cout << "MinY: " << minY << ", MaxY: " << maxY << "\n";
   

    int32_t rowToCheckSilver = 2000000;
    uint32_t silver = 0;


    std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> beacons;
    for(SensorClosestBeacon &s: sensorBeacons) {
        beacons.insert(std::pair<int32_t, int32_t>{s.closestBeacon.x, s.closestBeacon.y});
    }

    /*
    std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> nonBeacons;
    for(int32_t x = minX; x <= maxX; x++) {
        for(SensorClosestBeacon &s: sensorBeacons) {
            Position pToCheck = {x, rowToCheckSilver};
            if(beacons.find(std::pair<int32_t,int32_t> {pToCheck.x, pToCheck.y}) == beacons.end()) {
                int32_t manhattanDistance = manhattanDistanceTwoPoints(s.sensor, pToCheck);
                if(manhattanDistance <= s.manhattanDistance){
                    nonBeacons.insert(std::pair<int32_t, int32_t>{pToCheck.x, pToCheck.y});
                }
            }
        }
    }
    silver = nonBeacons.size();
    */
    
    // Distress beacon
    int32_t distressMin = 0;
    int32_t distressMax = 4000000;

    int64_t tuningFrequency = 4000000;
    Position distressBeacon = getPositionNotCovered(sensorBeacons, distressMax);   

    if(distressBeacon.x != -1) {
        std::cout << "Found distress beacon at x: " << distressBeacon.x << ", y:" << distressBeacon.y << "\n";
        tuningFrequency = tuningFrequency * distressBeacon.x + distressBeacon.y;
    }else{
        std::cout << "NOT FOUND" << "\n";
    }

    
    std::cout << "\n\n";
    std::cout << "Silver: " << silver << " for row: " << rowToCheckSilver << "\n";
    std::cout << "Gold: " << tuningFrequency << " for min: " << distressMin << ", max: " << distressMax << "\n";


    return 0;
}