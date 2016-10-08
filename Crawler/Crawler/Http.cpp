#include "Http.h"

void Http::parseUrl(string url, string &serverName, string &filePath)
{
	string::size_type urlCheck;

	if (url.substr(0, 7) == "http://") {
		url.erase(0, 7);
	}

	if (url.substr(0, 8) == "https://") {
		url.erase(0, 8);
	}

	urlCheck = url.find('/');

	if (urlCheck != string::npos) {
		serverName = url.substr(0, urlCheck);
		filePath = url.substr(urlCheck);
		urlCheck = filePath.rfind('/');
	}
	else {
		serverName = url;
		filePath = "/";
	}
}

SOCKET Http::connectToServer(char *nextServerName, WORD portNum)
{
	struct hostent *he;
	unsigned int addr = 0;
	struct sockaddr_in server;
	SOCKET s;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		return NULL;
	}

	if (inet_addr(nextServerName) == INADDR_NONE) {
		he = gethostbyname(nextServerName);
	}
	else {
		addr = inet_addr(nextServerName);
		he = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (he == NULL) {
		closesocket(s);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)he->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);

	if (connect(s, (struct sockaddr*)&server, sizeof(server))) {
		closesocket(s);
		return NULL;
	}

	return s;
}

int Http::getHeaderLength(char *content)
{
	const char *searchStringOne = "\r\n\r\n", *searchStringTwo = "\n\r\n\r";
	char *findPos;
	int offSet = -1;

	findPos = strstr(content, searchStringOne);

	if (findPos != NULL) {
		offSet = findPos - content;
		offSet += strlen(searchStringOne);
	}
	else {
		findPos = strstr(content, searchStringTwo);
		if (findPos != NULL) {
			offSet = findPos - content;
			offSet += strlen(searchStringTwo);
		}
	}
	return offSet;
}

char * Http::readUrl(string nextUrl, long &bytesReturnedOut, char **headerOut)
{
	const int bufferSize = 512;
	char readBuffer[bufferSize], sendBuffer[bufferSize], tempBuffer[bufferSize];
	char *tempResult = NULL, *result;
	SOCKET s;
	string server, filePath;
	long totalBytesRead = 0, thisReadSize = 0, headerLength = 0, contentLength = 0;
	char *temp;

	parseUrl(nextUrl, server, filePath);

	// Initiate Connection
	s = connectToServer((char*)server.c_str(), 80);

	// Send GET request
	sprintf(tempBuffer, "GET %s HTTP/1.1", filePath.c_str());
	strcpy(sendBuffer, tempBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tempBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tempBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(s, sendBuffer, strlen(sendBuffer), 0);
	std::cout << "GET request successfully sent" << std::endl;

	// Receive until the peer closes the connection
	while (true) {
		memset(readBuffer, 0, bufferSize);
		thisReadSize = recv(s, readBuffer, bufferSize, 0);

		if (thisReadSize <= 0) {
			break;
		}

		tempResult = (char*)realloc(tempResult, thisReadSize + totalBytesRead);

		memcpy(tempResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	// Remove unneeded header details
	headerLength = getHeaderLength(tempResult);
	
	// Process the result
	contentLength = totalBytesRead - headerLength;
	result = new char[contentLength + 1];
	memcpy(result, tempResult + headerLength, contentLength);
	result[contentLength] = 0x0;
	temp = new char[headerLength + 1];
	strncpy(temp, tempResult, headerLength);
	temp[headerLength] = NULL;
	*headerOut = temp;
	bytesReturnedOut = contentLength;

	delete(tempResult);
	closesocket(s);
	return (result);
}

// For Testing
/*int main() {
	WSADATA wsaData;
	Http http;
	const int bufferLength = 1024;
	char *nextUrl = "http://www.neopets.com";
	long fileSize = 0;
	char *memBuffer, *headerBuffer;
	FILE *fp;

	memBuffer = headerBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0) {
		return -1;
	}

	memBuffer = http.readUrl(nextUrl, fileSize, &headerBuffer);
	std::cout << "Returned from readUrl" << std::endl;

	// Writing the results to file for next stage of parsing
	if (fileSize != 0) {
		std::cout << "Writing data" << std::endl;
		fp = fopen("http.result", "wb");
		fwrite(memBuffer, 1, fileSize, fp);
		fclose(fp);
	}

	// Cleanup
	delete(memBuffer);
	delete(headerBuffer);
	WSACleanup();
	return 0;
}*/