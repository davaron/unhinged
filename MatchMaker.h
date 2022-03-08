#ifndef _MATCHMAKER_H_
#define _MATCHMAKER_H_

#include "provided.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "RadixTree.h"
class MatchMaker {
public:
	MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at): m_mdb(mdb), m_at(at) {

	}
	std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
private:
	MemberDatabase m_mdb;
	AttributeTranslator m_at;
};

#endif