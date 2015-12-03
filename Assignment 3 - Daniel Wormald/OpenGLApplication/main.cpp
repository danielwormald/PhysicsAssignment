#include "MyApplication.h"

int main()
{
	MyApplication* theApp = new MyApplication();
	theApp->Run();

	delete theApp;
	return 0;
}
