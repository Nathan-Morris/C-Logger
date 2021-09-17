#include "Logger.h"


int main() {
	int i = 10;
	Logger log(&i, R"(C:/Users/natha/Desktop/output.log)");
	log.addOutputStream(fopen(R"(C:/Users/natha/Desktop/re.txt)", "w"));
	log.logSuccess("success");
	log.logWarning("Warning");
}