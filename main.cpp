//main.cpp

#include "PersonProfile.h"
#include <iostream>
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>

const std::string MEMBERS_FILE = "members.txt";
const std::string TRANSLATOR_FILE = "translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);

int main() {
    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        std::cout << "Error loading " << MEMBERS_FILE << std::endl;
        return 1;
    }
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return 1;
    }

    while (findMatches(mdb, at))
        ;

    std::cout << "Happy dating!" << std::endl;
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
    }
    std::cout << std::endl;
    return true;
}
/*using namespace std;
int main() {
    //test person profile
    PersonProfile y("Dave", "davertito@gmail.com");
    std::cout << y.GetName() << std::endl;
    std::cout << y.GetEmail() << std::endl;
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
    AttValPair d("jobbr", "yobb");
    y.GetAttVal(0, d);
    std::cout << d.attribute << ", " << d.value << endl;
    y.GetAttVal(1, d);
    cout << d.attribute << ", " << d.value << endl;
    y.GetAttVal(2, d);
    cout << d.attribute << ", " << d.value << endl;
    y.GetAttVal(3, d);
    cout << d.attribute << ", " << d.value << endl;
    y.GetAttVal(4, d);
    cout << d.attribute << ", " << d.value << endl;
    y.GetAttVal(5, d);
    cout << d.attribute << ", " << d.value << endl;
    y.GetAttVal(6, d);
    cout << d.attribute << ", " << d.value << endl;
    cout << y.GetNumAttValPairs() << endl; //6
    cout << "-------------END OF PERSONPROFILE TESTING------------" << endl;
    //test AttributeTranslator
    AttributeTranslator at;
    const string afile = "trans.txt";
    if (!at.Load(afile))
    {
        cout << "Error loading " << afile << endl;
        return 1;
    }
    AttValPair sourcer("favorite_food", "del taco");
    vector<AttValPair> vl = at.FindCompatibleAttValPairs(sourcer);
    for (int i = 0; i < vl.size(); i++) {
        cout << vl[i].attribute << " " << vl[i].value << endl;
    }
    cout << "------------------- yay finished ------------------------" << endl;
    MemberDatabase mb;
    const string mfile = "mems.txt";
    if (!mb.LoadDatabase(mfile)) {
        cout << "Error loading " << mfile << endl;
        return 1;
    }
    cout << "begin-- testing pp " << endl;
    const PersonProfile* pp = mb.GetMemberByEmail("sGv@gmail.com");
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }
    AttValPair mince("job", "fervor");
    vector<string> vect = mb.FindMatchingMembers(mince);
    for (int i = 0; i < vect.size(); i++) {
        cout << vect[i] << endl;
    }
    //mems.txt contains 9 members
}*/