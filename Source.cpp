#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"


int main(int argc, char* argv[])
{
	Plan obj;
	Robot bot;
	obj.Dezip();
	obj.Display();
	obj.IsolationIti();
	obj.Build();
	
	return 0;
}