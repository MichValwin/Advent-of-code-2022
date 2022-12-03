#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <set>

const char* INPUT_FILE = "input.txt";

// a - z --> 1 - 26
// A - Z --> 27 - 52

std::map<char, uint8_t> priorities;

char findCharDuplicated(std::string line1, std::string line2) {
	std::vector<char> out;
	
	// set_intersection needs input to be sorted
	std::sort(line1.begin(), line1.end());
	std::sort(line2.begin(), line2.end());

	std::set_intersection(line1.begin(), line1.end(), 
							line2.begin(), line2.end(),
							std::back_inserter(out));
	return out[0];
}

char getBadge(std::string line1, std::string line2, std::string line3) {
	std::vector<char> line1IntLine2;
	std::vector<char> out;
	
	// set_intersection needs input to be sorted
	std::sort(line1.begin(), line1.end());
	std::sort(line2.begin(), line2.end());
	std::sort(line3.begin(), line3.end());

	std::set_intersection(line1.begin(), line1.end(), 
							line2.begin(), line2.end(),
							std::back_inserter(line1IntLine2));
	std::sort(line1IntLine2.begin(), line1IntLine2.end());

	std::set_intersection(line3.begin(), line3.end(), 
							line1IntLine2.begin(), line1IntLine2.end(),
							std::back_inserter(out));
				
	return out[0];
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
		std::string part2 = line.substr(lineMidIndex);
	
		std::cout << "size: " << line.size() << " middle index: " << lineMidIndex << " Part1: " << part1 << " # part2: " << part2 << std::endl;
		char duplicatedChar = findCharDuplicated(part1, part2);
		std::cout << "Duplicated: " << duplicatedChar << std::endl;

		sumPriority += priorities[duplicatedChar];

		if(lineCount == 3) {
			lineCount = 0;
			char badge = getBadge(group[0], group[1], group[2]);
			std::cout << "Group badge: " << badge << std::endl;
			sumPriorityBadges += priorities[badge];
		}
	}

	std::cout << "1º part solution: " << sumPriority << std::endl;
	std::cout << "2º part solution: " << sumPriorityBadges << std::endl;

	return 0;
}