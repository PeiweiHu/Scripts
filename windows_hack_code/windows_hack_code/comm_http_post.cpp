#include "stdafx.h"
#include "comm_http_post.h"

#include <iostream>
#include <winsock2.h>
#include <string>
using namespace std;

//link libwsock32.a

unsigned long WinsockStart()
{
	WSADATA wsa;
	unsigned long ulong;
	struct hostent *host;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) < 0)
	{
		cout << "Error WinsockStart()" << endl;
		WSACleanup();
		return 1;
	}

	if ((host = gethostbyname("www.example.com"))<0)
	{
		cout << "Fehler gethostbyname()" << endl;
		WSACleanup();
		return 2;
	}

	ulong = *(unsigned long*)host->h_addr;

	return ulong;
}

void error_exit(string text)
{
	cout << text;
	WSACleanup();
	exit(EXIT_FAILURE);
}

//upload file to **192.168.179.128/upload.php**, server can access the uploaded file with identifier **upfile**

int comm_http_post_test()
{
	SOCKET sock;
	struct sockaddr_in addr;
	unsigned long win = 0;
	int con = 0, gr = 0, send_r = 0, rec = 0;
	char header[2048], puffer[2048] = { 0 };
	//char header[MAX_PATH], puffer[MAX_PATH];
	string to_send = "hello world";
	string name = "test99.txt";

	win = WinsockStart();
	if (win == 1 || win == 2)
		error_exit("Error WinsockStart()");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	//addr.sin_addr.s_addr = win;
	addr.sin_addr.s_addr = inet_addr("192.168.179.128");

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0)
		error_exit("Error socket()");

	gr = (to_send.size() + name.size() + 287);

	sprintf(header, "POST /upload.php HTTP/1.1\r\n");
	//sprintf(header, "%sHost: www.example.com\r\n", header);
	sprintf(header, "%sHost: 192.168.179.128\r\n", header);
	sprintf(header, "%sConnection: Keep-Alive\r\n", header);
	sprintf(header, "%sContent-Type: multipart/form-data; boundary=---------------------------90721038027008\r\n", header);
	sprintf(header, "%sContent-Length: %d\r\n", header, gr);
	sprintf(header, "%s\r\n", header);
	sprintf(header, "%s-----------------------------90721038027008\r\n", header);
	sprintf(header, "%sContent-Disposition: form-data; name=\"upfile\"; filename=\"%s\"\r\n", header, name.c_str());
	sprintf(header, "%sContent-Type: text/plain\r\n", header);
	sprintf(header, "%s\r\n", header);
	sprintf(header, "%s%s\r\n", header, to_send.c_str());
	//sprintf(header, "%s-----------------------------90721038027008\r\n", header);
	//sprintf(header, "%sContent-Disposition: form-data; name=\"post\"\r\n", header);
	//sprintf(header, "%s\r\n", header);
	//sprintf(header, "%supload\r\n\r\n", header);
	sprintf(header, "%s-----------------------------90721038027008--\r\n\r\n\0", header);

	con = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (con < 0)
		error_exit("Error connect()");

	if (send_r = send(sock, header, strlen(header), 0)<0)
		error_exit("Error send()");

	while (rec = recv(sock, puffer, 2048, 0))
	{
		if (rec == 0)
			error_exit("Server quit");

		printf("%s", puffer);
	}

	closesocket(sock);
	WSACleanup();
	return EXIT_SUCCESS;
}