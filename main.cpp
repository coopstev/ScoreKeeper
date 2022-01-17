// main.cpp

#include <string>
#include <vector>
#include <array>
#include <iostream>
#include <fstream>
#include "Game.h"
#include <map>
#include "csvstream.h"
#include <set>
#include <string>

#include <fcntl.h> // This is for the © at the end
#include <io.h> // This is for the © at the end

using namespace std;

string getGameName() {
    cout << "What game are you playing? ";
    string gameName;
    getline(cin, gameName);
    if (gameName == "") {
        cout << endl << "You didn't tell me the name of the game." << endl;
        cout << "Please enter the name of the game you are playing." << endl;
        return getGameName();
    }
    return gameName;
}

int getRoundInfo() {
    cout << "Does your game have a fixed number of rounds? [yes / no] ";
    string roundsDecider;
    cin >> roundsDecider;
    bool roundIsDecider = (roundsDecider == "yes");
    int roundsToEnd = 1;
    if (roundIsDecider) {
        cout << "How many rounds until the game is over? ";
        cin >> roundsToEnd;
    }
    if (roundsToEnd <= 0) {
        cout << endl << "You need at least one round to play a game." << endl;
        cout << "Please enter a positive number of rounds." << endl;
        return getRoundInfo();
    }
    if (!roundIsDecider) roundsToEnd = -1;
    return roundsToEnd;
}

int getPointInfo(const bool roundIsDecider) {
    int pointsToEnd = 1;
    if (roundIsDecider) {
        return -1;
    }
    else { // Points must be the decider
        cout << "How many points until the game is over? ";
        cin >> pointsToEnd;
    }
    if (pointsToEnd <= 0) {
        cout << endl << "Sorry, but the ScoreKeeper only works with positive scores." << endl;
        cout << "Please enter a positive number of points." << endl;
        return getPointInfo(roundIsDecider);
    }
    return pointsToEnd;
}

bool getPtsGoodOrBad() {
    string ptsGoodOrBad;
    cout << "Is it good or bad to have the most points? [good / bad] ";
    cin >> ptsGoodOrBad;
    return ptsGoodOrBad == "good";
}

int getNumTeams(const bool wantsTeams) {
    int numTeams = 0;
    if (wantsTeams) {
        cout << "How many teams are you playing with? ";
        cin >> numTeams;
        if (numTeams <= 0) {
            cout << endl << "You need at least one team to play a game." << endl;
            cout << "Please enter a positive number of teams." << endl;
            return getNumTeams(wantsTeams);
        }
        else {
            return numTeams;
        }
    }
    else {
        return 0;
    }
}

int* getTeamInfo() {
    cout << "Are you playing with teams? [yes / no] ";
    string wantsTeams;
    cin >> wantsTeams;
    int numTeams = getNumTeams(wantsTeams == "yes");
    int* numPlayersOnEachTeam = new int[numTeams + 1];
    numPlayersOnEachTeam[0] = numTeams;
    for (int i = 1; i <= numTeams; ++i) {
        cout << "How many players are on Team" << i << "? ";
        int numTeammates = 0;
        cin >> numTeammates;
        if (numTeammates <= 0) {
            cout << endl << "You need at least one player on a team." << endl;
            cout << "Please enter a positive number of players." << endl;
            --i;
        }
        else {
            numPlayersOnEachTeam[i] = numTeammates;
        }
    }
    return numPlayersOnEachTeam;
}

int getNumPlayers(const int* numPlayersOnEachTeam, const bool areTeams) {
    int numPlayers = 0;
    if (!areTeams) {
        cout << "How many players are playing? ";
        cin >> numPlayers;
        if (numPlayers <= 0) {
            cout << endl << "You need at least one player to play a game." << endl;
            cout << "Please enter a positive number of players." << endl;
            return getNumPlayers(numPlayersOnEachTeam, areTeams);
        }
        else {
            return numPlayers;
        }
    }
    else { // There are teams
        int numTeams = numPlayersOnEachTeam[0];
        for (int i = 1; i < numTeams + 1; ++i) {
            numPlayers += numPlayersOnEachTeam[i];
        }
        return numPlayers;
    }
}

