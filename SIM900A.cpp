#include "SIM900A.h"
# define M_PI 3.14159265358979323846  //Pi
#define R 6373.0 // rayon de la terre

SIM900A::SIM900A(int porta, int portb)
{
	SIM = new SoftwareSerial(porta, portb);
	SIM->begin(19200);
}

String SIM900A::lectureSerial(unsigned tempo)
{

	String result = SIM->readString();
	Serial.println(result);
	delay(tempo);
	return result;
}

float SIM900A::Distance(float lat_a, float  lon_a, float  lat_b, float  lon_b)
{

	float distance; 
	distance = (acos(cos(lat_a*M_PI / 180)*cos(lat_b*M_PI / 180)*cos((lon_b*M_PI / 180)
		- (lon_a*M_PI / 180)) + sin(lat_a*M_PI / 180)*sin(lat_b*M_PI / 180))*R);
	return distance; //retour de la distance, en KM
}

void SIM900A::verification()
{
	if (ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].battery < 20)
	{
		this->alerteBatterie(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].battery);
	}
	delay(5000);
	if (ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].temps < ArchivageHorodatage2::Donnees_Seuil.seuil_Tmp_Min
		|| ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].temps> ArchivageHorodatage2::Donnees_Seuil.seuil_Tmp_Max);
	{
		this->alerteTemperature(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].temps);
	}
	delay(5000);
	if (ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].poids < ArchivageHorodatage2::Donnees_Seuil.seuil_Poids_Min 
		|| ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].poids > ArchivageHorodatage2::Donnees_Seuil.seuil_Poids_Max);
	{
		this->alertePoids(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].poids);
	}
	delay(5000);
	if (ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].hmd < ArchivageHorodatage2::Donnees_Seuil.seuil_Hmd_Min
		|| ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].hmd  > ArchivageHorodatage2::Donnees_Seuil.seuil_Hmd_Max);
	{
		this->alerteHumidite(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].hmd);
	}
	delay(5000);
	if (Distance(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].gps_1,
		ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].gps_2,
		ArchivageHorodatage2::Donnees_Seuil.seuil_GPS_1, ArchivageHorodatage2::Donnees_Seuil.seuil_GPS_2) > 0.5)
	{
		this->alerteGPS();
	}
	delay(5000);
}

void SIM900A::alerteHumidite(int humidite)
{
	String retour;
	SIM->println(this->sendSMS());
	retour = lectureSerial(100);
	SIM->print(F("alerte humidite : "));
	retour = lectureSerial(100);
	SIM->print(humidite);
	retour = lectureSerial(100);
	SIM->write((char)26);//ctrl+z*/
}
void SIM900A::alerteTemperature(float temperature)
{
	String retour;
	SIM->println(this->sendSMS());
	retour = lectureSerial(100);
	SIM->print(F("alerte temperature : "));
	retour = lectureSerial(100);
	SIM->print(temperature);
	SIM->write((char)26);//ctrl+z*/
}
void SIM900A::alertePoids(float poids)
{

	String retour;
	SIM->println(this->sendSMS());
	retour = lectureSerial(100);
	SIM->print(F("alerte poids : "));
	retour = lectureSerial(100);
	SIM->print(poids);
	SIM->write((char)26);//ctrl+z*/
}
void SIM900A::alerteGPS()
{
	String retour;
	SIM->println(this->sendSMS());
	retour = lectureSerial(100);
	SIM->println(F("la ruche a subi un deplacement anormal"));
	retour = lectureSerial(100);
	SIM->write((char)26);//ctrl+z*/
}
void SIM900A::alerteBatterie(int batterie)
{
	String retour;
	SIM->println(this->sendSMS());
	retour = lectureSerial(100);
	SIM->print(F("Alerte batterie : "));
	retour = lectureSerial(100);
	SIM->print(batterie);
	retour = lectureSerial(100);
	SIM->write((char)26);//ctrl+z*/

}
boolean SIM900A::envoiDonnees()
{
	String retour;
	String URL;
	URL= F("AT+HTTPPARA=\"URL\",");
	URL += ArchivageHorodatage2::Donnees_Seuil.URL;
	URL += F("/index/");
	URL += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].temps;
	URL += F("/");
	URL += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].hmd;
	URL += F("/");
	URL += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].poids;
	URL += F("/");
	URL += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].gps_1;
	URL += F("/");
	URL += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].gps_2;
	URL += F("/");
	URL += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].battery;
	URL += F("\"");
	SIM->println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));
	retour=lectureSerial();
	SIM->println(F("AT+SAPBR=3,1,\"APN\",\"free\""));
	retour = lectureSerial();
	SIM->println(F("AT+SAPBR=1,1"));
	retour = lectureSerial();
	SIM->println(F("AT+HTTPINIT"));
	retour = lectureSerial();
	SIM->println(F("AT+HTTPPARA=\"CID\",1"));
	retour = lectureSerial();
	SIM->println(URL);
	retour = lectureSerial();
	SIM->println(F("AT+HTTPACTION=0"));
	retour = lectureSerial(2000);
	int errpos = retour.indexOf(F("ERROR"));
	if (errpos != -1)
	{
		SIM->println(F("AT+HTTPREAD"));
		retour = lectureSerial(2000);
		return true;
	}
	return false;
}
String SIM900A::sendSMS()
{
	String sms = F("AT+CMGS=\"");
	sms += ArchivageHorodatage2::Donnees_Seuil.telephone;
	sms += "\"";
	return sms;
}

