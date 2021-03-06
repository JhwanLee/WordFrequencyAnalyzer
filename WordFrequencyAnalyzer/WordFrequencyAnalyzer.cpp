#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include <ctype.h>
#include <stdlib.h>

std::unordered_set<std::string> stopWords;
std::unordered_set<std::string> commonNouns;
std::unordered_set<std::string> commonVerbs;

int initializeWordSets();
void analyzeText(bool includeStopWords);
void browseLog();
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
	std::cout << "****************************************\n";
	std::cout << "\tWord Frequency Analyzer\n";
	std::cout << "****************************************\n";
	bool includeStopWords = false;
	while (true) {
		std::cin.clear();
		if (includeStopWords) {
			std::cout << "------------------------------------------\n";
			std::cout << "Current Setting: Include Stop Words\n";
			std::cout << "------------------------------------------\n";
		}
		else {
			std::cout << "------------------------------------------\n";
			std::cout << "Current Setting: Don't Include Stop Words\n";
			std::cout << "------------------------------------------\n";
		}
		std::cout << "Select an option:\n";
		std::cout << "1. Input txt file name for analysis\n";
		std::cout << "2. Change stop words inclusion settings\n";
		std::cout << "3. View previous analysis\n";
		std::cout << "4. Quit Program\n";
		std::cout << "Enter your choice: ";
		//I orginally used simple cin stream to get the choice, but if the user doesn't input anything, it had a weird bug of not getting the input for the next loop
		//I modified my code to use getline with a string varaible to fix this bug
		std::cin.clear();
		std::string choice_str;
		std::getline(std::cin, choice_str, '\n');
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
				std::cin.clear();
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
				browseLog();
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
	std::cin.ignore(); // Ignore '\n'
	std::ifstream txtFileStream;
	txtFileStream.open(filename.c_str());
	if (!txtFileStream.is_open()) {
		std::cout << "TXT Filename not found\n\n";
		return;
	}

	//Modify log and save the analysis
	std::ifstream logStream;
	logStream.open("Data/log.txt");
	if (!logStream.is_open()) {
		std::cout << "/Data/log.txt not found\n\n";
		return;
	}

	std::ofstream newLogStream;
	const char * oldName = "Data/log_temp.txt"; 
	const char * newName = "Data/log.txt"; //New name
	newLogStream.open(oldName); //Create a temp log file

	unsigned int newFileID; // 
	logStream >> newFileID;
	newFileID++; //Increment new file number
	newLogStream << newFileID << '\n';

	int numRecord;
	logStream >> numRecord;
	std::string logTemp;
	if (numRecord == 10) {
		newLogStream << 10;
		newLogStream << '\n';
		logStream >> logTemp;
		std::string deleteTextName = "Data/record_text_" + std::to_string(newFileID - 10) + "_" + logTemp;
		std::string deleteResultName = "Data/record_result_" + std::to_string(newFileID - 10) + "_" + logTemp;
		remove(deleteTextName.c_str());
		remove(deleteResultName.c_str());
	}
	else {
		newLogStream << numRecord + 1;
		newLogStream << '\n';
	}

	//Copy the old log to temporary log
	std::vector<std::string> logEntries;
	while (logStream >> logTemp) {
		logEntries.push_back(logTemp);
	}
	logEntries.push_back(filename);
	for (unsigned int i = 0; i < logEntries.size(); i++) {
		newLogStream << logEntries[i] << '\n';
	}
	logStream.close();
	newLogStream.close();
	remove("Data/log.txt"); //Remove the old log file
	rename(oldName, newName); //Rename the log_temp.txt file to log.txt file

	std::string newTextCopyName = "Data/record_text_" + std::to_string(newFileID) + "_" + filename;
	std::string contentCopyTemp;
	std::ifstream contentCopyStream;
	contentCopyStream.open(filename.c_str());
	std::ofstream newFileStream;
	newFileStream.open(newTextCopyName.c_str());
	while (getline(contentCopyStream, contentCopyTemp)) {
		newFileStream << contentCopyTemp << '\n';
	}
	newFileStream.close();
	contentCopyStream.close();

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
	txtFileStream.close();
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
	std::string newResultName = "Data/record_result_" + std::to_string(newFileID) + "_" + filename;
	newFileStream.open(newResultName.c_str());
	newFileStream << includeStopWords << '\n';
	for (int i = 0; i < numberOfEntry; i++) {
		std::cout << i + 1 << ". " << map_vector[i].first << " " << map_vector[i].second << "\n";
		newFileStream << i + 1 << ". " << map_vector[i].first << " " << map_vector[i].second << "\n";
	}
	newFileStream.close();
	std::cout << "\n";
	//Save the Analysis and update the log
}

