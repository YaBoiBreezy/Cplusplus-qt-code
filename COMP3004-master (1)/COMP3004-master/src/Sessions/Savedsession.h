#ifndef SAVEDSESSION_H
#define SAVEDSESSION_H
#include <QString>

//Saved session class, stores basic session attributes for a given user
class SavedSession {
    private:
        //the user's name
        QString user;
        //the saved group
        int group;
        //the saved session
        int session;
        //the saved intensity
        int intensity;
    public:
        //constructor
        SavedSession(QString,int,int,int);
        //returns the username
        QString getUsername();
        //returns the group value
        int getGroup();
        //returns the session value
        int getSession();
        //returns the intensity value
        int getIntensity();
};

#endif // SAVEDSESSION_H
