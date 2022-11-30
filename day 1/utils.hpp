#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <cstdint>

namespace Utils{

	char* readFileText(const char* fileName);
	void writeFile(const char* fileName, const uint8_t* contents, unsigned int sizebytes);
	void appendFile(const char* fileName, const uint8_t* contents, unsigned int sizebytes);
}


#endif
