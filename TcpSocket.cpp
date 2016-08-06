#include "TcpSocket.h"
#include &lt;string.h&gt;

/**
 * Constructor
 */
TcpSocket::TcpSocket()
{
}

/**
 * Destructor
 */
TcpSocket::~TcpSocket()
{
    close();
}


/**
 * Checks if connected
 */
bool TcpSocket::isConnected() const
{
    return isConn;
}


// ------------------------------------------ SERVER TCP SOCKET --------------------------------------------
/**
 * Constructs server socket
 */
TcpServerSocket::TcpServerSocket( unsigned int port )
{
    this-&gt;protocol              = TCP;
    this-&gt;port                  = port;
    
    memset( &amp;serverAddr, 0, sizeof(serverAddr) );
    serverAddr.sin_family       = AF_INET;
	serverAddr.sin_port         = htons( port );
    serverAddr.sin_addr.s_addr  = htonl( INADDR_ANY );

    // create socket
    socketFD =  socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server socket!";

    // set socket common options
    setCommonOptions();    
    
    // set extra TCP options
    int option;
    // keep connection alive
    setsockopt( socketFD, SOL_SOCKET, SO_KEEPALIVE, (char*)&amp;(option = 1), sizeof(option) );
    // no delay
    setsockopt( socketFD, IPPROTO_TCP, TCP_NODELAY, (char*)&amp;(option = 1), sizeof(option) );

    // bind to the port
    if ( !bind( serverAddr ) )
        throw "Failed to bind server socket to the port!";
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
    
}

/**
 * Destructor
 */
TcpServerSocket::~TcpServerSocket()
{
    close();
}

/**
 * Listens for connection
 * @return SUCCESS=true, or NOCONNECT=false
 * @note Only apply to TCP
 * @note Must check isConnected() for connection status
 */
bool TcpServerSocket::listenForConnection()
{
    if ( isConn || ( protocol != TCP ) )
        return true;
    
    // listen for new connection
    if ( listen( socketFD, MAX_QUEUE_CONNECTIONS ) )
        return false;
  
    // accept new connection from TCP client
    clientSockFD  =   -1; // reset
    sockaddr    clientAddr;
#ifdef WIN32
    int         clientAddrSize  =   sizeof(clientAddr);
#else
	socklen_t   clientAddrSize  =   (socklen_t) sizeof(clientAddr);
#endif
    if ( ( clientSockFD = accept( socketFD, &amp;clientAddr, &amp;clientAddrSize ) ) &lt; 0 )
        return false;
    
    isConn =   true;
    return true;
}

/**
 * Sends msg to client
 * @return number of bytes written or -1=ERROR
 */
int TcpServerSocket::send( const void* buffer, int len )
{
    if ( len == 0 )
        return 0;
    else if ( !isConn || ( clientSockFD &lt; 0 ) )
        return -1;
    
    return ::send( clientSockFD, (char*)buffer, len, 0 );
}

/**
 * Receives msg from client
 * @return number of bytes read or -1=ERROR
 * @note blocking function
 */
int TcpServerSocket::recv( void* buffer, int len )
{
    if ( len == 0 )
        return 0;
    else if ( !isConn || ( clientSockFD &lt; 0 ) )
        return -1;
    
    return  ::recv( clientSockFD, (char*)buffer, len, 0 );
}

// ------------------------------------------ CLIENT TCP/UDP SOCKET --------------------------------------------
/**
 * Constructs client socket
 */
TcpClientSocket::TcpClientSocket( const char* hostIP, unsigned int port )
{
    if ( !gethostbyname( hostIP ) )
        throw "Invalid host name!";
    
    strncpy( this-&gt;ip, hostIP, 255 );
    this-&gt;protocol             = TCP;
    this-&gt;port                 = port;
    
    memset( &amp;serverAddr, 0, sizeof(serverAddr) );
    serverAddr.sin_family      = AF_INET;
	serverAddr.sin_port        = htons( port );
	serverAddr.sin_addr        = *( (in_addr*) (gethostbyname(ip)-&gt;h_addr) );
    
    // create socket
    socketFD =  socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server socket!";

    // set socket common options
    setCommonOptions();
    
    // set extra TCP options
    int option;
    // keep connection alive
    setsockopt( socketFD, SOL_SOCKET, SO_KEEPALIVE, (char*)&amp;(option = 1), sizeof(option) );
    // no delay
    setsockopt( socketFD, IPPROTO_TCP, TCP_NODELAY, (char*)&amp;(option = 1), sizeof(option) );

    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
 
}

/**
 * Constructs client socket
 */
TcpClientSocket::TcpClientSocket( struct sockaddr_in addr )
{
    this-&gt;serverAddr    =   addr;
	this-&gt;protocol      =   TCP;
    
    // create socket
    socketFD =  socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( socketFD &lt; 0 )
        throw "Failed to create client socket!";

    // set socket common options
    setCommonOptions();
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
 
};

/**
 * Destructor
 */
TcpClientSocket::~TcpClientSocket()
{
    close();
}

/**
 * Connects to an address
 * @return SUCCESS=true, or NOCONNECT=false
 * @note Only apply to TCP
 * @note must check isConnected() for link status
 */
bool TcpClientSocket::connect()
{
    if ( isConn || ( protocol != TCP ) )
        return true;
    
    if ( ::connect( socketFD, (sockaddr*) &amp;serverAddr, sizeof( serverAddr ) ) )
        return false;
            
    isConn =   true;        
    return true;
}

/**
 * Sends msg to server
 * @return number of bytes written or -1=ERROR
 */
int TcpClientSocket::send( const void* buffer, int len )
{
    if ( len == 0 )
        return 0;
    else if ( !isConn || ( socketFD &lt; 0 ) )
        return -1;
    
    return ::send( socketFD, (char*)buffer, len, 0 );
}

/**
 * Receives msg from server
 * @return number of bytes read or -1=ERROR
 * @note blocking function
 */
int TcpClientSocket::recv( void* buffer, int len )
{
    if ( len == 0 )
        return 0;
    else if ( !isConn || ( socketFD &lt; 0 ) )
        return -1;
    
    return  ::recv( socketFD, (char*)buffer, len, 0 );
}