string* getPlayerNames(const int& numPlayers, const int* numPlayersOnEachTeam, const bool areTeams) {
    string* nameArr = new string[numPlayers];
    int index = 0;
    int numTeams = numPlayersOnEachTeam[0];
    if (areTeams) {
        for (int i = 1; i <= numTeams; ++i) {
            cout << endl <<  "Decide who is on Team" << i << "." << endl;
            for (int j = 0; j < numPlayersOnEachTeam[i]; ++j) {
                cout << "On Team" << i << ", what is Player" << j + 1 << "'s name? ";
                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
                // while ((getchar()) != '\n');
                getline(cin, nameArr[index]);
                if (nameArr[index] == "") getline(cin, nameArr[index]);
                // cin >> nameArr[index];
                if (nameArr[index] == "") {
                    cout << endl << "You didn't enter a name for Player" << j + 1 << " on Team" << i << "." << endl;
                    cout << "Please enter Player" << j + 1 << "'s name." << endl;
                    --index;
                    --j;
                }
                ++index;
            }
        }
    }
    else { // There are no teams
        for (int i = 0; i < numPlayers; ++i) {
            cout << "What is Player" << i + 1 << "'s name? ";
            // cin.ignore(numeric_limits<streamsize>::max(), '\n');
            // while ((getchar()) != '\n');
            getline(cin, nameArr[i]);
            if (nameArr[i] == "") getline(cin, nameArr[i]);
            // cin >> nameArr[i];
            if (nameArr[i] == "") {
                cout << endl << "You didn't enter a name for Player" << i + 1 << "." << endl;
                cout << "Please enter Player" << i + 1 << "'s name." << endl;
                --i;
            }
        }
        cout << endl;
    }
    return nameArr;
}

string* getTeamNames(const string* namesArr, const int& numTeams, const int* numPlayersOnEachTeam, const bool areTeams) {
    if (areTeams) {
        string* teamNames = new string[numTeams];
        int index = 0;
        cout << endl;
        for (int i = 0; i < numTeams; ++i) {
            cout << "Do you want " << namesArr[index] << "'s team to have a name? [yes / no] ";
            string wantsName;
            cin >> wantsName;
            if (wantsName == "yes") {
                cout << "What do you want " << namesArr[index] << "'s team's name to be? ";
                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
                // while ((getchar()) != '\n');
                getline(cin, teamNames[i]);
                if (teamNames[i] == "") getline(cin, teamNames[i]);
                if (teamNames[i].length() >= 13) cout << "Great name!" << endl;
            }
            else { // The team does NOT want a name, so we assign an empty name
                while ((getchar()) != '\n'); // Clears the ENTER stuck in cin
                teamNames[i] = "";
            }
            index += numPlayersOnEachTeam[i + 1];
        }
        cout << endl;
        return teamNames;
    }
    else { // There are no teams
        return nullptr;
    }
}

double getNewSkill(const int& place, const int& numPlayers, const int& score, const int& maxScore, const bool ptsAreGood) {
    if (ptsAreGood) {
        return ((1.0 - ((place - 1) / (double)(numPlayers - 1))) + (score / (double)maxScore)) * 50.0;
    }
    else { // Points are bad
        return ((1.0 - ((place - 1) / (double)(numPlayers - 1))) + ((maxScore - score) / (double)maxScore)) * 50.0;
    }
}

double getNewAvg(const double& prev_avg, const int& prev_n, const double& new_value) {
    double prev_total = prev_avg * prev_n;
    double new_total = prev_total + new_value;
    return new_total / (double)(prev_n + 1);
}

