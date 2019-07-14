#include "UserCommands.h"
//#include "Operations.h"
//#include "RegularOperations.h"
#include "RegularExpression.h"
#include "OnlyCommands.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

unsigned int NFAObject::idCounter = 0;


/**
 * Reads a single automat from a file,
 * creates a NFAObject and saves in the vector
 * */
void UserCommands::readAutomat(ifstream& file)
{
    int id, sizeSt;
    NFAObject newObject;
    file >> id;
    if(!file) return;

    file >> sizeSt;
    for (int i = 0; i < sizeSt; i++) {
        string name;
        bool final, starting;
        file >> name >> final >> starting;
        State newState(name.c_str(), final, starting);
        newObject.automat.addState(newState);
    }

    for (int i = 0; i < sizeSt; i++) {
        int currStIndex, numTransitions, nextStIndex;
        char transitionLetter;

        file >> currStIndex >> numTransitions;
        for (int j = 0; j < numTransitions; j++) {
            file >> transitionLetter >> nextStIndex;
            newObject.automat.addTransition(newObject.automat.getStateWithAutomatIndex(currStIndex),
                    transitionLetter,
                    newObject.automat.getStateWithAutomatIndex(nextStIndex));
        }
    }
    newObject.setID(id);
    this->readAutomats.push_back(newObject);
}

/**
 * Returns a reference to a read NFAObject with the specified id*/
NFA& UserCommands::findAutomatWithID(int id)
{
    int i = 0;
    while (i < this->readAutomats.size() && readAutomats[i].getID() != id)
    {
        i++;
    }
    if(i < this->readAutomats.size())
    {
        return this->readAutomats[i].automat;
    }
}

/**
 * Writes the information about an NFAObject in a file.
 * Object is found in the vector of objects by it's ID*/
void UserCommands::write(int id, ofstream& file)
{
    NFA currentAutomat = this->findAutomatWithID(id);
    file << id << endl;
    file << currentAutomat.getSizeSt() << endl;

    for(int i = 0; i < currentAutomat.getSizeSt(); i++)
    {
        file << currentAutomat.states[i].getName() <<  " " <<
             currentAutomat.states[i].isFinal() <<  " " <<
             currentAutomat.states[i].isStarting() << endl;
    }
    for(int i = 0; i < currentAutomat.sizeSt; i++)
    {
        file << i << " " << currentAutomat.transitionFunction[i].size() << endl;
        for(int j = 0; j < currentAutomat.transitionFunction[i].size(); j++)
        {
            char transitionLetter = currentAutomat.transitionFunction[i][j].first;
            int nextSt = currentAutomat.transitionFunction[i][j].second;
            file << transitionLetter << " " << nextSt << endl;
        }
    }
}

/**
 * Function used in createAutomat*/
void UserCommands::createState(State& newState)
{
    string name, starting, final;
    cout << "Name: ";
    cin >> name;
    newState.setName(name.c_str());
    cout <<"Is it final (yes/no): ";
    cin >> final;
    if(final == "yes")
        newState.setFinal(1);
    cout << "Is it starting (yes/no): ";
    cin >> starting;
    if(starting == "yes")
        newState.setStarting(1);

}

/**
 * Check if a state with such ID exisists*/
bool UserCommands::validStateID(int id, NFA automat)
{
    int i = 0;
    while(i < automat.sizeSt && automat.states[i].getID() != id)
    {
        i++;
    }
    if(i == automat.sizeSt)
        return 0;
    else
        return 1;
}

/**
 * Function used in createAutomat*/
void UserCommands::createTransition(NFAObject& newAutomat)
{
    int source, destination;
    char transitionLetter;
    cout << "From (input state's id): "; cin >> source;
    if(!this->validStateID(source, newAutomat.automat))
    {
        cout << "Invalid state ID" << endl;
        return;
    }
    cout << endl << "With letter (possible: a-z, 0-9, $): ";
    cin >> transitionLetter;
    cout << endl << "To (input state's id): "; cin >> destination;
    if(!this->validStateID(source, newAutomat.automat))
    {
        cerr << "Invalid state ID" << endl;
        return;
    }

    newAutomat.automat.addTransition(newAutomat.automat.getStateWithID(source),
            transitionLetter, newAutomat.automat.getStateWithID(destination));

    cout << "Successfully added a transition in the automat" << endl;

}

/**
 * Adds a new automat to the vector of objects*/
