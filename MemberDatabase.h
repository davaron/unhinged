#include "provided.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "PersonProfile.h"
using namespace std;
class MemberDatabase {
public:
	MemberDatabase() {

	}
	~MemberDatabase();
	bool LoadDatabase(string filename);
	vector<string> FindMatchingMembers(const AttValPair& input) const;
	const PersonProfile* GetMemberByEmail(string email) const;
private:
	map<string, PersonProfile*> m_database;
	map<string, list<AttValPair>> m_emailMap;
	void AddPair(const string email, const AttValPair& attval);
};