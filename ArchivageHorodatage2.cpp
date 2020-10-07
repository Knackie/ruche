#include "ArchivageHorodatage2.h"
#include "Arduino.h"

bloc ArchivageHorodatage2::TableauDesMesures[20] = { 0 };
int ArchivageHorodatage2::A = 0;
data ArchivageHorodatage2::Donnees_Seuil = { 0, 0, 0, 0, 0, 0, 0, 0, "", "", "" };
int ArchivageHorodatage2::debut = 0;
int ArchivageHorodatage2::fin = 0;
bool ArchivageHorodatage2::overfloat = false;
bool ArchivageHorodatage2::flag1 = true;
bool ArchivageHorodatage2::flag2 = true;
bool ArchivageHorodatage2::ok = false;
RTC_DS1307 ArchivageHorodatage2::RTC;

ArchivageHorodatage2::ArchivageHorodatage2()
{
	Wire.begin();
	RTC.begin();
	debut = 0; fin = 0;
	BROCHE_CAPTEUR_VIN = 0xA0;
	overfloat = false;
	flag1 = true;
	flag2 = true;
	ok = false;
}

unsigned int ArchivageHorodatage2::analogReadReference(void)
{
	/* Elimine toutes charges résiduelles */
#if defined(__AVR_ATmega328P__)
	ADMUX = 0x4F;
#elif defined(__AVR_ATmega2560__)
	ADCSRB &= ~(1 << MUX5);
	ADMUX = 0x5F;
#elif defined(__AVR_ATmega32U4__)
	ADCSRB &= ~(1 << MUX5);
	ADMUX = 0x5F;
#endif
	delayMicroseconds(5);

	/* Sélectionne la référence interne à 1.1 volts comme point de mesure, avec comme limite haute VCC */
#if defined(__AVR_ATmega328P__)
	ADMUX = 0x4E;
#elif defined(__AVR_ATmega2560__)
	ADCSRB &= ~(1 << MUX5);
	ADMUX = 0x5E;
#elif defined(__AVR_ATmega32U4__)
	ADCSRB &= ~(1 << MUX5);
	ADMUX = 0x5E;
#endif
	delayMicroseconds(200);
	
	/* Active le convertisseur analogique -> numérique */
	ADCSRA |= (1 << ADEN);

	/* Lance une conversion analogique -> numérique */
	ADCSRA |= (1 << ADSC);

	/* Attend la fin de la conversion */
	while (ADCSRA & (1 << ADSC));

	/* Récupère le résultat de la conversion */
	return ADCL | (ADCH << 8);
}


void ArchivageHorodatage2::Engine()
{
	unsigned int raw_vin = analogRead(BROCHE_CAPTEUR_VIN);
	unsigned int raw_ref = analogReadReference();
	/* Calcul de la tension réel avec un produit en croix */
	double real_vin = ((raw_vin * 1.1) / raw_ref);
	analogReadReference();

	if (4.11 <= real_vin)
		TableauDesMesures[A].battery = 100;
	
	if (3.98 <= real_vin && real_vin <= 4.10)
		TableauDesMesures[A].battery = 90;

	if (3.93<=real_vin && real_vin<=3.97)
		TableauDesMesures[A].battery = 80;

	if (3.88<=real_vin && real_vin<=3.92)
		TableauDesMesures[A].battery = 70;

	if (3.84<=real_vin && real_vin<=3.87)
		TableauDesMesures[A].battery = 60;

	if (3.80<=real_vin && real_vin<=3.83)
		TableauDesMesures[A].battery = 50;

	if (3.76 <= real_vin && real_vin <= 3.79)
		TableauDesMesures[A].battery = 40;

	if (3.71 <= real_vin && real_vin <= 3.75)
		TableauDesMesures[A].battery = 30;

	if (3.61<=real_vin && real_vin<=3.70)
		TableauDesMesures[A].battery = 20;

	if (3.31<=real_vin && real_vin<=3.60)
		TableauDesMesures[A].battery = 10;

	if (3<=real_vin && real_vin<=3.30)
		TableauDesMesures[A].battery = 5;

	if (3>=real_vin )
		TableauDesMesures[A].battery = 0;

}