void UserCommands::createAutomat(NFAObject& newAutomat)
{
    cout << "Add states: " << endl;
    string command;
    cout << "Commands: '>+state' - add a state " <<
            "          '>+transition' - add a transition"
            "          '>ready' - finish th automat"<< endl;
    do
    {
        cin >> command;
        if(command == ">+state")
        {
            State newState;
            this->createState(newState);
            newAutomat.automat.addState(newState);
            cout << "Added state with ID: " << newState.getID() <<
                    " Name: " << newState.getName() << endl;
        }
        if(command == ">+transition")
        {
            this->createTransition(newAutomat);
        }
    }while(command != ">ready");
}

/**
 * Prints all posssible commands
 * */
void UserCommands::printCommands()
{
    cout << ">save - saves an automat in the currently opened file" << endl
         << ">save as - saves an automat in a different file" << endl
         << ">print - prints an automat" << endl
         << ">create automat - creates a new automat" << endl
         << ">list - lists all of the id's" << endl
         << ">empty - checks if the language is empty" << endl
         << ">deterministic - checks if the language is deterministic" << endl
         << ">finite - checks if the language is finite" << endl
         << ">union - union of two automats" << endl
         << ">concat - concatenation of two automats" << endl
         << ">un - Kleene plus operation on one automat" << endl
         << ">remove epsilons - removes the epsilons" << endl
         << ">reg - creates an automat from a reg expression" << endl
         << ">recognize - checks if an automat recognizes a word" << endl;
}

/**
 * Reads the user's commands before he has opend a file
 * */
void UserCommands::fileManager()
{
    string command, fileName;
    do
    {
        cout << "There is no opened file at the moment" << endl;
        getline(cin, command);
        if(command == ">open")
        {
            ifstream file;
            cout << "Input file name: "; cin >> fileName;

            file.open(fileName, ios::in);
            clog << "Successfully opened " << fileName << endl;

            while(file)
            {
                this->readAutomat(file);
            }
            file.close();
            this->commandsManager(fileName);
        }
    }
    while(command != ">exit");
    clog << "Exiting the program" << endl;
}

/**
 * Reads the user's commands after a file has been opend.
 * Finishes on command:close
 * */
void UserCommands::commandsManager(string fileName)
{
    string command;
    cout << endl << "Write '>info' for information about possible commands" << endl;
    do{
        if(command == ">info")
        {
            this->printCommands();
        }
        getline(cin, command);
        if(command == ">list")
        {
            this->listCom();
        }
        if(command == ">print")
        {
            this->printCom();
        }
        if(command == ">create automat")
        {
            NFAObject newAutomat;
            this->createAutomat(newAutomat);
            readAutomats.push_back(newAutomat);
            clog << "Successfully added a new automat" << endl;
        }
        if(command == ">save")
        {
            ofstream file;
            this->saveCom(file, fileName);
        }
        if(command == ">save as")
        {
            this->saveAsCom();
        }
        if(command == ">empty")
        {
            this->emptyCom();
        }
        if(command == ">deterministic")
        {
            this->deterministicCom();
        }
        if(command == ">union")
        {
            NFAObject newAutomat;
            this->unionCom(newAutomat);
            readAutomats.push_back(newAutomat);
        }
        if(command == ">concat")
        {
            NFAObject newAutomat;
            this->concatCom(newAutomat);
            readAutomats.push_back(newAutomat);
        }
        if(command == ">un")
        {
            NFAObject newAutomat;
            this->unCom(newAutomat);
            readAutomats.push_back(newAutomat);
        }
        if(command == ">recognize")
        {
            this->recognizeCom();
        }
        if(command == ">reg")
        {
            NFAObject newAutomat;
            this->regCom(newAutomat);
            readAutomats.push_back(newAutomat);
        }
        if(command == ">remove epsilons")
        {
            NFAObject newAutomat;
            this->removeEpsilonTransitionsCom(newAutomat);
            readAutomats.push_back(newAutomat);
        }
        if(command == ">finite")
        {
            this->finiteCom();
        }

    }while(command != ">close");
    clog << "Successfully closed file" << endl;
}

/**
 * Checks if an automat with this ID exists.
 * */
bool UserCommands::validID(int id)
{
    int i = 0;
    while(i < this->readAutomats.size() && this->readAutomats[i].getID() != id)
    {
        i++;
    }
    if(i == this->readAutomats.size())
        return 0;
    else
        return 1;
}

