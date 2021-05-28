#include "Robot.h"

#ifdef _ROBOT_PRESENT
Robot::Robot(Com& com)
{
	//connect devices with their handles
	omni.setComId(com.id());
	odom.setComId(com.id());
	//reset odometry
	odom.set(0.0, 0.0, 0.0, true);
}
#endif

Robot::Robot()
{

}

void Robot::Rejoindre(double x, double y)
{
	unsigned int seq;
	double vel_x, vel_y;
	double cur_x, cur_y, cur_angle;

	cur_angle = 0;
#ifdef _ROBOT_PRESENT
	odom.set(0.0, 0.0, 0.0, true);
#endif
	cur_x = 0;
	cur_y = 0;

	while (pow(x - cur_x, 2) + pow(y - cur_y, 2) > pow(0.1, 2))
	{
		std::cout << pow(x - cur_x, 2) + pow(y - cur_y, 2) << pow(0.1, 2) << std::endl;

		if (pow(x - cur_x, 2) + pow(y - cur_y, 2) > 6)
		{
			vel_x = 0.2 * (x - cur_x);
			vel_y = 0.2 * (y - cur_y);
		}
		else if (pow(x - cur_x, 2) + pow(y - cur_y, 2) > 3)
		{
			vel_x = 0.4 * (x - cur_x);
			vel_y = 0.4 * (y - cur_y);
		}
		else if (pow(x - cur_x, 2) + pow(y - cur_y, 2) <= 3)
		{
			vel_x = 0.6 * (x - cur_x);
			vel_y = 0.6 * (y - cur_y);
		}
		else if (pow(x - cur_x, 2) + pow(y - cur_y, 2) <= 1)
		{
			vel_x = x - cur_x;
			vel_y = y - cur_y;
		}

#ifdef _ROBOT_PRESENT
		omni.setVelocity(vel_x, vel_y, 0);
		odom.readings(&cur_x, &cur_y, &cur_angle, &seq);

#endif
	}
}

Plan::Plan()
{

}

Plan::~Plan()
{

}

void Plan::AfectDes(int y, int x)
{
	des.x = x;
	des.y = y;
}

void Plan::AfectIti(int y, int x)
{
	iti.x = x;
	iti.y = y;
}

void Plan::Dezip()
{
	int temp[3] = { 0 };
	std::fstream flux;
	flux.open("pos.txt");
	Plan obj;

	if (flux.is_open())
	{
		flux.seekg(0, flux.end);
		int lenght = flux.tellg();								//nb éléments du fichier * 8 (car octets)
		flux.seekg(0, flux.beg);

		for (int i = 0; i <= (lenght / 8); i++)
		{
			flux >> temp[0];
			flux.seekg(1, flux.cur);
			flux >> temp[1];
			flux.seekg(1, flux.cur);
			flux >> temp[2];
			flux.seekg(1, flux.cur);

			if (temp[0] == 30)
			{
				dim[0] = temp[2];
				dim[0] /= LARGEUR;
			}
			else if (temp[1] == 30)
			{
				dim[1] = temp[2];
				dim[1] /= LONGUEUR;
			}
			else
			{
				valeurs[temp[0]][temp[1]] = temp[2];
			}
		}
	}
	else
	{
		std::cout << "ERREUR ! Impossible d'ouvrir le fichier ! " << std::endl;
	}
}

void Plan::Display()
{
	for (int i = 0; i < LONGUEUR; i++)
	{
		int j = 0;
		do
		{
			printf("%d ", valeurs[i][j]);
			j++;
		} while (j < LARGEUR);
		std::cout << std::endl;
	}
}

void Plan::IsolationIti()
{
	Point old = { 0 };
	Point iti = { 0 };
	for (int i = 0; i < MAX; i++)
	{
		int j = 0;
		while (j < MAX)
		{
			if (valeurs[i][j] == 2)
			{
				iti.x = j;
				iti.y = i;
				if (iti.x == old.x)
				{
					isoled[nbPt] = iti;
					nbPt++;
				}
				else if (iti.y == old.y)
				{
					isoled[nbPt] = iti;
					nbPt++;
				}
			}
			old.x = iti.x;
			old.y = iti.y;
			j++;
		}
	}
}

void Plan::Build()
{
	Point temp = { 0 };
	Point ftemp = { 0 };
	int j = -1;
	int indice = 0;
	int oldindice = 0;

	for (int i = 0; i < nbPt; i++)
	{
		temp = isoled[i];
		ftemp = isoled[i + 1];
		oldindice = indice;

		if (ftemp.x >= 0 || ftemp.y >= 0)
		{
			if (ftemp.x == (temp.x + 1))
			{
				indice = 1;
			}
			else if (ftemp.y == (temp.y + 1))
			{
				indice = 2;
			}
		}

		if (indice != oldindice)
		{
			if (j >= 0)
				nodes[j] = temp;
			j++;
		}
		else if (ftemp.x < 0 || ftemp.y < 0)
		{
			nodes[j] = temp;
		}
	}
}

void Plan::Itineraire()
{
	Robot bot;
	int i = 0;
	Point temp = { 0 };
	Point old = { 0 };

	for(int i = 0; i < nbPt ; i++)
	{
		old = temp;
		temp = nodes[i];
		if (temp.x >= 0 || temp.y >= 0)
		{
			temp.x *= dim[0];
			temp.y *= dim[1];
		}
		else
			break;

		if (temp.x != old.x)
		{
#ifdef _ROBOT_PRESENT
			bot.Rejoindre(temp.x, 0);
#endif
		}
		else if (temp.y != old.y)
		{
#ifdef _ROBOT_PRESENT
			bot.Rejoindre(0, temp.y);
#endif
		}
	}
}

void Plan::Calcul()
{
	
}
