
#include "UdpSocket.h"
#include <string.h>;

/**
 * Constructor
 */
UdpSocket::UdpSocket()
{
}

/**
 * Destructor
 */
UdpSocket::~UdpSocket()
{
    close();
}

/**
 * Sends bytes
 * @return number of bytes written, -1: timeout, or -2: SOCKET_ERROR
 */
int UdpSocket::sendTo( const void* buffer, int len, sockaddr* toAddr, int timeoutS, int timeoutUs )
{
    if ( len == 0 )
        return 0;
    else if ( (protocol == TCP) &amp;&amp; !isConn )
        return -1;
	else if ( !toAddr )
		return -1;
    
    unsigned int sentBytes = 0;
    
    // timeout setup if neccessary
	if ( ( timeoutS &gt; 0 ) || (timeoutUs &gt; 0 ) )
	{
        // add socket descriptor to the list
        fd_set      fdList;
        FD_ZERO( &amp;fdList );
        FD_SET( (unsigned int)socketFD, &amp;fdList );
		
        struct timeval  timeOutTimeVal;
        timeOutTimeVal.tv_sec   = timeoutS;
        timeOutTimeVal.tv_usec  = timeoutUs;
		
        // try to select the next socket that is ready to send
        int ret;
        if ( ( ret = select( socketFD + 1, 0, &amp;fdList, 0, &amp;timeOutTimeVal ) ) &lt;= 0 )
            return ( ret - 1 );   // -1: timeout occured or -2: SOCKET_ERROR
	}
	
	// ok, so send the msg
	sentBytes   =   sendto( socketFD, (char*)buffer, len, 0, toAddr, sizeof(sockaddr) );
    return sentBytes;
}

/**
 * Receives bytes
 * @return number of bytes written, -1: timeout, or -2: SOCKET_ERROR
 * @note blocking function if timeout is not set
 */
int UdpSocket::recvFrom( void* buffer, int len, sockaddr* fromAddr, int timeoutS, int timeoutUs )
{
    if ( len == 0 )
        return 0;
    else if ( (protocol == TCP) &amp;&amp; !isConn )
        return -1;
    
	// timeout setup if neccessary
	if ( ( timeoutS &gt; 0 ) || (timeoutUs &gt; 0 ) )
	{
        // add socket descriptor to the list
        fd_set      fdList;
        FD_ZERO( &amp;fdList );
        FD_SET( (unsigned int)socketFD, &amp;fdList );
		
        struct timeval  timeOutTimeVal;
        timeOutTimeVal.tv_sec   = timeoutS;
        timeOutTimeVal.tv_usec  = timeoutUs;
		
        // try to select the next socket that is ready to send
        int     ret;
        if ( ( ret = select( socketFD + 1, &amp;fdList, 0, 0, &amp;timeOutTimeVal ) ) &lt;= 0 )
            return ( ret - 1 );   // -1: timeout occured or -2: SOCKET_ERROR
	}
	
    unsigned int recvBytes = 0;
    
#ifdef WIN32
    int         addrSize  =   sizeof(sockaddr);
#else
	socklen_t   addrSize  =   (socklen_t) sizeof(sockaddr);
#endif

    if ( fromAddr )
        recvBytes   =   ::recvfrom( socketFD, (char*)buffer, len, 0, fromAddr, &amp;addrSize );
    else
    {
        sockaddr    tmpAddr;
        recvBytes   =   ::recvfrom( socketFD, (char*)buffer, len, 0, &amp;tmpAddr, &amp;addrSize );
    }
    return recvBytes;
}

// ---------------------------------------- UDP SERVER SOCKET ------------------------------------------
/**
 * Constructs UDP-unicast server socket
 */
UdpServerSocket::UdpServerSocket( unsigned int port )
{
    memset( &amp;serverAddr, 0, sizeof(serverAddr) );
    memset( &amp;clientAddr, 0, sizeof(clientAddr) );

    this-&gt;protocol              = UDP_UNICAST;
    this-&gt;port                  = port;
    serverAddr.sin_family       = AF_INET;
	serverAddr.sin_port         = htons( port );
    serverAddr.sin_addr.s_addr  = htonl( INADDR_ANY );

    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server socket!";

    // set socket common options
    setCommonOptions();    
    
    // bind to the port
    if ( !bind( serverAddr ) )
        throw "Failed to bind server socket to the port!";
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
}

/** 
 * Sends to client
 * @return number of bytes written or -1=ERROR
 */
int UdpServerSocket::send( const void* buffer, int len, int timeoutS, int timeoutUs  )
{
    return sendTo( buffer, len, (sockaddr*)&amp;clientAddr, timeoutS, timeoutUs );
}

