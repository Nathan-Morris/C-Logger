#include "Logger.h"


int main() {
	Logger log(R"(C:/Users/natha/Desktop/output.log)");
	log.logSuccess("success");
	log.logWarning("Warning");
}