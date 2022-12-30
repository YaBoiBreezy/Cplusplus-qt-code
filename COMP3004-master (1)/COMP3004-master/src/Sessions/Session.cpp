#include "Session.h"
#include "Components/Battery.h"
#include "Components/Electrode.h"
#include <QObject>

//create a new session and initialize the default values
Session::Session(){
    batt   = new Battery();
    rtrode = new Electrode();
    ltrode = new Electrode();
    group  = 0;
    session   = 1200;
    intensity = 2;
    outputBar = 0;
}

//delete the session and the contained electrodes and battery
Session::~Session(){
    delete batt;
    delete rtrode;
    delete ltrode;
}

//return the session group
int Session::getGroup(){
    return group;
}

//if the given value is between 0 and 3, set it to the group value
bool Session::setGroup(int value){
    if (value >= 0 && value < 4){
        group = value;
        return true;
    }
    return false;
}

//return the intensity
int Session::getIntensity(){
    return intensity;
}

//if the given value is between 0 and 7, set it to the intensity
bool Session::setIntensity(int value){
    if(value >= 0 && value < 8){
        intensity = value;
        return true;
    }
    return false;
}

//return the value of session
int Session::getSession(){
    return session;
}

//if the given value is greater that 0, set it to session
bool Session::setSession(int value){
    if (value > 0){
        session = value;
        return true;
    }
    return false;
}

//set the value of outputBar to the given value
void Session::setOutputBar(int bar){
    outputBar = bar;
}

//return the value of outputBar
int Session::getOutputBar(){
    return outputBar;
}

//for storing whether to do softon
void Session::setIsSavedSession(bool is){
    isSavedSession = is;
}

//for deciding whether to do softon
bool Session::getIsSavedSession(){
    return isSavedSession;
}

//return the battery object
Battery* Session::getBattery(){
    return batt;
}

//return the right electrode
Electrode* Session::getRElectrode(){
    return rtrode;
}

//return the left electrode
Electrode* Session::getLElectrode(){
    return ltrode;
}
