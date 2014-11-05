#pragma once
#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__


namespace myengine {

	class ClientSocket
	{
	public:
			//ClientSocket();
			//~ClientSocket();

		static ClientSocket		*create();

		virtual bool		connect(const char *address, const char *port) = 0;
		virtual void		disconnect() = 0;
		
		virtual bool		send(const char *data, unsigned size) = 0;
		virtual unsigned	receive(char *buffer, unsigned size) = 0;
	};

}


#endif //!__CLIENT_SOCKET_H__
