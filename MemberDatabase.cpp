#include "MemberDatabase.h"
#include <iostream>
using namespace std;

MemberDatabase::~MemberDatabase() {
	map<string, PersonProfile*>::const_iterator it = m_database.begin();
	while (it != m_database.end()) {
		delete it->second;
		it = m_database.erase(it);
	}
}
bool MemberDatabase::LoadDatabase(string filename) {
	ifstream file(filename);
	string str;
	while (getline(file, str)) {
		//construct a new profile and add it to the map
		if (str != "\n") {
			string name;
			string email;
			int numAttribs = 0;
			for(int i = 0; i < 3; i++){
				switch (i) {
					case 0:
						name = str;
						break;
					case 1:
						email = str;
						break;
					case 2:
						numAttribs = stoi(str);
						if (numAttribs < 0) {
							return false;
						}
						break;
				}
				getline(file, str);
			}
			PersonProfile* prof;
			for (int i = 0; i < numAttribs; i++) {
				string attrib;
				string val;
				int word = 0;
				int firstLetter = 0;
				for (int i = 0; i < str.length(); i++) {
					if (str[i] == ',' || i == str.length() - 1) {
						if (word == 0) {
							attrib = str.substr(firstLetter, i - firstLetter);
							word++;
							firstLetter = i + 1;
						}
						if (word == 1) {
							val = str.substr(firstLetter, i - firstLetter);
							break;
						}
					}
				}
				AttValPair characteristic(attrib, val);
				AddPair(email, characteristic);
				prof = new PersonProfile(name, email);
				(*prof).AddAttValPair(characteristic);
				getline(file, str);
			}
			int dbSize = m_database.size();
			m_database.insert({ email, prof });
			if (dbSize == m_database.size()) {
				return false;
			}
		}
	}
	return true;
}
vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
	vector<string> matchingmems;
	map<string, list<AttValPair>>::const_iterator emailItr = m_emailMap.begin();
	list<AttValPair>::const_iterator pairListItr = emailItr->second.begin();
	while (emailItr != m_emailMap.end()) {
		pairListItr = emailItr->second.begin();
		while (pairListItr != emailItr->second.end()) {
			if (*pairListItr == input) {
				matchingmems.push_back(emailItr->first);
			}
			pairListItr++;
		}
		emailItr++;
	}
	return matchingmems;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const {
	map<string, PersonProfile*>::const_iterator memberMapIt = m_database.begin();
	while (memberMapIt != m_database.end()) {
		if (memberMapIt->first == email) {
			return memberMapIt->second;
		}
		memberMapIt++;
	}
	return nullptr;
}

void MemberDatabase::AddPair(const string email, const AttValPair& attval) {
	AttValPair addMe(attval.attribute, attval.value);
	list<AttValPair> listPairs;
	if (m_emailMap.find(email) != m_emailMap.end()) {
		listPairs = (m_emailMap.find(email))->second;
	}
	listPairs.push_back(addMe);
	int mapSizeBefore = m_emailMap.size();
	m_emailMap.insert({ email, listPairs });
	if (mapSizeBefore == m_emailMap.size()) {
		m_emailMap.erase(email);
		m_emailMap.insert({ email, listPairs });
	}
}