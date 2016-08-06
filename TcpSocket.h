#ifndef _TCP_SOCKET_H
#define	_TCP_SOCKET_H

#include "Socket.h"

/**
 * @class TcpSocket
 * Defines top-level non-instantiable TCP socket class
 */
class TcpSocket : public Socket
{
public:
    /** 
     * Destructor
     */
	virtual ~TcpSocket() = 0;
	
    /** 
     * Checks if connected
     */
	virtual bool isConnected() const;
	
	/** 
     * Sends bytes
     * @param toAddr address of the receiver, only needed in UDP
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len  ) = 0;
    
    /** 
     * Receives bytes
     * @param fromAddr address of the sender, only needed in UDP
     * @return number of bytes read or -1=ERROR
     * @note blocking function
     */
    virtual int recv( void* buffer, int len ) = 0;

protected:
    // prevents public instantiation
    TcpSocket();

};

/**
 * @class TcpServerSocket
 * TcpServerSocket is the TCP or UDP host socket who owns the port.
 * TcpServerSocket is the sender in UDP broadcasting.
 */
class TcpServerSocket : public TcpSocket
{
public:
    /** 
     * Constructs server socket
     */
    TcpServerSocket( unsigned int port );
    
    /** 
     * Destructor
     */
    virtual ~TcpServerSocket();
    
    /** 
     * Listens for connection
     * @return SUCCESS=true, or NOCONNECT=false
	 * @note Only apply to TCP
     * @note blocking function
     * @note Must check isConnected() for connection status:
     *      while ( !isConnected() )
     *          listenForConnection();
     */
	virtual bool listenForConnection();	
    
    /** 
     * Sends bytes to client
     * @param toAddr address of the receiver, only needed in UDP
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len  );
    
    /** 
     * Receives bytes from client
     * @param fromAddr address of the sender, only needed in UDP
     * @return number of bytes read or -1=ERROR
     * @note blocking function
     */
    virtual int recv( void* buffer, int len );

protected:
    // Prevents default constructor
    TcpServerSocket();
    
    // --------------- data ------------------
    sockaddr_in        serverAddr;
    int                clientSockFD;
    
};


/**
 * @class TcpClientSocket
 * TcpClientSocket is the TCP client or the UDP client.
 * TcpClientSocket is the receiver in UDP broadcasting.
 */
class TcpClientSocket : public TcpSocket
{
public:
    /** 
     * Constructs client socket 
     * @hostIP IP address
     */
    TcpClientSocket( const char* hostIP, unsigned int port );
    
    /** 
     * Constructs client socket
     */
    TcpClientSocket( struct sockaddr_in addr );
    
    /** 
     * Destructor
     */
    virtual ~TcpClientSocket();
    
	/** 
     * Connects to an address
     * @return SUCCESS=true, or NOCONNECT=false
	 * @note Only apply to TCP
     * @note Must check isConnected() for link status
     */
	virtual bool connect();
	
    /** 
     * Sends bytes to server
     * @param toAddr address of the receiver, only needed in UDP
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len  );
    
    /** 
     * Receives bytes from server
     * @param fromAddr address of the sender, only needed in UDP
     * @return number of bytes read or -1=ERROR
     * @note blocking function
     */
    virtual int recv( void* buffer, int len );

protected:
    // Prevents default constructor
    TcpClientSocket();
    
    // --------------- data ------------------
    sockaddr_in        serverAddr;
};

#endif
