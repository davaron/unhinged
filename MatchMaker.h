#ifndef _MATCHMAKER_H_
#define _MATCHMAKER_H_

#include "provided.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "RadixTree.h"
#include <set>
#include <vector>
#include <map>
class MatchMaker {
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at): m_mdb(&mdb), m_at(&at) {

	}
	std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
private:
	const MemberDatabase* m_mdb;
	const AttributeTranslator* m_at;
};

#endif