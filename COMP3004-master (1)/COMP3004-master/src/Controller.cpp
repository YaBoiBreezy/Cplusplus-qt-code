#include "Controller.h"
#include "UI/mainwindow.h"
#include <stdlib.h>
#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>
#include <cstdlib>

Controller::Controller(){
    sessionsManager = new SessionsManager();
    currSession = new Session();
    timer.start();
    state = off;
    stateTimer = 0;
    batteryDisplay = new bool[8];
    setBatteryDisplay(0);
}

Controller::~Controller(){
    delete[] batteryDisplay;
    delete sessionsManager;
    delete currSession;
}

//gives the controller a pointer to the main window
void Controller::setWindow(QMainWindow* w){
    mainWindow = w;
}

//this function will fire every 0.2 seconds
void Controller::tick(){

    stateTimer-=1;
    if (currSession->getBattery()->getCharge()<10){ //if really low battery, end session and blink. Low battery handled in running
        state=idle;
        if (stateTimer%4<2){
            setBatteryDisplay(0);
        } else {
            setBatteryDisplay(1);
        }
    }
    if (stateTimer>0){
        switch (state){ //handle state still ongoing
            case off:
                currSession->setOutputBar(0); //off, so clear outputs
                setBatteryDisplay(0);
                break;
            case startup:
                setBatteryDisplay(); //starting up, so display battery charge
                break;
            case idle:
                currSession->setOutputBar(0); //idle, so clear outputs
                setBatteryDisplay(0);
                break;
            case startSession: //flashes session for 5 seconds, handled in getSession
                break;
            case connectR: //flashing icon implemented in getRightEarIcon
                if (stateTimer>4){ //flashes middle battery icon for a bit
                    setBatteryDisplay(28);
                } else if (stateTimer>2){ //turn off half time to flash
                    setBatteryDisplay(0);
                } else { //turn on to display current right electrode connection strength
                    if (currSession->getRElectrode()->getEmitting()>7){ //strongly connected
                        setBatteryDisplay(224);
                        break; //skips stateTimer reset to allow stateTimer to reach 0, trigger next state
                    } else if (currSession->getRElectrode()->getEmitting()>=5) { //medium connection
                        setBatteryDisplay(28);
                    } else {
                        setBatteryDisplay(3); //badly connected
                    }
                    if (stateTimer==1){ //continue flashing indefinitely
                        stateTimer=4;
                    }
                }
                break;
                case connectL: //flashing icon implemented in getLeftEarIcon
                if (stateTimer>4){ //flashes middle battery icon for a bit
                    setBatteryDisplay(28);
                } else if (stateTimer>2){ //turn off half time to flash
                    setBatteryDisplay(0);
                } else { //turn on to display current right electrode connection strength
                    if (currSession->getLElectrode()->getEmitting()>7){ //strongly connected
                        setBatteryDisplay(224);
                        break; //skips stateTimer reset to allow stateTimer to reach 0, trigger next state
                    } else if (currSession->getLElectrode()->getEmitting()>=5) { //medium connected
                        setBatteryDisplay(28);
                    } else {
                        setBatteryDisplay(3); //badly connected
                    }
                    if (stateTimer==1){ //continue flashing indefinitely
                        stateTimer=4;
                    }
                }
                break;
            case softon:
                setBatteryDisplay(0);
                batteryDisplay[int((24-stateTimer)/3)]=true; //slowly increase battery display to show startup of saved session
                break;
            case running:
                if (currSession->getRElectrode()->getEmitting()<5){ //if electrode is <5, insufficient connection, return to conenction test
                    state=connectR;
                    stateTimer=6;
                }
                if (currSession->getLElectrode()->getEmitting()<5){ //if electrode is <5, insufficient connection, return to conenction test
                    state=connectL;
                    stateTimer=6;
                }
                //Battery depletion rate based on intensity, connection to skin. Should deplete in ~30 hours of normal use, 1%/20min
                //Battery drain increased by 10^2 times FOR DEMO
                currSession->getBattery()->setCharge(currSession->getBattery()->getCharge()
                    - (((float)rand() / (float)RAND_MAX) < (0.02 * (currSession->getIntensity()*(currSession->getRElectrode()->getEmitting()+currSession->getLElectrode()->getEmitting())/100))));
                currSession->setOutputBar((currSession->getIntensity())+5-(stateTimer%5)); //outputBar increases size when intensity increases
                if (stateTimer%60<5){ //every 12 seconds display battery
                    setBatteryDisplay();
                } else {
                    setBatteryDisplay(0);
                }
                if (currSession->getBattery()->getCharge()<25){ //if low battery, blink alert
                    if (stateTimer%4<2){
                        setBatteryDisplay(0);
                    } else {
                        setBatteryDisplay(3);
                    }
                }
                break;
            case softoff:
                setBatteryDisplay(0);
                batteryDisplay[int((stateTimer-1)/3)]=true; //slowly reduce intensity and battery display
                break;
        }
    } else {
        switch (state){ //handle end of state, where stateTimer=0 so must switch to new state
            case off:
                break;
            case startup:
                state=idle; //2 second startup done, go to idle
                setBatteryDisplay(0);
                stateTimer=600; //2 minute delay before automatic idle shutoff
                break;
            case idle:
                state=off; //2 min elapsed with no interaction, so power off
                break;
            case startSession:
                state=connectR; //startSession done, go to connection test
                stateTimer=6;
                break;
            case connectR: //proceed in connection test
                state=connectL;
                break;
            case connectL:
                if (currSession->getIsSavedSession()){ //if saved session, go to softon. Else just start session
                    state=softon;
                    stateTimer=24;
                } else {
                    state=running;
                    stateTimer=currSession->getSession() * 5; //stateTimer controls session run length
                }
                break;
            case softon:
                state=running;
                stateTimer=currSession->getSession() * 5; //stateTimer controls session run length
                break;
            case running:
                state=softoff; //softoff to end session
                stateTimer=24;
                currSession->setOutputBar(0);
                break;
            case softoff:
                state=off; //softoff done, turn off device
                break;
        }
    }
}

