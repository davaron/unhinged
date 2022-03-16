#include "AttributeTranslator.h"

//#include <iostream>
bool AttributeTranslator::Load(std::string filename) {
	std::ifstream file(filename);
	std::string str;
	int linesLoaded = 0;
	while (getline(file, str)) {
		linesLoaded++;
		if (str != "\n" && str.length() > 0) {
			int firstLetter = 0;
			std::string sAtt, sVal, cAtt, cVal;
			int word = 0;
			for (int i = 0; i < str.length(); i++) {
				if (str[i] == ',' || i == str.length()-1) {
					if (word == 0) {
						sAtt = str.substr(firstLetter, i - firstLetter);
						word++;
						firstLetter = i + 1;
					}
					else if (word == 1) {
						sVal = str.substr(firstLetter, i - firstLetter);
						word++;
						firstLetter = i + 1;
					}
					else if (word == 2) {
						cAtt = str.substr(firstLetter, i - firstLetter);
						word++;
						firstLetter = i + 1;
					}
					else if (word == 3) {
						cVal = str.substr(firstLetter, str.length() - firstLetter);
						break;
					}
				}
			}
			word = 0;
			firstLetter = 0;
			if (sVal.size() != 0 && cAtt.size() != 0 && cVal.size() != 0)
			{
				std::string compatiblePair = cAtt + "," + cVal;
				if (compatTree.search(sVal) != nullptr) {
					compatTree.search(sVal)->insert(compatiblePair);
				}
				else {
					std::unordered_set<std::string> compatSet;
					compatSet.insert(compatiblePair);
					compatTree.insert(sVal, compatSet);
				}
			}
			else {
				return false;
			}
		}
	}
	if (linesLoaded > 0) {
		return true;
	}
	else {
		return false;
	}	
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const{
	std::vector<AttValPair> compatPairs;
	std::unordered_set<std::string>* setPtr = compatTree.search(source.value);
	if (setPtr == nullptr) {
		return compatPairs;
	}
	std::unordered_set<std::string>::const_iterator itr = setPtr->begin();
	while (itr != setPtr->end()) {
		std::string compatSource, compatValue;
		int firstLetter = 0;
		bool isSource = true;
		for (int i = 0; i < (*itr).length(); i++) {
			if ((*itr)[i] == ',' || i == (itr->length() - 1)) {
				if (isSource) {
					compatSource = itr->substr(firstLetter, i - firstLetter);
					firstLetter = i + 1;
					isSource = false;
				}
				if (!isSource) {
					compatValue = itr->substr(firstLetter, i - firstLetter);
					break;
				}
			}
		}
		AttValPair insertMe;
		insertMe.attribute = compatSource;
		insertMe.value = compatValue;
		compatPairs.push_back(insertMe);
		itr++;
	}
	return compatPairs;
}