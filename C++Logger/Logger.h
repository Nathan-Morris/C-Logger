#ifndef _LOGGER_
#define _LOGGER_

#ifdef _WIN32
#	include <Windows.h>
#endif

#include "Ansi.h"

#include <iostream>
#include <fstream>
#include <cstdarg>

#include <filesystem>

#pragma once

namespace files = std::filesystem;

class Logger {
private:
	static bool __LOGGER_INIT__;

public:
	static const char* DEFAULT_SUCCESS_PRE;
	static const char* DEFAULT_INFO_PRE;
	static const char* DEFAULT_WARN_PRE;
	static const char* DEFAULT_ERROR_PRE;

private:
	// log file output stream (append)
	std::ofstream fout;


	// string formatting buffer
	size_t formatBufferLen = 32;
	char* formatBuffer = new char[this->formatBufferLen];


	// standard logger prefixes
	const char* successPre = Logger::DEFAULT_SUCCESS_PRE;
	const char* infoPre = Logger::DEFAULT_INFO_PRE;
	const char* warnPre = Logger::DEFAULT_WARN_PRE;
	const char* errorPre = Logger::DEFAULT_ERROR_PRE;

private:
	void logFormat(const char* format, va_list varArgs);
	void logCstr(const char* cstr);
	void logChar(const char c);

	void logWithPrefix(const char* prefix, const char* format, va_list varArgs);

public:
	Logger(
		const files::path& logFilePath = "",
		const char* successPre = NULL,
		const char* infoPre = NULL,
		const char* warnPre = NULL,
		const char* errorPre = NULL
	);
	//Logger(const Logger& lref);
	~Logger();

	void logSuccess(const char* format, ...);
	void logInfo(const char* format, ...);
	void logWarn(const char* format, ...);
	void logError(const char* format, ...);


};

#endif