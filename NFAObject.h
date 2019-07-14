
#ifndef NFA_NFAOBJECT_H
#define NFA_NFAOBJECT_H

#include <string>
#include "NFA.h"
#include <vector>
using namespace std;

/**
 * Class used in UserCommands.
 * Adds an ID to each read automat and file
 * from which it has been read
 * */
class NFAObject
{
    NFA automat;
    static unsigned int idCounter;
    unsigned int id;
    string fileOrigin; // the file from with it is read
    friend class UserCommands;
public:

    NFAObject()
    {
        this->idCounter++;
        this->id = idCounter;
    }
    void setID(unsigned int id)
    {
        this->id = id;
    }
    void setFileOrigin(string file)
    {
        this->fileOrigin = file;
    }
    unsigned int getID() const
    {
        return this->id;
    }
    string getFileOrigin() const
    {
        return this->fileOrigin;
    }
};


#endif //NFA_NFAOBJECT_H
