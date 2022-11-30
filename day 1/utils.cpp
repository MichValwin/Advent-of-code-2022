#include "utils.hpp"
#include <cstdio>


bool Utils::fileExist(const char* fileName) {
	if (FILE *file = fopen(fileName, "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

char* Utils::readFileText(const char* fileName) {
	std::FILE *file = std::fopen(fileName, "r");

	char* contents = nullptr;
	if (file) {
		std::fseek(file, 0, SEEK_END);
		int size = std::ftell(file);
		contents = new char[size + 1]; //Make sure there is size for the termination character
		std::rewind(file);
		std::fread(contents, 1, size, file);
		std::fclose(file);

		//Put termination character
		contents[size] = '\0';

		return contents;
	} else {
		printf("File: %s cannot be found\n", fileName);
		return nullptr;
	}
}

void Utils::writeFile(const char* fileName, const uint8_t* contents, unsigned int sizebytes) {
	std::FILE *file = std::fopen(fileName, "wb");
	fwrite(contents, sizeof(uint8_t), sizebytes, file);
	fclose(file);
}

void Utils::appendFile(const char* fileName, const uint8_t* contents, unsigned int sizebytes) {
	std::FILE *file = std::fopen(fileName, "ab");
	fwrite(contents, sizeof(uint8_t), sizebytes, file);
	fclose(file);
}