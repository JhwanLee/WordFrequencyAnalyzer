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
std::unordered_set<std::string> commonNouns;
std::unordered_set<std::string> commonVerbs;

int initializeWordSets();
void analyzeText(bool includeStopWords);
std::string findRootWord(std::string original);

//Defining a custom comparator class to sort the count_map by value
struct compare {
	bool operator()(std::pair<std::string, unsigned int> &lhs, std::pair<std::string, unsigned int> &rhs) {
		return lhs.second > rhs.second;
	}
};

int main() {
	if (initializeWordSets() == -1) {
		return -1;
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
		std::cout << "4. Quit Program\n";
		std::cout << "Enter your choice: ";
		//I orginally used simple cin stream to get the choice, but if the user doesn't input anything, it had a weird bug of not getting the input for the next loop
		//I modified my code to use getline with a string varaible to fix this bug
		std::string choice_str;
		std::getline(std::cin, choice_str);
		if (choice_str != "1" && choice_str != "2" && choice_str != "3" && choice_str != "4") {
			std::cout << "Invalid Choice. Please input 1, 2, 3, or 4\n\n";
			continue;
		}
		choice = std::stoi(choice_str);
		if (choice == 4) {
			break;
		}
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

int initializeWordSets() {
	//Stop Words Initialization
	std::ifstream stopWordsStream;
	stopWordsStream.open("Data/stopWords.txt");
	if (!stopWordsStream.is_open()) {
		std::cout << "Please make sure that stopWords.txt file is present in the Data directory\n";
		return -1;
	}
	std::string temp = "";
	while (stopWordsStream >> temp) {
		stopWords.insert(temp);
	}
	stopWordsStream.close();

	//Common Nouns Initialization
	std::ifstream commonNounsStream;
	commonNounsStream.open("Data/commonNouns.txt");
	if (!commonNounsStream.is_open()) {
		std::cout << "Please make sure that commonNouns.txt file is present in the Data directory\n";
		return -1;
	}
	while (commonNounsStream >> temp) {
		commonNouns.insert(temp);
	}
	commonNounsStream.close();

	//Common Verbs Initialization
	std::ifstream commonVerbsStream;
	commonVerbsStream.open("Data/commonVerbs.txt");
	if (!commonVerbsStream.is_open()) {
		std::cout << "Please make sure that commonVerbs.txt file is present in the Data directory\n";
		return -1;
	}
	while (commonVerbsStream >> temp) {
		commonVerbs.insert(temp);
	}
	commonVerbsStream.close();

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
	//Convert to lower case (No C++ standard library for to_lower)
	for (unsigned int i = 0; i < original.size(); i++) {
		if (original[i] >= 'A' && original[i] <= 'Z') {
			original[i] = original[i] - ('A' - 'a');
		}
	}

	std::string rootWord = original;
	//'s' case
	if (commonNouns.find(original) == commonNouns.end()) { //Not a common noun
		if (original.back() == 's') { //Check if this word might be a plural form of a common noun
			//If the 's'-removed word is one of the common nouns, return the root word. Else, return the original
			rootWord.pop_back(); // delete the 's'
			if (commonNouns.find(rootWord) != commonNouns.end()) { //rootWord is one of the common nouns
				return rootWord;
			}
			if (commonVerbs.find(rootWord) != commonVerbs.end()) {
				return rootWord;
			}
		}
	}

	//Verb Case
	if (commonNouns.find(original) == commonNouns.end()) { //Not a common noun
		if (original[original.size() - 2] == 'e' && original.back() == 'd') { //ends wtih 'ed'
			rootWord.pop_back(); //delete 'd'
			rootWord.pop_back(); //delete 'e'
			if (commonVerbs.find(rootWord) != commonVerbs.end()) {
				return rootWord;
			}
		}
		if (original[original.size() - 3] == 'i' && original[original.size() - 2] == 'n' && original.back() == 'g') {
			rootWord.pop_back(); //delete 'i'
			rootWord.pop_back(); //delete 'n'
			rootWord.pop_back(); //delete 'g'
			if (commonVerbs.find(rootWord) != commonVerbs.end()) {
				return rootWord;
			}
		}

		return original;
	}
	return original;
}