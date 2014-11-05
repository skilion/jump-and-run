#pragma once
#ifndef __WINDOWS_CLIENT_SOCKET_H__
#define __WINDOWS_CLIENT_SOCKET_H__


#include "ClientSocket.h"
#include "System/WindowsHeader.h"


namespace myengine {

	class WindowsClientSocket: public ClientSocket
	{
	public:
			WindowsClientSocket();
			~WindowsClientSocket();
	
		virtual bool		connect(const char *address, const char *port);
		virtual void		disconnect();
		
		virtual bool		send(const char *data, unsigned size);
		virtual unsigned	receive(char *buffer, unsigned size);


	private:
		SOCKET	socket;
	};

}


#endif //!__WINDOWS_CLIENT_SOCKET_H__
