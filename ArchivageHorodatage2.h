#pragma once

#ifndef ArchivageHorodatage2_h
#define ArchivageHorodatage2_h

#include "Arduino.h"
#include <EEPROM.h>
#include <Wire.h>
#include "RTClib.h"
#include <String.h>


class EEPROM;
class RTClib;
class DateTime;

 struct bloc{
	float poids;
	float temps;
	int hmd;
	float gps_1;
	float gps_2;
	int battery;
	int annee;
	int mois;
	int jour;
	int heure;
	int minut;
};

struct data
{
	float seuil_Tmp_Min;
	float seuil_Tmp_Max;
	float seuil_Poids_Min;
	float seuil_Poids_Max;
	float seuil_GPS_1;
	float seuil_GPS_2;
	int seuil_Hmd_Min;
	int seuil_Hmd_Max;
	String telephone;
	String password;
	String URL;
};

class ArchivageHorodatage2
{
private : 
	static int debut, fin;
	static bool overfloat, flag1, flag2, ok;
	void Engine();
	unsigned int analogReadReference(void);
	uint8_t BROCHE_CAPTEUR_VIN;
	static RTC_DS1307 RTC;

public:
	static bloc	TableauDesMesures[20];
	static data Donnees_Seuil;
	static int A;
	void Affichage();
	bool Enregistrer(float A_poids, float A_temp, float A_hmd, float A_gps_1, float A_gps_2);			// main
	bool Flash_Enregistrer(float A_poids, float A_temp, float A_hmd, float A_gps_1, float A_gps_2);		// main
	static void Vider(bool Vide = false);	// Mathieu	//  Apres un enregistrement !!
	static void Sauvegarde_Seuil();
	static void Lecture_Seuil();
	// Apres un enregistrement !!
	// Mathieu : Ecrire (+ Enregister auto)
	// Mathieu : Lire

	ArchivageHorodatage2();
	~ArchivageHorodatage2();
};

#endif