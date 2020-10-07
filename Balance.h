#ifndef Balance_h
#define Balance_h

#include "Arduino.h"
#include <HX711.h>

class HX711;

class Balance
{
public:
	Balance(int pinDT = 7, int pinCLK = 6);
	~Balance();
	void Reset();       // mise à zéro de la balance
	void NormalMode();  // affiche : valeurs triées
	void DetailMode();  // affiche : valeurs brutes + triées
	float getMasse();   // retourne la masse enregistrée

private:
	int _pinDOUT = 7;
	int _pinCLOK = 6;
	int taille_echantillon;
	int median_echantillon;
	float facteur_de_calibration;
	float tableau[21];
	float resultat;
	bool detailActif;
	bool debut;
	void Remplissage(); // remplie le tableau
	void Affichage();   // affiche selon le mode activé
	HX711 *scale;
};

#endif