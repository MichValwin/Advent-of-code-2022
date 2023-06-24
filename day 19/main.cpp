#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <algorithm>
#include <queue>
#include <set>
#include <cstdlib>

#include <string.h>

static const char* INPUT_FILE = "input.txt";
static const char* INPUT_TEST_FILE = "input_test.txt";

static const char* ORE_BOTS_INPUT = "Each ore robot costs ";
static const char* CLAY_BOTS_INPUT = "Each clay robot costs ";
static const char* OBSIDIAN_BOTS_INPUT = "Each obsidian robot costs ";
static const char* OBSIDIAN_GEODE_IN_BETWEEN_INPUT = " ore and ";
static const char* GEODE_BOTS_INPUT = "Each geode robot costs ";

static constexpr int NUM_MINUTES_MAX_FIRST = 24;
static constexpr int NUM_MINUTES_MAX_SECOND = 32;

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

bool isCharNumber(char ch) {
    if(ch >= '0' && ch <= '9') {
        return true;
    }else{
        return false;
    }
}

int getEndSequenceNumber(const std::string& str, size_t start) {
    if(!isCharNumber(str.at(start))) {
        return -1;
    }
    
    size_t end = start + 1;
    while(isCharNumber(str.at(end))) {
        end++;
    }
    return end;
}

int getIntFromStrAtCertainPoint(const std::string& str, size_t start) {
    int end = getEndSequenceNumber(str, start);
    if(end == -1) {
        return -1;
    }

    int length = end - start;

    return std::atoi(str.substr(start, length).c_str());
}






struct Blueprint {
    int id;
    int oreRobotOre;
    int clayRobotOre;
    int obsidianRobotOre;
    int obsidianRobotClay;
    int geodeRobotOre;
    int geodeRobotObsidian;

};

enum StateAction {doNothing, makeOreRobot, makeClayRobot, makeObsidianRobot, makeGeodeRobot};

struct State {
    uint8_t minute;

    uint8_t oreRobots;
    uint8_t clayRobots;
    uint8_t obsidianRobots;
    uint8_t geodeRobots;

    uint8_t ore;
    uint8_t clay;
    uint8_t obsidian;
    uint8_t geode;

    void mine() {
        ore += oreRobots;
        clay += clayRobots;
        obsidian += obsidianRobots;
        geode += geodeRobots;
    }

    bool canBuildOreRobot(Blueprint& blueprint) {
        if(ore >= blueprint.oreRobotOre)return true;
        return false;
    }

    bool canBuildClayRobot(Blueprint& blueprint) {
        if(ore >= blueprint.clayRobotOre)return true;
        return false;
    }

    bool canBuildObsidianRobot(Blueprint& blueprint) {
        if(ore >= blueprint.obsidianRobotOre && clay >= blueprint.obsidianRobotClay)return true;
        return false;
    }

    bool canBuildGeodeRobot(Blueprint& blueprint) {
        if(ore >= blueprint.geodeRobotOre && obsidian >= blueprint.geodeRobotObsidian)return true;
        return false;
    }

    void subtractResourcesOreRobot(Blueprint& blueprint) {
        ore -= blueprint.oreRobotOre;
    }

    void subtractResourcesClayRobot(Blueprint& blueprint) {
        ore -= blueprint.clayRobotOre;
    }

    void subtractResourcesObsidianRobot(Blueprint& blueprint) {
        ore -= blueprint.obsidianRobotOre;
        clay -= blueprint.obsidianRobotClay;
    }

    void subtractResourcesGeodeRobot(Blueprint& blueprint) {
        ore -= blueprint.geodeRobotOre;
        obsidian -= blueprint.geodeRobotObsidian;
    }

    void finishConstruction(uint8_t buildingOreRobot, uint8_t buildingClayRobot, uint8_t buildingObsidianRobot, uint8_t buildingGeodeRobot) {
        oreRobots += buildingOreRobot;
        clayRobots += buildingClayRobot;
        obsidianRobots += buildingObsidianRobot;
        geodeRobots += buildingGeodeRobot;
    }

