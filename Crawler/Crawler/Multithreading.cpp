#include "Multithreading.h"

std::ofstream file("filename");

std::vector<string> Multithreading::runNextUrl(string nextUrl) {

	if (checkDuplicate(nextUrl)) {
		std::cout << nextUrl << std::endl;
		WSADATA wsaData;
		Http http;
		Parser parser;
		std::vector<string> result;
		const int bufferLength = 1024;
		long fileSize = 0;
		char *memBuffer, *headerBuffer, *processedBuffer;

		memBuffer = headerBuffer = processedBuffer = NULL;

		if (WSAStartup(0x101, &wsaData) != 0) {
			std::cout << "WSAStartup Failed" << std::endl;
			exit(1);
		}

		// Processing the url to get the results
		auto wallClockStartTime = std::chrono::system_clock::now();
		memBuffer = http.readUrl(nextUrl, fileSize, &headerBuffer);
		std::cout << "Returned from readUrl" << std::endl;
		std::chrono::duration<double> wallClockDuration = (std::chrono::system_clock::now() - wallClockStartTime);
		saveData(nextUrl, wallClockDuration);
		result = parser.readHtml(memBuffer);

		// Cleanup
		delete(memBuffer);
		delete(processedBuffer);
		delete(headerBuffer);
		WSACleanup();

		return (result);
	}
	/*else {
		// No repeated crawling of url
		std::cout << "URL already crawled" << std::endl;
		return std::vector<string>();
	}*/

}

void Multithreading::startRunning(string nextUrl) {
	std::vector<string> currentUrlList = runNextUrl(nextUrl);

	// Will be empty if the url has already been crawled; the chain of that url stops
	for (string s : currentUrlList) {
		Sleep(250);
		runNextUrl(s);
	}
}

bool Multithreading::checkDuplicate(string newUrl) {
	vectorMutex.lock();

	// Check if the url has already been crawled
	bool isNotCrawled = (std::find(urlVector.begin(), urlVector.end(), newUrl) == urlVector.end());

	// Adding the crawled url to the list of crawled urls
	if (isNotCrawled) {
		urlVector.push_back(newUrl);
	}

	if (urlVector.size() > maxUrls) {
		exit(0);
	}

	vectorMutex.unlock();

	return isNotCrawled;
}

void Multithreading::saveData(string newUrl, std::chrono::duration<double> newTime) {

	fileMutex.lock();

	// Writing the url to file
	file.open("filename", std::ios::app);
	file << newUrl;
	file << " ";
	file << newTime.count();
	file << std::endl;
	file.close();

	fileMutex.unlock();

}

void Multithreading::initialize() {
	std::thread t[numThreads];
	std::vector<string> urlList;
	string nextUrl;

	// Take in the five URLs to start crawling with
	for (int i = 0; i < numThreads; i++) {
		std::cout << "Enter in the next URL to crawl" << std::endl;
		std::cin >> nextUrl;
		urlList.push_back(nextUrl);
	}

	// Launching several threads
	for (int i = 0; i < numThreads; i++) {
		t[i] = std::thread(&Multithreading::startRunning, this, urlList[i]);
		Sleep(5000);
	}

	//Join the threads with the main thread
	for (int i = 0; i < numThreads; i++) {
		if (t[i].joinable()) {
			t[i].join();
		}
	}

}