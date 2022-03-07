#ifndef _personprofile_
#define _personprofile_
#include <string>
#include <map>
#include <list>
#include "provided.h"
using namespace std;
//incorrect implementation, radix tree needed
class PersonProfile {
public:
	PersonProfile(string name, string email) : m_name(name), m_email(email), numAttVals(0) {};
	string GetName() const {
		return m_name;
	}
	string GetEmail() const {
		return m_email;
	}
	void AddAttValPair(const AttValPair& attval);
	int GetNumAttValPairs() const {
		return numAttVals;
	}
	bool GetAttVal(int attribute_num, AttValPair& attval) const;
private:
	string m_name;
	string m_email;
	int numAttVals;
	map<string, list<AttValPair>> attMap;
};
#endif