// Si Lecture a été faite !
void ArchivageHorodatage2::Affichage()
{
	A = 0;
	if (flag1)
	{
		Serial.print(F("Bienvenue sur Ruche_2018, du groupe BTS SNIR-2 !")); Serial.println();
		flag1 = false;
	}
	else
	{
		if (flag2)
		{
			Serial.print(F("+---------------------------------------------------------------+")); Serial.println();
			flag2 = false;
		}
		Serial.print(F("|	  Date : ")); Serial.print(TableauDesMesures[A].jour);
		Serial.print(F("/")); Serial.print(TableauDesMesures[A].mois);
		Serial.print(F("/")); Serial.print(TableauDesMesures[A].annee);
		Serial.print(F("	Horaire : ")); Serial.print(TableauDesMesures[A].heure);
		Serial.print(F(" h ")); Serial.print(TableauDesMesures[A].minut); Serial.print(F("		|"));

		Serial.println(); Serial.print(F("| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |")); Serial.println();

		Serial.print(F("| Poids : ")); Serial.print(TableauDesMesures[A].poids);
		Serial.print(F(" Kg	Temperature : ")); Serial.print(TableauDesMesures[A].temps);
		Serial.print(F("*C	Humidité : ")); Serial.print(TableauDesMesures[A].hmd);
		Serial.print(F(" %	|")); Serial.println();
		Serial.print(F("| Latitude : ")); Serial.print(TableauDesMesures[A].gps_1,6);
		Serial.print(F("	Longitude : ")); Serial.print(TableauDesMesures[A].gps_2,6);
		Serial.print(F(" 	Batterie : ")); Serial.print(TableauDesMesures[A].battery);
		Serial.print(F(" %	|")); Serial.println();
		Serial.print(F("+---------------------------------------------------------------+")); Serial.println();
		
		//	Bienvenu sur Ruche_2018, du groupe BTS SNIR-2 !
		//	Veuillez patienter 20 secondes, s'il vous plait...
		//	+-----------------------------------------------------------+
		//	|		Date : AA/MM/JJ				Horaire : HH:mm			|
		//	| - - - - - - - - - - - - - - - - - - - - - - - - - - - - - |
		//	| Poids : 0.0 Kg	Temperature : 24*C	Humidité : 32.0 %	|
		//	| Latitude : 2.0	Longitude : 3.0 	Batterie : 70 %		|
		//	+-----------------------------------------------------------+
	}
}

bool ArchivageHorodatage2::Flash_Enregistrer(float A_poids, float A_temp, float A_hmd, float A_gps_1, float A_gps_2)
{
	// Ecriture
	ok = true; A = 0;
	DateTime now = RTC.now();
	if (!RTC.isrunning()) RTC.adjust(DateTime(__DATE__, __TIME__));
	TableauDesMesures[A].poids = A_poids;
	TableauDesMesures[A].temps = A_temp;
	TableauDesMesures[A].hmd = A_hmd;
	TableauDesMesures[A].gps_1 = A_gps_1;
	TableauDesMesures[A].gps_2 = A_gps_2;
	TableauDesMesures[A].annee = now.year();
	TableauDesMesures[A].mois = now.month();
	TableauDesMesures[A].jour = now.day();
	TableauDesMesures[A].heure = now.hour();
	TableauDesMesures[A].minut = now.minute();
	Engine();
	EEPROM.put(20, *(TableauDesMesures + A) );

	// Lecture
	EEPROM.get(20, *(TableauDesMesures + A));
	return ok;
}

// Ecriture
bool ArchivageHorodatage2::Enregistrer(float A_poids, float A_temp, float A_hmd, float A_gps_1, float A_gps_2)
{
	ok = false;
	DateTime now = RTC.now();
	
	if (!RTC.isrunning()) RTC.adjust(DateTime(__DATE__, __TIME__));
	A = 0;
	TableauDesMesures[A].poids = A_poids;
	TableauDesMesures[A].temps = A_temp;
	TableauDesMesures[A].hmd = A_hmd;
	TableauDesMesures[A].gps_1 = A_gps_1;
	TableauDesMesures[A].gps_2 = A_gps_2;
	TableauDesMesures[A].annee = now.year();
	TableauDesMesures[A].mois = now.month();
	TableauDesMesures[A].jour = now.day();
	TableauDesMesures[A].heure = now.hour();
	TableauDesMesures[A].minut = now.minute();
	Engine();

	if (now.hour() == 12 || now.hour() == 24 || now.hour() == 0)
	{
		ok = true;
		EEPROM.put(fin, *(TableauDesMesures + 0) );

		if ((fin != 19) && (fin + 1 == debut)) overfloat = true;
		else fin++;

		if ((fin == 19) && (debut == 0)) overfloat = true;
		else fin = 0;
	}
	return ok;
}

// Lecture
void ArchivageHorodatage2::Vider(bool Vide)
{
	DateTime now = RTC.now();
	if (!RTC.isrunning()) RTC.adjust(DateTime(__DATE__, __TIME__));
	A = 0;
	if (now.hour() == 12 || now.hour() == 24 || now.hour() == 0)
	{
		if (fin > debut)
		{
			for (int i = debut; i < fin; i++)
			{
				EEPROM.get(i, *(TableauDesMesures + A) );
				A++;
			}
		}
		
		if (fin < debut)
		{
			for (int i = debut; i < 20; i++)
			{
				EEPROM.get(i, *(TableauDesMesures + A) );
				A++;
			}
			for (int i = 0; i < fin; i++)
			{
				EEPROM.get(i, *(TableauDesMesures + A) );
				A++;
			}
		}
		
		if (fin == debut)
		{
			EEPROM.get(fin, *(TableauDesMesures + A) );
		}

		if (Vide)
		{
			debut = fin;
			overfloat = false;
		}
	}
}
// Si Internet Ok
// Ensuite Mathieu

static void ArchivageHorodatage2::Sauvegarde_Seuil()
{
	EEPROM.put(21, Donnees_Seuil);
}

static void ArchivageHorodatage2::Lecture_Seuil()
{
	EEPROM.get(21, Donnees_Seuil);
}

ArchivageHorodatage2::~ArchivageHorodatage2()
{
}
