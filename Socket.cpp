
#include "Socket.h"
#include "TyzxOSsupport.h"


/**
 * Constructor
 */
Socket::Socket()
{
    protocol        =   TCP;
    isConn          =   false;
    ip[0]         =   '\0';
    port            =   0;

#ifdef WIN32
    // only need to initilize once
    static  bool    firstTime   =   true;
    
    if ( firstTime )
    {
        WSADATA info;
        if ( WSAStartup( MAKEWORD(2,0), &amp;info) ) 
            throw "Failed to start WSA in Windows!";
        else
            firstTime   =   false;
    }
#endif
}

/**
 * Destructor
 */
Socket::~Socket()
{
}

/**
 * Closes socket
 */
void Socket::close()
{
# ifdef WIN32
	// to make sure the socket is closed
    #ifndef SD_BOTH
    #define SD_BOTH         0x02
	shutdown( socketFD, SD_BOTH );
    closesocket( socketFD );
    WSACleanup();
    #endif
# else
	// to make sure the socket is closed
	shutdown( socketFD, SHUT_RDWR );
	::close( socketFD );
# endif
    isConn  =  false;   
}

/** 
 * Sets socket options
 */
void Socket::setCommonOptions()
{
    int option = 0;
    
    // reuse the address, allow binding to the port already being used
    setsockopt( socketFD, SOL_SOCKET, SO_REUSEADDR, (char*)&amp;(option = 1), sizeof(option) );
    
    // linger on close
    struct linger ling;
    ling.l_onoff    = 1;	// ON=1, OFF=0
    ling.l_linger   = 1;	// seconds
    setsockopt( socketFD, SOL_SOCKET, SO_LINGER, (char*)&amp;ling, sizeof(struct linger) );
}
    
/**
 * Binds the socket to the address
 */
bool Socket::bind( const sockaddr_in&amp; addr )
{
    int status;

    // try to bind a few times
    for ( int i = 0; i &lt; MAX_BIND_TRIES; ++i )
    {
        if ( ( status = ::bind( socketFD, (sockaddr*)&amp;addr, sizeof(addr) ) ) != 0 )
            tyzxMilliSleep( 500 );   // 0.5s
        else
            break;
    }
    
    if ( status != 0 )
    {
        close();
        return false;
    }
    return true;
}

/**
 * Gets the current host IP
 */
char* Socket::getHostIP()
{
    return ip;
}

/**
 * Gets the current port
 */
unsigned int Socket::getPort()
{
    return port;
}

/** 
 * Gets socket option
 */
bool Socket::getSocketOption( int optName, int&amp; optVal )
{
# ifdef WIN32
	int optSize;
# else
    socklen_t optSize;
# endif
    return ( getsockopt( socketFD, SOL_SOCKET, optName, (char*)&amp;optVal, &amp;optSize ) &gt;= 0 );
}

/** 
 * Sets socket option
 */
bool Socket::setSocketOption( int optName, int optVal )
{
    return ( setsockopt( socketFD, SOL_SOCKET, optName, (char*)&amp;optVal, sizeof(optVal) ) &gt;= 0 );   
}
