//main.cpp
//#define _CRTDBG_MAP_ALLOC

#include "PersonProfile.h"
#include <iostream>
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <map>
const std::string MEMBERS_FILE = "members.txt";
const std::string TRANSLATOR_FILE = "translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);
void wholeProg() {
    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        std::cout << "Error loading " << MEMBERS_FILE << std::endl;
        return;
    }
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return;
    }

    while (findMatches(mdb, at))
        ;

    std::cout << "Happy dating!" << std::endl;
}
int main() {
    wholeProg();
}

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at)
{
    // Prompt for email
    std::string email;
    const PersonProfile* pp;
    for (;;) {
        std::cout << "Enter the member's email for whom you want to find matches: ";
        std::getline(std::cin, email);
        if (email.empty())
            return false;
        pp = mdb.GetMemberByEmail(email);
        if (pp != nullptr)
            break;
        std::cout << "That email is not in the member database." << std::endl;
    }

    // Show member's attribute-value pairs
    std::cout << "The member has the following attributes:" << std::endl;
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }
    std::cout << pp->GetNumAttValPairs() << std::endl;
    // Prompt user for threshold
    int threshold;
    std::cout << "How many shared attributes must matches have? ";
    std::cin >> threshold;
    std::cin.ignore(10000, '\n');

    // Print matches and the number of matching translated attributes
    MatchMaker mm(mdb, at);
    std::vector<EmailCount> emails = mm.IdentifyRankedMatches(email, threshold);
    if (emails.empty())
        std::cout << "No member was a good enough match." << std::endl;
    else {
        std::cout << "The following members were good matches:" << std::endl;;
        for (const auto& emailCount : emails) {
            const PersonProfile* pp = mdb.GetMemberByEmail(emailCount.email);
            std::cout << pp->GetName() << " at " << emailCount.email << " with "
                << emailCount.count << " matches!" << std::endl;
        }
        std::cout << "Member had " << emails.size() << " matches! " << std::endl;
    }
    std::cout << std::endl;
    return true;
}
using namespace std;
/*int main() {
    //test person profile
    PersonProfile y("Dave", "davertito@gmail.com");
    assert(y.GetName() == "Dave");
    assert(y.GetEmail() == "davertito@gmail.com");
    AttValPair a("hobby", "dancing");
    AttValPair b("hobby", "sitting");
    AttValPair c("hobby", "basketweaving");
    AttValPair g("trait", "evil");
    AttValPair e("occupation", "fisherman");
    AttValPair f("occupation", "janitor");
    AttValPair j("trait", "salacious");
    y.AddAttValPair(a);
    y.AddAttValPair(b);
    y.AddAttValPair(c);
    y.AddAttValPair(g);
    y.AddAttValPair(e);
    y.AddAttValPair(f);
    y.AddAttValPair(j);
    assert(y.GetNumAttValPairs() == 7);
    y.AddAttValPair(a);
    assert(y.GetNumAttValPairs() == 7);
    AttValPair d("jobbr", "yobb");
    y.GetAttVal(0, d);
    assert(d.attribute == "hobby" && d.value == "dancing");
    y.GetAttVal(1, d);
    assert(d.attribute == "hobby" && d.value == "sitting");
    y.GetAttVal(2, d);
    assert(d.attribute == "hobby" && d.value == "basketweaving");
    y.GetAttVal(3, d);
    assert(d.attribute == "trait" && d.value == "evil");
    y.GetAttVal(4, d);
    assert(d.attribute == "occupation" && d.value == "fisherman");
    y.GetAttVal(5, d);
    assert(d.attribute == "occupation" && d.value == "janitor");
    y.GetAttVal(6, d);
    assert(d.attribute == "trait" && d.value == "salacious");
    assert(y.GetNumAttValPairs() == 7);
    cout << "-------------END OF PERSONPROFILE TESTING------------" << endl;
    //test AttributeTranslator
    AttributeTranslator at;
    const string afile = "translator.txt";
    assert(at.Load("transl.txt") == false);
    if (!at.Load(afile))
    {
        cout << "Error loading " << afile << endl;
        return 1;
    }
    AttValPair sourcer("job", "chef");
    AttValPair source("trait", "obnoxious");
    std::map<string, AttValPair> attPair;
    AttValPair bowling("hobby", "bowling");
    AttValPair historian("job", "historian");
    attPair.insert({ "hobby", bowling });
    attPair.insert({ "job", historian });
    vector<AttValPair> vl = at.FindCompatibleAttValPairs(source);
    assert(vl.size() == 2);
    for (int i = 0; i < vl.size(); i++) {
        assert(attPair.find(vl[i].attribute)->second.value == "bowling" || attPair.find(vl[i].attribute)->second.value == "historian");
    }
    cout << "------------------- yay finished ------------------------" << endl;
    MemberDatabase mb;
    const string mfile = "members.txt";
    assert(mb.LoadDatabase(mfile) && !mb.LoadDatabase("membs.txt"));
    if (!mb.LoadDatabase(mfile)) {
        cout << "Error loading " << mfile << endl;
        return 1;
    }
    cout << "begin-- testing pp " << endl;
    const PersonProfile* pp = mb.GetMemberByEmail("MHa7@sky.com");
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }
    MemberDatabase kb;
    const string pfile = "mems.txt";
    kb.LoadDatabase(pfile);
    AttValPair mince("job", "librarian");
    vector<string> vect = kb.FindMatchingMembers(mince);
    for (int i = 0; i < vect.size(); i++) {
        cout << vect[i] << endl;
    }
    //mems.txt contains 9 members
    std::cout << "----------------- testing dupe traits ------------" << std::endl;
    //testing if vector contains dupes
    AttributeTranslator bt;
    const string bfile = "trans.txt";
    bt.Load(bfile);
    AttValPair mice("favorite_food", "del taco");
    vector<AttValPair> vs = bt.FindCompatibleAttValPairs(mice);
    for (int i = 0; i < vs.size(); i++) {
        cout << vs[i].attribute << " " << vs[i].value << endl;
    }

    //testing if there are duplicate matches
    MemberDatabase ab;
    const string gfile = "mems.txt";
    ab.LoadDatabase(gfile);
    MatchMaker bruh(ab, bt);
    vector<EmailCount> jump = bruh.IdentifyRankedMatches("muk@gmail.com", 1);
    for (int k = 0; k != jump.size(); k++) {
        std::cout << jump[k].email << " --> " << jump[k].count << std::endl;
    }
}*/