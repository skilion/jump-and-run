#include <Memory.h>

#include "WindowsClientSocket.h"

using namespace myengine;


//=============================================================================
// Creates a WindowsClientSocket class
//=============================================================================
ClientSocket *ClientSocket::create()
{
	return new WindowsClientSocket;
}


//=============================================================================
// Costruttore
//=============================================================================
WindowsClientSocket::WindowsClientSocket()
{
	//WSAStartup(MAKEWORD(2, 2), &wsaData); moved into System
	//WSACleanup();
	socket = INVALID_SOCKET;
}

//=============================================================================
// Distruttore
//=============================================================================
WindowsClientSocket::~WindowsClientSocket()
{
	disconnect();
}

//=============================================================================
// Connects the socket
//=============================================================================
bool WindowsClientSocket::connect(const char *address, const char *port)
{
	//TCP stream socket
	addrinfo hints;
	Memory::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo *result;
	if(getaddrinfo(address, port, &hints, &result) != 0) {
		return false;
	}

	//Attempt to connect to an address until one succeeds
    for(addrinfo *ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		socket = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(socket == INVALID_SOCKET) {
			disconnect();
			return false;
		}
		
		if(::connect(socket, ptr->ai_addr, static_cast<int>(ptr->ai_addrlen)) == 0) {
			return true;
		}
		
		disconnect();
	}
	
	return false;
}

//=============================================================================
// Disconnects the socket
//=============================================================================
void WindowsClientSocket::disconnect()
{
	if(socket != INVALID_SOCKET) {
		closesocket(socket);
		socket = INVALID_SOCKET;
	}
}

//=============================================================================
// Sends data trought the socket
//=============================================================================
bool WindowsClientSocket::send(const char *data, unsigned size)
{
	if(::send(socket, data, size, 0) == SOCKET_ERROR) {
	    return false;
	}

	return true;
}

//=============================================================================
// Receives data trought the socket
//=============================================================================
unsigned WindowsClientSocket::receive(char *buffer, unsigned size)
{
	int received;
	received = recv(socket, buffer, size, 0);
	
	if(received > 0) return received;
	
	disconnect();
	return 0;
}
