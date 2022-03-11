#include "MatchMaker.h"
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <map>
#include <iostream>
bool operator<(const EmailCount& lhs, const EmailCount& rhs) {
	if (lhs.count < rhs.count) {
		return true;
	}
	else if (lhs.count == rhs.count) {
		return(lhs.email > rhs.email);
	}
	else {
		return false;
	}
}
std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const {
	const PersonProfile* member = (m_mdb.GetMemberByEmail(email));
	std::vector<AttValPair> attVector;
	AttValPair avp;
	int k = 0;
	while ((*member).GetAttVal(k, avp)) {
		attVector.push_back(avp);
		k++;
	}
	//consolidated set of attribute value pairs with no duplicates
	//all compatible attributes with target profile
	std::map<std::string, AttValPair> pairSet;
	for (int i = 0; i < attVector.size(); i++) {
		std::vector<AttValPair> compatVec = m_at.FindCompatibleAttValPairs(attVector[i]);
		for (int j = 0; j < compatVec.size(); j++) {
			pairSet.insert({ compatVec[j].value, compatVec[j] });
		}
	}
	std::map<std::string, AttValPair>::iterator it = pairSet.begin();
	//emails
	std::vector<std::string> compatMembers;
	while (it != pairSet.end()) {
		std::vector<std::string> matchingMembers = m_mdb.FindMatchingMembers((*it).second);
		for (int i = 0; i < matchingMembers.size(); i++) {
			compatMembers.push_back(matchingMembers[i]);
		}
		it++;
	}
	//email and num of matching attributes
	std::map<std::string, int> freqMap;
	for (int i = 0; i < compatMembers.size(); i++) {
		if (!freqMap.emplace(compatMembers[i], 1).second) {
			int frequency = freqMap.find(compatMembers[i])->second;
			freqMap.erase(compatMembers[i]);
			freqMap.insert({ compatMembers[i], frequency + 1 });
		}
	}
	std::map<std::string, int>::iterator mapIt = freqMap.begin();
	std::vector<EmailCount> matchingEmails;
	while (mapIt != freqMap.end()) {
		if (mapIt->second >= threshold && mapIt->first != email) {
			matchingEmails.push_back(EmailCount(mapIt->first, mapIt->second));
		}
		mapIt++;
	}
	std::sort(matchingEmails.begin(), matchingEmails.end(), operator<);
	std::reverse(matchingEmails.begin(), matchingEmails.end());
	return matchingEmails;
}