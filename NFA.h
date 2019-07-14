
#ifndef NFA_NFA_H
#define NFA_NFA_H

#include "States.h"
#include<iostream>
#include<vector>
#include<utility>
using namespace std;
/**
 * Class NFA:
 *uses dynamic memory for the array of states
 * */
class NFA
{
private:
    unsigned int capacitySt; //allocated memory for states
    unsigned int sizeSt; //number of added states at the current moment
    State* states;
    vector< vector< pair<char, unsigned int> > > transitionFunction;

    void copy(NFA const& other);
    void resizeSt(); //resize states array;

    State& getStateWithID(int id) const;
    State& getStateWithAutomatIndex(int index) const;
    State& getStartState() const;
    void reachableStatesWith(char* word, int currentLetter, int currentSt, vector<int>& reachableStates, vector<bool>& used);
    void DFS(int startSt, int realStart, int letterCounter, char letter, vector<vector<pair<char, int> > >& newTransition, vector<bool>& used);
    void BFS(int startSt, vector<bool>& usedStates);

    friend class Operation;
    friend class NFAObject;
    friend class UserCommands;

public:
    NFA();
    NFA(NFA const& other);
    NFA& operator= (NFA const& other);
    ~NFA();

    int getSizeSt() const;
    void addState(State& newState);
    void addTransition(State const& source, char letter, State const& destination);

    void print();
    bool empty();

    void removeNonReachableStates();
    void removeEpsilonTransitions();
    bool deterministic();
    bool recognize(char* word);

    bool isFinite();
    bool checkForCycles(int startSt, vector<bool> used);

};

#endif //NFA_NFA_H
