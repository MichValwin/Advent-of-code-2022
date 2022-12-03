#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

const char* INPUT_FILE = "input.txt";

// a - z --> 1 - 26
// A - Z --> 27 - 52

std::map<char, uint8_t> priorities;

char findCharduplicated(std::string line1, std::string line2) {
	std::unordered_set<char> charSet;
	for(size_t i = 0; i < line1.size(); i++) {
		charSet.insert(line1.at(i));
	}
	
	for(size_t i = 0; i < line2.size(); i++) {
		char searchChar = line2.at(i);
		if(charSet.find(searchChar) != charSet.end()) {
			return searchChar;
		}
	}
	
	return '?';
}

char getBadge(std::string line1, std::string line2, std::string line3) {
	std::unordered_set<char> charSet;
	for(size_t i = 0; i < line1.size(); i++) {
		charSet.insert(line1.at(i));
	}

	std::unordered_set<char> duplicates;
	for(size_t i = 0; i < line2.size(); i++) {
		char searchChar = line2.at(i);
		if(charSet.find(searchChar) != charSet.end()) {
			duplicates.insert(searchChar);
		}
	}

	std::cout << line1 << "\n" << line2 << "\n" << line3 << std::endl;

	for (auto const &i: duplicates) {
        std::cout << i << std::endl;
    }

	for(size_t i = 0; i < line3.size(); i++) {
		char searchChar = line3.at(i);
		if(duplicates.find(searchChar) != duplicates.end()) {
			return searchChar;
		}
	}
	return '?'; 
}

int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

	// init map
	char toSet = 'a';
	for(int i = 1; i <= 26; i++) {
		priorities.insert(std::pair<char, uint8_t>(toSet++, i));
	}
	toSet = 'A';
	for(int i = 27; i <= 52; i++) {
		priorities.insert(std::pair<char, uint8_t>(toSet++, i));
	}

	uint64_t sumPriority = 0;
	uint64_t sumPriorityBadges = 0;

	std::string line;
	size_t lineCount = 0;
	std::string group[3];

	while(std::getline(inputFile, line)) {
		group[lineCount] = line;
		lineCount++;

		uint32_t lineMidIndex = uint32_t(line.size() / 2.0);
		std::string part1 = line.substr(0, lineMidIndex);
		std::string part2 = line.substr(lineMidIndex, line.size()-1);
	
		std::cout << "size: " << line.size() << " middle index: " << lineMidIndex << " Part1: " << part1 << " # part2: " << part2 << std::endl;
		char duplicatedChar = findCharduplicated(part1, part2);
		if(duplicatedChar == '?') {
			std::cout << "Bad" << std::endl;
			exit(0);
		}
		std::cout << "Duplicated: " << duplicatedChar << std::endl;

		sumPriority += priorities[duplicatedChar];

		if(lineCount == 3) {
			lineCount = 0;
			char badge = getBadge(group[0], group[1], group[2]);
			std::cout << "Group badge: " << badge << std::endl;
			if (badge == '?')exit(0);
			sumPriorityBadges += priorities[badge];
		}
	}

	std::cout << "1º part solution: " << sumPriority << std::endl;
	std::cout << "2º part solution: " << sumPriorityBadges << std::endl;

	return 0;
}