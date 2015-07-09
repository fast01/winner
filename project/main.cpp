#include "main.h"

#define PROFILER 0
#define ENABLE_TEST 0

/** app **/
project::Application* g_app =0;

/** func decl **/
void on_int(int);

/** entry **/
int main(int argc, const char** argv){
	signal(SIGINT, on_int);
	g_app =CppNew<project::Application>();
	g_app->retain();
	{
#if ENABLE_TEST
		test::run_test();
#else
		g_app->run(argc, argv);
#endif
	}
	CppDelete(g_app);
	g_app =0;
	return 0;
}

/** signal **/
void on_int(int){
	DEBUG(__func__);
	if(g_app){
		g_app->quit();
	}
}
