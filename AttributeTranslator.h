#ifndef _attributetranslator_
#define _attributetranslator_
#include <string>
#include <vector>
#include <unordered_set>
#include "RadixTree.h"
#include "provided.h"
class AttributeTranslator {
public:
	AttributeTranslator() {

	}
	bool Load(std::string filename);
	std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	RadixTree<std::unordered_set<std::string>> compatTree;
};
#endif