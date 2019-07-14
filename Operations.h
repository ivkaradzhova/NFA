
#ifndef NFA_OPERATIONS_H
#define NFA_OPERATIONS_H

#include<iostream>
#include<vector>
#include<utility>
#include "NFA.h"

/**
 * Class is used for implementing the regular operations
 * automats.
 * Result of operation is stored in 'result'.
 * Friend class in NFA * */
class Operation {
private:
    NFA result;
    vector<int> startingStates;
    vector<int> finalStates;

    void combineStates(NFA& A, NFA& B);
    void combineTransitions(NFA& A, NFA& B);
    void renumberStateIndexes(NFA& A, int n);

public:
    NFA& automatConcat(NFA& A, NFA& B);
    NFA& automatUnion(NFA& A, NFA& B);
    NFA& automatUn(NFA& A);
    NFA& automatKleeneStar(NFA& A);
};


#endif //NFA_OPERATIONS_H