    State* createNewState(StateAction action, Blueprint& blueprint) const {
        State* newState = copyNewState();

        uint8_t buildingOreRobot = 0;
        uint8_t buildingClayRobot = 0;
        uint8_t buildingObsidianRobot = 0;
        uint8_t buildingGeodeRobot = 0;

        switch(action) {
            case StateAction::makeGeodeRobot:
                newState->subtractResourcesGeodeRobot(blueprint);
                buildingGeodeRobot++;
                break;
            case StateAction::makeObsidianRobot:
                newState->subtractResourcesObsidianRobot(blueprint);
                buildingObsidianRobot++;
                break;
            case StateAction::makeClayRobot:
                newState->subtractResourcesClayRobot(blueprint);
                buildingClayRobot++;
                break;
            case StateAction::makeOreRobot:
                newState->subtractResourcesOreRobot(blueprint);
                buildingOreRobot++;
                break;
            case StateAction::doNothing:
                break;
        }
       
        newState->mine();
        newState->finishConstruction(buildingOreRobot, buildingClayRobot, buildingObsidianRobot, buildingGeodeRobot);
        newState->minute++;
        return newState;
    }

     State createState(StateAction action, Blueprint& blueprint) const {
        State newState = copyState();

        uint8_t buildingOreRobot = 0;
        uint8_t buildingClayRobot = 0;
        uint8_t buildingObsidianRobot = 0;
        uint8_t buildingGeodeRobot = 0;

        switch(action) {
            case StateAction::makeGeodeRobot:
                newState.subtractResourcesGeodeRobot(blueprint);
                buildingGeodeRobot++;
                break;
            case StateAction::makeObsidianRobot:
                newState.subtractResourcesObsidianRobot(blueprint);
                buildingObsidianRobot++;
                break;
            case StateAction::makeClayRobot:
                newState.subtractResourcesClayRobot(blueprint);
                buildingClayRobot++;
                break;
            case StateAction::makeOreRobot:
                newState.subtractResourcesOreRobot(blueprint);
                buildingOreRobot++;
                break;
            case StateAction::doNothing:
                break;
        }
       
        newState.mine();
        newState.finishConstruction(buildingOreRobot, buildingClayRobot, buildingObsidianRobot, buildingGeodeRobot);
        newState.minute++;
        return newState;
    }

    State* copyNewState() const {
        return new State{minute, oreRobots, clayRobots, obsidianRobots, geodeRobots, 
            ore, clay, obsidian, geode};
    }

    State copyState() const {
        return State{minute, oreRobots, clayRobots, obsidianRobots, geodeRobots, 
            ore, clay, obsidian, geode};
    }

    void printCout() {
        std::cout << "OreBots: " << (int)oreRobots << ", ClayBots: " << (int)clayRobots << ", ObsidianBots: " << (int)obsidianRobots 
            << ", GeodeRobots: " << (int)geodeRobots << ", Ore: " << (int)ore << ", clay: " << (int)clay << ", Obsidian: " << (int)obsidian << ", Geodes: " << (int)geode << "\n";
    }

    bool operator==(const State& s2) const {
        return minute == s2.minute && oreRobots == s2.oreRobots && clayRobots == s2.clayRobots &&
            obsidianRobots == s2.obsidianRobots && geodeRobots == s2.geodeRobots && ore == s2.ore &&
            clay == s2.clay && obsidian == s2.obsidian && geode == s2.geode;
    }
};

struct StateHash {
    std::size_t operator()(const State& state) const {
        std::size_t seed = 0;
        std::hash<int> intHash;
        
        seed ^= intHash(state.ore) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= intHash(state.clay) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= intHash(state.obsidian) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= intHash(state.geode) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        seed ^= intHash(state.minute) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        seed ^= intHash(state.oreRobots) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= intHash(state.clayRobots) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= intHash(state.obsidianRobots) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= intHash(state.geodeRobots) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};



struct NodeState {
    State state;
    std::vector<NodeState*> leafStates;

    bool finished;
    bool processed;

    NodeState* parentNode;
    

    NodeState* createNewNode(State& state) {
        NodeState* newNodeState = new NodeState;
        newNodeState->finished = false;
        newNodeState->processed = false;
        newNodeState->parentNode = this;
        newNodeState->state = state;
        return newNodeState;
    }

