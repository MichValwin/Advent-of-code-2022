#include <fstream>
#include <iostream>
#include <string>
#include <vector>

const char* INPUT_FILE = "input.txt";

struct SplitString{
    char delimiter;
    std::vector<std::string> strings;
};

struct SectionsWorking{
    uint32_t minSection;
    uint32_t maxSection;
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

SectionsWorking getSections(std::string sectionsUsed) {
    SplitString sectionMinMax = splitString(sectionsUsed, '-');

    uint32_t min = std::stoi(sectionMinMax.strings[0]);
    uint32_t max = std::stoi(sectionMinMax.strings[1]);

    SectionsWorking sections = {min, max};
    return sections;
}

bool isSectionsContained(SectionsWorking sections1, SectionsWorking sections2) {
    // Section 2 contained in 1
    if(sections1.minSection <= sections2.minSection && sections1.maxSection >= sections2.maxSection) return true;
    // Section 1 contained in 2
    if(sections2.minSection <= sections1.minSection && sections2.maxSection >= sections1.maxSection) return true;

    return false;
}


int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

    uint32_t assigmentsPairFullyContained = 0;

    std::string line;
    while(std::getline(inputFile, line)) {
        SplitString pairsNumbers = splitString(line, ',');

        SectionsWorking sectionsElf1 = getSections(pairsNumbers.strings[0]);
        SectionsWorking sectionsElf2 = getSections(pairsNumbers.strings[1]);

        if(isSectionsContained(sectionsElf1, sectionsElf2)){
            assigmentsPairFullyContained++;
        }
    }

    std::cout << "Assigments pair fully contained: " << assigmentsPairFullyContained << std::endl;

	return 0;
}