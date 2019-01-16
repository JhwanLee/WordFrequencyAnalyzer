#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <utility>

std::unordered_set<std::string> stopWords;

int initializeStopWords();
void analyzeText(bool includeStopWords);
std::string findRootWord(std::string original);

//Defining a custom comparator class to sort the count_map by value
struct compare {
	bool operator()(std::pair<std::string, unsigned int> &lhs, std::pair<std::string, unsigned int> &rhs) {
		return lhs.second > rhs.second;
	}
};

int main() {
	if (initializeStopWords() == -1) {
		std::cout << "Please make sure that stopWords.txt file is present in the directory\n";
	}
	int choice = -1;
	std::cout << "WordFrequency Analyzer\n";
	bool includeStopWords = false;
	while (true) {
		if (includeStopWords) {
			std::cout << "Current Setting: Include Stop Words\n";
		}
		else {
			std::cout << "Current Setting: Don't Include Stop Words\n";
		}
		std::cout << "Select an option:\n";
		std::cout << "1. Input txt file name for analysis\n";
		std::cout << "2. Change stop words inclusion settings\n";
		std::cout << "3. View previous analysis\n";
		std::cout << "Enter your choice: ";
		//I orginally used simple cin stream to get the choice, but if the user doesn't input anything, it had a weird bug of not getting the input for the next loop
		//I modified my code to use getline with a string varaible to fix this bug
		std::string choice_str;
		std::getline(std::cin, choice_str);
		if (choice_str != "1" && choice_str != "2" && choice_str != "3") {
			std::cout << "Invalid Choice. Please input 1, 2, or 3\n\n";
			continue;
		}
		choice = std::stoi(choice_str);
		switch (choice) {
			case 1:
				analyzeText(includeStopWords);
				break;
			case 2:
				std::cout << "\nSelect an option:\n";
				std::cout << "1. Include Stop Words\n";
				std::cout << "2. Don't Include StopWords\n";
				std::cout << "Enter your choice: ";
				std::getline(std::cin, choice_str);
				if (choice_str != "1" && choice_str != "2") {
					std::cout << "Invalid Choice. Please input 1 or 2\n\n";
					break;
				}
				if (choice == 1) {
					includeStopWords = true;
				}
				else if (choice == 2) {
					includeStopWords = false;
				}
				else {
					break;
				}
				std::cout << "Setting changed\n\n";
				break;
			case 3:
				//View previous analysis
				break;
		}
	}

	return 0;
}

int initializeStopWords() {
	std::ifstream stopWordsStream;
	stopWordsStream.open("/Data/stopWords.txt");
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
	//Iterate through each word
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
	std::vector<std::pair<std::string, unsigned int>> map_vector;
	//Copy all the map entry to a vector because C++ does not support sorting of a hashmap by value
	std::copy(count_map.begin(), count_map.end(), std::back_inserter<std::vector<std::pair<std::string, unsigned int>>>(map_vector));
	std::sort(map_vector.begin(), map_vector.end(), compare());
	//Print out top 25
	std::cout << "Word Frequency Analysis Result\n";
	int numberOfEntry = 25;
	if (map_vector.size() < 25) {
		numberOfEntry = map_vector.size();
	}
	
	for (int i = 0; i < numberOfEntry; i++) {
		std::cout << i + 1 << ". " << map_vector[i].first << " " << map_vector[i].second << "\n";
	}
	std::cout << "\n";
	//Save the Analysis and update the log
}

std::string findRootWord(std::string original) {
	//Implement root word extraction algorithm

	//Convert to lower case (No C++ standard library for to_lower)
	for (unsigned int i = 0; i < original.size(); i++) {
		if (original[i] >= 'A' && original[i] <= 'Z') {
			original[i] = original[i] - ('A' - 'a');
		}
	}
	return original;
}