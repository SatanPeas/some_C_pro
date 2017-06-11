#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32.lib") //load ws2_32.dll

int main()
{
	// init dll
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//creat socket
	SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//bind socket
	struct sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;	//user IPv4 addr
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);	//port
	bind(servSock, (SOCKADDR*) &sockAddr, sizeof(SOCKADDR));

	//begin listen
	listen(servSock, 20);

	SOCKADDR clientAddr;
	int nSize = sizeof(clientAddr);
	SOCKET clientSock = accept(servSock, (SOCKADDR*) &clientAddr, &nSize);

	//send data to client
	char *data = "hello world";
	send(clientSock, data, strlen(data) + sizeof(char), NULL);

	//close socket
	closesocket(servSock);
	closesocket(clientSock);

	//end dll
	WSACleanup();

	return 0;
}