float *SIM900A::PositionGPS()
{
	
	String lati;
	String longi;
	boolean verif;
	SIM->println(F("AT+SAPBR=3,1,\"Contype\",\"GPRS\""));
	longi = lectureSerial();		
	SIM->println(F("AT+SAPBR=3,1,\"APN\",\"free\""));
	longi = lectureSerial();
	SIM->println(F("AT+SAPBR=1,1"));
	longi = lectureSerial(1000);
	SIM->println(F("AT+SAPBR=2,1"));
	longi = lectureSerial();
	SIM->println(F("AT+CIPGSMLOC=1,1"));
	delay(1000);
	longi = lectureSerial(1000);
	verif = this->Section(longi);
	float *pos=new float[2];
	pos[0] = latit;
	pos[1] = longit;
	while (pos[0] == 0 || pos[1] == 0)
	{
		SIM->println(F("AT+CIPGSMLOC=1,1"));
		delay(5000);
		longi = lectureSerial(1000);
		verif = this->Section(longi);
		pos[0] = latit;
		pos[1] = longit;
	}
	return pos;

}
void SIM900A::ReceptionSMS()
{
	String retour;
	String MessageAEnvoyer;
	MessageAEnvoyer = F("Humidite = ");
	MessageAEnvoyer += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].hmd;
	MessageAEnvoyer += F(" Poids = ");
	MessageAEnvoyer += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].poids;
	MessageAEnvoyer += F(" Temperature = ");
	MessageAEnvoyer += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].temps;
	MessageAEnvoyer += F(" Batterie = ");
	MessageAEnvoyer += ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].battery;
	MessageAEnvoyer += F(" Latitude = ");
	MessageAEnvoyer += String(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].gps_1,6);
	MessageAEnvoyer += F(" Longitude = ");
	MessageAEnvoyer += String(ArchivageHorodatage2::TableauDesMesures[ArchivageHorodatage2::A].gps_2,6);
	SIM->println(F("AT+CMGL=\"REC UNREAD\""));
	retour = lectureSerial();
	if (int errpos = retour.indexOf(F("+CMGL:")) != -1)
	{
		
		SIM->println(F("AT+CMGF=1"));
		retour = lectureSerial(1000);
		SIM->println(this->sendSMS());
		retour = lectureSerial();
		SIM->println(MessageAEnvoyer);
		retour = lectureSerial();
		SIM->write((char)26);
		retour = lectureSerial(1000);
		SIM->println(F("AT+CMGD=3"));
	}
}

