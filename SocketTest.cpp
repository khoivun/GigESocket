



<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<!-- ViewVC :: http://www.viewvc.org/ -->
<head>
<title>[cvs] View of /TyzxRoot/Devel/Source/Emerging/Socket/SocketTest.cpp</title>
<meta name="generator" content="ViewVC 1.0.3" />
<link rel="stylesheet" href="/viewvc-static/styles.css" type="text/css" />

</head>
<body>
<div class="vc_navheader">

<form method="get" action="/cgi-bin/viewvc.cgi/">

<table style="padding:0.1em;">
<tr>
<td>
<strong>

<a href="/cgi-bin/viewvc.cgi/">

[cvs]</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/">

TyzxRoot</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/">

Devel</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/">

Source</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/">

Emerging</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/">

Socket</a>
/

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/SocketTest.cpp?view=log">

SocketTest.cpp</a>


</strong>

</td>
<td style="text-align:right;">


<strong>Repository:</strong>
<select name="root" onchange="submit()">


<option value="*viewroots*">Repository Listing</option>




















<optgroup label="CVS Repositories"><option selected="selected">cvs</option></optgroup>


<optgroup label="Subversion Repositories"><option>svn</option></optgroup>

</select>
<input type="submit" value="Go" />

</td>
</tr>
</table>

</form>

</div>
<div style="float: right; padding: 5px;"><a href="http://www.viewvc.org/"><img src="/viewvc-static/images/logo.png" alt="ViewVC logotype" width="128" height="48" /></a></div>
<h1>View of /TyzxRoot/Devel/Source/Emerging/Socket/SocketTest.cpp</h1>

<p style="margin:0;">

<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/"><img src="/viewvc-static/images/back_small.png" width="16" height="16" alt="Parent Directory" /> Parent Directory</a>

| <a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/SocketTest.cpp?view=log#rev1.5"><img src="/viewvc-static/images/log.png" width="16" height="16" alt="Revision Log" /> Revision Log</a>




</p>

