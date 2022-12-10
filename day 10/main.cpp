#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_set>
#include <vector>

static const char* INPUT_FILE = "input.txt";

static const int32_t SCREEN_WIDTH = 40;
static const int32_t SCREEN_HEIGHT = 6;

// Helpers
struct SplitString{
    char delimiter;
    std::vector<std::string> strings;
};

SplitString splitString(std::string str, char delimiter) {
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

struct SignalStrength{
	uint64_t programCounter;
	int32_t regX;
};

void printSignalStrength(std::vector<SignalStrength> log) {
	for(SignalStrength &signalLog: log) {
		std::cout << "Signal strength at: " << signalLog.programCounter << " with regX: " << signalLog.regX << " is: " << signalLog.programCounter*signalLog.regX << std::endl;
	}
}

int32_t getSumSignalStrength(std::vector<SignalStrength> log) {
	int32_t signalStrength = 0;
	for(SignalStrength &signalLog: log) {
		signalStrength += signalLog.programCounter * signalLog.regX;
	}
	return signalStrength;
}

void drawPixel(char screen[], int32_t programCounter, int32_t spritePosition) {
	// Sprite position counts in +0, +1, +2
	if(programCounter%SCREEN_WIDTH >= spritePosition && programCounter%SCREEN_WIDTH <= spritePosition+2) {
		screen[programCounter] = '#';
	}
}

void drawScreen(char screen[]) {
	for(int32_t y = 0; y < SCREEN_HEIGHT; y++) {
		for(int32_t x = 0; x < SCREEN_WIDTH; x++) {
			std::cout << screen[y*SCREEN_WIDTH + x];
		}
		std::cout << std::endl;
	}
}

enum Instructions{noop, addx};

static uint64_t programCounter = 0;
static int32_t regX = 1;

static int32_t instructionTimer = 0;
static Instructions instruction = noop;
static int32_t instructionParam = 0;


int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

	uint32_t signalSaveSilver = 20;

	std::vector<SignalStrength> logSignalStrength;

	char screen[SCREEN_WIDTH*SCREEN_HEIGHT] = {'.'};
	for(int32_t i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++) screen[i] = '.';

	std::string line;
	uint32_t commandLine = 1;
	while(std::getline(inputFile, line)) {
		SplitString commandSplited = splitString(line, ' ');

		if(commandSplited.strings[0].compare("noop") == 0) {
			instructionTimer = 1;
			instruction = noop;
		}else if(commandSplited.strings[0].compare("addx") == 0) {
			instructionParam = std::atoi(commandSplited.strings[1].c_str());

			instructionTimer = 2;
			instruction = addx;
		}

		// Proccess instruction
		while(instructionTimer > 0) {
			programCounter++;

			drawPixel(screen, programCounter, regX);
			if(signalSaveSilver == programCounter) {
				signalSaveSilver+=40;
				SignalStrength signal = {programCounter, regX};
				logSignalStrength.push_back(signal);
				std::cout << "Program counter at: " << programCounter << ", at command line: " << commandLine << " with instr: " << instruction << ", param: " << instructionParam <<  std::endl;
			}

			switch(instruction) {
				case noop:
				break;
				case addx:
					if(instructionTimer == 1)regX += instructionParam;
				break;
			}

			instructionTimer--;
		}

		commandLine++;
	}

	printSignalStrength(logSignalStrength);
	std::cout << "Silver: " << getSumSignalStrength(logSignalStrength) << std::endl; 
	drawScreen(screen);

	return 0;
}