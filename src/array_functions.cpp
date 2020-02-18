/*
 * functionstocomplete.cpp
 *
 *  Created on: Sep 10, 2017
 *      Author: keith, Jonathan Ramey
 */

//============================================================================
//	TODO add necessary includes here
//============================================================================
#include <sstream>
#include <algorithm>
#include "../src/array_functions.h"
#include "../src/utilities.h"
#include "constants.h"

using namespace std;
using namespace constants;
//============================================================================
//	stuff you will need
//============================================================================
//TODO define a structure to track words and number of times they occur
struct tracker {
	string word, upper;
	int count;
};
//TODO add a global array of entry structs (global to this file)
tracker entries[MAX_WORDS];
//TODO add variable to keep track of next available slot in array
int nextAvailable = 0;
//TODO define all functions in header file
void extractTokensFromLine(string &myString) {
	stringstream ss(myString);
	string tempToken;
	while (getline(ss, tempToken, CHAR_TO_SEARCH_FOR)) {
		processToken(tempToken);
	}
}

bool compareCount(tracker &t1, tracker &t2) {
	return t1.count > t2.count;
}

bool compareWord(tracker &t1, tracker &t2) {
	return t1.word > t2.word;
}

void clearArray() {
	nextAvailable = 0;
}

int getArraySize() {
	return nextAvailable;
}

string getArrayWordAt(int i) {
	if (i > getArraySize() || i < 0) {
		return "";
	}
	return entries[i].word;
}

int getArrayWord_NumbOccur_At(int i) {
	if (i > getArraySize() || i < 0) {
		return 0;
	}
	return entries[i].count;
}

bool processFile(fstream &myfstream) {
	if (!myfstream.is_open()) {
		return false;
	}
	string tempLine;
	while (getline(myfstream, tempLine)) {
		processLine(tempLine);
	}
	return true;
}

void processLine(string &myString) {
	extractTokensFromLine(myString);
}

void processToken(string &token) {
	int index = 0;
	bool newWord = true;
	string upcase = token;
	toUpper(upcase);
	strip_unwanted_chars(token);
	if (token.length() == 0) {
		return;
	}
	for (; index < nextAvailable; index++) {
		if (entries[index].upper.compare(upcase) == 0) {
			entries[index].count = entries[index].count + 1;
			newWord = false;
		}
	}
	if (newWord) {
		entries[nextAvailable].word = token;
		toUpper(token);
		entries[nextAvailable].upper = token;
		toUpper(entries[nextAvailable].upper);
		entries[nextAvailable].count = 1;
		nextAvailable++;
	}

}

bool openFile(fstream &myfile, const string &myFileName,
		ios_base::openmode mode) {
	myfile.open(myFileName, mode);
	if(!myfile.is_open()){
		return false;
	}
	return true;
}

void closeFile(fstream &myfile) {
	myfile.close();
}

int writeArraytoFile(const string &outputfilename) {
	ofstream myFile;
	myFile.open(outputfilename, ofstream::binary);
	if (myFile.fail()) {
		return FAIL_FILE_DID_NOT_OPEN;
	} else if (getArraySize() == 0) {
		return FAIL_NO_ARRAY_DATA;
	}
	for (int i = 0; i < getArraySize(); i++) {
		myFile << entries[i].word << " " << entries[i].count <<endl;
	}
	return SUCCESS;
}

void sortArray(constants::sortOrder so) {
	switch (so) {
	case ASCENDING:
		int a, b;
		for (a = 1; a < nextAvailable; a++) {
			tracker key = entries[a];
			b = a - 1;
			while (b >= 0 && entries[b].upper.compare(key.upper) > 0) {
				entries[b + 1] = entries[b];
				b--;
			}
			entries[b + 1] = key;
		}
		break;
	case DESCENDING:
		int c, d;
		for (c = 1; c < nextAvailable; c++) {
			tracker key = entries[c];
			d = c - 1;
			while (d >= 0 && entries[d].upper.compare(key.upper) < 0) {
				entries[d + 1] = entries[d];
				d--;
			}
			entries[d + 1] = key;
		}
		break;
	case NUMBER_OCCURRENCES:
		int i, j;
		for (i = 1; i < nextAvailable; i++) {
			tracker key = entries[i];
			j = i - 1;
			while (j >= 0 && entries[j].count < key.count) {
				entries[j + 1] = entries[j];
				j--;
			}
			entries[j + 1] = key;
		}
		break;
	default:
		return;

	}
}

//TODO look in utilities.h for useful functions, particularly strip_unwanted_chars!

