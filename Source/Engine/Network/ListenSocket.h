#pragma once
#ifndef __LISTEN_SOCKET_H__
#define __LISTEN_SOCKET_H__


class ListenSocket
{
public:
		//ListenSocket();
		//~ListenSocket();

	static ListenSocket		*create();

	bool	listen(const char *port) = 0;
	void	stop() = 0;
		
	bool	accept(Socket *socket) = 0;
};


#endif //!__LISTEN_SOCKET_H__
