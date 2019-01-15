#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

std::unordered_set<std::string> stopWords;

int initializeStopWords();

int main() {
	int choice = -1;
	std::cout << "WordFrequency Analyzer\n";
	std::cout << "Select an option to begin:\n";
	std::cout << "1. Input txt file name for analysis\n";
	std::cout << "2. Change stop words inclusion settings\n";
	std::cout << "3. View previous analysis\n";
	std::cin >> choice;


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