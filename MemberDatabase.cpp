#include "MemberDatabase.h"

MemberDatabase::~MemberDatabase() {
	//std::cout << "destructing member database" << std::endl;
	for (int i = deleteProfiles.size()-1; i >= 0; i--) {
		//std::cout << "deletin " << deleteProfiles[i]->GetEmail() << std::endl;
		delete deleteProfiles[i];
		deleteProfiles.erase(deleteProfiles.begin()+i);
	}
	deleteProfiles.clear();
}
bool MemberDatabase::LoadDatabase(std::string filename) {
	std::ifstream file(filename);
	std::string str;
	while (getline(file, str)) {
		//construct a new profile and add it to the map
		if (str != "\n") {
			std::string name;
			std::string email;
			int numAttribs = 0;
			for (int i = 0; i < 3; i++) {
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
			PersonProfile* prof = new PersonProfile(name, email);
			deleteProfiles.push_back(prof);
			for (int i = 0; i < numAttribs; i++) {
				std::string attrib;
				std::string val;
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
				(*prof).AddAttValPair(characteristic);
				getline(file, str);
			}

			///////REVERETT ME
			if (m_database.search(email) != nullptr) {
				PersonProfile** pa = (m_database.search(email));
				if ((*pa)->GetEmail() == email) {
					return false;
				}
			}
			m_database.insert(email, prof);
		}
	}
	return true;
}
std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const {
	std::vector<std::string> matchingMems;
	if (m_emailTree.search(input.value) != nullptr) {
		matchingMems = *(m_emailTree.search(input.value));
	}
	return matchingMems;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const {
	//std::cout << (m_database.search(email)) << std::endl;
	if ((m_database.search(email)) == nullptr) {
		return nullptr;
	}
	return *(m_database.search(email));
}

void MemberDatabase::AddPair(const std::string email, const AttValPair& attval) {
	std::vector<std::string> listEmails;
	if (m_emailTree.search(attval.value) != nullptr) {
		m_emailTree.search(attval.value)->push_back(email);
	}
	else {
		std::vector<std::string> newEmailList;
		newEmailList.push_back(email);
		m_emailTree.insert(attval.value, newEmailList);
	}
}