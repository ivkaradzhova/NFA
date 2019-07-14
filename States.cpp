
#include "States.h"
#include<iostream>
#include<cstring>
using namespace std;

unsigned int State::idCounter = 0;

State::State() {

    idCounter++;
    this->id = idCounter;
    this->automatIndex = -1;
    this->copy("No name", 0, 0);
}

State::State(char const* name, bool final, bool starting)
{
    idCounter++;
    this->id = idCounter;
    this->automatIndex = -1;

    this->copy(name, final, starting);
}

State::State(State const& other)
{
    this->id = other.id;
    this->automatIndex = other.automatIndex;

    this->copy(other.name, other.final, other.starting);
}


State& State::operator=(State const& other)
{
    if(this != &other)
    {
        delete[] this->name;
        this->id = other.id;
        this->automatIndex = other.automatIndex;
        this->copy(other.name, other.final, other.starting);
    }
    return *this;
}

State::~State()
{
    delete[] name;
}

void State::copy(char const* name, bool final, bool starting)
{
    int length = strlen(name);

    this->name = new char[length + 1];
    strcpy(this->name, name);

    this->final = final;
    this->starting = starting;
}

/**
 * Returns the ID of a state
 * */
int State::getID() const
{
    return this->id;
}

/**
 * Function used in addTransition(). Sets the automatIndex variable
 * to the index the state has in the array of states (in it's automat)
 * */
void State::setAutomatIndex(int n)
{
    this->automatIndex = n;
}

/**
 * returns the index which the state has been given
 * when added to the automat
 * */
int State::getAutomatIndex() const
{
    return this->automatIndex;
}

char* State::getName() const
{
    return this->name;
}


bool State::isFinal() const
{
    return this->final;
}

bool State::isStarting() const
{
    return this->starting;
}
void State::setName(char const* name)
{
    strcpy(this->name, name);
}

/**
 * Changes the state's condition: sets the starting variable
 * */
void State::setStarting(bool starting)
{
    this->starting = starting;
}

/**
 * Changes the state's condition: sets the final variable
 * */
void State::setFinal(bool final)
{
    this->final = final;
}

void State::print()
{
    cout << "ID: " << this->id << "| index in the automat " <<
            this->getAutomatIndex() <<"|name: " << this->getName();
    if(this->isStarting())
        cout << "| STARTING ";
    if(this->isFinal())
        cout << "| FINAL" ;

    cout << endl;
}
