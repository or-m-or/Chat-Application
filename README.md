<h2 align='center'>🧠 Chat Application (C++ & WinForms)</h2>

<p align='center'>
C++, Winform 기반 TCP 소켓 채팅 애플리케이션
<br><br>
</p>


## Table of Contents

1. [Introduction](#introduction)
2. [Configuration](#configuration)
3. [Skill](#skill)
4. [Features](#features)
5. [Quick Start](#quick-start)
6. [Version History](#version-history)
7. [Direction for improvement](#direction-for-improvement)
8. [License](#license)
9. [Contact](#contact)

---

## Introduction

- TCP 기반의 간단한 채팅 애플리케이션으로, C++로 구현된 서버와 C# WinForms 기반의 GUI 클라이언트로 구성되어 있습니다.  
- 클라이언트는 메시지를 전송하면 서버가 연결된 모든 클라이언트에 메시지를 브로드캐스트하는 구조를 가지고 있습니다.
- 각 클라이언트는 독립적인 스레드에서 수신 처리를 수행하며, 동시에 여러 명이 실시간으로 채팅할 수 있습니다.

---

## Configuration

```
Chat-Application
├── 📁 ChatCore       # 공통 네트워크/스레드 유틸리티 라이브러리 (C++)
├── 📁 ChatServer     # TCP 서버 애플리케이션 (C++)
└── 📁 ChatClient     # WinForms GUI 클라이언트 (C#)
```

---

## Skill

### 공통
- Visual Studio / C++17 /  C# (.NET Framework) / WinForms
- Windows 플랫폼 기반 개발
- **Precompiled Header 사용** (`pch.h`, `pch.cpp`): 컴파일 속도 최적화
- **공통 타입 정의** (`Types.h`): 고정 크기 정수 타입 정의 (`int32`, `uint64` 등)
- **STL 활용** (`vector`, `thread`, `mutex`, `function` 등 - `ThreadManager 클래스`, `ChatServer.cpp` 등에서 사용)

### ChatServer

| 기술 | 설명 | 적용 파일 |
|------|------|-----------|
| **Winsock2 API** | TCP 소켓 초기화, 바인딩, 수신, 전송 | `SocketUtils 클래스`, `ChatServer.cpp` |
| **Blocking 소켓 통신** | `recv`, `send`, `accept` 등 사용 | `ChatServer.cpp` |
| **멀티스레딩 (std::thread)** | 클라이언트별로 스레드 생성 | `ThreadManager 클래스`, `ChatServer.cpp` |
| **전역 객체 관리** | `GThreadManager`를 통한 스레드 접근 | `GlobalManager 클래스`, `ChatServer.cpp` |
| **Broadcast 메시지 처리** | 클라이언트 목록 순회 후 `send()` | `ChatServer.cpp` |
| **Socket 유틸리티 클래스화** | `SocketUtils::Init()`, `Accept()` 등으로 추상화 | `SocketUtils 클래스` |


### ChatClient

| 기술 | 설명 | 적용 파일 |
|------|------|-----------|
| **.NET TcpClient** | 서버 접속 및 통신 스트림 생성 | `Form1.cs` |
| **NetworkStream 사용** | 메시지 송수신 처리 | `Form1.cs` |
| **WinForms GUI** | 입력창, 출력창, 접속 버튼 구성 | `Form1.Designer.cs`, `Form1.cs` |
| **멀티스레드 수신 처리** | `Thread`로 백그라운드 수신 루프 실행 | `Form1.cs` |
| **UI 쓰레드 접근 보호** | `Invoke()` 사용하여 TextBox 안전하게 업데이트 | `Form1.cs` |

---

## Features

- ✅ 다중 클라이언트 접속 처리
- ✅ 브로드캐스트 방식의 메시지 전송
- ✅ 독립적인 스레드에서 수신 처리
- ✅ WinForms 기반 GUI 채팅 인터페이스
- ✅ TCP 기반 안정적 송수신 처리
- ✅ 서버 및 클라이언트 모두 콘솔/GUI 실행

---

## Quick Start

### 🔧 서버 실행 (ChatServer)
1. Visual Studio에서 `ChatServer` 프로젝트 실행
2. 콘솔 창에서 서버가 포트 `27015`에서 대기

### 🖥 클라이언트 실행 (ChatClient)
1. Visual Studio에서 `ChatClient` 실행
2. "Connect" 버튼 클릭 → 서버 접속
3. 메시지 입력 후 Enter 키로 채팅 전송

---

## Version History

| 날짜 | 버전 | 내용 |
|------|------|------|
| 2025.04 | v0.1.0 | 다중 접속, 채팅 브로드캐스트, WinForms UI 구성 |

---

## Direction for improvement

- Google Protobuf 도입으로 메시지 구조 직렬화
- 유저 정보 및 닉네임 기반 세션 처리
- Room 구조 기반의 다중 채팅방 기능 확장
- Select / IOCP 모델로 비동기 방식 도입
- 채팅 로그 저장 및 관리 기능 추가

---

## License

본 프로젝트는 MIT License 하에 배포됩니다.

---

## Contact

📧 hth815@naver.com
문의 사항이나 피드백은 언제든지 환영합니다!
