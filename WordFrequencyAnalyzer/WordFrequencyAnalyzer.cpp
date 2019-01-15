#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

std::unordered_set<std::string> stopWords;

int initializeStopWords();
void analyzeText(bool includeStopWords);

int main() {
	initializeStopWords();
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
	stopWordsStream.open("stopWords");
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

}