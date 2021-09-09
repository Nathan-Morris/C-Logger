#include "logger.h"

const bool Logger::__LOGGER_IS_INITD__ = Logger::__LOGGER_INIT__();

const LoggerPrefixes Logger::DEFAULT_LOGGER_PREFIXES = {
	"[ " ANSI_FORE_GREEN_BRIGHT "\xFB" ANSI_RESET " ] : ",
	"[ " ANSI_FORE_BLUE_BRIGHT "i" ANSI_RESET " ] : ",
	"[ " ANSI_FORE_YELLOW "?" ANSI_RESET " ] : ",
	"[ " ANSI_FORE_RED_BRIGHT "!" ANSI_RESET " ] : ",
	"[" ANSI_FORE_MAGENTA_BRIGHT "%s" ANSI_RESET "@" ANSI_FORE_CYAN_BRIGHT "0x%p" ANSI_RESET "] - "
};

//
// (De)Constructors
//

Logger::Logger(
	const char* logFilePath,
	const LoggerPrefixes* prefixes
) {
	if (logFilePath) {
		this->_logFilePath = files::path(logFilePath);

		if (!files::exists(this->_logFilePath) && this->_logFilePath.has_parent_path()) {
			files::create_directories(this->_logFilePath.parent_path());
		}

		this->_logFileAppendStream = std::ofstream(this->_logFilePath, std::ios::app);
	}

	if (prefixes) {
		this->_prefixes = *prefixes;
	}
}

Logger::Logger(
	const char* logFilePath
) : Logger(logFilePath, NULL) {

}

Logger::Logger() : Logger(NULL, NULL) { }

Logger::~Logger() {
	if (this->_logFileAppendStream.is_open()) {
		this->_logFileAppendStream.close();
	}
}

//
//
//

void Logger::logChar(const char c) {
	if (this->_logFileAppendStream.is_open()) {
		this->_logFileAppendStream.put(c);
	}

	fputc(c, stdout);
}

void Logger::logString(const char* cstr) {
	bool ansiStyleFlag;

	if (this->_logFileAppendStream.is_open()) {
		ansiStyleFlag = false;

		for (size_t i = 0; cstr[i]; i++) {
			switch (cstr[i])
			{
			case '\u001b':
				ansiStyleFlag = true;
				continue;

			case 'm':
				if (ansiStyleFlag) {
					ansiStyleFlag = false;
					continue;
				}

			default:
				if (!ansiStyleFlag) {
					this->_logFileAppendStream.put(cstr[i]);
				}
			}
		}
	}

	fputs(cstr, stdout);
}

void Logger::logVarFormat(const char* format, va_list varArgs) {
	int writtenLen;
	
	while ((writtenLen = vsnprintf(this->_formatBuffer.data(), this->_formatBuffer.size(), format, varArgs))
		>= (int)this->_formatBuffer.size()) {
		this->_formatBuffer.resize((size_t)writtenLen + 1);
	}

	this->logString(this->_formatBuffer.data());
}

void Logger::logFormat(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logVarFormat(format, varArgs);
	va_end(varArgs);
}

void Logger::logPrefix(const char* prefix, const char* format, va_list varArgs) {
	if (this->_baseTypeName) {
		this->logFormat(this->_prefixes.instanceInfoPrefix, this->_baseTypeName, (void*)this);
	}
	this->logString(prefix);
	this->logVarFormat(format, varArgs);
	this->logChar('\n');
}


//
//
//

void Logger::logSuccess(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logPrefix(this->DEFAULT_LOGGER_PREFIXES.successPrefix, format, varArgs);
	va_end(varArgs);
}

void Logger::logInformation(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logPrefix(this->DEFAULT_LOGGER_PREFIXES.informationPrefix, format, varArgs);
	va_end(varArgs);
}
void Logger::logWarning(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logPrefix(this->DEFAULT_LOGGER_PREFIXES.warningPrefix, format, varArgs);
	va_end(varArgs);
}

void Logger::logError(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->logPrefix(this->DEFAULT_LOGGER_PREFIXES.errorPrefix, format, varArgs);
	va_end(varArgs);
}