<hr />
<div class="vc_summary">
Revision <strong>1.5</strong> -
(<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/SocketTest.cpp?revision=1.5"><strong>download</strong></a>)

(<a href="/cgi-bin/viewvc.cgi/TyzxRoot/Devel/Source/Emerging/Socket/SocketTest.cpp?annotate=1.5"><strong>annotate</strong></a>)

<br /><em>Sat Feb  2 02:21:07 2008 UTC</em>
(2 months, 1 week ago)
by <em>khoi</em>


<br />Branch: <strong>MAIN</strong>


<br />CVS Tags: <strong>Rev_6_1, HEAD</strong>




<br />Changes since <strong>1.4: +46 -20 lines</strong>





<pre class="vc_log">updated 2/1
</pre>

</div>
<div id="vc_markup"><pre>
#include &lt;process.h&gt;
#include &lt;string&gt;
#include &lt;iostream&gt;
#include "Socket.h"
using namespace std;

#define MAX_MSG             1024
#define TCP_PORT            3000
#define UDP_PORT            2000
#define UDP_BROADCAST_PORT  2002
#define UDP_MULTICAST_IP    "225.0.0.0"
#define UDP_MULTICAST_PORT  2003

// Gets local host's IP in network byt order
char* getLocalHostIp()
{
	char localhost[128];
    if ( gethostname( localhost, sizeof( localhost )) == SOCKET_ERROR) 
    {  
        printf( "Failed to get local host name!\n" );
        return 0;
    }    
    printf( "Host name: %s\n", localhost );

    struct hostent * hostDesc = gethostbyname( localhost );
    if ( hostDesc == 0 ) 
    {
        printf( "Failed to get host description!\n" );
        return 0;
    }
    
    struct in_addr hostAddr;
    memcpy( &amp;hostAddr, hostDesc-&gt;h_addr_list[0], sizeof(struct in_addr) );
    printf( "Host Address: %s, network IP: %d\n", inet_ntoa( hostAddr ), hostAddr.s_addr );
    return inet_ntoa( hostAddr );
}

/**
 * TCP Server thread
 */
unsigned __stdcall tcpServerThread( void* a ) 
{
    try
    {
        TcpServerSocket* tcpServer  =   new TcpServerSocket( TCP_PORT );

        // listen for connection
        while ( !tcpServer-&gt;isConnected() ) 
        {
            tcpServer-&gt;listenForConnection();
        }

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;

        // connected, so start receiving msgs
        while (true) 
        {
            size    =   tcpServer-&gt;recv( buffer, MAX_MSG ); // blocking
            if ( size &gt; 0 ) 
                cout &lt;&lt; "TCP Server received: " &lt;&lt; buffer &lt;&lt; endl;
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
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
        while ( !tcpClient-&gt;isConnected() ) 
        {
            tcpClient-&gt;connect();   
        }

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;
        
        // connected, so start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            sprintf( buffer, "TCP client msg %d\0", i );
            size    =   tcpClient-&gt;send( buffer, strlen( buffer ) ); // blocking
            if ( size &gt; 0 ) 
                cout &lt;&lt; "TCP Client sent: " &lt;&lt; buffer &lt;&lt; endl;

            Sleep( 2000 );
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
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
            size    =   udpServer-&gt;recv( buffer, MAX_MSG ); // blocking
            if ( size &gt; 0 ) 
                cout &lt;&lt; "UDP Server received: " &lt;&lt; buffer &lt;&lt; endl;
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
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
        char* localIp   =   getLocalHostIp();
        printf( "Local IP = %s\n", localIp );

        UdpClientSocket* udpClient  =   new UdpClientSocket( localIp, UDP_PORT );

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;
        
        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            sprintf( buffer, "UDP client msg %d\0", i );
            size    =   udpClient-&gt;send( buffer, strlen( buffer ) ); // blocking
            if ( size &gt; 0 ) 
                cout &lt;&lt; "UDP Client sent: " &lt;&lt; buffer &lt;&lt; endl;

            Sleep( 1000 );
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
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

        char        buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;
        sockaddr    clientAddr;

        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            // broadcasting msgs
            sprintf( buffer, "Broadcast Sender msg %d\0", i );
            size    =   bcastSender-&gt;send( buffer, MAX_MSG ); // broadcast msg
            if ( size &gt; 0 ) 
                printf( "Broadcast Sender sent: %s\n", buffer );

            Sleep( 1000 );

            // receive msg through the same port
            size    =   bcastSender-&gt;recvFrom( buffer, MAX_MSG, &amp;clientAddr );
            if ( size &gt; 0 ) 
                printf( "Broadcast Sender received: %s\n", buffer );

            Sleep( 3000 );
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
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
        sockaddr    senderAddr;
        int size;

        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            // reveive from broadcasting sender
            size    =   bcastReceiver-&gt;recvFrom( buffer, MAX_MSG, &amp;senderAddr ); // from any host
            if ( size &gt; 0 ) 
                printf( "Broadcast Receiver received: %s\n", buffer );

            Sleep( 2000 );

            // send msg to the broadcasting sender
            sprintf( buffer, "Broadcast Receiver msg %d\0", i );
            size    =   bcastReceiver-&gt;sendTo( buffer, MAX_MSG, &amp;senderAddr ); 
            if ( size &gt; 0 ) 
                printf( "Broadcast Receiver sent: %s\n", buffer );

            Sleep( 4000 );
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
    }

    return 0;
}


/**
 * UDP Multicast Sender thread
 */
unsigned __stdcall udpMulticastSenderThread( void* a ) 
{
    try
    {
        MulticastSender* mcastSender  =   new MulticastSender( UDP_MULTICAST_IP, UDP_MULTICAST_PORT );    

        char        buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;
        
        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            // broadcasting msgs
            sprintf( buffer, "Multicast Sender msg %d\0", i );
            size    =   mcastSender-&gt;send( buffer, MAX_MSG ); 
            if ( size &gt; 0 ) 
                printf( "Multicast Sender sent: %s\n", buffer );

            Sleep( 3000 );
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
    }

    return 0;
}

/**
 * UDP Multicast Receiver thread
 */
unsigned __stdcall udpMulticastReceiverThread( void* a ) 
{
    try
    {
        MulticastReceiver* mcastReceiver  =   new MulticastReceiver( UDP_MULTICAST_IP, UDP_MULTICAST_PORT );

        char    buffer [MAX_MSG];
        memset( buffer, 0, MAX_MSG );
        int size;

        // start sending msgs
        for ( int i = 0; true; ++i ) 
        {
            // reveive from broadcasting sender
            size    =   mcastReceiver-&gt;recv( buffer, MAX_MSG ); 
            if ( size &gt; 0 ) 
                printf( "Multicast Receiver received: %s\n", buffer );
        }

    } catch ( const char* ex ) {
        cerr &lt;&lt; "Exception: " &lt;&lt; ex &lt;&lt; endl;
    } catch ( ... ) {
        cerr &lt;&lt; "Unhandled Exception!" &lt;&lt; endl;
    }

    return 0;
}


/**
 * MAIN
 */
int main( int argc, char* argv[] ) 
{
#ifdef WIN32
        // ----- temp: get local hostname and IP address ----
        // Init WinSock
        WSADATA wsa_Data;
        int wsa_ReturnCode = WSAStartup(0x101,&amp;wsa_Data);
        /*char    hostname[128];
        gethostname( hostname, 128 );
        printf( "host name: %s\n", hostname );
        struct hostent *hostEntry;
        hostEntry = gethostbyname( hostname );
		printf( "host IP in network byte order: %s\n", *hostEntry-&gt;h_addr_list );
        char * localIP;
        localIP = inet_ntoa (*(struct in_addr *)*hostEntry-&gt;h_addr_list);
        printf( "host IP: %s\n", localIP );
        WSACleanup();*/
		// ------------
#endif

    cout &lt;&lt; "Start tests ... \n" &lt;&lt; endl;

    if ( argc != 2 )
    {
        cout &lt;&lt; "Usage:\tSocketTest &lt;number = 0-TCP, 1-UDP_UNICAST, 2-BROADCAST, 3-MULTICAST&gt; " &lt;&lt; endl;
        return 0;
    }

    switch ( argv[1][0] )
    {
        case '0':   // TCP test
                    unsigned int    thrId1, thrId2;
                    HANDLE          hThread1, hThread2;
                    // create TCP server thread
                    if ( ( hThread1 = (HANDLE)_beginthreadex( 0, 0, tcpServerThread, NULL, 0, &amp;thrId1 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create TCP server thread!" &lt;&lt; endl;

                    // create TCP client thread
                    if ( ( hThread2 = (HANDLE)_beginthreadex( 0, 0, tcpClientThread, NULL, 0, &amp;thrId2 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create UDP server thread!" &lt;&lt; endl;

                    // wait for threads to terminate
                    WaitForSingleObject( hThread1, INFINITE );
                    WaitForSingleObject( hThread2, INFINITE );
                    break;

        case '1':   // UDP Unicast test
                    unsigned int    thrId3, thrId4;
                    HANDLE          hThread3, hThread4;
                    // create UDP server thread
                    if ( ( hThread3 = (HANDLE)_beginthreadex( 0, 0, udpServerThread, NULL, 0, &amp;thrId3 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create UDP server thread!" &lt;&lt; endl;

                    // create UDP client thread
                    if ( ( hThread4 = (HANDLE)_beginthreadex( 0, 0, udpClientThread, NULL, 0, &amp;thrId4 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create UDP client thread!" &lt;&lt; endl;
                     
                    // wait for threads to terminate
                    WaitForSingleObject( hThread3, INFINITE );
                    WaitForSingleObject( hThread4, INFINITE );
                    break;

        case '2':   // UDP Broadcast test
                    unsigned int    thrId5, thrId6;
                    HANDLE          hThread5, hThread6;
                    // create Broadcast Sender thread
                    if ( ( hThread5 = (HANDLE)_beginthreadex( 0, 0, udpBroadcastSenderThread, NULL, 0, &amp;thrId5 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create Broadcast sender thread!" &lt;&lt; endl;

                    // create 2 broadcast receivers thread
                    if ( ( hThread6 = (HANDLE)_beginthreadex( 0, 0, udpBroadcastReceiverThread, "Receiver1", 0, &amp;thrId6 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create Broadcast receiver thread!" &lt;&lt; endl;
                    
                     // wait for threads to terminate
                    WaitForSingleObject( hThread5, INFINITE );
                    WaitForSingleObject( hThread6, INFINITE );
                    break;

        case '3':   // UDP Multicast test
                    unsigned int    thrId7, thrId8, thrId9;
                    HANDLE          hThread7, hThread8, hThread9;
                    // create multicast Sender thread
                    if ( ( hThread7 = (HANDLE)_beginthreadex( 0, 0, udpMulticastSenderThread, NULL, 0, &amp;thrId7 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create Multicast sender thread!" &lt;&lt; endl;

                    // create multicast receivers thread
                    if ( ( hThread8 = (HANDLE)_beginthreadex( 0, 0, udpMulticastReceiverThread, "MulticastReceiver1", 0, &amp;thrId8 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create Multicast receiver thread!" &lt;&lt; endl;
                    if ( ( hThread9 = (HANDLE)_beginthreadex( 0, 0, udpMulticastReceiverThread, "MulticastReceiver2", 0, &amp;thrId9 ) ) == 0 )
                        cerr &lt;&lt; "Failed to create Multicast receiver thread!" &lt;&lt; endl;
                    
                     // wait for threads to terminate
                    WaitForSingleObject( hThread7, INFINITE );
                    WaitForSingleObject( hThread8, INFINITE );
                    WaitForSingleObject( hThread9, INFINITE );
                    break;

        default:
                    break;
    }

    cout &lt;&lt; "Exiting ... \n" &lt;&lt; endl;
    return 0;
}</pre></div>

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

