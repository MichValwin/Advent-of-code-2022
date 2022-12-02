#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

const char* INPUT_FILE = "input.txt";

struct Game {
	uint8_t oponent;
	uint8_t myHand;
};

int8_t calculateHand[3][3] = {
	{2, 0, 1}, // oponent Rock
	{0, 1, 2}, // oponent Paper
	{1, 2, 0} // oponent Scissors
};

int8_t matchDecider[3][3] = {
	{0, -1, 1}, // Rock
	{1, 0, -1}, // Paper
	{-1, 1, 0} // Scissors
};

uint32_t calculatePuntuation(Game* game) {
	int32_t handPoint = game->myHand + 1;
	int32_t matchPoint = (matchDecider[game->myHand][game->oponent]+1) *3 ;
	return handPoint + matchPoint;
}

int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

	std::string line;
	std::vector<uint32_t> gamesPuntuation1;
	std::vector<uint32_t> gamesPuntuation2;

	while(std::getline(inputFile, line)) {
		Game game;
		game.oponent = uint8_t(line.c_str()[0] - 'A');
		uint8_t secondChar = uint8_t(line.c_str()[2] - 'X');

		// First part
		game.myHand = secondChar;
		uint32_t gamePoints1 = calculatePuntuation(&game);
		gamesPuntuation1.push_back(gamePoints1);

		// Second part
		game.myHand = calculateHand[game.oponent][secondChar];
		uint32_t gamePoints2 = calculatePuntuation(&game);
		gamesPuntuation2.push_back(gamePoints2);
	}

	uint32_t total_puntuation = 0;
	for(uint32_t i = 0; i < gamesPuntuation1.size(); i++) {
		total_puntuation += gamesPuntuation1[i];
	}
	std::cout << "Total points first part: " << total_puntuation << std::endl;
	total_puntuation = 0;
	for(uint32_t i = 0; i < gamesPuntuation2.size(); i++) {
		total_puntuation += gamesPuntuation2[i];
	}
	std::cout << "Total points sedond part: " << total_puntuation << std::endl;

	return 0;
}