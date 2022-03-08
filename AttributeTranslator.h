#ifndef _attributetranslator_
#define _attributetranslator_
#include <string>
#include <vector>
#include <list>
#include "RadixTree.h"
#include "provided.h"
class AttributeTranslator {
public:
	AttributeTranslator() {

	}
	bool Load(std::string filename);
	std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
private:
	RadixTree<std::list<AttValPair>> compatTree;
};
#endif