void Controller::setBatteryDisplay(unsigned char arg){
    for (int i=0; i < 8; i++)
        batteryDisplay[i] = (bool)((arg >> i) & 0x1);
}

//sets bools to true left-to-right until it runs out of charge. 0=allFalse, 100=allTrue.
void Controller::setBatteryDisplay(){
    int charge=currSession->getBattery()->getCharge();
    bool display[] = {charge>=100,charge>85,charge>71,charge>57,charge>42,charge>28,charge>14,charge>0};
    char ret = 0;
    int tmp;
    for (int i = 0; i < 8; i++) {
        tmp = display[i];
        ret |= tmp << (8 - i - 1);
    }
    setBatteryDisplay(ret);
}

void Controller::userInteracted(){ //reset inactivity timer
    if (state==idle){
        stateTimer=2*5*60;
    }
}

void Controller::powerPressed(){  //different results based on time between pressed and released!
    powerPressedWhen = timer.elapsed();
    userInteracted();
}

void Controller::powerReleased(){
    //get how long the button has been pressed
    int timeHeld = timer.elapsed() - powerPressedWhen;
    if (timeHeld < 1000){
        switch(state){
            case off:
                state=startup;
                stateTimer=10; //2 seconds for startup
                break;
            case running:
                state=softoff;
                stateTimer=24; //2.4 seconds for softoff
                break;
            case idle:
                currSession->setGroup((currSession->getGroup()+1)%4); //cycle group
                break;
            default:
                state=off;
                break;
        }
    }
    else {
        switch(state){
            case off:
                state=startup;
                stateTimer=10; //2 seconds for startup
                break;
            case running:
                state=softoff;
                stateTimer=24; //2.4 seconds for softoff
                break;
            default:
                state=off;
                break;
        }
    }
    userInteracted();
}

void Controller::setSessionGroup(int group){
    currSession->setGroup(group);
    state=idle;
    userInteracted();
}

void Controller::setSession(int session){
    currSession->setSession(session);
    state=idle;
    userInteracted();
}

void Controller::setIntensity(int intensity){ //WARNING: IF NO SESSION RUNNING, THIS SHOULD ALTER SESSION GROUP
    if (state==running){ //session is running, so set intensity
        currSession->setIntensity(intensity);
    } else {
        switch (currSession->getSession()){
            case 1200:
                currSession->setSession(45*60); //hardcoded 20min and 45min, all else is custom
                break;
            case 45*60:
                currSession->setSession(60);
                break;
            default:
                currSession->setSession(1200);
        }
    }
    userInteracted();
}

void Controller::startSessionPressed(){  //different results based on time between pressed and released!
    startPressedWhen = timer.elapsed();
    userInteracted();
}

void Controller::startSessionReleased(QString username){
    if (state!=off){
        //get how long the button has been pressed
        int timeHeld = timer.elapsed() - startPressedWhen;
        if (timeHeld < 1000){ //held less than a second, start session
            state=startSession;
            stateTimer=25; //wait 5 seconds to start session
            currSession->setIsSavedSession(false); //no softon
        }
        else { //held more than a second, save current session
            sessionsManager->saveSession(username, currSession->getGroup(), currSession->getSession(), currSession->getIntensity());

        }
        userInteracted();
    }
}

void Controller::startSavedSession(SavedSession* s){ //if not off, get properties of given saved session, start
    if (state!=off){
        currSession->setGroup(s->getGroup());
        currSession->setSession(s->getSession());
        currSession->setIntensity(s->getIntensity());
        state=startSession;
        stateTimer=25; //wait 5 seconds to start session
        currSession->setIsSavedSession(true); //will trigger softon
    }
}

void Controller::setBattery(int strength){
    currSession->getBattery()->setCharge(strength);
}

void Controller::setLeftEarpiece(int strength){
    currSession->getLElectrode()->setEmitting(strength);
}

void Controller::setRightEarpiece(int strength){
    currSession->getRElectrode()->setEmitting(strength);
}

int Controller::getBattery(){
    return currSession->getBattery()->getCharge();
}

bool* Controller::getBatteryDisplay(){ //returns bool[8]
    return batteryDisplay;
}

//gets all saved sessions for given username
QList<SavedSession*> Controller::getSavedSessions(QString username){
    return sessionsManager->getSavedSessions(username);
}

int Controller::getOutputBar(){ //display to user, shows electric output in realtime
    currSession->getOutputBar();
}

bool Controller::getLeftEarIcon(){
    if (state==connectL && stateTimer<2){ //connection test, flash icon
        return true;
    }
    return currSession->getLElectrode()->getEmitting() > 5; //if strength>5 then display connected icon
}

bool Controller::getRightEarIcon(){
    if (state==connectR && stateTimer<2){ //connection test, flash icon
        return true;
    }
    return currSession->getRElectrode()->getEmitting() > 5; //if strength>5 then display connected icon
}

int Controller::getGroup(){
    return currSession->getGroup();
}

int Controller::getSession(){
    if (state==startSession && stateTimer%4<2){ //starting session, so flash session selection
        return -1;
    }
    return currSession->getSession();
}

int Controller::getIntensity(){ //for when intensity toggled before session, should change session and reset intensity to 4 so this updates GUI
    return currSession->getIntensity();
}

int Controller::getTime(){
    return timer.elapsed();
}
