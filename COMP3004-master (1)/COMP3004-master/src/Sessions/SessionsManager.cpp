#include "SessionsManager.h"

SessionsManager::SessionsManager(){
    savedSessions = QList<SavedSession*>();
}

//creates a new SavedSession and adds it to the QList
void SessionsManager::saveSession(QString username, int group, int session, int intensity){
    SavedSession* newSession = new SavedSession(username, group, session, intensity);
    savedSessions.push_back(newSession);
}
//returns a list of sessions when given a username
QList<SavedSession*> SessionsManager::getSavedSessions(QString username){
    QList<SavedSession*> output;
    //check if each session's username matches the given username
    for (SavedSession* session : savedSessions){
        if (username == session->getUsername()){//if so add it to the output list
            output.push_back(session);
        }
    }
    return output;
}
