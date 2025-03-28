#include "pch.h"
#include "SocketUtils.h"

bool SocketUtils::Init()
{
	WSADATA wsaData;
	int32 result = WSAStartup(MAKEWORD(2, 2), &wsaData); // ���� 2.2 ��� ��û
	if (result != 0)
	{
		std::cerr << "Windows ���� �ʱ�ȭ ����: " << result << std::endl;
		return false;
	}
	return true;
}

bool SocketUtils::GetAddress(const string& port, addrinfo*& outResult)
{
	addrinfo hints;						// �ּ� �˻� ����
	ZeroMemory(&hints, sizeof(hints));	// ����ü �ʱ�ȭ
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP
	hints.ai_protocol = IPPROTO_TCP;	// TCP ��������
	hints.ai_flags = AI_PASSIVE;		// ���� ���ε���

	// �ּ� ���� ��ȸ
	int32 result = getaddrinfo(NULL, port.c_str(), &hints, &outResult);
	if (result != 0)
	{
		std::cerr << "�ּ����� ��ȸ ����: " << result << std::endl;
		return false;
	}
	return true;
}

SOCKET SocketUtils::CreateSocket()
{
	return socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

bool SocketUtils::Bind(SOCKET socket, const string& port, addrinfo*& outResult)
{
	if (!GetAddress(port, outResult))
		return false;

	if (bind(socket, outResult->ai_addr, static_cast<int32>(outResult->ai_addrlen)) == SOCKET_ERROR)
	{
		std::cerr << "���ε� ����: " << WSAGetLastError() << std::endl;
		freeaddrinfo(outResult);
		return false;
	}
	return true;
}

bool SocketUtils::Listen(SOCKET socket)
{
	if (listen(socket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

SOCKET SocketUtils::Accept(SOCKET listenSocket)
{
	return accept(listenSocket, NULL, NULL);
}

void SocketUtils::Cleanup()
{
	WSACleanup();
}

void SocketUtils::Close(SOCKET& socket)
{
	if (socket != INVALID_SOCKET)
		closesocket(socket);
	socket = INVALID_SOCKET;
}
