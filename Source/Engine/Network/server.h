#include "socket.h"

class Server
{
public:
		Server();
		//~Server();
		
	bool	listen(const char *port);
	void	stop();
	
	bool	receive(char *buffer, unsigned size);
	
	bool	isListening();
	
	
private:
	addrinfo	*result, hints;
	SOCKET		listenSocket, clientSocket;
};
