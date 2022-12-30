#ifndef SESSIONSMANAGER_H
#define SESSIONSMANAGER_H

#include "Savedsession.h"
#include <QList>
#include <QString>

//Stores saved sessions, can search for sessions by username
class SessionsManager
{
    private:
        //this list saves every saved session
        QList<SavedSession*> savedSessions;
    public:
        //constructor
        SessionsManager();
        //saves a new session
        void saveSession(QString username, int group, int session, int intensity);
        //returns every session that matches a given username
        QList<SavedSession*> getSavedSessions(QString username);
};

#endif // SESSIONSMANAGER_H
