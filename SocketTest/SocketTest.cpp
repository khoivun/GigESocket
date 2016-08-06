
#include <process.h>
#include <string>
#include <iostream>
#include "Socket.h"
using namespace std;

#define MAX_MSG             1024
#define TCP_PORT            2000
#define UDP_PORT            2001
#define UDP_BROADCAST_PORT  2002


/**
 * TCP Server thread
 */
unsigned __stdcall tcpServerThread( void* a ) 
{
    try
    {
        TcpServerSocket* tcpServer  =   new TcpServerSocket( TCP_PORT );

        // listen for connection
        while ( !tcpServer->isConnected() ) 
        {
            tcpServer->listenForConnection();
        }

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;

        // connected, so start receiving msgs
        while (true) 
        {
            size    =   tcpServer->recv( buffer, MAX_MSG ); // blocking
            if ( size > 0 ) 
                cout << "TCP Server received: " << buffer << endl;
        }

    } catch ( const char* ex ) {
        cerr << "Exception: " << ex << endl;
    } catch ( ... ) {
        cerr << "Unhandled Exception!" << endl;
    }

    return 0;
}

/**
 * TCP Client thread
 */
unsigned __stdcall tcpClientThread( void* a ) 
{
    try
    {
        TcpClientSocket* tcpClient  =   new TcpClientSocket( "127.0.0.1", TCP_PORT );

        // connect to the server
        while ( !tcpClient->isConnected() ) 
        {
            tcpClient->connect();   
        }

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;
        
        // connected, so start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            sprintf( buffer, "TCP client msg %d\0", i );
            size    =   tcpClient->send( buffer, strlen( buffer ) ); // blocking
            if ( size > 0 ) 
                cout << "TCP Client sent: " << buffer << endl;

            Sleep( 1000 );
        }

    } catch ( const char* ex ) {
        cerr << "Exception: " << ex << endl;
    } catch ( ... ) {
        cerr << "Unhandled Exception!" << endl;
    }

    return 0;
}


/**
 * UDP (unicast) Server thread
 */
unsigned __stdcall udpServerThread( void* a ) 
{
    try
    {
        UdpServerSocket* udpServer  =   new UdpServerSocket( UDP_PORT );

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;

        // start receiving msgs
        while (true) 
        {
            size    =   udpServer->recv( buffer, MAX_MSG ); // blocking
            if ( size > 0 ) 
                cout << "UDP Server received: " << buffer << endl;
        }

    } catch ( const char* ex ) {
        cerr << "Exception: " << ex << endl;
    } catch ( ... ) {
        cerr << "Unhandled Exception!" << endl;
    }

    return 0;
}

/**
 * UDP (unicast) Client thread
 */
unsigned __stdcall udpClientThread( void* a ) 
{
    try
    {
        UdpClientSocket* udpClient  =   new UdpClientSocket( "127.0.0.1", UDP_PORT );

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;
        
        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            sprintf( buffer, "UDP client msg %d\0", i );
            size    =   udpClient->send( buffer, strlen( buffer ) ); // blocking
            if ( size > 0 ) 
                cout << "UDP Client sent: " << buffer << endl;

            Sleep( 1000 );
        }

    } catch ( const char* ex ) {
        cerr << "Exception: " << ex << endl;
    } catch ( ... ) {
        cerr << "Unhandled Exception!" << endl;
    }

    return 0;
}


/**
 * UDP Broadcast Sender thread
 */
unsigned __stdcall udpBroadcastSenderThread( void* a ) 
{
    try
    {
        BroadcastSender* bcastSender  =   new BroadcastSender( UDP_BROADCAST_PORT );    // receive thru the same port

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;

        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            sprintf( buffer, "Broadcast sender msg %d\0", i );
            size    =   bcastSender->sendTo( buffer, MAX_MSG ); // broadcast msg
            if ( size > 0 ) 
                cout << "Broadcast sender sent: " << buffer << endl;

            Sleep( 2000 );
        }

    } catch ( const char* ex ) {
        cerr << "Exception: " << ex << endl;
    } catch ( ... ) {
        cerr << "Unhandled Exception!" << endl;
    }

    return 0;
}

