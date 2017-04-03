#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include "Arduino.h"
#include "Button/Button.h"
#include "max6675.h"

class Button;

/*
TEMPERATURA
*/
class Temperature
{
	private:
		float val; //obecna wartość temperatury
	
	public:
				Temperature(float=0); //konstruktor
		float 	getTempValue(MAX6675);
		float	value();
		float 	set(float); //ustawia podaną temperaturę i zwraca jej wartość
};


/*
	TIMER
*/
class Timer
{
	private:
		unsigned long 	millisecNow;
		unsigned long 	millisecStart;
	public: 
						Timer();
		long int		threshold;
		boolean 		stepTimer(unsigned long); //zwraca true cyklicznie co określoną ilość milisekund, pozwala na wykonanie jakiejś akcji co pewien czas
};

/*
	POWER SUPPLY - MANSON 2405
*/
class Manson2405
{
	public:
		String sendCommand(SoftwareSerial, String); //wysyła komendę do zasilacza
		String startSession(SoftwareSerial, int); // wyłącza przedni panel
		String endSession(SoftwareSerial, int); // włącza przedni panel i kończy sesję
};

/*	
	Regulator PID
*/
	
class RegPID 
{
	private:
		float k;
		float k_i;
		float k_d;
		
	public:
		void setK(float);
		void setKI(float);
		void setKD(float);
		RegPID(float =100,float=0.002,float =20);
		float regulator(float , float );
};



//INNE
extern void lcdPrint(LiquidCrystal lcd, String napis, int rzad);
extern void displayTemp(float value, LiquidCrystal lcd, int row);