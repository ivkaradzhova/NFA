
#ifndef NFA_STATES_H
#define NFA_STATES_H

/**
 * Class contains the information about a state*/
class State
{
private:

    static unsigned int idCounter;
    unsigned int id;
    int automatIndex;
    char* name;

    bool final;
    bool starting;
    void copy(char const* name, bool final, bool starting);
public:
    State();
    State(char const* name, bool final, bool starting);
    State(State const& other);
    State& operator= (State const& other);
    ~State();

    void setAutomatIndex(int n); //used in NFA addState
    int getAutomatIndex() const;

    int getID() const ;
    char* getName() const;
    bool isFinal() const;
    bool isStarting() const;
    void setName(char const* name);
    void setStarting(bool starting);
    void setFinal(bool final);
    void print();


};

#endif //NFA_STATES_H
