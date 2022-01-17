// Team.h

#include "Player.h"
#include <vector>

using namespace std;

class Team {
public:
    Team() : size(0), name(""), score(0) {
        members.clear();
    }
    
    Team(const string& name_in, int numTeammates) : name(name_in), size(numTeammates), score(0) {
        members.clear();
    }

    // copy ctor
    Team(const Team& other) {
        size = other.size;
        name = other.name;
        score = other.score;
        for (int i = 0; i < size; ++i) {
            members.push_back(other.members[i]);
        }
    }

    string getTeamName() const {
        if (name == "") {
            return members[0]->getName() + "'s team";
        }
        else { // There is a team name
            return name;
        }
    }

    void addMember(Player* p) {
        members.push_back(p);
    }

    int getScore() const {
        return score;
    }

    void scorePoints(const int ptsEarned) {
        score += ptsEarned;
        for (int i = 0; i < size; ++i) {
            members[i]->scorePoints(ptsEarned);
        }
    }

    int getSize() const {
        return size;
    }

    bool containsPlayer(const Player* p) const {
        for (Player* member : members) {
            if (*member == *p) {
                return true;
            }
        }
        return false;
    }

    static bool containSameMembers(const Team& lhs, const Team& rhs) {
        for (Player* lhsMember : lhs.members) {
            if (!rhs.containsPlayer(lhsMember)) {
                return false;
            }
        }
        for (Player* rhsMember : lhs.members) {
            if (!lhs.containsPlayer(rhsMember)) {
                return false;
            }
        }
        return true;
    }


private:
    vector<Player*> members;
    int size;
    string name;
    int score;

};

bool operator<(const Team& lhs, const Team& rhs) {
    return lhs.getScore() < rhs.getScore();
}

bool operator==(const Team& lhs, const Team& rhs) {
    return Team::containSameMembers(lhs, rhs);
}

bool areTied(const Team& t1, const Team& t2) {
    if (t1 == t2) { return false; } // A team cannot be tied with itself
    else { return t1.getScore() == t2.getScore(); }
}