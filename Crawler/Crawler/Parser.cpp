#include "Parser.h"

std::vector<string> Parser::readHtml(char * memBuffer) {
	std::regex extractUrl(R"~(href=["']([^"']*)["'])~");
	string savedBuffer;
	int bufferLength;
	std::vector<string> urlList;

	if (memBuffer != NULL) {
		bufferLength = strlen(memBuffer);
		savedBuffer.assign(memBuffer, bufferLength);
	}

	auto wordsBegin = std::sregex_iterator(savedBuffer.begin(), savedBuffer.end(), extractUrl);
	auto wordsEnd = std::sregex_iterator();

	for (std::sregex_iterator i = wordsBegin; i != wordsEnd; ++i) {
		std::smatch match = *i;
		string match_str = match.str();
		if ((match_str.find("http://") != string::npos) || (match_str.find("https://") != string::npos)) {
			match_str.erase(0, 6);
			match_str.erase(match_str.end() - 1, match_str.end());
			urlList.push_back(match_str);
		}
	}

	return (urlList);
}

// For Testing
/*int main() {
	WSADATA wsaData;
	Http http;
	std::vector<string> result;
	const int bufferLength = 1024;
	char *nextUrl = "http://www.neopets.com";
	long fileSize = 0;
	char *memBuffer, *headerBuffer, *processedBuffer;

	memBuffer = headerBuffer = processedBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0) {
		return -1;
	}

	memBuffer = http.readUrl(nextUrl, fileSize, &headerBuffer);
	std::cout << "Returned from readUrl" << std::endl;

	result = readHtml(memBuffer);

	// Cleanup
	delete(memBuffer);
	delete(processedBuffer);
	delete(headerBuffer);
	WSACleanup();
	return 0;
}*/