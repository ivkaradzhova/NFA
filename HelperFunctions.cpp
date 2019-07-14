
#include "NFA.h"
#include<iostream>
#include<queue>
#include <cstring>
#include <string>
#include "States.h"

using namespace std;

/**
 * Function removes the transitions from the non-reachable states
 * and makes those states non-final.
 * Function is used is empty()
 * */
void NFA::removeNonReachableStates()
{
    vector<bool> usedStates(this->sizeSt, 0);
    State startSt = this->getStartState();
    this->BFS(startSt.getAutomatIndex(), usedStates);

    //removes the unreached with the BFS algorithm states
    for(int i = 0; i < this->sizeSt; i++)
    {
        if(usedStates[i] == 0 && this->getStateWithAutomatIndex(i).isStarting() != 1)
        {
            this->states[i].setFinal(0); //make it a non-final
            this->transitionFunction[i].clear();//removes all the transitions from it
        }
    }
}

/**
 * Function finds all of the states connected to the
 * starting state using the BFS algorithm.
 * Stores those states in usedStates.
 * Used in removeNonReachableStates()
 * */
void NFA::BFS(int startSt, vector<bool>& usedStates)
{
    queue<int> q;
    q.push(startSt);
    usedStates[startSt] = 1;
    while(!q.empty())
    {
        int queueFront = q.front();
        q.pop();
        for(int i = 0; i < this->transitionFunction[queueFront].size(); i++)
        {
            int nextSt = this->transitionFunction[queueFront][i].second;
            if(usedStates[nextSt] == 0)
            {
                q.push(nextSt);
                usedStates[nextSt] = 1;
            }
        }
    }
}


/**
 * Function removes the epsilon-transitions from the automat.
 * */
void NFA::removeEpsilonTransitions()
{
    this->removeNonReachableStates();

    vector<bool> used;
    used.resize(this->sizeSt);
    vector< vector< pair<char, int> > > newTransitions;
    newTransitions.resize(this->sizeSt + 1);
    for(int i = 0; i < this->sizeSt; i++)
    {
        for(char j = 'a'; j <= 'z'; j++)
        {
            this->DFS(i, i, 0, j, newTransitions, used); //finds all the new transitions(without epsilon)
        }
        for(char j = '0'; j <= '9'; j++)
        {
            this->DFS(i, i, 0, j, newTransitions, used); //finds all the new transitions(without epsilon)
        }
    }
    //copies the new transitions without epsilons to the transition function
    this->transitionFunction.clear();
    this->transitionFunction.resize(newTransitions.size());

    for(int i = 0; i < newTransitions.size(); i++)
        for(int j = 0; j < newTransitions[i].size(); j++)
        {
            this->transitionFunction[i].push_back(newTransitions[i][j]);
        }
}


/**
 * Function find all the state that could be reached
 * by reading a single letter and creates new transitions from the startSt
 * to the reached state with that letter.
 * Also determines the final states in the newly created trnsitions
 * Function is used in removeEpsilonTransitions()*
 * */
void NFA::DFS(int startSt, int realStart, int letterCounter, char letter,
              vector<vector<pair<char, int> > >& newTransition, vector<bool>& used)
{
    used[startSt] = 1;
    for(int i = 0; i < this->transitionFunction[startSt].size(); i++)
    {
        char transitionLetter = this->transitionFunction[startSt][i].first;
        int nextSt = this->transitionFunction[startSt][i].second;

        if (used[nextSt] == 0) {
            if (letterCounter == 0)
            {
                if (transitionLetter == '$') {
                    //determines the new final states
                    if(this->getStateWithAutomatIndex(nextSt).isFinal() == 1)
                        this->getStateWithAutomatIndex(startSt).setFinal(1);

                    DFS(nextSt, realStart, letterCounter, letter, newTransition, used);
                }
                if (transitionLetter == letter) {
                    newTransition[realStart].push_back(make_pair(letter, nextSt));
                    DFS(nextSt, realStart, letterCounter + 1, letter, newTransition, used);
                }
            }
            if (letterCounter == 1)
            {
                if (transitionLetter == '$')
                {
                    newTransition[realStart].push_back(make_pair(letter, nextSt));
                    DFS(nextSt, realStart, letterCounter, letter, newTransition, used);
                }
                if (transitionLetter == letter)
                {
                    used[startSt] = 0;
                    return;
                }
            }
        }
        else{
            if ((letterCounter == 0 && transitionLetter == letter)
                || (letterCounter == 1 && transitionLetter == '$')) {
                newTransition[realStart].push_back(make_pair(letter, nextSt));
            }

        }

    }
    used[startSt] = 0;

}

/**
 * Used in recognize().
 * Find all the states that can be reached with a certain word
 * */
void NFA::reachableStatesWith(char* word, int currentLetter, int currentSt, vector<int>& reachableStates, vector<bool>& used)
{
    if(currentLetter == strlen(word) )
    {
        reachableStates.push_back(currentSt);
    }

    for(int i = 0; i < this->transitionFunction[currentSt].size(); i++)
    {
        if(currentLetter < strlen(word) )
        {
            char transitionLetter = this->transitionFunction[currentSt][i].first;
            int nextSt = this->transitionFunction[currentSt][i].second;

            if(transitionLetter == word[currentLetter])
            {
                reachableStatesWith(word, currentLetter + 1, nextSt, reachableStates, used);
            }
            if(transitionLetter == '$')
            {
                reachableStatesWith(word, currentLetter, nextSt, reachableStates, used);
            }
        }
    }
}


