#ifndef _personprofile_
#define _personprofile_
#include <string>
//STORE AS STRINGS AND USE STRING PROCESSORS TO DIVIDE BETWEEN ATTRIBUTE AND VALUE FASTER THIS WAY TRUST
//deal with duplicate traits
#include <unordered_set>
#include <vector>
#include "RadixTree.h"
#include "provided.h"
//incorrect implementation, radix tree needed
class PersonProfile {
public:
	PersonProfile(std::string name, std::string email) : m_name(name), m_email(email), numAttVals(0) {};
	std::string GetName() const {
		return m_name;
	}
	std::string GetEmail() const {
		return m_email;
	}
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const {
		return numAttVals;
	}
	bool GetAttVal(int attribute_num, AttValPair& attval) const;
private:
	std::string m_name;
	std::string m_email;
	int numAttVals;
	RadixTree<std::unordered_set<std::string>> attTree;
	std::vector<AttValPair> attVec;
};
#endif
