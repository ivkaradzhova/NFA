
#ifndef NFA_USERCOMMANDS_H
#define NFA_USERCOMMANDS_H

#include <string>
#include "NFA.h"
#include "NFAObject.h"
#include <vector>

/**
 * Object from the class is the user command box
 * Implements all of the possible commands
 * */
class UserCommands {

    vector<NFAObject> readAutomats;

    void createState(State& newState);
    void createTransition(NFAObject& newAutomat);

    bool validStateID(int id, NFA automat);
    bool validID(int id);
public:
    void readAutomat(ifstream& file);

    void write(int id, ofstream& file);
    NFA& findAutomatWithID(int id);

    void fileManager();
    void commandsManager(string fileName);
    void printCommands();

    ///functions for the commands
    void saveCom(ofstream& file, string fileName);
    void saveAsCom();
    void listCom();
    void printCom();
    void createAutomat(NFAObject& newAutomat);
    void emptyCom();
    void deterministicCom();
    void unionCom(NFAObject& newAutomat);
    void concatCom(NFAObject& newAutomat);
    void unCom(NFAObject& newAutomat);
    void recognizeCom();
    void regCom(NFAObject& newAutomat);
    void removeEpsilonTransitionsCom(NFAObject& newAutomat);
    void finiteCom();
};



#endif //NFA_USERCOMMANDS_H
