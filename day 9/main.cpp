#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_set>


static const char* INPUT_FILE = "bigboy.txt";

struct Position{
	int32_t x;
	int32_t y;
};

std::string toStringPos(Position &p) {
	return std::to_string(p.x) + "," + std::to_string(p.y);
}

void doPhysics(Position &heads, Position &tails) {
	// Calculate distance
	int32_t totalDistanceX = std::abs(heads.x - tails.x);
	int32_t totalDistanceY = std::abs(heads.y - tails.y);

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
}

void moveDirection(Position &pos, char direction) {
	switch(direction){
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

	std::unordered_set<std::string> uniquePositionsSilver;
	std::unordered_set<std::string> uniquePositionsGold;

	Position heads = {0, 0};
	Position tails = {0, 0};

	Position longerRope[10] = {0, 0};

	uniquePositionsSilver.insert(toStringPos(tails));
	uniquePositionsGold.insert(toStringPos(longerRope[9]));

    std::string line;
    while(std::getline(inputFile, line)) {
		char direction = line[0];
		int32_t numSteps = std::atoi(line.substr(2).c_str());

		for(int32_t i = 0; i < numSteps; i++) {
			// Move heads
			moveDirection(heads, direction);
			// Compute tail physics
			doPhysics(heads, tails);
			uniquePositionsSilver.insert(toStringPos(tails));

			// Gold
			//Move head??
			moveDirection(longerRope[0], direction);
			for(int i = 1; i < 10; i++) {
				doPhysics(longerRope[i-1], longerRope[i]);
			}

			uniquePositionsGold.insert(toStringPos(longerRope[9]));
		}
    }

	std::cout << "Unique positions Silver: " << uniquePositionsSilver.size() << std::endl;
	std::cout << "Unique positions Gold: " << uniquePositionsGold.size() << std::endl; 

	return 0;
}