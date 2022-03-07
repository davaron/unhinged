#include "provided.h"
#include "AttributeTranslator.h"
#include <fstream>
#include <algorithm>
using namespace std;
bool AttributeTranslator::Load(string filename) {
	ifstream file(filename);
	string str;
	while (getline(file, str)) {
		if (str != "\n" && str.length() > 0) {
			int firstLetter = 0;
			string sAtt, sVal, cAtt, cVal;
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
			//AttValPair src(sAtt, sVal);
			if (sVal.size() != 0 && cAtt.size() != 0 && cVal.size() != 0)
			{
				AttValPair comp(cAtt, cVal);
				list<AttValPair> setPairs;
				if (compatMap.find(sVal) != compatMap.end()) {
					setPairs = compatMap.find(sVal)->second;
				}
				setPairs.push_back(comp);
				int mapSizeBefore = compatMap.size();
				compatMap.insert({ sVal, setPairs });
				if (mapSizeBefore == compatMap.size()) {
					compatMap.erase(sVal);
					compatMap.insert({ sVal, setPairs });
				}
			}
			else {
				return false;
			}
		}
	}
	if (compatMap.empty()) {
		return false;
	}
	else {
		return true;
	}		
}

vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const{
	vector<AttValPair> compatPairs;
	map<string, list<AttValPair>>::const_iterator itr = compatMap.begin();
	while (itr != compatMap.end()) {
		if (source.value == itr->first) {
			list<AttValPair>::const_iterator it = itr->second.begin();
			while (it != itr->second.end()) {
				if (find(compatPairs.begin(), compatPairs.end(), *it) == compatPairs.end()) {
					compatPairs.push_back(*it);
				}
				it++;
			}
		}
		itr++;
	}
	return compatPairs;
}