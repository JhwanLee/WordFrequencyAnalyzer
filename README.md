# WordFrequencyAnalyzer
Jaehwan Lee
## Introduction
This program is a word frequency Analyzer developed in C++. This program analyzes the .txt file input and displays the top 25 most frequently used words.
The user can choose to include or exclude English common stop words (https://www.ranks.nl/stopwords).
Any simple inflections of a common verb will be treated as one word (e.g. roll, rolls, rolled, rolling count as the same word). 
Any simple plural form of a common noun will be treated as one word (e.g. cat and cats count as the same word). 
The most recent 10 analysis are stored, and the user can view the original text, anaylsis result, and the stop word inclusion setting. 

## File Structure
The permanent resource files are stored under Data folder. 'log.txt' file stores the current fileID, number of stored analyses, and name of the input .txt file of each stored analysis.
'record_text_FILEID_FILENAME' stores the copy of the input .txt file.
'record_result_FILEID_FILENAME' stores the stop words inclusion settings as well as the analysis result.

## Usage
To start an analysis, place the .txt file inside the working directory. Type in the option number to navigate around the program.

### Resetting Log file
To reset the log.txt file, erase the content in DATA/log.txt file. 
Type in Staring ID '\n' 0 '\n' to initialize the log.txt file.

## Compiling
Use the makefile in the repo, or use
'g++ -std=c++17 WordFrequencyAnalyzer.cpp -o'

## TODO
1. Implement FILE_ID flush. When the file_id goes over the limit of unsigned int, reset the file_id to 0 and update the current history accordingly.
2. Implement a better stemming algorithm to account for commonly used irregular verbs or other edge cases


## Resources
Stop Words Source: https://www.ranks.nl/stopwords
Verb List Source: https://github.com/aaronbassett/Pass-phrase/blob/master/verbs.txt
Noun List Source: https://www.wordexample.com/list/most-common-nouns-english
