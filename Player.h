// Player.h

#include <string>
#include <vector>
#include <array>
#include <cstdlib>

using namespace std;

class Player {
public:
    Player() : name(""), score(0) { }
    
    // ctor
    Player(string name_in) : name(name_in), score(0) { }

    // copy ctor
    Player(const Player& other) {
        name = other.name;
        score = other.score;
    }

    string getName() const {
        return name;
    }

    void scorePoints(int ptsEarned) {
        score += ptsEarned;
    }

    int getScore() const {
        return score;
    }

private:
    string name;
    int score;
};

bool operator<(const Player& lhs, const Player& rhs) {
    return lhs.getScore() < rhs.getScore();
}

bool operator==(const Player& lhs, const Player& rhs) {
    return lhs.getName() == rhs.getName() && lhs.getScore() == rhs.getScore();
}

bool areTied(const Player& p1, const Player& p2) {
    if (p1 == p2) { return false; } // You cannot be tied with yourself
    else { return p1.getScore() == p2.getScore(); }
}