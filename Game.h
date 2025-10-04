// Game.h

#include <vector>
#include <algorithm>
#include <string>
#include <array>
#include <iostream>
#include <map>
#include "Team.h"

using namespace std;

string pluralize(const string& singular, const string& plural, const int num) {
    if (num == 1) {
        return singular;
    }
    else {
        return plural;
    }
}

class Game {
public:
    Game() : gameName(""), numPlayers(0), roundsToEnd(0), roundsSoFar(0),
        roundsDecideEnd(false), pointsToWin(0), ptsAreGood(true),
        areTeams(false), numTeams(0) {
        players.clear();
        teams.clear();
    }

    Game(string gameName_in, int numPlyrs_in, int numRnds_in, bool rndsDcd_in,
        int ptsToWn_in, bool ptsGd_in, string playerNames[], bool areTeams_in,
        int numTeams_in, const int* numPlayersOnEachTeam, const string* teamNames_in)
        : gameName(gameName_in), numPlayers(numPlyrs_in), roundsToEnd(numRnds_in), roundsSoFar(0),
        roundsDecideEnd(rndsDcd_in), pointsToWin(ptsToWn_in), ptsAreGood(ptsGd_in),
        areTeams(areTeams_in), numTeams(numTeams_in) {
        for (int i = 0; i < numPlayers; ++i) {
            players.push_back(new Player(playerNames[i]));
        }
        if (areTeams) { // There are teams
            int index = 0;
            for (int i = 0; i < numTeams; ++i) {
                int numPlayersOnTeam = numPlayersOnEachTeam[i + 1];
                string teamName = teamNames_in[i];
                teams.push_back(new Team(teamName, numPlayersOnTeam));
                for (int j = 0; j < numPlayersOnTeam; ++j) {
                    teams[i]->addMember(players[index]);
                    ++index;
                }
            }
        }
        else { // There are no teams
            teams.clear();
        }
    }

    ~Game() {
        for (int i = 0; i < numPlayers; ++i) {
            delete players[i];
        }
        players.clear();
        for (int i = 0; i < numTeams; ++i) {
            delete teams[i];
        }
        teams.clear();
    }

    void playGame_hlpr() {
        if (roundsDecideEnd) playByRounds();
        else { // Points decide
            playByPoints();
        }
    }

    bool someoneHasWon() const {
        if (!areTeams) { // There are NOT teams
            for (int i = 0; i < numPlayers; ++i) {
                if (players[i]->getScore() >= pointsToWin) return true;
            }
            return false;
        }
        else { // There ARE teams
            for (int i = 0; i < numTeams; ++i) {
                if (teams[i]->getScore() >= pointsToWin) return true;
            }
            return false;
        }
    }

    void printScoreUpdate() const {
        if (!areTeams) { // There are NOT teams
            for (int i = 0; i < numPlayers; ++i) {
                cout << players[i]->getName() << " now has " << players[i]->getScore()
                    << " " << pluralize("point", "points", players[i]->getScore()) << "." << endl;
            }
        }
        else { // There ARE teams
            for (int i = 0; i < numTeams; ++i) {
                cout << teams[i]->getTeamName() << " now has " << teams[i]->getScore()
                    << " " << pluralize("point", "points", players[i]->getScore()) << "." << endl;
            }
        }
    }

    map<string, pair<int, int>> reportResults() const {
        map<string, pair<int, int>> nameToPlaceAndScore;

        vector<Player> pList;
        for (int i = 0; i < numPlayers; ++i) {
            pList.push_back(*(players[i]));
        }
        sort(pList.begin(), pList.end());

        vector<Team> tList;
        for (int i = 0; i < numTeams; ++i) {
            tList.push_back(*(teams[i]));
        }
        sort(tList.begin(), tList.end());

        if (ptsAreGood) {
            // The Player with the most points is at the end of pList
            for (int i = numPlayers - 1; i >= 0; --i) {
                int place = numPlayers - i;
                for (int j = i + 1; j < numPlayers; ++j) {
                    if (areTied(pList[i], pList[j])) {
                        place = numPlayers - j;
                    }
                }
                if (!areTeams) printPlacePlayer(place, pList[i]);
                nameToPlaceAndScore[pList[i].getName()] = pair<int, int>(place, pList[i].getScore());
            }
        }
        else if (!ptsAreGood) {
            // The Player with the least points is at the front of pList
            for (int i = 0; i < numPlayers; ++i) {
                int place = i + 1;
                for (int j = i - 1; j >= 0; --j) {
                    if (areTied(pList[i], pList[j])) {
                        place = j + 1;
                    }
                }
                if (!areTeams) printPlacePlayer(place, pList[i]);
                nameToPlaceAndScore[pList[i].getName()] = pair<int, int>(place, pList[i].getScore());
            }
        }

        if (areTeams) {
            if (ptsAreGood) {
                // The Team with the most points is at the end of tList
                for (int i = numTeams - 1; i >= 0; --i) {
                    int place = numTeams - i;
                    for (int j = i + 1; j < numTeams; ++j) {
                        if (areTied(tList[i], tList[j])) {
                            place = numTeams - j;
                        }
                    }
                    printPlaceTeam(place, tList[i]);
                }
            }
            else if (!ptsAreGood) {
                // The Team with the least points is at the front of tList
                for (int i = 0; i < numTeams; ++i) {
                    int place = i + 1;
                    for (int j = i - 1; j >= 0; --j) {
                        if (areTied(tList[i], tList[j])) {
                            place = j + 1;
                        }
                    }
                    printPlaceTeam(place, tList[i]);
                }
            }
        }
        return nameToPlaceAndScore;
    }

