#include "PersonProfile.h"
bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const {
	if (attribute_num >= numAttVals || attribute_num < 0) {
		return false;
	}
	attval = attVec[attribute_num];
	return true;
}
void PersonProfile::AddAttValPair(const AttValPair& attval) {
	if (attTree.search(attval.attribute) == nullptr) {
			std::unordered_set<std::string> attSet;
			attSet.insert(attval.value);
			attTree.insert(attval.attribute, attSet);
			attVec.push_back(attval);
			numAttVals++;
			return;
	}
	else if (attTree.search(attval.attribute)->find(attval.value) == attTree.search(attval.attribute)->end()) {
		attTree.search(attval.attribute)->insert(attval.value);
		attVec.push_back(attval);
		numAttVals++;
	}
}
