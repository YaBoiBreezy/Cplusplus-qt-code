#include "Electrode.h"

//set the default value of emitting to false
Electrode::Electrode(){
    emitting = 9;
}

//return the value of emitting
int Electrode::getEmitting(){
    return emitting;
}

//set the value of emitting between 0 and 9
bool Electrode::setEmitting(int value){
    if (value >= 0 && value < 10){
        emitting = value;
        return true;
    }
    return false;
}