/**
 * UDP Broadcast Receiver thread
 */
unsigned __stdcall udpBroadcastReceiverThread( void* a ) 
{
    try
    {
        BroadcastReceiver* bcastReceiver  =   new BroadcastReceiver( UDP_BROADCAST_PORT );

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;

        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            size    =   bcastReceiver->recvFrom( buffer, MAX_MSG, NULL ); // from any host
            if ( size > 0 ) 
                cout << "Broadcast Receiver " << (char*)a << ": " << buffer << endl;
        }

    } catch ( const char* ex ) {
        cerr << "Exception: " << ex << endl;
    } catch ( ... ) {
        cerr << "Unhandled Exception!" << endl;
    }

    return 0;
}

/**
 * MAIN
 */
int main( int argc, char* argv[] ) 
{
    cout << "Start tests ... \n" << endl;

    if ( argc != 2 )
    {
        cout << "Usage:\tSocketTest <number = 0-TCP, 1-UDP, 2-BROADCAST> " << endl;
        return 0;
    }

    switch ( argv[1][0] )
    {
        case '0':   // TCP test
                    unsigned int    thrId1, thrId2;
                    HANDLE          hThread1, hThread2;
                    // create TCP server thread
                    if ( ( hThread1 = (HANDLE)_beginthreadex( 0, 0, tcpServerThread, NULL, 0, &thrId1 ) ) == 0 )
                        cerr << "Failed to create TCP server thread!" << endl;

                    // create TCP client thread
                    if ( ( hThread2 = (HANDLE)_beginthreadex( 0, 0, tcpClientThread, NULL, 0, &thrId2 ) ) == 0 )
                        cerr << "Failed to create UDP server thread!" << endl;

                    // wait for threads to terminate
                    WaitForSingleObject( hThread1, INFINITE );
                    WaitForSingleObject( hThread2, INFINITE );
                    break;

        case '1':   // UDP Unicast test
                    unsigned int    thrId3, thrId4;
                    HANDLE          hThread3, hThread4;
                    // create UDP server thread
                    if ( ( hThread3 = (HANDLE)_beginthreadex( 0, 0, udpServerThread, NULL, 0, &thrId3 ) ) == 0 )
                        cerr << "Failed to create TCP server thread!" << endl;

                    // create UDP client thread
                    if ( ( hThread4 = (HANDLE)_beginthreadex( 0, 0, udpClientThread, NULL, 0, &thrId4 ) ) == 0 )
                        cerr << "Failed to create UDP server thread!" << endl;
                     
                    // wait for threads to terminate
                    WaitForSingleObject( hThread3, INFINITE );
                    WaitForSingleObject( hThread4, INFINITE );
                    break;

        case '2':   // UDP Broadcast test
                    unsigned int    thrId5, thrId6, thrId7;
                    HANDLE          hThread5, hThread6, hThread7;
                    // create Broadcast Sender thread
                    if ( ( hThread5 = (HANDLE)_beginthreadex( 0, 0, udpBroadcastSenderThread, NULL, 0, &thrId5 ) ) == 0 )
                        cerr << "Failed to create TCP server thread!" << endl;

                    // create 2 broadcast receivers thread
                    if ( ( hThread6 = (HANDLE)_beginthreadex( 0, 0, udpBroadcastReceiverThread, "Receiver1", 0, &thrId6 ) ) == 0 )
                        cerr << "Failed to create UDP server thread!" << endl;
                    /*if ( ( hThread7 = (HANDLE)_beginthreadex( 0, 0, udpBroadcastReceiverThread, "Receiver2", 0, &thrId7 ) ) == 0 )
                        cerr << "Failed to create UDP server thread!" << endl;
                     */

                     // wait for threads to terminate
                    WaitForSingleObject( hThread5, INFINITE );
                    WaitForSingleObject( hThread6, INFINITE );
                    //WaitForSingleObject( hThread7, INFINITE );
                    break;

        default:
                    break;
    }

    cout << "Exiting ... \n" << endl;
    return 0;
}