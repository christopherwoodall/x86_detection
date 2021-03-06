#include <winsock2.h> 
#include <iostream.h> 
#include <stdio.h> 
#include <tchar.h> 
#include <windows.h> 
using namespace std;

int main(int argc, char* argv[]) 
{ 
 // Initialize WinSock2.2 DLL 
 // low word = major, highword = minor 
 WSADATA wsaData = {0};
 WORD wVer = MAKEWORD(2,2);
 int nRet = WSAStartup( wVer, &wsaData );
 if( nRet == SOCKET_ERROR ) { 
 cout << "Failed to init Winsock library" << endl;
 return -1;
 } 
 cout << "Opening connection to server" << endl;
 WORD WSAEvent = 0;
 WORD WSAErr = 0;
 SOCKET hServer = {0};
 // open a socket 
 // 
 // for the server we do not want to specify a network address 
 // we should always use INADDR_ANY to allow the protocal stack 
 // to assign a local IP address 
 hServer = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
 if( hServer == INVALID_SOCKET ) { 
 cout << "Invalid socket, failed to create socket" << endl;
 return -1;
 } 
 // name a socket 
 sockaddr_in saServer = {0};
 
 saServer.sin_family = PF_INET; 
 saServer.sin_port = htons( 27015 ); 
 saServer.sin_addr.s_addr = inet_addr( "192.168.137.1" );
 // connect 
 nRet = connect( hServer, (sockaddr*)&saServer, sizeof( sockaddr ) ); 
 if( nRet == SOCKET_ERROR ) { 
 cout << "Connection to server failed" << endl;
 closesocket( hServer );
 return -1;
 } 
 cout << "Connected to server" << endl;
 cout << "Sending data to server" << endl;
 // process data 
 char wzRec[1024] = "Hello from client!!!"; 
 int nLeft = 512;
 int iPos = 0;
 int nData = 0;
 if( argc == 2 ) { 
 // copy input string from command argument 
 //strcpy( wzRec, 1024, argv[1] );
 } 
 do
 { 
 nData = send( hServer, &wzRec[iPos], nLeft, 0 );
 if( nData == SOCKET_ERROR ) { 
 cout << "Error sending data" << endl;
 break;
 } 
 nLeft -= nData;
 iPos += nData;
 } while( nLeft > 0 );
 // clear data buffer 
 memset( &wzRec, 0, sizeof( wzRec ) );
 nLeft = 512;
 iPos = 0;
 do
 { 
 nData = recv( hServer, &wzRec[iPos], nLeft, 0 );
 if( nData == SOCKET_ERROR ) { 
 cout << "Error receiving data" << endl;
 break;
 } 
 nLeft -= nData;
 iPos += nData;
 } while( nLeft > 0 );
 cout << "Data Echoed: " << wzRec << endl;
 cout << "Closing connection" << endl;
 // shutdown socket 
 nRet = shutdown( hServer, SD_BOTH );
 if( nRet == SOCKET_ERROR ) { 
 // WSAGetLastError() 
 cout << "Error trying to perform shutdown on socket" << endl;
 return -1;
 } 
 // close server socket 
 nRet = closesocket( hServer );
 hServer = 0;
 if( nRet == SOCKET_ERROR ) { 
 cout << "Error failed to close socket" << endl;
 } 
 
 // Release WinSock DLL 
 nRet = WSACleanup();
 if( nRet == SOCKET_ERROR ) { 
 cout << "Error cleaning up Winsock Library" << endl;
 return -1;
 } 
 cout << "Data sent successfully" << endl;
 return 0;
}