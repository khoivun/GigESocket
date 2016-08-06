#ifndef _UDP_SOCKET_H
#define	_UDP_SOCKET_H

#include "Socket.h"

// ---------------------------------------- UDP SOCKET ------------------------------------------
/**
 * @class UdpSocket
 * Defines top-level non-instantiable UDP socket class
 */
class UdpSocket : public Socket
{
public:
    /** 
     * Destructor
     */
	virtual ~UdpSocket() = 0;
	
    /** 
     * Sends a message to an address
     * @param toAddr address of the receiver
	 * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written, -1: timeout, or -2: SOCKET_ERROR
     */
    virtual int sendTo( const void* buffer, int len, sockaddr* toAddr, int timeoutS = 0, int timeoutUs = 0  );
    
    /** 
     * Receives a message from an address
     * @param fromAddr address of the sender
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written, -1: timeout, or -2: SOCKET_ERROR
     * @note blocking function if timeout is not set
     */
    virtual int recvFrom( void* buffer, int len, sockaddr* fromAddr, int timeoutS = 0, int timeoutUs = 0 );

    /** 
     * Sends to the client that was previously received from
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written, -1: timeout, or -2: SOCKET_ERROR
     */
    virtual int send( const void* buffer, int len, int timeoutS = 0, int timeoutUs = 0  ) { return -1; }
    
    /** 
     * Receives from a client
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written, -1: timeout, or -2: SOCKET_ERROR
     * @note blocking function if timeout is not set
     */
    virtual int recv( void* buffer, int len, int timeoutS = 0, int timeoutUs = 0 )  { return -1; }

protected:
    // prevent public instantiation
    UdpSocket();

};


// ---------------------------------------- UDP SERVER/CLIENT SOCKET ------------------------------------------
/**
 * @class UdpServerSocket
 * Defines UDP-Unicast server socket class
 */
class UdpServerSocket : public UdpSocket
{
public:
    /** 
     * Constructs UDP-Unicast server socket
     */
    UdpServerSocket( unsigned int port );
    
    /** 
     * Sends to the client that was previously received from
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len, int timeoutS = 0, int timeoutUs = 0  );
    
    /** 
     * Receives from a client
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes read or -1=ERROR
     * @note blocking function if timeout is not set
     */
    virtual int recv( void* buffer, int len, int timeoutS = 0, int timeoutUs = 0 );

    /** 
     * Gets the last client's socket address
     */
    virtual sockaddr_in getClientAddress() const; 

protected:
    // prevents public instantiation
    UdpServerSocket();

    // ----------------------- data --------------------
    sockaddr_in     serverAddr;	// server address
    sockaddr_in     clientAddr;	// client address
};

/**
 * @class UdpClientSocket
 * Defines UDP-Unicast client socket class
 */
class UdpClientSocket : public UdpSocket
{
public:
    /** 
     * Constructs UDP-Unicast client socket 
     * @hostIP IP address of the server
     */
    UdpClientSocket( const char* hostIP, unsigned int port );
    
    /** 
     * Constructs UDP-Unicast client socket
     */
    UdpClientSocket( struct sockaddr_in addr );    
    
    /** 
     * Sends to the server
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len, int timeoutS = 0, int timeoutUs = 0  );
    
    /** 
     * Receives from the server
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes read or -1=ERROR
     * @note blocking function if timeout is not set
     */
    virtual int recv( void* buffer, int len, int timeoutS = 0, int timeoutUs = 0 );

protected:
    // prevents public instantiation
    UdpClientSocket();

    // ----------------------- data --------------------
    sockaddr_in     serverAddr;	// server address
    sockaddr_in     clientAddr;	// client address
};


// ---------------------------------------- BROADCAST SOCKET ------------------------------------------
/**
 * @class BroadcastSender
 * BroadcastSender is the sender socket in Broadcasting.
 * @note Broadcast IP is this sender special IP address that sends to all host in the same subnet.
 */
class BroadcastSender : public UdpSocket
{
public:
    /** 
     * Constructs sender broadcasting socket
     * @note Allow both send(broadcast) and receive throught the same port
     */
    BroadcastSender( unsigned int port );
    
    /** 
     * Destructor
     */
    virtual ~BroadcastSender();
    
    /** 
     * Broadcasts to all hosts in the subnet
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len  );
    
protected:
    // Prevents default constructor
    BroadcastSender();
    
    // ----------------------- data --------------------
    sockaddr_in     bcastAddr;	// broadcast address
    
};


/**
 * @class BroadcastReceiver
 * BroadcastReceiver is the receiver socket in UDP Broadcasting that is essentially a UDP server socket.
 * @note Receives from any sender broadcasts to the same port.
 */
class BroadcastReceiver : public UdpSocket
{
public:
    /** 
     * Constructs receiver broadcast socket 
     */
    BroadcastReceiver( unsigned int port );
    
    /** 
     * Destructor
     */
    virtual ~BroadcastReceiver();
	
protected:
    // Prevents default constructor
    BroadcastReceiver();
    
    // ----------------------- data --------------------
    sockaddr_in     bcastAddr;	// broadcast address
    
};


// ---------------------------------------- MULTICAST SOCKET ------------------------------------------
/**
 * @class MulticastSender
 * MulticastSender is the sender socket in multicasting.
 */
class MulticastSender : public UdpSocket
{
public:
    /** 
     * Constructs sender socket
     * @param multicastIP the host IP that must be in range of [224.0.0.0, 239.255.255.255.255]
     */
    MulticastSender( const char* multicastIP, unsigned int port );
    
    /** 
     * Destructor
     */
    virtual ~MulticastSender();
    
	/** 
     * Sends to all receivers in the multicast group
     * @return number of bytes written or -1=ERROR
     */
    virtual int send( const void* buffer, int len  );
    
protected:
    // Prevents default constructor
    MulticastSender();
    
    // ----------------- data ---------------
    sockaddr_in     mcastSockAddr;
    
};


/**
 * @class MulticastReceiver
 * MulticastReceiver is the receiver socket in UDP multicasting.
 */
class MulticastReceiver : public UdpSocket
{
public:
    /** 
     * Constructs receiver socket 
     * @param multicastIP the host IP that must be in range of [224.0.0.0, 239.255.255.255.255]
     */
    MulticastReceiver( const char* multicastIP, unsigned int port );
    
    /** 
     * Constructs UDP client socket
     */
    MulticastReceiver( struct sockaddr_in addr );
    
    /** 
     * Destructor
     */
    virtual ~MulticastReceiver();
	
	/** 
     * Receives from the multicast sender
     * @param timeoutS timeout seconds, default 0 - no timeout
	 * @param timeoutUs timeout microseconds, default 0 - no timeout
     * @return number of bytes written or -1=ERROR
     */
    virtual int recv( void* buffer, int len, int timeoutS = 0, int timeoutUs = 0  );
    
protected:
    // Prevents default constructor
    MulticastReceiver();
    
    // ----------------- data ---------------
    sockaddr_in     mcastSockAddr;
    ip_mreq         mcastGrpAddr;      // multicast group address
    
};

#endif
