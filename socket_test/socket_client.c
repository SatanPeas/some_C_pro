#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

#pragma comment (lib, "ws2_32.lib")

int main()
{
	//init dll
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//creat socket
	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//request from server
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	connect(sock, (SOCKADDR*) &sockAddr, sizeof(SOCKADDR));

	//accept data from server
	char szBuffer[MAXBYTE] = {0};
	recv(sock, szBuffer, MAXBYTE, NULL);

	printf("Message from server: %s\n", szBuffer);

	closesocket(sock);

	WSACleanup();

	system("pause");

	return 0;
}