/** 
 * Receives from client
 * @return number of bytes read or -1=ERROR
 * @note blocking function
 */
int UdpServerSocket::recv( void* buffer, int len, int timeoutS, int timeoutUs )
{
    return recvFrom( buffer, len, (sockaddr*)&amp;clientAddr, timeoutS, timeoutUs ); // record client address
}

/** 
 * Gets the last client's socket address
 */
sockaddr_in UdpServerSocket::getClientAddress() const
{
    return clientAddr;
}

/**
 * Constructs UDP-Unicast client socket
 */
UdpClientSocket::UdpClientSocket( const char* host, unsigned int port )
{
    if ( !gethostbyname( host ) )
        throw "Invalid host name!";

    memset( &amp;serverAddr, 0, sizeof(serverAddr) );
    memset( &amp;clientAddr, 0, sizeof(clientAddr) );

    strncpy( this-&gt;ip, host, 255 );
    this-&gt;protocol             = UDP_UNICAST;
    this-&gt;port                 = port;
    serverAddr.sin_family      = AF_INET;
	serverAddr.sin_port        = htons( port );
	serverAddr.sin_addr        = *( (in_addr*) (gethostbyname( host )-&gt;h_addr) );
    
    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server socket!";

    // set socket options
    setCommonOptions();
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
 
}

/**
 * Constructs UDP-Unicast client socket
 */
UdpClientSocket::UdpClientSocket( struct sockaddr_in addr )
{
    this-&gt;serverAddr    =   addr;
	this-&gt;protocol      =   UDP_UNICAST;
    
    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create client socket!";

    // set socket options
    setCommonOptions();
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
 
};

/** 
 * Sends to server
 * @return number of bytes written or -1=ERROR
 */
int UdpClientSocket::send( const void* buffer, int len, int timeoutS, int timeoutUs  )
{
    return sendTo( buffer, len, (sockaddr*)&amp;serverAddr, timeoutS, timeoutUs );
}

/** 
 * Receives from server
 * @return number of bytes read or -1=ERROR
 * @note blocking function
 */
int UdpClientSocket::recv( void* buffer, int len, int timeoutS, int timeoutUs )
{
    return recvFrom( buffer, len, (sockaddr*)&amp;serverAddr, timeoutS, timeoutUs ); // may override server address??
}



// ---------------------------------------- BROADCAST SOCKET ------------------------------------------
/** 
 * Constructs sender broadcasting socket
 */
BroadcastSender::BroadcastSender( unsigned int port )
{
    this-&gt;protocol              = UDP_BROADCAST;
    this-&gt;port                  = port;
    
    memset( &amp;bcastAddr, 0, sizeof( bcastAddr ) );
    bcastAddr.sin_family       = AF_INET;
	bcastAddr.sin_port         = htons( port );
    bcastAddr.sin_addr.s_addr  = htonl( INADDR_BROADCAST );
                                
    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server socket!";

    // set socket common options
    setCommonOptions();    
    
    // socket broadcast option
    int option;
    setsockopt( socketFD, SOL_SOCKET, SO_BROADCAST, (char*)&amp;(option = 1), sizeof(option) );

    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
    
}

/**
 * Destructor
 */
BroadcastSender::~BroadcastSender()
{
    close();
}
   

/** 
 * Broadcasts to all hosts in the subnet
 * @return number of bytes written or -1=ERROR
 */
int BroadcastSender::send( const void* buffer, int len  )
{
    return UdpSocket::sendTo( buffer, len, (sockaddr*)&amp;bcastAddr );
}


/** 
 * Constructs receiver broadcasting socket
 */
BroadcastReceiver::BroadcastReceiver( unsigned int port )
{
    this-&gt;protocol              = UDP_BROADCAST;
    this-&gt;port                  = port;
    
    memset( &amp;bcastAddr, 0, sizeof( bcastAddr ) );
    bcastAddr.sin_family       = AF_INET;
	bcastAddr.sin_port         = htons( port );
    bcastAddr.sin_addr.s_addr  = htonl( INADDR_ANY );    // from any host
    
    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server socket!";

    // set socket common options
    setCommonOptions();    
    
    // bind to the host port
    if ( !bind( bcastAddr ) )
        throw "Failed to bind the broadcast receiver socket to the port!";
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
}

/** 
 * Destructor
 */
BroadcastReceiver::~BroadcastReceiver()
{
}

