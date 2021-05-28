#define _CRT_SECURE_NO_WARNINGS
#include "Robot.h"


int main(int argc, char* argv[])
{
	Plan obj;
#ifdef _ROBOT_PRESENT
	Com com;
	Robot bot(com);
	if (argc == 2)
	{
		com.setAddress(argv[1]);
		std::cout << "Tentative de connection a l'adresse " << argv[1] << " ... " << std::endl;
	}
	else
	{
		std::cout << "Pas d'adresse renseignee ! Renseignez une adresse cible ! " << std::endl;
		system("PAUSE");
		return 0;
	}

	try
	{
		com.connectToServer(true);
		std::cout << "connecte a " << argv[1] << " ! " << std::endl;
	}
	catch (...)
	{
		std::cout << "Connection impossible! Arret du programme ... " << std::endl;
		return 0;
	}
#else
	Robot bot;
#endif

	obj.Dezip();
	obj.Display();
	obj.IsolationIti();
	obj.Build();
	obj.Itineraire();
	
	return 0;
}