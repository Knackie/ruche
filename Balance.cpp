#include "Arduino.h"
#include "Balance.h"
#include <ArduinoSort.h>

using namespace ArduinoSort;

Balance::Balance(int pinDT, int pinCLK)
{
	_pinDOUT = pinDT;
	_pinCLOK = pinCLK;
		
	facteur_de_calibration = 22951;
	taille_echantillon = 21;
	median_echantillon = 10;

	scale = new HX711(_pinDOUT, _pinCLOK);
	detailActif = false;
	debut = true;
}

void Balance::Reset()
{
	if (debut)
	{
		scale->set_scale();
		scale->tare();
		scale->set_scale(facteur_de_calibration);
		debut = false;
	}
	else
	{
		scale->tare();
		Serial.print(F("Calibration en cours..."));
		Serial.println();
	}
}

void Balance::Affichage()
{
	if (detailActif)
	{
		for (int i = 0; i < taille_echantillon; i++)
		{
			Serial.print(F("Valeur n°"));
			Serial.print(i);
			Serial.print(F(" : "));
			Serial.print(tableau[i], 1);
			Serial.print(F(" Kg"));
			Serial.println();
			delay(500);
		}
		Serial.print(F("+---------------------------------------------------------------+"));
		Serial.println();
	}
	else
	{
		Serial.print(F("|    Poids : "));
		Serial.print(tableau[median_echantillon], 1);
		Serial.print(F(" Kg"));
	}
}

void Balance::Remplissage()
{
	for (int i = 0; i < taille_echantillon; i++)
	{
		tableau[i] = scale->get_units(10);
	//	tableau[i] *= -1;
		if (tableau[i] < 0) tableau[i] -= tableau[i];
	}

	if (detailActif)
	{
		Affichage();
		detailActif = false;
	}
	sortArray(tableau, taille_echantillon);
	delay(1000);
}

void Balance::NormalMode()
{
	detailActif = false;
	Remplissage();
	Affichage();
}

void Balance::DetailMode()
{
	detailActif = true;
	Remplissage();
	Affichage();
}

float Balance::getMasse()
{
	detailActif = false;
	Remplissage();
	resultat = tableau[median_echantillon];
	
	return resultat;
}

Balance::~Balance()
{
	delete scale;
}