boolean SIM900A::Section(String longi)
{
	String lati;
	pos = longi.indexOf(F("+CIPGSMLOC:"));
	if (pos != -1)
	{
	pos += 11;
	longi.remove(0, pos);
	pos = longi.indexOf(F(","));
	pos += 1;
	longi.remove(0, pos);
	lati = longi;
	pos = lati.indexOf(F(","));
	longi.remove(pos);
	pos += 1;
	lati.remove(0, pos);
	lati.remove(pos);
	longit = longi.toFloat();
	latit = lati.toFloat();
	if (latit != 0 && longit != 0)
	return true;
	return false;
	}
}
boolean SIM900A::lectureSeuils()
{
	String retour;
	bool succes;
	String URL = "AT+HTTPPARA=\"URL\",\"";
	URL += "http://ruchecdf18.000webhostapp.com/test.php\"";
	SIM->println(F("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""));
	retour = lectureSerial();
	SIM->println(F("AT+SAPBR=3,1,\"APN\",\"free\""));
	retour = lectureSerial();
	SIM->println(F("AT+SAPBR=1,1"));
	retour = lectureSerial();
	SIM->println(F("AT+HTTPINIT"));
	retour = lectureSerial();
	SIM->println(F("AT+HTTPPARA=\"CID\",1"));
	retour = lectureSerial();
	SIM->println(URL);
	retour = lectureSerial();
	SIM->println(F("AT+HTTPACTION=0"));
	delay(5000);
	retour = lectureSerial(1000);
	pos = retour.indexOf(F("200"));
	if (pos == -1)
	{
		succes = false;
	}
	SIM->println(F("AT+HTTPREAD"));;
	retour = lectureSerial();

	delay(1000);
	retour = getTempMin(retour);
	retour = getTempMax(retour);
	retour = gethumMin(retour);
	retour = gethumMax(retour);
	retour = getPoidsMin(retour);
	retour = getPoidsMax(retour);
	retour = getLat(retour);
	retour = getLong(retour);
	retour = getTel(retour);
	retour = getPasswd(retour);
	retour = getURL(retour);
	ArchivageHorodatage2::Sauvegarde_Seuil();
	return true;
}
String SIM900A::getTel(String initiale)
{
	String tel;
	pos = initiale.indexOf(F("tel"));
	pos += 6;
	initiale.remove(0, pos);
	tel = initiale;
	pos = tel.indexOf(F("\""));
	tel.remove(pos);
	Serial.print(F("numero de telephone : "));
	Serial.println(tel);
	ArchivageHorodatage2::Donnees_Seuil.telephone = tel;
	return initiale;
}
String SIM900A::getTempMin(String initiale)
{

	pos = initiale.indexOf(F("seuil_temp_min"));
	pos += 17;
	initiale.remove(0, pos);
	String tempmin;
	tempmin = initiale;
	pos = tempmin.indexOf(F("\""));
	tempmin.remove(pos);
	Serial.print(F("temperature minimale : "));
	Serial.println(tempmin);
	ArchivageHorodatage2::Donnees_Seuil.seuil_Tmp_Min = tempmin.toFloat();
	return initiale;

}
String SIM900A::getTempMax(String initiale)
{
	pos = initiale.indexOf(F("seuil_temp_max"));
	pos += 17;
	initiale.remove(0, pos);
	String tempmax;
	tempmax = initiale;
	pos = tempmax.indexOf(F("\""));
	tempmax.remove(pos);
	Serial.print(F("temperature maximale : "));
	Serial.println(tempmax);
	ArchivageHorodatage2::Donnees_Seuil.seuil_Tmp_Max = tempmax.toFloat();
	return initiale;
}
String SIM900A::gethumMin(String initiale)
{
	pos = initiale.indexOf(F("seuil_humidite_min"));
	pos += 21;
	initiale.remove(0, pos);
	String *humin = new String;
	*humin = initiale;
	humin->remove(3);
	pos = humin->indexOf(F("\""));
	humin->remove(pos);
	Serial.print(F("humidite minimale : "));
	Serial.println(*humin);
	ArchivageHorodatage2::Donnees_Seuil.seuil_Hmd_Min = humin->toInt();
	delete humin;
	return initiale;
}
String SIM900A::gethumMax(String initiale)
{
	pos = initiale.indexOf(F("seuil_humidite_max"));
	pos += 21;
	initiale.remove(0, pos);
	String hummax;
	hummax = initiale;
	pos = hummax.indexOf(F("\""));
	hummax.remove(pos);
	Serial.print(F("humidite maximale : "));
	Serial.println(hummax);
	ArchivageHorodatage2::Donnees_Seuil.seuil_Hmd_Max = hummax.toInt();
	return initiale;
}
String SIM900A::getPoidsMin(String initiale)
{
	pos = initiale.indexOf(F("seuil_poids_min"));
	pos += 18;
	initiale.remove(0, pos);
	String poidsmin;
	poidsmin = initiale;
	pos = poidsmin.indexOf(F("\""));
	poidsmin.remove(pos);
	Serial.print(F("poids minimal : "));
	Serial.println(poidsmin);
	ArchivageHorodatage2::Donnees_Seuil.seuil_Poids_Min = poidsmin.toFloat();
	return initiale;
}
String SIM900A::getPoidsMax(String initiale)
{
	pos = initiale.indexOf(F("seuil_poids_max"));
	pos += 18;
	initiale.remove(0, pos);
	String poidsmax;
	poidsmax = initiale;
	pos = poidsmax.indexOf(F("\""));
	poidsmax.remove(pos);
	Serial.print(F("poids maximal : "));
	Serial.println(poidsmax);
	ArchivageHorodatage2::Donnees_Seuil.seuil_Poids_Max = poidsmax.toFloat();
	return initiale;
}
String SIM900A::getLat(String initiale)
{
	pos = initiale.indexOf(F("seuil_lat"));
	pos += 12;
	initiale.remove(0, pos);
	String seuillatt;
	seuillatt = initiale;
	pos = seuillatt.indexOf(F("\""));
	seuillatt.remove(pos);
	Serial.print(F("seuil lattitude : "));
	Serial.println(seuillatt);
	ArchivageHorodatage2::Donnees_Seuil.seuil_GPS_1 = seuillatt.toFloat();
	return initiale;
}
String SIM900A::getLong(String initiale)
{
	pos = initiale.indexOf(F("seuil_longi"));
	pos += 14;
	initiale.remove(0, pos);
	String seuillong;
	seuillong = initiale;
	pos = seuillong.indexOf(F("\""));
	seuillong.remove(pos);
	Serial.print(F("seuil longitude : "));
	Serial.println(seuillong);
	ArchivageHorodatage2::Donnees_Seuil.seuil_GPS_2 = seuillong.toFloat();
	return initiale;
}
String SIM900A::getPasswd(String initiale)
{
	pos = initiale.indexOf(F("mot_de_passe"));
	pos += 15;
	initiale.remove(0, pos);
	String mdp;
	mdp = initiale;
	pos = mdp.indexOf(F("\""));
	mdp.remove(pos);
	Serial.print(F("mdp : "));
	Serial.println(mdp);
	ArchivageHorodatage2::Donnees_Seuil.password = mdp;
	return initiale;
}

String SIM900A::getURL(String initiale)
{
	pos = initiale.indexOf(F("URL"));
	pos += 6;
	initiale.remove(0, pos);
	String Url;
	Url = initiale;
	pos = Url.indexOf(F("\""));
	Url.remove(pos);
	while (pos != -1)
	{
		pos = Url.indexOf(F("\\"));
		Url.remove(pos, 1);
	}
	Serial.print(F("Url : "));
	Serial.println(Url);
	ArchivageHorodatage2::Donnees_Seuil.URL = Url;
	return initiale;
}





SIM900A::~SIM900A()
{
}
