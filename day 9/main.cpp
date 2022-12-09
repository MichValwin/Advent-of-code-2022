#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_set>


#define BIGBOY

#ifdef BIGBOY
	/*
		BIGBOY
		SILVER: 8129855
		GOLD: 7750850
	*/
    static const char* INPUT_FILE = "bigboy.txt";
#else
    static const char* INPUT_FILE = "input.txt";
#endif

struct Position {
	int32_t x;
	int32_t y;
};

uint64_t toU64ForIndexing(const Position &p) {
	return ((uint64_t)p.x << 32) + (uint64_t)p.y;
}

void doPhysics(const Position &heads, Position &tails) {
	// Calculate distance
	int32_t totalDistanceX = std::abs(heads.x - tails.x);
	int32_t totalDistanceY = std::abs(heads.y - tails.y);
	
	/*
	int32_t totalDistance = totalDistanceX + totalDistanceY;
	if(totalDistanceX > 1 || totalDistanceY > 1){
		if(totalDistance > 2) {
			//Move diagonally
			if(heads.x > tails.x){
				tails.x++;
			}else{
				tails.x--;
			}

			if(heads.y > tails.y){
				tails.y++;
			}else{
				tails.y--;
			}
		}else{
			//Move straight
			if(totalDistanceX > totalDistanceY) {
				if(heads.x > tails.x){
					tails.x++;
				}else{
					tails.x--;
				}
			}else{
				if(heads.y > tails.y){
					tails.y++;
				}else{
					tails.y--;
				}
			}
		}
	}
	*/

	
	if(totalDistanceX > 1 || totalDistanceY > 1) {
		if(heads.x > tails.x) {
			tails.x++;
		}else if(heads.x < tails.x) {
			tails.x--;
		}

		if(heads.y > tails.y) {
			tails.y++;
		}else if(heads.y < tails.y) {
			tails.y--;
		}
	}
	
}

void moveDirection(Position &pos, char direction) {
	switch(direction) {
		case 'U':
			pos.y++;
		break;
		case 'D':
			pos.y--;
		break;
		case 'R':
			pos.x++;
		break;
		case 'L':
			pos.x--;
		break;
	}
}


int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

	std::unordered_set<uint64_t> uniquePositionsSilver;
	std::unordered_set<uint64_t> uniquePositionsGold;
	
	Position longerRope[10] = {0, 0};
	
	uniquePositionsSilver.reserve(9000000);
	uniquePositionsGold.reserve(9000000);
	uniquePositionsSilver.insert(toU64ForIndexing(longerRope[1]));
	uniquePositionsGold.insert(toU64ForIndexing(longerRope[9]));
	

	std::string line;
	while(std::getline(inputFile, line)) {
		char direction = line[0];
		int32_t numSteps = std::atoi(line.substr(2).c_str());

		for(int32_t i = 0; i < numSteps; i++) {
			// Move head to position indicated
			// Compute physics in order 
			moveDirection(longerRope[0], direction);
			for(uint8_t i = 1; i < 10; i++) {
				doPhysics(longerRope[i-1], longerRope[i]);
			}
			// Silver
			uniquePositionsSilver.insert(toU64ForIndexing(longerRope[1]));
			// Gold
			uniquePositionsGold.insert(toU64ForIndexing(longerRope[9]));
		}
	}

	std::cout << "Unique positions Silver: " << uniquePositionsSilver.size() << std::endl;
	std::cout << "Unique positions Gold: " << uniquePositionsGold.size() << std::endl;

	return 0;
}