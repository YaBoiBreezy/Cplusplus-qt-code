#include "Battery.h"

//used int not float because additional precision was not required
Battery::Battery(){
    charge = 100;
}

int Battery::getCharge(){
    return charge;
}

//charge is in the range [0, 100]
bool Battery::setCharge(int newCharge){
    if (newCharge >= 0 && newCharge <= 100){
        charge = newCharge;
        return true;
    }
    return false;
}
