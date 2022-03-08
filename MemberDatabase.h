#ifndef _MEMBERDATABASE_H_
#define _MEMBERDATABASE_H_


#include "provided.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "PersonProfile.h"
class MemberDatabase {
public:
	MemberDatabase() {

	}
	~MemberDatabase();
	bool LoadDatabase(std::string filename);
	std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(std::string email) const;
private:
	RadixTree<PersonProfile*> m_database;
	RadixTree<std::vector<std::string>> m_emailTree;
	void AddPair(const std::string email, const AttValPair& attval);
};


#endif