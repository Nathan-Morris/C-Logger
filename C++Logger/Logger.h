#ifndef _LOGGER_
#define _LOGGER_

#ifdef _WIN32
#	include <Windows.h>
#endif

#include "Ansi.h"

#include <iostream>
#include <fstream>
#include <typeinfo>
#include <cstdarg>

#include <filesystem>

#pragma once

namespace files = std::filesystem;


typedef struct {
	const char* successPrefix;
	const char* infoPrefix;
	const char* warningPrefix;
	const char* errorPrefix;
} LoggerPrefixes, *pLoggerPrefixes;

// [BaseType|MemAddr] - [Time] - [Prefix] : message

typedef enum : unsigned char {
	// base type name
	LOGGER_VERBOSE_TYPE_NAME = 0x1,

	// base instance address
	LOGGER_VERBOSE_BASE_ADDR = 0x2,

	// time of logged output
	LOGGER_VERBOSE_TIME = 0x4
} LoggerVerboseFlag;

class Logger {
private:
	static bool __INIT_LOGGER__();
	static const bool __LOGGER_INITD__;

	static const LoggerPrefixes DEFAULT_PREFIXES;

private:
	std::ofstream _logFileAppendStream;

	const char* _baseTypeName = NULL;

	const LoggerPrefixes* _prefixes;

	unsigned char _verboseOpts;


public:
	Logger(
		const files::path& logFilePath = "",
		unsigned char verboseOpts = LOGGER_VERBOSE_TYPE_NAME | LOGGER_VERBOSE_BASE_ADDR | LOGGER_VERBOSE_TIME,
		const LoggerPrefixes& logPrefixes = Logger::DEFAULT_PREFIXES
	);

	Logger(
		const files::path& logFilePath = "",
		const LoggerPrefixes& logPrefixes = Logger::DEFAULT_PREFIXES
	);

	template<typename BaseType>
	Logger(
		const files::path& logFilePath = "",
		unsigned char verboseOpts = LOGGER_VERBOSE_TYPE_NAME | LOGGER_VERBOSE_BASE_ADDR | LOGGER_VERBOSE_TIME,
		const LoggerPrefixes& logPrefixes = Logger::DEFAULT_PREFIXES
	) : Logger(logFilePath, verboseOpts, logPrefixes) { 
		this->baseTypeName = typeid(BaseType).name; 
	}

	Logger(const Logger& logRef);

	~Logger();


};

#endif