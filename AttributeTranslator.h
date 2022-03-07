#ifndef _attributetranslator_
#define _attributetranslator_
#include <string>
#include <vector>
#include <map>
#include <list>
#include "provided.h"
using namespace std;
class AttributeTranslator {
public:
	bool Load(string filename);
	vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	map<string, list<AttValPair>> compatMap;
};
#endif