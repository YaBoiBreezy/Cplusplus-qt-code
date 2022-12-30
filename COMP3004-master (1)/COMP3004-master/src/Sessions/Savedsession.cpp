#include "Savedsession.h"

//set the default values of SavedSession to the given values
SavedSession::SavedSession(QString u, int g, int s, int i)
{
    user      = u;
    group     = g;
    session   = s;
    intensity = i;
}

//return the session's username
QString SavedSession::getUsername(){
    return user;
}

//return the session's group
int SavedSession::getGroup(){
    return group;
}

//return the session's session
int SavedSession::getSession(){
    return session;
}

//return the session's intensity
int SavedSession::getIntensity(){
    return intensity;
}
