#include "provided.h"
#include "AttributeTranslator.h"
#include <fstream>
#include <algorithm>

#include <iostream>
bool AttributeTranslator::Load(std::string filename) {
	std::ifstream file(filename);
	std::string str;
	while (getline(file, str)) {
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
				AttValPair comp(cAtt, cVal);
				if (compatTree.search(sVal) != nullptr) {
					compatTree.search(sVal)->push_back(comp);
				}
				else {
					std::list<AttValPair> newAttList;
					newAttList.push_back(comp);
					compatTree.insert(sVal, newAttList);
				}
			}
			else {
				return false;
			}
		}
	}
	return true;
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const{
	std::vector<AttValPair> compatPairs;
	std::list<AttValPair>* listPtr = compatTree.search(source.value);
	if (listPtr == nullptr) {
		return compatPairs;
	}
	std::list<AttValPair>::const_iterator itr = listPtr->begin();
	while (itr != listPtr->end()) {
		compatPairs.push_back(*itr);
		itr++;
	}
	return compatPairs;
}