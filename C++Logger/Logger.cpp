#include "Logger.h"

bool Logger::__LOGGER_INIT__ = 0;

//
//
//

const char* Logger::DEFAULT_SUCCESS_PRE = "[ " ANSI_FORE_GREEN_BRIGHT "\xFB" ANSI_RESET " ] => ";
const char* Logger::DEFAULT_INFO_PRE = "[ " ANSI_FORE_BLUE_BRIGHT "i" ANSI_RESET " ] => ";;
const char* Logger::DEFAULT_WARN_PRE = "[ " ANSI_FORE_YELLOW "?" ANSI_RESET " ] => ";;
const char* Logger::DEFAULT_ERROR_PRE = "[ " ANSI_FORE_RED_BRIGHT "!" ANSI_RESET " ] => ";;

//
//
//

void Logger::logFormat(const char* format, va_list varArgs) {
	bool ansiFormFlag = 0;

	while (vsnprintf(this->formatBuffer, this->formatBufferLen, format, varArgs)
		>= this->formatBufferLen) {

		delete[] this->formatBuffer;
		this->formatBufferLen += 32;
		this->formatBuffer = new char[this->formatBufferLen];
	}

	fputs(this->formatBuffer, stdout);


	if (!this->fout.is_open())
		return;

	for (size_t charIter = 0; this->formatBuffer[charIter]; charIter++) {
		switch (this->formatBuffer[charIter])
		{
		case '\u001b':
			printf("GOT\n");
			ansiFormFlag = 1;
			continue;

		case 'm':
			if (ansiFormFlag) {
				ansiFormFlag = 0;
				continue;
			}

		default:
			if (!ansiFormFlag) {
				fout.put(this->formatBuffer[charIter]);
			}
		}
	}
}

void Logger::logCstr(const char* cstr) {
	if (this->fout.is_open())
		this->fout.write(cstr, strlen(cstr));
	fputs(cstr, stdout);
}

void Logger::logChar(const char c) {
	if (this->fout.is_open())
		this->fout.put(c);
	fputc(c, stdout);
}

void Logger::logWithPrefix(const char* prefix, const char* format, va_list varArgs) {
	this->logCstr(prefix);
	this->logFormat(format, varArgs);
	this->logChar('\n');
}

//
//
//

Logger::Logger(
	const files::path& logFilePath,
	const char* successPre,
	const char* infoPre,
	const char* warnPre,
	const char* errorPre
) {

#ifdef _WIN32
	if (!Logger::__LOGGER_INIT__) {
		HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD currentMode;
		
		if (GetConsoleMode(stdoutHandle, &currentMode)) {
			Logger::__LOGGER_INIT__ = (bool)SetConsoleMode(
				stdoutHandle,
				currentMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
			);
		}
	}
#endif

	if (successPre)	this->successPre = successPre;
	if (infoPre)	this->infoPre = infoPre;
	if (warnPre)	this->warnPre = warnPre;
	if (errorPre)	this->errorPre = errorPre;


	if (logFilePath.empty()) {
		return;
	}


	if (!files::exists(logFilePath) && logFilePath.has_parent_path()) {
		files::create_directories(logFilePath.parent_path());
	}

	this->fout.open(logFilePath, std::ios::app);
}

Logger::~Logger() {
	if (this->formatBuffer) {
		delete[] this->formatBuffer;
	}

	if (this->fout.is_open()) {
		this->fout.close();
	}
}

//
//
//

void Logger::logSuccess(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logWithPrefix(this->successPre, format, varArgs);
	va_end(varArgs);
}

void Logger::logInfo(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logWithPrefix(this->infoPre, format, varArgs);
	va_end(varArgs);
}

void Logger::logWarn(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logWithPrefix(this->warnPre, format, varArgs);
	va_end(varArgs);
}
void Logger::logError(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logWithPrefix(this->errorPre, format, varArgs);
	va_end(varArgs);
}