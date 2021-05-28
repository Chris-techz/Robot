#ifndef ROBOT_H
#define Robot_H

#define LARGEUR 25
#define LONGUEUR 25
#define MAX 25
//#define _ROBOT_PRESENT

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include "rec/robotino/api2/Odometry.h"
#include "rec/robotino/api2/Com.h"
#include "rec/robotino/api2/OmniDrive.h"

using namespace rec::robotino::api2;

struct Point			//Structure de type point
{
	double x, y;
};

class Robot
{
private:
#ifdef _ROBOT_PRESENT
	Odometry odom;				//objet pour controler l'odométrie 
	OmniDrive omni;				//objet pour controler les moteurs
#endif
public:
#ifdef _ROBOT_PRESENT
	Robot(Com& com);					//constructeur
#endif
	Robot();
	void Rejoindre(double x, double y);		//Gère le fonctionnement moteur, l'odométrie
};

class Plan
{
	private:
		Point des;
		Point iti;
		Point nodes[MAX];
		Point isoled[MAX * MAX];
		int nbPt = 0;
		int valeurs[LONGUEUR][LARGEUR] = { 0 };
		double dim[2] = { 0 };

	public:
		Plan();
		~Plan();
		void AfectDes(int x, int y);
		void AfectIti(int x, int y);
		void Dezip();
		void Display();
		void IsolationIti();
		void Itineraire();
		void Build();
		void Calcul();
};


#endif ROBOT_H