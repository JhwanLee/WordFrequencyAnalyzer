#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

std::unordered_set<std::string> stopWords;

int initializeStopWords();
void analyzeText(bool includeStopWords);
std::string findRootWord(std::string original);

int main() {
	if (initializeStopWords() == -1) {
		std::cout << "Please make sure that stopWords.txt file is present in the directory\n";
	}
	int choice = -1;
	std::cout << "WordFrequency Analyzer\n";
	bool includeStopWords = false;
	while (true) {
		if (includeStopWords) {
			std::cout << "Current Setting: Include Stop Words\n\n";
		}
		else {
			std::cout << "Current Setting: Don't Include Stop Words\n\n";
		}
		std::cout << "Select an option:\n";
		std::cout << "1. Input txt file name for analysis\n";
		std::cout << "2. Change stop words inclusion settings\n";
		std::cout << "3. View previous analysis\n";
		std::cin >> choice;
		switch (choice) {
			case 1:
				analyzeText(includeStopWords);
				break;
			case 2:
				//Change stop word settings
				break;
			case 3:
				//View previous analysis
				break;
			default:
				std::cout << "Invalid Choice. Please input 1, 2, or 3\n\n";
				break;
		}
	}

	return 0;
}

int initializeStopWords() {
	std::ifstream stopWordsStream;
	stopWordsStream.open("stopWords.txt");
	if (!stopWordsStream.is_open()) {
		return -1;
	}
	std::string temp = "";
	while (stopWordsStream >> temp) {
		stopWords.insert(temp);
	}

	return 0;
}

void analyzeText(bool includeStopWords) {
	std::string filename;
	std::unordered_map<std::string, unsigned int> count_map; //Hash map that counts each root word
	std::cout << "Enter TXT Filename: ";
	std::cin >> filename;
	std::ifstream txtFileStream;
	txtFileStream.open(filename.c_str());
	if (!txtFileStream.is_open()) {
		std::cout << "TXT Filename not found\n";
		return;
	}
	std::string temp = "";
	while (txtFileStream >> temp) {
		if (!includeStopWords) {
			if (stopWords.find(temp) != stopWords.end()) { // this word is a stop word
				continue;
			}
		}

		std::string rootWord = findRootWord(temp);
		if (count_map.find(rootWord) == count_map.end()) { //This word hasn't been encountered before
			count_map[rootWord] = 1;
		}
		else {
			count_map[rootWord]++;
		}
	}
	
	//Sort by value
	//Print out top 25
	//Save the Analysis and update the log
}

std::string findRootWord(std::string original) {
	//Implement root word extraction algorithm
	return "";
}