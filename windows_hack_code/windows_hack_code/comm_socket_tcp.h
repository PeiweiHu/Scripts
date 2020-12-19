#pragma once

#pragma comment(lib, "Ws2_32.lib")

#include "stdafx.h"
#include <Winsock2.h>

// --------- for server ----------
INT ServerInit(char * ip, int port);
void SendMsg(char * buf);
void ReadMsg();
// --------- for client -----------
int ClientInit(char * ip, int port);
void ClientReadMsg();
void ClientSendMsg(char * buf);