std::string findRootWord(std::string original) {
	//Convert to lower case (No C++ standard library for to_lower) and remove non-alphabetical characters
	for (unsigned int i = 0; i < original.size(); i++) {
		if (original[i] >= 'A' && original[i] <= 'Z') {
			original[i] = original[i] - ('A' - 'a');
		}
		if (!isalpha(original[i])) {
			original.erase(i, 1);
			i--;
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
		if (original.size() >= 2 && original[original.size() - 2] == 'e' && original.back() == 'd') { //ends wtih 'ed'
			rootWord.pop_back(); //delete 'd'
			rootWord.pop_back(); //delete 'e'
			if (commonVerbs.find(rootWord) != commonVerbs.end()) {
				return rootWord;
			}
		}
		if (original.size() >= 3 && original[original.size() - 3] == 'i' && original[original.size() - 2] == 'n' && original.back() == 'g') {
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

void browseLog() {
	std::ifstream logStream;
	logStream.open("Data/log.txt");
	if (!logStream.is_open()) {
		std::cout << "/Data/log.txt not found\n\n";
		return;
	}
	
	unsigned int newFileID;
	int numEntries;
	logStream >> newFileID;
	logStream >> numEntries;
	
	if (numEntries == 0) {
		std::cout << "No previous analyses to browse\n\n";
		return;
	}
	std::string filename;
	std::vector<std::string> filenames;
	while (std::getline(logStream, filename)) {
		filenames.push_back(filename);
	}
	logStream.close();
	std::reverse(filenames.begin(), filenames.end()); //Reverse the order to display the most recent analysis first
	filenames.pop_back(); //Delete Extra new line at the end
	std::cout << "Select a previous Analysis (Most recent first):\n";

	for (unsigned int i = 0; i < filenames.size(); i++) {
		std::cout << i + 1 << ". " << filenames[i] << '\n';
	}
	int choice;
	std::cin >> choice;
	std::cin.ignore();
	if (choice > filenames.size()) {
		std::cout << "Invalid Choice\n\n";
		return;
	}

	std::string textFileName = "Data/record_text_" + std::to_string(newFileID - (choice - 1)) + "_" + filenames[choice - 1];
	std::string resultFileName = "Data/record_result_" + std::to_string(newFileID - (choice - 1)) + "_" + filenames[choice - 1];

	std::ifstream textFileStream;
	textFileStream.open(textFileName.c_str());

	std::ifstream resultFileStream;
	resultFileStream.open(resultFileName.c_str());

	//Display Setting
	resultFileStream >> choice;
	if (choice == 1) {
		std::cout << "--------------------------------------------\n";
		std::cout << "\nStop Word Setting: Include Stop Words\n\n";
		std::cout << "--------------------------------------------\n";
	}
	else {
		std::cout << "--------------------------------------------\n";
		std::cout << "\nStop Word Setting: Don't Include Stop Words\n\n";
		std::cout << "--------------------------------------------\n";
	}
	//Display Original Text
	std::string displayBuffer;
	std::cout << "\n--------------------------------------------\n";
	std::cout << "Original Text Input:\n";
	std::cout << "--------------------------------------------\n";
	while (std::getline(textFileStream, displayBuffer)) {
		std::cout << displayBuffer << "\n";
	}

	//Display Analysis Result
	std::cout << "\n--------------------------------------------\n";
	std::cout << "Analysis Result: \n";
	std::cout << "--------------------------------------------\n";
	std::getline(resultFileStream, displayBuffer); //To get rid of the '\n' after getting the setting
	while (std::getline(resultFileStream, displayBuffer)) {
		std::cout << displayBuffer << "\n";
	}
	std::cout << "\n";
	textFileStream.close();
	resultFileStream.close();
}