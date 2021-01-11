#include "stdafx.h"
#include "comm_http_get.h"

#include <iostream>
#include <winsock2.h>
#include <string>
using namespace std;

//link libwsock32.a


unsigned long WinsockStartGet()
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

void error_exit_get(string text)
{
	cout << text;
	WSACleanup();
	exit(EXIT_FAILURE);
}

//upload file to **192.168.179.128/upload.php**, server can access the uploaded file with identifier **upfile**

int comm_http_get_test()
{
	SOCKET sock;
	struct sockaddr_in addr;
	unsigned long win = 0;
	int con = 0, gr = 0, send_r = 0, rec = 0;
	char header[2048], puffer[2048] = { 0 };
	//char header[MAX_PATH], puffer[MAX_PATH];
	//string to_send = "hello world";
	//string name = "test99.txt";

	win = WinsockStartGet();
	if (win == 1 || win == 2)
		error_exit_get("Error WinsockStart()");

	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	//addr.sin_addr.s_addr = win;
	addr.sin_addr.s_addr = inet_addr("192.168.179.129");

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock<0)
		error_exit_get("Error socket()");

	//gr = (to_send.size() + name.size() + 287);

	//GET / HTTP/1.1
	//Host: www.xxxxxxxx.com
	//Proxy-Connection: keep-alive
	//Cache-Control: max-age=0
	//DNT: 1
	//Upgrade-Insecure-Requests: 1
	//User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36
	//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*; q = 0.8, application / signed - exchange; v = b3; q = 0.9
	//Accept - Encoding: gzip, deflate
	//Accept - Language : zh - CN, zh; q = 0.9, nb; q = 0.8, en; q = 0.7
	//If - None - Match: "5526-5b896e8e83562-gzip"
	//If - Modified - Since : Mon, 11 Jan 2021 02 : 46 : 15 GMT

	sprintf(header, "GET %s HTTP/1.1\r\n", "/cc.html");
	//sprintf(header, "%sHost: www.example.com\r\n", header);
	sprintf(header, "%sHost: 192.168.179.129\r\n", header);
	//sprintf(header, "%sConnection: Keep-Alive\r\n", header);
	sprintf(header, "%sUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36\r\n", header);
	sprintf(header, "%s\r\n", header);
	//sprintf(header, "%sContent-Type: multipart/form-data; boundary=---------------------------90721038027008\r\n", header);
	//sprintf(header, "%sContent-Length: %d\r\n", header, gr);
	//sprintf(header, "%s\r\n", header);
	//sprintf(header, "%s-----------------------------90721038027008\r\n", header);
	//sprintf(header, "%sContent-Disposition: form-data; name=\"upfile\"; filename=\"%s\"\r\n", header, name.c_str());
	//sprintf(header, "%sContent-Type: text/plain\r\n", header);
	//sprintf(header, "%s\r\n", header);
	//sprintf(header, "%s%s\r\n", header, to_send.c_str());
	//sprintf(header, "%s-----------------------------90721038027008\r\n", header);
	//sprintf(header, "%sContent-Disposition: form-data; name=\"post\"\r\n", header);
	//sprintf(header, "%s\r\n", header);
	//sprintf(header, "%supload\r\n\r\n", header);
	//sprintf(header, "%s-----------------------------90721038027008--\r\n\r\n\0", header);

	con = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
	if (con < 0)
		error_exit_get("Error connect()");

	if (send_r = send(sock, header, strlen(header), 0)<0)
		error_exit_get("Error send()");

	while (rec = recv(sock, puffer, 2048, 0))
	{
		if (rec == 0)
			error_exit_get("Server quit");

		printf("%s", puffer);
	}

	closesocket(sock);
	WSACleanup();
	return EXIT_SUCCESS;
}