// ------------------------------------------ MULTICAST SOCKET --------------------------------------------
/**
 * Constructs multicast server/sender socket
 * @param multicastIP the host IP that must be in range of [224.0.0.0, 239.255.255.255.255]
 */
MulticastSender::MulticastSender( const char* multicastIP, unsigned int port )
{
    if ( !gethostbyname( multicastIP ) )
        throw "Invalid multicast IP!";
    
    this-&gt;protocol              = UDP_MULTICAST;
    strncpy( ip, multicastIP, 255 );
    this-&gt;port                  = port;
    
    memset( &amp;mcastSockAddr, 0, sizeof( mcastSockAddr ) );
    mcastSockAddr.sin_family       = AF_INET;
	mcastSockAddr.sin_port         = htons( port );
    mcastSockAddr.sin_addr         = *( (in_addr*) (gethostbyname(ip)-&gt;h_addr) );
    
    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server multicast socket!";

    // set socket common options
    setCommonOptions();    
    
    // set multicast ttl = 1s
    int     option;
    setsockopt( socketFD, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&amp;(option = 1), sizeof(option) );
                            
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
    
}

/**
 * Destructor
 */
MulticastSender::~MulticastSender()
{
    close();
}

/** 
 * Sends to all receivers in the multicast group
 * @return number of bytes written or -1=ERROR
 */
int MulticastSender::send( const void* buffer, int len  )
{
    return UdpSocket::sendTo( buffer, len, (sockaddr*)&amp;mcastSockAddr );
}


/**
 * Constructs multicast client/receiver socket
 * @param multicastIP the host IP that must be in range of [224.0.0.0, 239.255.255.255.255]
 */
MulticastReceiver::MulticastReceiver( const char* multicastIP, unsigned int port )
{
    if ( !gethostbyname( multicastIP ) )
        throw "Invalid multicast IP!";
    
    this-&gt;protocol              = UDP_MULTICAST;
    strncpy( ip, multicastIP, 255 );
    this-&gt;port                  = port;
    
    memset( &amp;mcastSockAddr, 0, sizeof( mcastSockAddr ) );
    mcastSockAddr.sin_family       = AF_INET;
	mcastSockAddr.sin_port         = htons( port );
    mcastSockAddr.sin_addr.s_addr  = htonl( INADDR_ANY );
    
    // create socket
    socketFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketFD &lt; 0 )
        throw "Failed to create server multicast socket!";

    // set socket common options
    setCommonOptions();    
    
    // bind to the multicast port
    if ( !bind( mcastSockAddr ) )
        throw "Failed to bind to the multicast port!";
    
    // set multicast group address
    memset( &amp;mcastGrpAddr, 0, sizeof( mcastGrpAddr ) );
    mcastGrpAddr.imr_multiaddr.s_addr   =   *( (unsigned long*) (gethostbyname(ip)-&gt;h_addr) );    //*( (in_addr*) (gethostbyname(ip)-&gt;h_addr) );
    mcastGrpAddr.imr_interface.s_addr   =   htonl( INADDR_ANY );

    // join the multicast group
	// NOTE: if use WINSOCK 2008, IP_ADD_SOURCE_MEMBERSHIP
    if ( setsockopt( socketFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&amp;mcastGrpAddr, sizeof(mcastGrpAddr) ) &lt; 0 )
        throw "Failed to join the multicast group!";
    
    // record the socket
//    SocketManager::instance()-&gt;recordSocket( socketFD, port );
    
}

/**
 * Destructor
 */
MulticastReceiver::~MulticastReceiver()
{
    // disconnect from the multicast group
	// NOTE: if use WINSOCK 2008, IP_DROP_SOURCE_MEMBERSHIP
    setsockopt( socketFD, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&amp;mcastGrpAddr, sizeof(mcastGrpAddr) );

    // close the socket
    close();
}

/** 
 * Receives from multicast sender
 * @return number of bytes written or -1=ERROR
 */
int MulticastReceiver::recv( void* buffer, int len, int timeoutS, int timeoutUs  )
{
    return UdpSocket::recvFrom( buffer, len, (sockaddr*)&amp;mcastSockAddr, timeoutS, timeoutUs );
}


</pre></div>

<hr />
<table>
<tr>
<td><address><a href="mailto:cvs-admin@insert.your.domain.here">No admin address has been configured</a></address></td>
<td style="text-align: right;"><strong><a href="/viewvc-static/help_rootview.html">ViewVC Help</a></strong></td>
</tr>
<tr>
<td>Powered by <a href="http://viewvc.tigris.org/">ViewVC 1.0.3</a></td>
<td style="text-align: right;">&nbsp;</td>
</tr>
</table>
</body>
</html>

