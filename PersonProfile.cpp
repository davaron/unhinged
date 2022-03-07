#include "PersonProfile.h"
using namespace std;
bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
	if (attribute_num >= numAttVals || attribute_num < 0) {
		return false;
	}
	map<string, list<AttValPair>>::const_iterator it = attMap.begin();
	list<AttValPair>::const_iterator itrList;
	int attCounter = 0;
	for (int i = 0; (i < attMap.size()); i++) {
		itrList = it->second.begin();
		for (int j = 1; j < it->second.size() && attCounter < attribute_num; j++) {
			itrList++;
			attCounter++;
			if (attCounter == attribute_num) {
				break;
			}
		}
		if (attCounter == attribute_num) {
				break;
		}
		attCounter++;
		it++;
	}
	if (it != attMap.end()) {
		attval = AttValPair(it->first, (*itrList).value);
		return true;
	}
	else {
		return false;
	}
}
void PersonProfile::AddAttValPair(const AttValPair& attval) {
	AttValPair addMe(attval.attribute, attval.value);
	list<AttValPair> listPairs;
	if (attMap.find(attval.attribute) != attMap.end()) {
		listPairs = (attMap.find(attval.attribute))->second;
	}
	listPairs.push_back(addMe);
	int mapSizeBefore = attMap.size();
	attMap.insert({ attval.attribute, listPairs });
	if (mapSizeBefore == attMap.size()) {
		attMap.erase(attval.attribute);
		attMap.insert({ attval.attribute, listPairs });
	}
	numAttVals++;
}
