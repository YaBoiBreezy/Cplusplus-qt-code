#ifndef ELECTRODE_H
#define ELECTRODE_H

//Electrode class, stores whether an electrode is connected to user.
class Electrode {
    public:
        //constructor
        Electrode();
    
        //getter and setter for emitting
        int getEmitting();
        bool setEmitting(int);

    private:
        //represents the strength of the electrode
        int emitting;
};

#endif // ELECTRODE_H
