#include "pch.h"
#include "SocketUtils.h"

bool SocketUtils::Init()
{
	WSADATA wsaData;
	int32 result = WSAStartup(MAKEWORD(2, 2), &wsaData); // 버전 2.2 사용 요청
	if (result != 0)
	{
		std::cerr << "Windows 소켓 초기화 실패: " << result << std::endl;
		return false;
	}
	return true;
}

bool SocketUtils::GetAddress(const string& port, addrinfo*& outResult)
{
	addrinfo hints;						// 주소 검색 조건
	ZeroMemory(&hints, sizeof(hints));	// 구조체 초기화
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP
	hints.ai_protocol = IPPROTO_TCP;	// TCP 프로토콜
	hints.ai_flags = AI_PASSIVE;		// 서버 바인딩용

	// 주소 정보 조회
	int32 result = getaddrinfo(NULL, port.c_str(), &hints, &outResult);
	if (result != 0)
	{
		std::cerr << "주소정보 조회 실패: " << result << std::endl;
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
		std::cerr << "바인딩 실패: " << WSAGetLastError() << std::endl;
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
