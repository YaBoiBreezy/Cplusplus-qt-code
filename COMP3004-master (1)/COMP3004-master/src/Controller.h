/*
The controller is responsible for connecting the GUI to the main code base, and manages all of the classes that the program requires
The controller does most of the work and keeps the GUI up to date with everything happening in the background
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QElapsedTimer>
#include <QTimer>
#include <QString>
#include <QList>
#include <QMainWindow>
#include "Sessions/Session.h"
#include "Sessions/Savedsession.h"
#include "Sessions/SessionsManager.h"

class MainWindow;

class Controller{
    public:
        Controller();
        ~Controller();
        void userInteracted(); //resets the 2-minute automatic shutoff countdown
        void tick(); //very large function, refer to state diagram for explanation
        void setWindow(QMainWindow* window); //Gives the controller a pointer to the main window
        void powerPressed();  //different results based on time between pressed and released
        void powerReleased(); //handles when the power button is released
        void setSessionGroup(int group); //sets the session group
        void setSession(int session); //sets the session value
        void setIntensity(int intensity); //sets the value of intensity if a session is running
        void startSessionPressed();  //different results based on time between pressed and released
        void startSessionReleased(QString username); //if held too long, save current sessings as SavedSession with username
        void startSavedSession(SavedSession* s); //starts the selected saved session

        void setBattery(int strength); //sets the battery's power level
        void setLeftEarpiece(int strength); //sets the left electrode's strength
        void setRightEarpiece(int strength);//sets the right electrode's strength

        int getBattery(); //returns the battery's power level
        bool* getBatteryDisplay(); //deprecated
        QList<SavedSession*> getSavedSessions(QString user); //gets all saved sessions for a given user
        int getOutputBar(); //gets the int value of the progress bar
        bool getLeftEarIcon(); //get if the left electrode's checkbox is ticked
        bool getRightEarIcon();//get if the right electrode's box is ticked
        int getGroup(); //get the group value from the GUI
        int getSession(); //get the session value from the GUI
        int getIntensity(); //for when intensity toggled before session, should change session and reset intensity to 4 so this updates GUI
        int getTime();

    private:
        //pointer to the main window
        QMainWindow* mainWindow;
        //keeps a sessionmanager
        SessionsManager *sessionsManager;
        //the current session
        Session *currSession;
        //these remember when a button was pressed
        int powerPressedWhen; //for knowing how long a button was pressed
        int startPressedWhen;
        //this timer starts when the object is created and will keep track of the time
        QElapsedTimer timer;
        //this enum is used for each state the program can be in
        enum State {off, startup, idle, startSession, connectR, connectL, softon, running, softoff};
        int stateTimer; //used for states to know what to do and when to switch
        State state; //used to know what state the controller is in
        bool* batteryDisplay; //for gui output
        void setBatteryDisplay(); //sets the battery display based on the actual battery power
        void setBatteryDisplay(unsigned char); //sets the battery display based on custom requirements
};

#endif // CONTROLLER_H