void determineNewHighAndLowScores(const string& name, const int& score,
    int& prev_high, int& prev_low, const bool ptsAreGood) {
    if (score > prev_high) {
        if (ptsAreGood) {
            cout << endl << "Personal best!" << endl;
            cout << name << " beat their previous high score of " << prev_high
                << " by " << score - prev_high << " "
                << pluralize("point", "points", score - prev_high) << "!" << endl;
        }
        else { // Points are bad
            cout << endl << "Oof! " << name << " had a rough game." << endl;
            cout << name << " scored higher than their previous worst score of "
                << prev_high << " by " << score - prev_high << " "
                << pluralize("point", "points", score - prev_high) << "." << endl;
        }
        prev_high = score;
    }
    if (score < prev_low) {
        if (ptsAreGood) {
            cout << endl << "Oof! " << name << " had a rough game." << endl;
            cout << name << " scored lower than their previous worst score of "
                << prev_low << " by " << prev_low - score << " "
                << pluralize("point", "points", prev_low - score) << "." << endl;

        }
        else {
            cout << endl << "Personal best!" << endl;
            cout << name << " beat their previous lowest score of " << prev_low
                << " by " << prev_low - score << " "
                << pluralize("point", "points", prev_low - score) << "!" << endl;
        }
        prev_low = score;
    }
}

void addNewEntry(const pair<string, pair<int, int>>& p, ofstream& newData, 
    const int& numPlayers, const int& maxScore, const bool ptsAreGood) {
    int place = (p.second).first;
    int score = (p.second).second;
    string name = p.first;
    newData << name // name
        << "," << 1 // times_played
        << "," << (place == 1) // wins
        << "," << getNewSkill(place, numPlayers, score, maxScore, ptsAreGood) // skill
        << "," << score // avg_score
        << "," << score // high_score
        << "," << score // low_score
        << endl;
}

void updateEntry(const pair<string, pair<int, int>>& p, map<string, string> oldData,
    ofstream& newData, const int& numPlayers, const bool ptsAreGood, const int& maxScore) {
    int place = (p.second).first;
    int score = (p.second).second;
    string name = p.first;
    int timesPlayedBefore = atoi((oldData["times_played"]).c_str());
    int winsBefore = atoi((oldData["wins"]).c_str());
    double avgRatingBefore = atof((oldData["skill"]).c_str());
    double avgScoreBefore = atof((oldData["avg_score"]).c_str());
    int highScore = atoi((oldData["high_score"]).c_str());
    int lowScore = atoi((oldData["low_score"]).c_str());

    determineNewHighAndLowScores(name, score, highScore, lowScore, ptsAreGood);

    newData << name // name 
        << "," << timesPlayedBefore + 1 // times_played
        << "," << winsBefore + (place == 1) // wins
        << "," << getNewAvg(avgRatingBefore, timesPlayedBefore, 
            getNewSkill(place, numPlayers, score, maxScore, ptsAreGood)) // skill
        << "," << getNewAvg(avgScoreBefore, timesPlayedBefore, score) // avg_score
        << "," << highScore // high_score
        << "," << lowScore // low_score
        << endl;
}

int getMaxScore(const map<string, pair<int, int>>& results, const int& numPlayers, const bool ptsAreGood) {
    if (ptsAreGood) {
        for (auto& p : results) {
            if ((p.second).first == 1) {
                return (p.second).second;
            }
        }
    }
    else { // Points are bad
        for (int i = numPlayers; i >= 1; --i) {
            for (auto& p : results) {
                if ((p.second).first == i) {
                    return (p.second).second;
                }
            }
        }
    }
}

void replaceFile(const string& oldDataFileName, const string& newDataFileName) {
    int status = remove(oldDataFileName.c_str());
    if (status == 0) { // The old data file was successfully deleted
        cout << endl << oldDataFileName << " successfully deleted." << endl;
        // Now to rename the new data file..
        int result = rename(newDataFileName.c_str(), oldDataFileName.c_str());
        if (result == 0) { // The new data file was successfully renamed
            cout << newDataFileName << " successfully renamed to "
                << oldDataFileName << "." << endl;
        }
        else { // The new data file was NOT successfully renamed
            cout << "The new data file couldn't be renamed." << endl;
            cout << "You will have to manually rename " << newDataFileName
                << " to " << oldDataFileName << " in the file explorer." << endl;
        }
    }
    else { // The old data file was NOT successfully deleted
        cout << endl << "The old data file couldn't be deleted." << endl;
        cout << "You will have to manually delete the file " << oldDataFileName
            << " and rename " << newDataFileName << " to "
            << oldDataFileName << " in the file explorer." << endl;
    }
}


