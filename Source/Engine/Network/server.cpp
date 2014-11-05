//http://msdn.microsoft.com/en-us/library/bb530742(v=VS.85).aspx

#include <cstring>

#include "server.h"


//=============================================================================
// Costruttore
//=============================================================================
Server::Server()
{
	result = 0;
	
	//Tipo di socket richiesto
	memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;



	listenSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;
}

//=============================================================================
// Distruttore
//=============================================================================
//Server::~Server() {}

//=============================================================================
// Prepara il server a ricevere sulla porta specificata
//=============================================================================
#include <iostream>
using namespace std;
bool Server::listen(const char *port)
{
	if(getaddrinfo(0, port, &hints, &result) != 0) {
		return false;
	}
	
	//Crea il socket
	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if(listenSocket == INVALID_SOCKET) {
		stop();
		return false;
	}
	
	//Prepara il socket a ricevere
    if(bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        //WSAGetLastError() ...
        stop();
        return false;
    }
    
    //Prepara il socket a ricevere connessioni
    if(::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR ) {
		//WSAGetLastError()...
		stop();
	    return false;
	}
	
	//Attende e accetta la prima connessione
	clientSocket = accept(listenSocket, 0, 0);
	if(clientSocket == INVALID_SOCKET) {
	    //WSAGetLastError() ...
	    stop();
	    return false;
	}

	return true;
}

//=============================================================================
// Ferma il server
//=============================================================================
void Server::stop()
{
	//Disconnette il client
	if(clientSocket != INVALID_SOCKET) {
		shutdown(clientSocket, SD_SEND);
		closesocket(clientSocket);
	}

	freeaddrinfo(result);
	closesocket(listenSocket);
	
	result = 0;
	listenSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;
}

//=============================================================================
// Riceve dati dal client
// Ritorna vero se dei dati sono stati effettivamente ricevuti e li mette nel buffer
// Ritorna falso se ci sono errori o il client si disconnette
//    size = dimensione del buffer
//=============================================================================
bool Server::receive(char *buffer, unsigned size)
{
	unsigned received;

	while(1) {
		received = recv(clientSocket, buffer, size, 0);
		
		if(received > 0) {
			//Ricevuto qualcosa
			
			// Echo the buffer back to the sender
	        /*iSendResult = send(ClientSocket, recvbuf, iResult, 0);
	        if (iSendResult == SOCKET_ERROR) {
	            printf("send failed: %d\n", WSAGetLastError());
	            closesocket(ClientSocket);
	            WSACleanup();
	            return 1;
	        }*/
	        return true;
		} else if(received == 0) {
			//Il client si è disconnesso
			return false;
		} else {
			//Errori
			
			//WSAGetLastError() ...
			stop();
			return false;
		}
		
		Sleep(5);
	};
}

//=============================================================================
// Ritorna vero se il server sta ascoltando
//=============================================================================
bool Server::isListening()
{
	return listenSocket != INVALID_SOCKET;
}
