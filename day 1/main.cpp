#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const char* INPUT_FILE = "input.txt";

int main() {
	std::fstream inputFile = std::fstream(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

	std::string line;
	uint64_t elfMax = 0;
	std::vector<uint64_t> elvesCaloriesCarried;

	uint64_t temp_sum = 0;
	while(std::getline(inputFile, line)) {
		if(line.empty()) {
			elvesCaloriesCarried.push_back(temp_sum);
			temp_sum = 0;
		}else{
			temp_sum += std::atoi(line.c_str());
		}
	}

	std::sort(elvesCaloriesCarried.begin(), elvesCaloriesCarried.end());


	uint64_t caloriesCarriedByTopThree = 0;
	for(int i = elvesCaloriesCarried.size()-1; i >= elvesCaloriesCarried.size()-3; i--) {
		caloriesCarriedByTopThree += elvesCaloriesCarried[i];
	}

	std::cout << "Sum calories carried by the top 3 elves: " << caloriesCarriedByTopThree << std::endl;
	

	return 0;
}