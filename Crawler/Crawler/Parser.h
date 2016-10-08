/*
* This file simply looks through the result and source for all links
* Built for Windows, tested on Windows 8.1/Visual Studio 2015
* Author: Wong Ming Yang, A0108511U, a0108511@u.nus.edu
*/

// Used deprecated, insecure functions
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include "Http.h"

using std::string;

#pragma comment(lib,"ws2_32.lib")

class Parser {
public:

	/*
	 * Reads in the buffer returned by the server
	 * and processes the reply to extract all the urls on the page
	 */
	std::vector<string> readHtml(char * savedBuffer);
};