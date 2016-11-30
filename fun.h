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
		float 	getTempValue(MAX6675 thermocouple);
		float	value();
		float 	set(float); //ustawia podaną temperaturę i zwraca jej wartość
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
extern boolean stepTimer(int miliseconds); // zwraca true cyklicznie co ileś milisekund, pozwala na wykonanie jakiejś akcji co pewien czas