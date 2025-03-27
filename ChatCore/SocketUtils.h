#pragma once

class SocketUtils
{
public:
	static bool		Init();
	static SOCKET	CreateSocket();
	static bool		Bind(SOCKET socket, const string& port, addrinfo*& outResult);
	static bool		Listen(SOCKET socket);
	static SOCKET	Accept(SOCKET listenSocket);
	static void		Cleanup();
	static void		Close(SOCKET& socket);

private:
	static bool		GetAddress(const string& port, addrinfo*& outResult);
};

