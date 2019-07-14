
#include "Operations.h"
#include<iostream>
#include<vector>
#include<utility>
#include "NFA.h"
#include<stack>
#include<string>

/**
 * Adds n to the variable automatIndex in each state of the automat A
 * */
void Operation::renumberStateIndexes(NFA& A, int n)
{
    for(int i = 0; i < A.sizeSt; i++)
    {
        A.states[i].setAutomatIndex(i + n);
    }
}

/**
 * Adds A's states and B's states to result.
 * Stores the final states of A and B in finalStates
 * and the starting states in startingState
 * */
void Operation::combineStates(NFA& A, NFA& B)
{
    for(int i = 0; i < A.sizeSt; i++)
    {
        if(A.states[i].isFinal()) //keeps all of the starting states
        {
            finalStates.push_back(A.states[i].getID());
        }
        if(A.states[i].isStarting()) //keeps all of the final states
        {
            startingStates.push_back(A.states[i].getID());
        }
        State newState = A.states[i];
        this->result.addState(newState);
    }
    for(int i = 0; i < B.sizeSt; i++)
    {
        if(B.states[i].isFinal()) //keeps all of the starting states
        {
            finalStates.push_back(B.states[i].getID());
        }
        if(B.states[i].isStarting()) //keeps all of the final states
        {
            startingStates.push_back(B.states[i].getID());
        }
        State newState = B.states[i];
        this->result.addState(newState);
    }
}

/**
 * Add the transitions from A and B to result
 * */
void Operation::combineTransitions(NFA& A, NFA& B )
{
    for(int i = 0; i < A.transitionFunction.size(); i++)
    {
        for(int j = 0; j < A.transitionFunction[i].size(); j++)
        {
            int nextSt = A.transitionFunction[i][j].second;
            char transitionLetter = A.transitionFunction[i][j].first;
            this->result.addTransition(A.getStateWithAutomatIndex(i), transitionLetter, A.getStateWithAutomatIndex(nextSt));
        }
    }
    //renumbering the variable automatIndex in the states in the second automat
    //by making them start from the last automatIndex in A
    //this->renumberStateIndexes(B, A.sizeSt + 1);

    for(int i = 0; i < B.transitionFunction.size(); i++)
    {
        for(int j = 0; j < B.transitionFunction[i].size(); j++)
        {
            int nextSt = B.transitionFunction[i][j].second;
            char transitionLetter = B.transitionFunction[i][j].first;
            this->result.addTransition(B.getStateWithAutomatIndex(i), transitionLetter, B.getStateWithAutomatIndex(nextSt));
        }
    }
}

/**
 * Implements the operation union over two automats.
 * Returns a reference to result
 * */
NFA& Operation::automatUnion(NFA& A, NFA& B)
{
    //creating the new starting state
    State start("starting", 0, 1);
    this->result.addState(start);

    //renumbering the indexes of the states in A and B to make "space" for the new starting state
    this->renumberStateIndexes(A, 1);
    this->renumberStateIndexes(B, A.sizeSt + 1);

    this->combineStates(A, B);
    this->combineTransitions(A, B);

    //adds an epsilon transition from the new starting state to the old ones
    int startStA = startingStates[0];
    int startStB = startingStates[1];

    this->result.addTransition(this->result.getStartState(), '$', A.getStateWithID(startStA));
    this->result.addTransition(this->result.getStartState(), '$', B.getStateWithID(startStB));


    //renumbers automat B and A with their original indexes
    this->renumberStateIndexes(B, 0);
    this->renumberStateIndexes(A, 0);

    return this->result;
}


/**
 * Implements the operation concatenation over two automats.
 * Returns a reference to result
 * */
NFA& Operation::automatConcat(NFA& A, NFA& B)
{
    this->renumberStateIndexes(B, A.sizeSt);

    this->combineStates(A, B);
    this->combineTransitions(A, B);

    int finalStatesCounter = 0; //counts the final states in A
    for(int i = 0; i < A.sizeSt; i++)
    {
        if(A.states[i].isFinal())
        {
            finalStatesCounter++;
        }
    }
    //adds epsilon-transitions from A's final states to B's starting state
    //and makes them non-final
    for(int i = 0; i < finalStatesCounter; i++)
    {
        this->result.addTransition(this->result.getStateWithID(finalStates[i]), '$', B.getStartState());
        this->result.getStateWithID(finalStates[i]).setFinal(0);
    }
    this->renumberStateIndexes(B, 0);

    return this->result;
}

/**
 * implements the operation 'Kleene plus' on a single automat
 * Return a reference to result
 * */
NFA& Operation::automatUn(NFA& A)
{
    // copy the ID's of the states from A to result
    for (int i = 0; i < A.sizeSt; i++)
    {
        if(A.states[i].isFinal())
        {
            finalStates.push_back(A.states[i].getID());
        }
    }
    this->result = A;
    //add an epsilon-transition from all final states to the starting state
    for (int i = 0; i < finalStates.size(); i++)
    {
        this->result.addTransition(this->result.getStateWithID(finalStates[i]), '$', A.getStartState());
    }

    return this->result;
}

/**
 * implements the operation 'Kleene stat' on a single automat
 * Return a reference to result
 * */
NFA& Operation::automatKleeneStar(NFA& A)
{
    //create a new starting state
    this->result = A;
    int oldStartID = this->result.getStartState().getID();
    this->result.getStartState().setStarting(0);
    State start("start",0,1); //create a new starting state
    this->result.addState(start);
    this->result.addTransition(this->result.getStartState(), '$', this->result.getStateWithID(oldStartID));

    //finds all final states
    finalStates.push_back(start.getID());
    for (int i = 0; i < this->result.sizeSt; i++)
    {
        if(this->result.states[i].isFinal())
        {
            finalStates.push_back(this->result.states[i].getID());
        }
    }
    //add an epsilon-transition from all final states to the starting state
    for (int i = 0; i < finalStates.size(); i++)
    {
        this->result.addTransition(this->result.getStateWithID(finalStates[i]), '$', this->result.getStartState());
    }
    this->result.getStartState().setFinal(1);

    return this->result;
}
