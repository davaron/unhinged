#include "PersonProfile.h"
#include <iostream>
bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
	if (attribute_num >= numAttVals || attribute_num < 0) {
		return false;
	}
	attval = attVec[attribute_num];
	return true;
}
void PersonProfile::AddAttValPair(const AttValPair& attval) {
	if (attTree.search(attval.attribute) != nullptr) {
		/*std::list<AttValPair>::iterator it = attTree.search(attval.attribute)->begin();
		while (it != attTree.search(attval.attribute)->end()) {
			if (attval.value == it->value) {
				return;
			}
		}*/
			attTree.search(attval.attribute)->push_back(attval);
	}
	else {
		std::list<AttValPair> newAttList;
		newAttList.push_back(attval);
		attTree.insert(attval.attribute, newAttList);
	}
	attVec.push_back(attval);
	numAttVals++;
}
