
#ifndef _SOCKETMANAGER_H
#define	_SOCKETMANAGER_H
      
# define MAX_SOCKETS            4096
# define UNINITIALIZED_PORT     -1
# define CLOSED_PORT            -2
# define UNSPECIFIED_PORT       -999
     
/**
 * SocketStatus
 */
enum SocketStatus
{
    OPEN            =   0,
    UNINTIALIZED    =   -1,
    CLOSED          =   -2
};


/**
 * @class SocketManager
 * Manages all sockets in the server/DSIF host 
 */
class SocketManager
{
public:
    virtual ~SocketManager();
    
    static SocketManager* instance();
    
    virtual void recordSocket( int which, int port = UNINITIALIZED_PORT );
    
    virtual int closeSocket( int socket, int port = UNINITIALIZED_PORT );
    virtual void closeAllSockets();
    
    virtual void socketError( const char *socketRoutine, const char *caller, int port = UNINITIALIZED_PORT );
    
protected: 
    SocketManager();
    SocketManager( SocketManager&amp; );
    
    int     openSockets [MAX_SOCKETS];
    bool    openSocketsInitialized;
};

#endif	/* _SOCKETMANAGER_H */
