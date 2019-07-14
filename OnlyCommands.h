
#ifndef NFA_ONLYCOMMANDS_H
#define NFA_ONLYCOMMANDS_H
#include "RegularExpression.h"
#include "UserCommands.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

/*Header contains all of the functions which are executed after
 * an user's command.
 * 'Com' - specifies that it is only used for the commands*/

void UserCommands::unionCom(NFAObject& newAutomat)
{
    int id1, id2;
    cout << "Input ID of first: "; cin >> id1;
    if(!this->validID(id1))
    {
        cerr << "Invalid first ID " << endl;
        return;
    }
    cout << "Input ID of second: "; cin >> id2;
    if(!this->validID(id2))
    {
        cerr << "Invalid second ID " << endl;
        return;
    }
    Operation calculate;
    newAutomat.automat = calculate.automatUnion(this->findAutomatWithID(id1), this->findAutomatWithID(id2));
    clog << "Successful union of automat with ID " << id1 <<
         " and automat with ID " << id2 << endl;
}


void UserCommands::concatCom(NFAObject& newAutomat)
{
    int id1, id2;
    cout << "Input ID of first: "; cin >> id1;
    if(!this->validID(id1))
    {
        cerr << "Invalid first ID " << endl;
        return;
    }
    cout << "Input ID of second: "; cin >> id2;
    if(!this->validID(id2))
    {
        cerr << "Invalid second ID " << endl;
        return;
    }
    Operation calculate;
    newAutomat.automat = calculate.automatConcat(this->findAutomatWithID(id1), this->findAutomatWithID(id2));
    clog << "Successful concatenation of automat with ID " << id1 <<
         " and automat with ID " << id2 << endl;
}

void UserCommands::unCom(NFAObject& newAutomat)
{
    int id;
    cout << "ID: "; cin >> id;
    if(!this->validID(id))
    {
        cerr << "Invalid ID " << endl;
        return;
    }
    Operation calculate;
    newAutomat.automat = calculate.automatUn(this->findAutomatWithID(id));
    clog << "Successful 'plus' operation on automat with ID: " << id << endl;
}

void UserCommands::listCom()
{
    cout << "Automat IDs: " << endl;
    for(int i = 0; i < readAutomats.size(); i++)
    {
        cout << readAutomats[i].getID() << endl;
    }
}

void UserCommands::printCom()
{
    int id;
    cout << "ID: "; cin >> id;
    if(!this->validID(id))
    {
        cerr << "Invalid ID " << endl;
        return;
    }
    this->findAutomatWithID(id).print();
}

void UserCommands::saveCom(ofstream& file, string fileName)
{
    file.open(fileName, ios::app |ios::out);
    int id;
    cout << "Input automat's ID: "; cin >> id;
    if(!this->validID(id))
    {
        cerr << "Invalid ID " << endl;
        return;
    }
    this->write(id, file);
    if(file)
        clog << "Successfully saved automat with ID: " << id << endl;
    file.close();
}

void UserCommands::saveAsCom()
{
    int id;
    cout << "Input automat's ID: "; cin >> id;
    string filePath;
    cout << "Input file path: "; cin >> filePath;
    ofstream newFile(filePath, ios::app | ios::out);
    this->write(id, newFile);
    if(newFile)
        clog << "Successfully saved automat with ID: " << id << endl
             << "In file: " << filePath << endl;
}

void UserCommands::emptyCom()
{
    int id;
    cout << "Input ID: "; cin >> id;
    cout << "Language of automat with ID: " << id;
    if(this->findAutomatWithID(id).empty())
        cout <<" is empty" << endl;
    else
        cout << " is NOT empty" << endl;
}

void UserCommands::deterministicCom()
{
    int id;
    cout << "Input ID: "; cin >> id;
    cout << "Automat with ID: " << id;
    if(this->findAutomatWithID(id).deterministic())
        cout << " is deterministic" << endl;
    else
        cout << " is NOT deterministic" << endl;
}

void UserCommands::recognizeCom()
{
    int id;
    cout << "ID: "; cin >> id;
    if(!this->validID(id))
    {
        cerr << "Invalid ID " << endl;
        return;
    }
    char word[100];
    cout << "Input word: "; cin >> word;
    if(this->findAutomatWithID(id).recognize(word))
        cout << "The word is recognizable " << endl;
    else
        cout << "The word isn't recognizable" << endl;
}

void UserCommands::regCom(NFAObject& newAutomat)
{
    string regex;
    cout << "Input regular expression: "; cin >> regex;
    newAutomat.automat = reg(regex);
    clog << "Successfully created an automat from the regular expression" << endl;
}

void UserCommands::removeEpsilonTransitionsCom(NFAObject& newAutomat)
{
    int id;
    cout << "ID: "; cin >> id;
    if(!this->validID(id))
    {
        cerr << "Invalid ID " << endl;
        return;
    }
    newAutomat.automat = this->findAutomatWithID(id);
    newAutomat.automat.removeEpsilonTransitions();
    clog << "Successfully removed epsilon-transitions " << endl;
}

void UserCommands::finiteCom()
{
    int id;
    cout << "Input ID: "; cin >> id;
    cout << "Automat with ID: " << id;
    if(this->findAutomatWithID(id).isFinite())
        cout << " has a finite language" << endl;
    else
        cout << " doesn't have a finite language" << endl;
}

#endif //NFA_ONLYCOMMANDS_H