    void printPlacePlayer(const int& place, const Player& p) const {
        if (place == 1) {
            cout << p.getName() << " wins ";
        }
        else if (place == numPlayers) {
            cout << p.getName() << " comes in last ";
        }
        else {
            cout << p.getName() << " gets ";
            if (place % 100 >= 4 && place % 100 <= 20) cout << place << "th";
            else if (place % 10 == 1) cout << place << "st";
            else if (place % 10 == 2) cout << place << "nd";
            else if (place % 10 == 3) cout << place << "rd";
            else cout << place << "th";
            cout << " place ";
        }
        cout << "with a score of " << p.getScore() << "." << endl;
    }

    void printPlaceTeam(const int& place, const Team& t) const {
        if (place == 1) {
            cout << t.getTeamName() << " wins ";
        }
        else if (place == numTeams) {
            cout << t.getTeamName() << " comes in last ";
        }
        else {
            cout << t.getTeamName() << " gets ";
            if (place % 100 >= 4 && place % 100 <= 20) cout << place << "th";
            else if (place % 10 == 1) cout << place << "st";
            else if (place % 10 == 2) cout << place << "nd";
            else if (place % 10 == 3) cout << place << "rd";
            else cout << place << "th";
            cout << " place ";
        }
        cout << "with a score of " << t.getScore() << "." << endl;
    }


private:
    string gameName;

    int numPlayers;
    vector<Player*> players;

    int roundsToEnd;
    bool roundsDecideEnd;
    int roundsSoFar;

    int pointsToWin;
    bool ptsAreGood;

    bool areTeams;
    int numTeams;
    vector<Team*> teams;

    void playByRounds() {
        for (int round = 1; round <= roundsToEnd; ++round) {
            if (roundsToEnd == 1) {
                cout << "Go ahead and play the game." << endl;
                cout << "Just press ENTER when it's over to report scores.";
            }
            else {
                cout << "Go ahead and play round " << round << "." << endl;
                if (round == roundsToEnd) cout << "Last round!" << endl;
                cout << "Press ENTER when you're ready with everyone's points from this round.";
            }
            cin.get();
            // string junk;
            // getline(cin, junk);

            if (!areTeams) { // There are NOT teams
                for (int j = 0; j < numPlayers; ++j) {
                    int ptsEarned;
                    cout << "How many points did " << players[j]->getName() << " get? ";
                    cin >> ptsEarned;
                    players[j]->scorePoints(ptsEarned);
                    while ((getchar()) != '\n'); // Clears the ENTER stuck in cin
                }
            }
            else { // There ARE teams
                for (int j = 0; j < numTeams; ++j) {
                    int ptsEarned;
                    cout << "How many points did " << teams[j]->getTeamName() << " get? ";
                    cin >> ptsEarned;
                    teams[j]->scorePoints(ptsEarned);
                    while ((getchar()) != '\n'); // Clears the ENTER stuck in cin
                }
            }

            if (round != 1 && round != roundsToEnd) printScoreUpdate();
            cout << endl;
        }
        cout << "Game over!" << endl;
    }

    void playByPoints() {
        for (int round = 1; !someoneHasWon(); ++round) {
            cout << "Go ahead and play round " << round << "." << endl;
            cout << "Press ENTER when you're ready with everyone's points from this round.";
            cin.get();
            // string junk;
            // getline(cin, junk);

            if (!areTeams) { // There are NOT teams
                for (int j = 0; j < numPlayers; ++j) {
                    int ptsEarned;
                    cout << "How many points did " << players[j]->getName() << " get? ";
                    cin >> ptsEarned;
                    players[j]->scorePoints(ptsEarned);
                    while ((getchar()) != '\n'); // Clears the ENTER stuck in cin
                }
            }
            else { // There ARE teams
                for (int j = 0; j < numTeams; ++j) {
                    int ptsEarned;
                    cout << "How many points did " << teams[j]->getTeamName() << " get? ";
                    cin >> ptsEarned;
                    teams[j]->scorePoints(ptsEarned);
                    while ((getchar()) != '\n'); // Clears the ENTER stuck in cin
                }
            }

            if (round != 1 && !someoneHasWon()) printScoreUpdate();
            cout << endl;
        }
        cout << "Game over!" << endl;
    }
};