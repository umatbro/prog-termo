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
		float 	desired; //pożądana wartosc temperatury - nieużywane
				Temperature(float=0); //konstruktor
		float 	increaseDesired(float howMuch); //zwiększa oczekiwaną (desidred) wartośc o howMuch i zwraca wynik dodawania
		float 	decreaseDesired(float howMuch); //zmniejsza oczekiwaną wartość (desired) o howMuch i zwraca wynik odejmowania
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
		boolean 		stepTimer(unsigned long); //zwraca true cyklicznie co ileś milisekund, pozwala na wykonanie jakiejś akcji co pewien czas
};

/*
	POWER SUPPLY - MANSON 2405
*/
class Manson2405
{
	private:
		//SoftwareSerial rs485; //połązcenie z zasilaczem
		//const byte rxPin = 0;
		//const byte txPin = 1;
		//const byte receiveEnablePin = 11;
		//const byte dataEnablePin = 12;
	public:
		//Manson2405(rs485,byte=0,byte=1,byte=11,byte=12);
		void sendCommand(String, SoftwareSerial); //wysyła komendę do zasilacza
		void sendBytes(String,byte,byte);
		String getResponse(SoftwareSerial); // zwraca odpowiedź zasilacza
		String startSession(SoftwareSerial); // wyłącza przedni panel
		String endSession(SoftwareSerial); // włącza przedni panel i kończy sesję
};

/*	
	Regulator PID
*/
	
class RegulacjaPID 
{
private:
	
public:
	float regulator(float w_zad, float wy_o);
};



//INNE
extern void lcd16RollString(LiquidCrystal lcd, String napis, int rzad);
extern void displayTemp(float value, LiquidCrystal lcd, int row);
extern void tempSimul(RegulacjaPID regulacja, float& tValue, float tDesired);
extern boolean stepTimer(int miliseconds); 