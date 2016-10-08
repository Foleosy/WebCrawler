/*
* This file initiates the connection to a target server and writes the server reply to file
* Built for Windows, tested on Windows 8.1/Visual Studio 2015
* Author: Wong Ming Yang, A0108511U, a0108511@u.nus.edu
*/

// Used deprecated, insecure functions
#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

using std::string;

#pragma comment(lib,"ws2_32.lib")

class Http {
public:

	/*
	* Parses the input URL to get the target server
	*/
	void parseUrl(string targetUrl, string &serverName, string &filePath);

	/*
	* Managing the connection to the server
	*/	
	SOCKET connectToServer(char *nextServerName, WORD portNum);
	
	/*
	* Parsing through the server reply to obtain the length of the header
	* Optionally called, depending on the parser whether the header details are needed
	*/
	int getHeaderLength(char *content);
	
	/*
	* Processing the URL from start to end
	* Starts the connection, sends the request, obtains the reply and processes the reply
	*/
	char *readUrl(string nextUrl, long &bytesReturnedOut, char **headerOut);
};