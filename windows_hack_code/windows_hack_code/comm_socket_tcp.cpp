#include "stdafx.h"
#include "comm_socket_tcp.h"

SOCKET server_socket = NULL;
SOCKET client_socket = NULL;


INT ServerInit(char * ip, int port) {
	// winsock init
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	// create socket
	server_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == INVALID_SOCKET) {
		return WSAGetLastError();
	}
	// create addr and bind
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = ::htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (::bind(server_socket, (sockaddr *)(&addr), sizeof(addr)) != 0) {
		return WSAGetLastError();
	}
	// listen
	if (::listen(server_socket, 1) != 0) {
		return WSAGetLastError();
	}
	// create another thread to accept and print
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ReadMsg, NULL, NULL, NULL);
}

void SendMsg(char * buf) {
	if (client_socket == NULL) {
		printf("Not connect yet, can not send msg.\n");
	} else {
		::send(client_socket, buf, 1 + lstrlenA(buf), 0);
	}
}

void ReadMsg() {
	sockaddr_in addr;
	int iLen = sizeof(addr);
	client_socket = ::accept(server_socket, (sockaddr *)(&addr), &iLen);
	printf("accept a client...\n");
	char buf[MAX_PATH] = { 0 };
	while (TRUE) {
		int ret = ::recv(client_socket, buf, MAX_PATH, 0);
		if (ret <= 0) {
			continue;
		}
		printf("GET: %s\n", buf);
	}
}

// ------------------------------------- for client -------------------------------------------

int ClientInit(char * ip, int port) {
	// Winsock init
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);
	// create socket
	client_socket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == INVALID_SOCKET) {
		return WSAGetLastError();
	}
	// connect
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (::connect(client_socket, (sockaddr *)(&addr), sizeof(addr)) != 0) {
		return WSAGetLastError();
	}
	// create thread to recv msg
	::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientReadMsg, NULL, NULL, NULL);
}

void ClientReadMsg() {
	char buf[MAX_PATH] = { 0 };
	while (TRUE) {
		int ret = ::recv(client_socket, buf, MAX_PATH, 0);
		if (ret <= 0) {
			continue;
		}
		printf("Get: %s\n", buf);
	}
}

void ClientSendMsg(char * buf) {
	::send(client_socket, buf, 1 + lstrlenA(buf), 0);
}