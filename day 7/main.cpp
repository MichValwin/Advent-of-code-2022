#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>
#include <unordered_set>

const char* INPUT_FILE = "input.txt";

const static uint64_t TOTAL_SIZE_DISK = 70000000;
const static uint64_t SPACE_NEEDED = 30000000;

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

struct File{
    std::string filename;
    uint64_t size;
};

struct TreeNode{
    std::string dirname;
    TreeNode* parentNode;

    std::vector<File> files;
    uint64_t localFileSize;
    uint64_t totalFileSize;
    std::vector<TreeNode*> directories;
};

int main() {
	std::fstream inputFile(INPUT_FILE);

	if(!inputFile.is_open()) {
		std::cout << "Can't open " << INPUT_FILE << std::endl;
		exit(0);
	}

    TreeNode* currentNode = new TreeNode();
    currentNode->dirname = "Source";
    currentNode->parentNode = nullptr;
    currentNode->localFileSize = 0;
    currentNode->totalFileSize = 0;

    std::vector<std::pair<std::string, uint64_t>> dirsWithSizeLess; 

    std::string line;
    while(std::getline(inputFile, line)) {
        std::cout << line << std::endl;

        //Is command
        if(line[0] == '$') {
            if(line[2] == 'c') {
                std::cout << "Command is cd ";

                // cd command
                std::string dirname = line.substr(5, std::string::npos);
                if(dirname.compare("..") == 0) {
                    std::cout << "to back directory" << std::endl;

                    // Before going back, calculate size
                    if(currentNode->directories.size() == 0){
                       currentNode->totalFileSize = currentNode->localFileSize;
                    }else{
                        currentNode->totalFileSize = currentNode->localFileSize;
                        for(TreeNode* n: currentNode->directories){
                            currentNode->totalFileSize += n->totalFileSize;
                        }
                    }
                
                    dirsWithSizeLess.push_back(std::pair<std::string, uint64_t>(currentNode->dirname, currentNode->totalFileSize));

                    // Go back
                    if(currentNode->parentNode != nullptr)currentNode = currentNode->parentNode;
                }else{
                    std::cout << "to new one, dirs inside local: " << currentNode->directories.size() << ", dir exist?: ";
                    // Go into
                    for(TreeNode* n: currentNode->directories) {
                        if(n->dirname.compare(dirname) == 0){
                            currentNode = n;
                            std::cout << dirname << std::endl;
                        }
                    }
                }
            }
        }else{
            SplitString resSplit = splitString(line, ' ');
            
            if(resSplit.strings[0].compare("dir") == 0) {
                std::cout << "Specifies directory: " << resSplit.strings[1] << std::endl;
                // Dir
                std::string dirname = resSplit.strings[1];
                TreeNode* dir = new TreeNode();
                dir->dirname = dirname;
                dir->parentNode = currentNode;
                dir->localFileSize = 0;
                dir->totalFileSize = 0;
                currentNode->directories.push_back(dir);
            }else{
                std::cout << "Specifies file: " << resSplit.strings[1] << ", with size: " << resSplit.strings[0] << std::endl;
                // File
                uint64_t sizeFile = std::atoi(resSplit.strings[0].c_str());
                std::string filename = resSplit.strings[1];
                currentNode->localFileSize += sizeFile;
                currentNode->files.push_back(File{filename, sizeFile});
            }
        }
    }

    while(currentNode->parentNode != nullptr) {
        // Before going back, calculate size
        if(currentNode->directories.size() == 0){
            currentNode->totalFileSize = currentNode->localFileSize;
        }else{
            currentNode->totalFileSize = currentNode->localFileSize;
            for(TreeNode* n: currentNode->directories){
                currentNode->totalFileSize += n->totalFileSize;
            }
        }
                
        dirsWithSizeLess.push_back(std::pair<std::string, uint64_t>(currentNode->dirname, currentNode->totalFileSize));
        currentNode = currentNode->parentNode;
    }
    currentNode->totalFileSize = currentNode->localFileSize;
    for(TreeNode* n: currentNode->directories){
        currentNode->totalFileSize += n->totalFileSize;
    }

    uint64_t usedSpace = currentNode->totalFileSize;
    uint64_t freeSpace = TOTAL_SIZE_DISK - usedSpace;
    uint64_t spaceNeeded = SPACE_NEEDED - freeSpace;

    std::cout << "current node: " << currentNode->totalFileSize << std::endl;
    std::cout << "prev node: " << currentNode->directories[0]->totalFileSize << std::endl;

    std::cout << "Used Space: " << usedSpace << std::endl;
    std::cout << "Free Space: " << freeSpace << std::endl;
    std::cout << "Space needed to be freed " << spaceNeeded << std::endl;

    // print dirs size less than 100000
    uint64_t silverSize = 0;
    std::string dirnameGold;
    uint64_t sizeGold = 30000000;

    for(auto dir: dirsWithSizeLess) {
        if(dir.second <= 100000){
            //std::cout << "Directory: " << dir.first << " size: " << dir.second << std::endl;
            silverSize += dir.second;
        }
        if(dir.second < sizeGold && dir.second >= spaceNeeded) {
            sizeGold = dir.second;
            dirnameGold = dir.first;
        }
    }
    std::cout << "Silver: " << silverSize << std::endl;
    std::cout << "Gold: " << sizeGold << " dirname: " << dirnameGold << std::endl;
    while(currentNode->parentNode != nullptr)currentNode = currentNode->parentNode;
    
   
	return 0;
}