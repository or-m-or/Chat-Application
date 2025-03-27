#undef UNICODE  // 유니코드 지원 비활성화 (멀티바이트 문자셋 사용)

#include <iostream>
#include "pch.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib") // Winsock 라이브러리 링킹

#define DEFAULT_BUFLEN 512         // 수신 버퍼 크기
#define DEFAULT_PORT "27015"		// 기본 포트 번호 (문자열 포맷)

// __cdecl : 함수 호출 규약(Calling Convention) 지정 키워드
//			 컴파일러가 함수 호출 시 메모리 정리를 어떻게 할지 정해주는 규칙
//			 함수를 호출할 때 스택을 누가 정리하냐, 인자를 어떤 순서로 푸시하냐를 정하는 약속
//			 C Declaration (함수 인자를 오른쪽에서 왼쪽으로 스택에 푸시)
int __cdecl main()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET; // 서버 소켓
	SOCKET ClientSocket = INVALID_SOCKET; // 클라이언트 소켓

	struct addrinfo* result = NULL; // 주소 정보 리스트
	struct addrinfo hints;          // 주소 검색 조건

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];   // 수신 버퍼
	int recvbuflen = DEFAULT_BUFLEN;


	// ========================
	// 1. Winsock 초기화
	// ========================
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData); // 버전 2.2 사용 요청
	if (iResult != 0) 
	{
		printf("Winsock 초기화 실패! 오류 코드: %d\n", iResult);
		return 1;
	}

	// ========================
	// 2. 주소 정보 설정
	// ========================
	ZeroMemory(&hints, sizeof(hints));     // 구조체 초기화
	hints.ai_family = AF_INET;             // IPv4
	hints.ai_socktype = SOCK_STREAM;       // TCP
	hints.ai_protocol = IPPROTO_TCP;       // TCP 프로토콜
	hints.ai_flags = AI_PASSIVE;           // 서버 바인딩용

	// 주소 정보 조회
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}


	// ========================
	// 3. 소켓 생성
	// ========================
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("소켓 생성 실패! 오류 코드: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// ========================
	// 4. 소켓에 주소 바인딩
	// ========================
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("바인딩 실패! 오류 코드: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result); // 주소 정보 해제 (더 이상 필요 없음)


	// ========================
	// 5. 소켓 리슨 (연결 대기)
	// ========================
	iResult = listen(ListenSocket, SOMAXCONN); // 최대 동시 대기 수 허용
	if (iResult == SOCKET_ERROR) {
		printf("리슨 실패! 오류 코드: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	// ========================
	// 6. 클라이언트 연결 수락 (Blocking)
	// ========================
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("연결 수락 실패! 오류 코드: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 서버 소켓은 더 이상 필요 없음
	closesocket(ListenSocket);

	// ========================
	// 7. 데이터 송수신 (반복)
	// ========================
	do {
		// 클라이언트로부터 데이터 수신
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("클라이언트로부터 %d바이트 수신됨\n", iResult);

			// 받은 데이터 그대로 다시 전송 (에코)
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("전송 실패! 오류 코드: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}

			printf("클라이언트에게 %d바이트 전송 완료\n", iSendResult);
		}
		else if (iResult == 0) {
			// 연결 정상 종료
			printf("클라이언트가 연결을 종료했습니다.\n");
		}
		else {
			// 오류 발생
			printf("수신 실패! 오류 코드: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// ========================
	// 8. 송신 종료 및 정리
	// ========================
	iResult = shutdown(ClientSocket, SD_SEND); // 더 이상 데이터 전송 안 함
	if (iResult == SOCKET_ERROR) {
		printf("종료 실패! 오류 코드: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket); // 소켓 닫기
	WSACleanup();              // Winsock 정리

	return 0;
}