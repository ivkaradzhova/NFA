
#include "NFA.h"
#include<iostream>
#include<queue>
#include <cstring>
#include "States.h"
using namespace std;

/**
 * Default constuctor: sets the capacity to 10
 */
NFA::NFA()
{
    this->capacitySt = 10;
    this->sizeSt = 0;
    this->states = new State[this->capacitySt];
    this->transitionFunction.resize(this->capacitySt);

}

NFA::NFA(NFA const& other)
{
    this->copy(other);
}

NFA& NFA::operator= (NFA const& other)
{
    if(this != &other)
    {
        delete[] this->states;
        this->transitionFunction.clear();
        this->copy(other);
    }
    return *this;

}

NFA::~NFA()
{
    delete[] states;
}

void NFA::copy(NFA const& other)
{
    this->capacitySt = other.capacitySt;
    this->states = new State[this->capacitySt];
    this->sizeSt = other.sizeSt;
    for(int i = 0; i < this->sizeSt; i++)
    {
        this->states[i] = other.states[i];
    }
    this->transitionFunction.resize(other.capacitySt);
    for(int i = 0; i < other.sizeSt; i++)
    {
        for(int j = 0; j < other.transitionFunction[i].size(); j++)
        {
            char transitionLetter = other.transitionFunction[i][j].first;
            int nextSt = other.transitionFunction[i][j].second;
            this->transitionFunction[i].push_back(make_pair(transitionLetter, nextSt));
        }
    }
}

/**
 * Resizes the states array
 */
void NFA::resizeSt()
{
    this->capacitySt *= 2;
    this->transitionFunction.resize(this->capacitySt);
    State* copyStates = new State[this->capacitySt];
    for(int i = 0; i < this->sizeSt; i++)
    {
        copyStates[i] = this->states[i];
    }
    delete[] this->states;
    this->states = copyStates;
}

/**
 * Adds a state to the automat and keeps track of the number of starting states.
 * Only the first added starting state remains as a starting,
 * if a second starting state is added it makes it a non-starting.
 */
void NFA::addState(State& newState)
{
    if(this->sizeSt >= this->capacitySt)
    {
        this->resizeSt();
        this->transitionFunction.resize(this->capacitySt);
    }
    newState.setAutomatIndex(this->sizeSt);

    if(newState.isStarting())
    {
        for(int i = 0; i < this->sizeSt; i++)
        {
            if(this->states[i].isStarting())
            {
                newState.setStarting(0);
            }
        }
    }
    this->states[sizeSt] = newState;
    this->sizeSt++;
}

/**
 * Adds a transition between two states in the automat
 */
void NFA::addTransition(State const& source, char letter, State const& destination)
{
    transitionFunction[source.getAutomatIndex()].push_back(make_pair(letter, destination.getAutomatIndex()));
}

/**
 * Returns a reference to a state in
 * the automat which has the specified id.
 * If such a state isn't found the function returns
 * the last added state.
 */
State& NFA::getStateWithID(int id) const
{
    int i = 0;
    while(i < this->sizeSt && this->states[i].getID() != id)
    {
        i++;
    }
    if(i == this->sizeSt)
    {
        cerr << "there is no state with ID: "<< id  << " in this automat" << endl;
        return this->states[i - 1];
    }
    else
        return this->states[i];
}


/**
 * Returns a reference to a state in
 * the automat which has the specified index in the array of states.
 * If such a state isn't found the function returns
 * the last added state.
 */
State& NFA::getStateWithAutomatIndex(int index) const
{

    if(index == this->sizeSt)
    {
        cerr << "there is no state with an index in the automat: "<< index  << endl;
        return this->states[index - 1];
    }
    else
        return this->states[index];
}

///returns a reference to the starting state of the automat
State& NFA::getStartState() const
{
    int i = 0;
    while(i < this->sizeSt && this->states[i].isStarting() == 0)
    {
        i++;
    }
    if(i != this->sizeSt)
    {
        return this->states[i];
    }
    else
    {
        cout << "No starting state" << endl;
        return this->states[i - 1];
    }
}

///returns the number of states in the automat
int NFA::getSizeSt() const
{
    return this->sizeSt;
}
