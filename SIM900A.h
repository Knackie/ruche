#include <String.h>
#include <math.h>
#include "ArchivageHorodatage2.h"
#include "SoftwareSerial.h"
#pragma once

class SIM900A
{

public:
	SIM900A(int a, int b);
	
	float Distance(float lat_a_degre, float  lon_a_degre, float  lat_b_degre, float  lon_b_degre);
	void verification(); // pour verifier avec les seuils
	void alerteHumidite(int humidite); // en cas de mauvaise humidité, envoie une alerte
	void alerteTemperature(float temperature); // en cae de mauvaise temperature, envoie une alerte
	void alertePoids(float poids); //en cas de poids mauvais, envoie une alerte
	void alerteGPS(); // en cas de changement de position gps, envoie une alerte
	void alerteBatterie(int batterie); // en cas de batterie faible, envoie une alerte
	boolean envoiDonnees(); // fonction pour contacter le site
	boolean lectureSeuils();
	String getTel(String initiale);
	String getTempMin(String initiale);
	String getTempMax(String initiale);
	String gethumMin(String initiale);
	String gethumMax(String initiale);
	String getPoidsMin(String initiale);
	String getPoidsMax(String initiale);
	String getLat(String initiale);
	String getLong(String initiale);
	String getPasswd(String initiale);
	String getURL(String initiale);
	String sendSMS(); // fonction pour envoyer un sms. 
	String lectureSerial(unsigned tempo = 100);
	boolean Section(String longi);
	void ReceptionSMS();
	float *PositionGPS();
	~SIM900A();
private:
	int pos;
	float latit, longit;
	SoftwareSerial *SIM;
	
};


