/*
* This file manages the multithreading, as well as the invoking of parser and http
* Built for Windows, tested on Windows 8.1/Visual Studio 2015
* Author: Wong Ming Yang, A0108511U, a0108511@u.nus.edu
*/

#pragma once

#include <algorithm>
#include <ctime>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <mutex>
#include <unordered_map>
#include <string>
#include <thread>
#include <vector>
#include <time.h>
#include "Parser.h"

using std::string;


class Multithreading {
public:
	static const int numThreads = 5;
	static const int maxUrls = 100;

	std::mutex vectorMutex;
	std::mutex fileMutex;
	std::vector <std::string> urlVector;
	std::vector<std::chrono::duration<double>> urlTime;

	/*
	 * Processes the url
	 */
	std::vector<string> runNextUrl(string nextUrl);
	
	/* 
	 * Middleman function to call the the actual url processor
	 */
	void startRunning(string nextUrl);
	
	/*
	 * Function to handle the duplicate url checking
	 */
	bool checkDuplicate(string newUrl);

	/*
	 * Function to save the data to file
	 */
	void saveData(string newUrl, std::chrono::duration<double> newTime);

	/* 
	 * Initializes the multithreading 
	 */
	void initialize();
};
