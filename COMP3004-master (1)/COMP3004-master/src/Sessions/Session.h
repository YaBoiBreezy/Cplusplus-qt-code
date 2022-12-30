#ifndef SESSION_H
#define SESSION_H

#include "Components/Battery.h"
#include "Components/Electrode.h"
#include <qobject.h>


//stores current session information for controller
class Session {
    public:
        //constructor and destructor
        Session();
        ~Session();

        //getter and setter for group
        int getGroup();
        bool setGroup(int value);

        //getter and setter for session
        int getSession();
        bool setSession(int value);

        //getter and setter for intensity
        int getIntensity();
        bool setIntensity(int value);

        //getter and setter for the outputbar
        int getOutputBar();
        void setOutputBar(int bar);

        //getter and setter for isSavedSession
        bool getIsSavedSession();
        void setIsSavedSession(bool);

        //returns the battery object
        Battery* getBattery();
        //returns the right and left electrodes respectively
        Electrode* getRElectrode();
        Electrode* getLElectrode();

    private:
        //the right and left electrodes
        Electrode* rtrode, *ltrode;
        //the battery level
        Battery *batt;
        //Values needed by the session
        int group, session, intensity, outputBar;
        //stores if the current session is the saved session
        bool isSavedSession;
};

#endif // SESSION_H
