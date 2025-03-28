#undef UNICODE  // 유니코드 지원 비활성화 (멀티바이트 문자셋 사용)

#include "pch.h"
#include "SocketUtils.h"
#include "GlobalManager.h"
#include "ThreadManager.h"

#define DEFAULT_BUFLEN 512         // 수신 버퍼 크기
#define DEFAULT_PORT "27015"		// 기본 포트 번호 (문자열 포맷)

GlobalManager GGlobal;				// 전역 객체
vector<SOCKET> GClients;            // 접속된 클라이언트 목록
mutex GClientLock;                  // 클라이언트 목록 보호용


int32 HandleError(const string& msg, SOCKET sock = INVALID_SOCKET)
{
	std::cerr << "[에러] " << msg << " 코드: " << WSAGetLastError() << "\n";

	if (sock != INVALID_SOCKET)
		SocketUtils::Close(sock);

	SocketUtils::Cleanup();
	return 1;
}


void Broadcast(const char* data, int32 len, SOCKET sender)
{
	lock_guard<mutex> guard(GClientLock);
	for (SOCKET client : GClients)
	{
		if (client != sender)
			send(client, data, len, 0); // 간단한 브로드캐스트
	}
}


void ClientWorker(SOCKET clientSocket)
{
	char recvbuf[DEFAULT_BUFLEN];
	int32 recvResult = 0;

	while (true)
	{
		recvResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (recvResult > 0)
		{
			std::cout << "[클라 " << clientSocket << "] 수신: " << std::string(recvbuf, recvResult) << "\n";
			Broadcast(recvbuf, recvResult, clientSocket);
		}
		else if (recvResult == 0)
		{
			std::cout << "[클라 " << clientSocket << "] 연결 종료\n";
			break;
		}
		else
		{
			std::cerr << "[클라 " << clientSocket << "] 수신 실패\n";
			break;
		}
	}

	{
		lock_guard<mutex> guard(GClientLock);
		GClients.erase(std::remove(GClients.begin(), GClients.end(), clientSocket), GClients.end());
	}
	SocketUtils::Close(clientSocket);
}



int main()
{	
	// Winsock 초기화
	if (!SocketUtils::Init())
		HandleError("Winsock 초기화 실패!");
	
	// 소켓 생성
	SOCKET listenSocket = SocketUtils::CreateSocket(); 
	if (listenSocket == INVALID_SOCKET)
		HandleError("소켓 생성 실패!");
	
	// 소켓에 주소 바인딩
	addrinfo* addrResult = nullptr;
	if (!SocketUtils::Bind(listenSocket, DEFAULT_PORT, addrResult))
		HandleError("바인딩 실패!", listenSocket);
	freeaddrinfo(addrResult);
	
	// 소켓 리슨 (연결 대기)
	if (!SocketUtils::Listen(listenSocket))
		HandleError("리슨 실패!", listenSocket);

	std::cout << "서버 실행 중... 클라이언트 접속 대기\n";


	while (true)
	{
		// 클라이언트 연결 수락 (Blocking)
		SOCKET clientSocket = SocketUtils::Accept(listenSocket);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "클라이언트 수락 실패\n";
			continue;
		}
		
		{
			lock_guard<mutex> guard(GClientLock);
			GClients.push_back(clientSocket);	
		}
		std::cout << "[클라 " << clientSocket << "] 연결됨\n";

		// 클라이언트별로 스레드 생성
		GThreadManager->Run([clientSocket]() {
			ClientWorker(clientSocket);
			});
	}

	SocketUtils::Close(listenSocket); // 리스닝 소켓

	////=============================== 데이터 송수신 루프 =======================================
	//char recvbuf[DEFAULT_BUFLEN];   // 수신 버퍼
	//int32 recvResult = 0;

	//do {
	//	// 클라이언트로부터 데이터 수신
	//	recvResult = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
	//	if (recvResult > 0) 
	//	{
	//		// 받은 데이터 그대로 다시 전송 (에코)
	//		std::cout << "클라이언트로부터 " << recvResult << "바이트 수신됨\n";
	//		if (send(clientSocket, recvbuf, recvResult, 0) == SOCKET_ERROR)
	//			HandleError("전송 실패!", clientSocket);
	//	}
	//	else if (recvResult == 0)
	//	{
	//		std::cout << "클라이언트가 연결을 종료했습니다.\n";
	//	}
	//	else
	//	{
	//		HandleError("수신 실패!", clientSocket);
	//	}
	//} while (recvResult > 0);

	//// 송신 종료 및 정리
	//if (shutdown(clientSocket, SD_SEND) == SOCKET_ERROR)
	//	HandleError("종료 실패!", clientSocket);

	SocketUtils::Close(listenSocket);
	SocketUtils::Cleanup();
	GThreadManager->Join();

	return 0;
}