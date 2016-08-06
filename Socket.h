
#ifndef _SOCKET_H
#define	_SOCKET_H

#ifdef WIN32
    #include &lt;windows.h&gt; 
#else   // LINUX
    #include &lt;sys/types.h&gt;
    #include &lt;sys/socket.h&gt;
    #include &lt;netinet/in.h&gt;
    #include &lt;netinet/tcp.h&gt;
    #include &lt;netdb.h&gt;
    #include &lt;arpa/inet.h&gt;
    #include &lt;unistd.h&gt;
    #define SOCKET_ERROR (-1)
#endif
        
  
#define     MAX_BIND_TRIES          5
#define     MAX_QUEUE_CONNECTIONS   5
        
        
/**
 * @enum Protocol type
 */
enum Protocol
{
    TCP				= 0,    // SOCK_STREAM
    UDP_UNICAST		= 1,    // SOCK_DGRAM
    UDP_MULTICAST   = 2,
    UDP_BROADCAST	= 3
};


/**
 * @class Socket
 * Defines top-level non-instantiable socket class
 */
class Socket
{
public:
    /** 
     * Destructor
     */
	virtual ~Socket() = 0;
	
    /** 
     * Closes socket
     */
    virtual void close();

    /** 
     * Gets the current host IP
     */
	virtual char* getHostIP();
    
    /** 
     * Gets the current port
     */
	virtual unsigned int getPort();

    /** 
     * Gets socket option
     */
	virtual bool getSocketOption( int optName, int&amp; optVal );
    
    /** 
     * Sets socket option
     */
	virtual bool setSocketOption( int optName, int optVal );

    
protected:
    // ----------------------- prevent public instantiation ------------------------
    Socket();
	Socket( Socket &amp;s );
    
    // --------------------------- protected methods --------------------------------
    /** 
     * Binds the socket to the address
     */
    virtual bool bind( const sockaddr_in&amp; addr );

    /** 
     * Sets socket common options
     */
    virtual void setCommonOptions();

    
    // ------------------------------ protected data --------------------------------
    int                 socketFD;   // socket file descriptor
	Protocol            protocol;
    bool                isConn; 
    
	char                ip [256];	// the host/IP address
	unsigned int        port;		// the port number
	
    //sockaddr_in         socketAddr;	// the socket address

    //	int                 rcvBufferSize;
    //	int                 transBufferSize;
};

#include "TcpSocket.h"
#include "UdpSocket.h"

#endif	/* _SOCKET_H */

