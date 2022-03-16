#include "MatchMaker.h"
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
	const PersonProfile* member = (m_mdb->GetMemberByEmail(email));
	std::vector<AttValPair> attVector;
	AttValPair avp;
	int k = 0;
	std::vector<AttValPair> compatVec;
	std::unordered_set<std::string> setOfCompatibleAttValPairs;
	while ((*member).GetAttVal(k, avp)) {
		compatVec = m_at->FindCompatibleAttValPairs(avp);
		for (int j = 0; j < compatVec.size(); j++) {
			setOfCompatibleAttValPairs.insert(compatVec[j].attribute + "," + compatVec[j].value);
		}
		k++;
	}
	//consolidated set of attribute value pairs with no duplicates
	//all compatible attributes with target profile

	//emails
	std::unordered_set<std::string>::iterator it = setOfCompatibleAttValPairs.begin();
	std::vector<std::string> compatMembers;
	while (it != setOfCompatibleAttValPairs.end())
	{
		int firstLetter = 0;
		bool isSource = true;
		//parsing the set of strings of compatible attribute value pairs
		for (int i = 0; i < (*it).length(); i++) {
			if ((*it)[i] == ',' || i == (it->length() - 1)) {
				if (isSource) {
					avp.attribute = it->substr(firstLetter, i - firstLetter);
					firstLetter = i + 1;
					isSource = false;
				}
				if (!isSource) {
					avp.value = it->substr(firstLetter, i - firstLetter);
					break;
				}
			}
		}
		//vector of compatible members, contains duplicates because it can track how many matches

		std::vector<std::string> matchingMembers = m_mdb->FindMatchingMembers(avp);
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