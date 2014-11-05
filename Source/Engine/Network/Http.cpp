#include "Http.h"
#include "myString.h"
#include "ClientSocket.h"

using namespace myengine;


bool myengine::httpGet(const char *url, char *data, unsigned size)
{
	ClientSocket *socket = ClientSocket::create();

	char *tmpUrl;
	String::strcpyAlloc(&tmpUrl, url);
	//String::strlwr(tmpUrl);

	/*char *host = const_cast<char*>(String::strstr(tmpUrl, "http://"));
	if(host) host += 7; //skip "http://"
	else host = tmpUrl;*/

	char *host = String::strtok(tmpUrl, "/");
	char *page = String::strtok(nullptr, "/");

	String request = "GET /";
	request.append(page);
	request.append(" HTTP/1.0\r\nHost: ");
	request.append(host);
	request.append("\r\n\r\n");

	if(!socket->connect(host, "80")) return false;
	if(!socket->send(request, request.getLenght())) return false;
	int rec = socket->receive(data, size - 1);
	data[rec] = 0;
	
	delete [] tmpUrl;
	delete socket;

	return true;
}