int main() {
    cout << "Welcome to the ScoreKeeper!" << endl;
    string gameName = getGameName();

    int roundsToEnd = getRoundInfo();
    bool roundIsDecider = (roundsToEnd > 0);

    bool ptsGood = getPtsGoodOrBad();
    int pointsToEnd = getPointInfo(roundIsDecider);

    int* numPlayersOnEachTeam = getTeamInfo();
    int numTeams = numPlayersOnEachTeam[0];
    bool areTeams = numTeams;


    int numPlayers = getNumPlayers(numPlayersOnEachTeam, areTeams);

    string* nameArr = getPlayerNames(numPlayers, numPlayersOnEachTeam, areTeams);
    string* teamNames = getTeamNames(nameArr, numTeams, numPlayersOnEachTeam, areTeams);

    Game game(gameName, numPlayers, roundsToEnd, 
        roundIsDecider, pointsToEnd, ptsGood, nameArr, 
        areTeams, numTeams, numPlayersOnEachTeam, teamNames);
    delete[] teamNames;
    delete[] nameArr;
    delete[] numPlayersOnEachTeam;


    game.playGame_hlpr();
    map<string, pair<int, int>> results = game.reportResults();

    int maxScore = getMaxScore(results, numPlayers, ptsGood);

    string oldDataFileName = gameName + "_data.csv";
    string header = "name,times_played,wins,skill,avg_score,high_score,low_score";
    try {
        csvstream oldData(oldDataFileName, ',', false);
        map<string, string> data;
        oldData >> data;

        string newDataFileName = gameName + "_dataNEW.csv";
        ofstream newData(newDataFileName);
        newData << header << endl;

        set<string> completedNames;
        while (!data.empty()) {
            for (auto& p : results) {
                string name = p.first;
                if (data["name"] == name) {
                    if (completedNames.find(name) == completedNames.end()) {
                        updateEntry(p, data, newData, numPlayers, ptsGood, maxScore);
                        // The updates for this Player are now completed
                        completedNames.insert(name);
                    }
                }
            }
            oldData >> data;
        }
        // All Players in the game that was just played that
        // have a score history just got analyzed and copied over.

        // Now to get the new players
        for (auto& p : results) {
            string name = p.first;
            if (completedNames.find(name) == completedNames.end()) {
                // This Player is a new Player for this game.
                addNewEntry(p, newData, numPlayers, maxScore, ptsGood);
                completedNames.insert(name);
            }
        }
        csvstream oldData2(oldDataFileName, ',', false);
        oldData2 >> data;
        while (!data.empty()) {
            if (completedNames.find(data["name"]) == completedNames.end()) { 
                // This Player was not in the game that was just played,
                // but their data still has to be copied over.
                newData << data["name"]
                    << "," << data["times_played"]
                    << "," << data["wins"]
                    << "," << data["skill"]
                    << "," << data["avg_score"]
                    << "," << data["high_score"]
                    << "," << data["low_score"]
                    << endl;
            }
            oldData2 >> data;
        }
        newData.close();
        oldData.closeIfOpen();
        oldData2.closeIfOpen();
        // oldData2.~csvstream();
        replaceFile(oldDataFileName, newDataFileName);
    }
    catch (const csvstream_exception & e) {
        // cout << endl << e.what() << endl;
        cout << endl << "The data file for this game probably hasn't been created yet." << endl;
        cout << "I'll fix that now and initialize one for you." << endl;
        ofstream newData(oldDataFileName);
        newData << header << endl;
        for (auto& p : results) {
            addNewEntry(p, newData, numPlayers, maxScore, ptsGood);
        }
        newData.close();
    }
    cout << endl << "Good game!" << endl;
    cout << "Thanks for using the ScoreKeeper!" << endl;
    _setmode(_fileno(stdout), _O_U16TEXT); // This is for the © at the end
    char copyright = '\xa9'; // This is the © symbol
    wcout << copyright << " Cooper Stevens" << endl << endl;
    // cout << "(c) Cooper Stevens" << endl << endl;
    wcout << "Press ENTER to close.";
    cin.get();
    return 0;
}