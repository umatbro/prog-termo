#include <LiquidCrystal.h>
#include <Arduino.h>

/*
TEMPERATURA
*/

class Temperature
{
	private:
		float val; //obecna wartość temperatury
		bool tempGrowth; //!!!tylko do tempSimul
	
	public:
		float 	desired; //pożądana temperatury
				Temperature(float); //konstruktor
		float 	increaseDesired(float howMuch); //zwiększa oczekiwaną (desidred) wartośc o howMuch i zwraca wynik dodawania
		float 	decreaseDesired(float howMuch); //zmniejsza oczekiwaną wartość (desired) o howMuch i zwraca wynik odejmowania
		float 	getTempValue();
		float	value();
		float 	set(float); //ustawia podaną temperaturę i zwraca jej wartość
};


/*
PRZYCISK
*/
class ButtonMat
{
	private:
		int 	pin;
		bool 	held;
	
	public:
				ButtonMat(int pn);
		bool 	isPushed(); //wciśnięcie przycisku; wciśnięty przycisk zwraca wartość true
		bool	isReleased();
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
