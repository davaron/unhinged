#include "MemberDatabase.h"
#include <iostream>
using namespace std;
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
			PersonProfile prof(name, email);
			for (int i = 0; i < numAttribs; i++) {
				getline(file, str);
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
				prof.AddAttValPair(characteristic);
			}
			int dbSize = m_database.size();
			m_database.insert({ email, prof });
			if (dbSize == m_database.size()) {
				//duplicate profile
				return false;
			}
		}
	}
	cout << m_database.size();
	return true;
}
vector<string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
	vector<string> boo;
	return boo;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(string email) const {
	return nullptr;
}