    NodeState* getNextNonFinishedState() const{
        for(NodeState* node: leafStates) {
            if(!node->finished) {
                return node;
            }
        }
        return nullptr;
    }


};

int qualityLevel(State& state, Blueprint& blueprint) {
    return blueprint.id * state.geode;
}

int getMaxGeodes(State& state, Blueprint& blueprint, int minutes) {
    int maxGeodes = state.geode;
    int geodeRobotsOptimistic = state.geodeRobots;
    for(int i = state.minute; i <= minutes; i++) {
        geodeRobotsOptimistic++;
        maxGeodes += geodeRobotsOptimistic;
    }
    return maxGeodes;
}


void goThroughAllStates(Blueprint& blueprint, int& bestQualityLevel, int& maxGeodesRegistered, int minutes) {
    std::cout << "Processing blueprint" << "\n";
    //NodeState* bestQualityNodeState = nullptr;

    NodeState* firstNodeState = new NodeState({State({0, 1, 0, 0, 0, 0, 0, 0, 0}), std::vector<NodeState*>(), false, false, nullptr});

    NodeState* currentNodeState = firstNodeState;
    
    std::unordered_set<State, StateHash> nodesFinalized;
    int collisions = 0;

    while(!firstNodeState->finished) {
        State currentState = currentNodeState->state;
        //std::cout << "Minute: " << currentState.minute << ", Ore: " << currentState.ore << ", OreRobots: " << currentState.oreRobots << "\n";

        if(!currentNodeState->processed) {
            if(currentState.minute < minutes) {
                if(currentState.canBuildGeodeRobot(blueprint)) {
                    State newState = currentState.createState(StateAction::makeGeodeRobot, blueprint);
                    if(nodesFinalized.find(newState) == nodesFinalized.end()) {
                        if(getMaxGeodes(newState, blueprint, minutes) > maxGeodesRegistered){
                            NodeState* newNode = currentNodeState->createNewNode(newState);
                            currentNodeState->leafStates.push_back(newNode);
                        }
                    }else{
                        ++collisions;
                    }
                }
                if(currentState.canBuildObsidianRobot(blueprint)) {
                    State newState = currentState.createState(StateAction::makeObsidianRobot, blueprint);
                    if(nodesFinalized.find(newState) == nodesFinalized.end()){
                        if(getMaxGeodes(newState, blueprint, minutes) > maxGeodesRegistered){
                            NodeState* newNode = currentNodeState->createNewNode(newState);
                            currentNodeState->leafStates.push_back(newNode);
                        }
                    }else{
                        ++collisions;
                    }
                }
                if(currentState.canBuildClayRobot(blueprint)) {
                    State newState = currentState.createState(StateAction::makeClayRobot, blueprint);
                    if(nodesFinalized.find(newState) == nodesFinalized.end()){
                        if(getMaxGeodes(newState, blueprint, minutes) > maxGeodesRegistered){
                            NodeState* newNode = currentNodeState->createNewNode(newState);
                            currentNodeState->leafStates.push_back(newNode);
                        }
                    }else{
                        ++collisions;
                    }
                }
                if(currentState.canBuildOreRobot(blueprint)) {
                    State newState = currentState.createState(StateAction::makeOreRobot, blueprint);
                    if(nodesFinalized.find(newState) == nodesFinalized.end()){
                        if(getMaxGeodes(newState, blueprint, minutes) > maxGeodesRegistered){
                            NodeState* newNode = currentNodeState->createNewNode(newState);
                            currentNodeState->leafStates.push_back(newNode);
                        }
                    }else{
                        ++collisions;
                    }
                }

                State newState = currentState.createState(StateAction::doNothing, blueprint);
                if(nodesFinalized.find(newState) == nodesFinalized.end()){
                    if(getMaxGeodes(newState, blueprint, minutes) > maxGeodesRegistered){
                        NodeState* newNode = currentNodeState->createNewNode(newState);
                        currentNodeState->leafStates.push_back(newNode);
                    }
                }else{
                    ++collisions;
                }
            }else{
                currentNodeState->finished = true;
                if(currentState.geode > maxGeodesRegistered)maxGeodesRegistered = currentState.geode;
                int quality = qualityLevel(currentState, blueprint);
                if(quality > bestQualityLevel) {
                    bestQualityLevel = quality;
                    std::cout << "New Quality level: " << bestQualityLevel << std::endl;
                    std::cout << "Collisions: " << collisions << "\n";
                    currentState.printCout();
                }
                nodesFinalized.insert(currentState.copyState());
            }
            currentNodeState->processed = true;
        }

        
        NodeState* nextNodeToVisit = currentNodeState->getNextNonFinishedState();
        if(nextNodeToVisit == nullptr) {
            // Go back to parent node
            //std::cout << "Finished into leaf termination (no more nodes in list), going back" << "\n";
            currentNodeState->finished = true;
            nodesFinalized.insert(currentState.copyState());

            // delete current state
            // Delete leaf nodes
            for(NodeState* leafNode: currentNodeState->leafStates) {
                delete leafNode;
            }
            currentNodeState->leafStates.clear();

            if(currentNodeState->parentNode != nullptr) {
                currentNodeState = currentNodeState->parentNode; 
            }
        }else{
            // Go to a leaf node
            //std::cout << "Visiting another leaf" << "\n";
            currentNodeState = nextNodeToVisit;
        }
    }

    delete firstNodeState;
}

int main() {
    std::fstream inputFile = std::fstream(INPUT_FILE);

    if(!inputFile.is_open()) {
        std::cout << "Can't open " << INPUT_FILE << std::endl;
        exit(0);
    }

    std::vector<Blueprint> blueprints;
    
    // Parse
    std::string line;
    int id = 1;
    while(std::getline(inputFile, line)) {
        int oreRobotOre = getIntFromStrAtCertainPoint(line, line.find(ORE_BOTS_INPUT) + strlen(ORE_BOTS_INPUT));
        int clayRobotOre = getIntFromStrAtCertainPoint(line, line.find(CLAY_BOTS_INPUT)+ strlen(CLAY_BOTS_INPUT));
        int obsidianRobotOre = getIntFromStrAtCertainPoint(line, line.find(OBSIDIAN_BOTS_INPUT) + strlen(OBSIDIAN_BOTS_INPUT));
      
        int obsidianClayStartPos = getEndSequenceNumber(line, line.find(OBSIDIAN_BOTS_INPUT) + strlen(OBSIDIAN_BOTS_INPUT));
        int obsidianRobotClay = getIntFromStrAtCertainPoint(line, line.find(OBSIDIAN_GEODE_IN_BETWEEN_INPUT, obsidianClayStartPos) + strlen(OBSIDIAN_GEODE_IN_BETWEEN_INPUT));
        int geodeRobotOre = getIntFromStrAtCertainPoint(line, line.find(GEODE_BOTS_INPUT) + strlen(GEODE_BOTS_INPUT));
        int geodeObsStartPos = getEndSequenceNumber(line, line.find(GEODE_BOTS_INPUT) + strlen(GEODE_BOTS_INPUT));
        int geodeRobotObsidian = getIntFromStrAtCertainPoint(line, line.find(OBSIDIAN_GEODE_IN_BETWEEN_INPUT, geodeObsStartPos) + strlen(OBSIDIAN_GEODE_IN_BETWEEN_INPUT));

        blueprints.push_back({id, oreRobotOre, clayRobotOre, obsidianRobotOre, obsidianRobotClay, geodeRobotOre, geodeRobotObsidian});
        id++;
    }

    for(Blueprint b: blueprints)  {
        std::cout << ORE_BOTS_INPUT << b.oreRobotOre << " ore. ";
        std::cout << CLAY_BOTS_INPUT << b.clayRobotOre << " ore. ";
        std::cout << OBSIDIAN_BOTS_INPUT << b.obsidianRobotOre << OBSIDIAN_GEODE_IN_BETWEEN_INPUT << b.obsidianRobotClay << " clay. ";
        std::cout << GEODE_BOTS_INPUT << b.geodeRobotOre << OBSIDIAN_GEODE_IN_BETWEEN_INPUT << b.geodeRobotObsidian << " obsidian.";
        std::cout << std::endl;
    }

    // First part
    int qualityLevelSum = 0;
    for(Blueprint& blueprint: blueprints) {
        int bestQualityLevel = 0;
        int maxGeodesRegistered = 0;
        goThroughAllStates(blueprint, bestQualityLevel, maxGeodesRegistered, NUM_MINUTES_MAX_FIRST);
        qualityLevelSum += bestQualityLevel;
    }
    std::cout << "Quality level sum with 24 Minutes: " << qualityLevelSum << "\n";
  
    // Second part
    int numGeodesMultiply = 0;
    for(int i = 0; i < 3; i++) {
        int bestQualityLevel = 0;
        int maxGeodesRegistered = 0;
        goThroughAllStates(blueprints[i], bestQualityLevel, maxGeodesRegistered, NUM_MINUTES_MAX_SECOND);
        if(i == 0) {
            numGeodesMultiply = maxGeodesRegistered;
        }else{
            numGeodesMultiply *= maxGeodesRegistered;
        }
    }
    std::cout << "Num Geodes multiply: " << numGeodesMultiply << "\n";

    return 0;
}