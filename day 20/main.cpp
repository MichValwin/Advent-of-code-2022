#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <algorithm>
#include <queue>
#include <set>
#include <cstdlib>

#include <string.h>

static const char* INPUT_FILE = "input.txt";
static const char* INPUT_TEST_FILE = "input_test.txt";
static const int64_t DECRYPTION_KEY = 811589153;

struct Node {
    Node* before;
    Node* next;
    int64_t number;
};

void printCircularLinkedList(Node& start) {
    Node* nextNode = start.next;
    std::cout << start.number;
    while(&start != nextNode) {
        std::cout << ", " << nextNode->number;
        nextNode = nextNode->next;
    }
    std::cout << "\n";
}

void printVectorNodes(std::vector<Node>& vectNodes) {
    for(size_t i = 0; i < vectNodes.size(); i++) {
        std::cout << vectNodes[i].number;
        if(i < vectNodes.size()-1)std::cout << ", ";
    }
    std::cout << "\n";
}

int64_t getNumberAfter(Node* node, int64_t numToSearch, int64_t count, int64_t sizeList) {
    int64_t positionsToMove = count % sizeList;

    // Find node
    Node* nodeFound = node;
    while(nodeFound->number != numToSearch) {
        nodeFound = nodeFound->next;
    }

    // Go to position
    for(int64_t i = 0; i < positionsToMove; i++) {
        nodeFound = nodeFound->next;
    }
    return nodeFound->number;
}

void mixNodeList(std::vector<Node>& nodeNumbers) {
    for(int i = 0; i < nodeNumbers.size(); i++) {
        Node* toMove = &nodeNumbers[i];
        int64_t positionsToMove = toMove->number % ((int64_t)nodeNumbers.size() - 1);
        //std::cout << toMove->number << " , posTomove: " << positionsToMove <<"\n";

        if(positionsToMove != 0) {
            // Get node to in which to insert 
            Node* nodeToInsert = toMove;
            bool isDirectionPositive = positionsToMove > 0;
            while(positionsToMove != 0) {
                if(positionsToMove < 0) {
                    nodeToInsert = nodeToInsert->before;
                    positionsToMove++;
                }else{
                    nodeToInsert = nodeToInsert->next;
                    positionsToMove--;
                }
            }

            //std::cout << toMove->number << " moves between ";

            // Remove node from linked list
            toMove->before->next = toMove->next;
            toMove->next->before = toMove->before;

            if(isDirectionPositive) {
                //std::cout << nodeToInsert->number << " and " << nodeToInsert->next->number << ":\n";

                // put node after
                toMove->next = nodeToInsert->next;
                toMove->before = nodeToInsert;

                nodeToInsert->next->before = toMove;
                nodeToInsert->next = toMove;
            }else{
                //std::cout << nodeToInsert->number << " and " << nodeToInsert->before->number << ":\n";

                // put node before
                toMove->next = nodeToInsert;
                toMove->before = nodeToInsert->before;

                nodeToInsert->before->next = toMove;
                nodeToInsert->before = toMove;
            }

        }else{
            //std::cout << "0 does not move:\n";
        }

        //printCircularLinkedList(nodeNumbers[0]);
        //std::cout << "\n";
    }
}

void initDoubleLinkedListVector(std::vector<Node>& vectorNodes) {
    for(int i = 0; i < vectorNodes.size(); i++) {
        if(i != 0){
            vectorNodes[i].before = &vectorNodes[(i-1) % vectorNodes.size()];
        }
        vectorNodes[i].next = &vectorNodes[(i+1) % vectorNodes.size()];
    }
    vectorNodes[0].before = &vectorNodes[vectorNodes.size()-1];
}

int main() {
    std::fstream inputFile = std::fstream(INPUT_FILE);

    if(!inputFile.is_open()) {
        std::cout << "Can't open " << INPUT_FILE << std::endl;
        exit(0);
    }
    
    std::vector<Node> numbersBeforeMixSol1;
    std::vector<Node> numbersBeforeMixSol2;

    // Parse
    std::string line;
    while(std::getline(inputFile, line)) {
        int64_t number = std::atoi(line.c_str());
        numbersBeforeMixSol1.push_back(Node{nullptr, nullptr, number});
        numbersBeforeMixSol2.push_back(Node{nullptr, nullptr, number});
    }

    // Init double circular linked list 
    initDoubleLinkedListVector(numbersBeforeMixSol1);
    initDoubleLinkedListVector(numbersBeforeMixSol2);

    // Solution 1
    std::cout << "Initial arrangement\n";
    //printCircularLinkedList(numbersBeforeMixSol1[0]);
    std::cout << "\n";
    

    mixNodeList(numbersBeforeMixSol1);
    int64_t first = getNumberAfter(&numbersBeforeMixSol1[0], 0, 1000, numbersBeforeMixSol1.size());
    int64_t second = getNumberAfter(&numbersBeforeMixSol1[0], 0, 2000, numbersBeforeMixSol1.size());
    int64_t third = getNumberAfter(&numbersBeforeMixSol1[0], 0, 3000, numbersBeforeMixSol1.size());
    std::cout << "Solution 1: First: " << first << ", second: " << second << ", third: " << third << ", solution: " << first + second + third << "\n";

    // Solution 2
    //Multiply each number
    for(size_t i = 0; i < numbersBeforeMixSol2.size(); i++) {
        numbersBeforeMixSol2[i].number *= DECRYPTION_KEY;
    }

    /*
    std::cout << "Initial arrangement\n";
    printCircularLinkedList(numbersBeforeMixSol2[0]);
    std::cout << "\n";
    */

    for(int i = 0; i < 10; i++) {
        mixNodeList(numbersBeforeMixSol2);
        //std::cout << "After " << i+1 << " round of mixing: \n";
        //printCircularLinkedList(numbersBeforeMixSol2[0]);
        //std::cout << "\n";
    }

    //std::cout << "nums on vector: \n";
    //printVectorNodes(numbersBeforeMixSol2);
    //std::cout << "List of nums mixed\n";
    //printCircularLinkedList(numbersBeforeMixSol2[0]);


    first = getNumberAfter(&numbersBeforeMixSol2[0], 0, 1000, numbersBeforeMixSol2.size());
    second = getNumberAfter(&numbersBeforeMixSol2[0], 0, 2000, numbersBeforeMixSol2.size());
    third = getNumberAfter(&numbersBeforeMixSol2[0], 0, 3000, numbersBeforeMixSol2.size());
    std::cout << "Solution 2: First: " << first << ", second: " << second << ", third: " << third << ", solution: " << first + second + third << "\n";

    return 0;
}