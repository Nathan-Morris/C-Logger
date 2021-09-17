#include <thread>
#include <conio.h>
#include "Logger.h"

using namespace std;

class DER : public Logger {
public:
	DER() : Logger(this) {
		
	}
};

static void seperateThread(DER& dref, bool& flag) {
	while (flag) {
		dref.logWarning("This Is A Warning, %u", 999);
	}
}

static void seperateThread2(DER& dref, bool& flag) {
	while (flag) {
		dref.logSuccess("This Is SUCCESS, %p", &flag);
	}
}

int main() {

	DER d;
	bool b = true;

	thread t(seperateThread, std::ref(d), std::ref(b));
	thread t2(seperateThread2, std::ref(d), std::ref(b));

	while (!_kbhit()) {
		d.logInformation("This Is Information, %u", 10);
	}

	b = false;

	t.join();
	t2.join();
}