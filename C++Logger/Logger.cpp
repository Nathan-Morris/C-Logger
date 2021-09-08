#include "logger.h"

bool Logger::__INIT_LOGGER__() {
#ifdef _WIN32
	HANDLE consoleOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD consoleCurrentMode;

	if (
		!GetConsoleMode(consoleOutHandle, &consoleCurrentMode) || !SetConsoleMode(consoleOutHandle, consoleCurrentMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING)
		) {
		throw std::runtime_error("Unable To Initialize Ansi Support In Windows Terminal");
	}
#endif

	return 1;
}

const bool Logger::__LOGGER_INITD__ = Logger::__INIT_LOGGER__();

const LoggerPrefixes Logger::DEFAULT_PREFIXES = {
	"[ " ANSI_FORE_GREEN_BRIGHT "\xFB" ANSI_RESET " ] : ",
	"[ " ANSI_FORE_BLUE_BRIGHT "i" ANSI_RESET " ] : ",
	"[ " ANSI_FORE_YELLOW_BRIGHT "?" ANSI_RESET " ] : ",
	"[ " ANSI_FORE_RED_BRIGHT "!" ANSI_RESET " ] : ",
};

//
// Constructors
//

Logger::Logger(const files::path& logFilePath, unsigned char verboseOpts, const LoggerPrefixes& logPrefixes) {
	if (!logFilePath.empty()) {
		if (!files::exists(logFilePath) && logFilePath.has_parent_path()) {
			files::create_directories(logFilePath.parent_path());
		}

		this->_logFileAppendStream = std::ofstream(logFilePath, std::ios::app);
	}

	this->_verboseOpts = verboseOpts;
	this->_prefixes = &logPrefixes;
}

Logger::Logger(const files::path& logFilePath = "", const LoggerPrefixes& logPrefixes = Logger::DEFAULT_PREFIXES)
	: Logger("", 0, Logger::DEFAULT_PREFIXES) { }