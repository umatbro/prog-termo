#include <LiquidCrystal.h>
#include "fun.h"


const int LED = 11;
/*
nieużywane przyciski
Button buttonPlus(10);
Button buttonMinus(11);
Button buttonAccept(12);*/
Temperature tValue(20);
Temperature tDesired(45);
bool ON = false;

LiquidCrystal lcdDisplay(2,3,4,5,6,7);


//funkcje
//void lcd16RollString (LiquidCrystal lcd, String napis, int rzad);
//void displayTemp(float value, LiquidCrystal lcd, int row); //wyświetl wartość value temperatury w wierszu row wyświetlacza lcd
float tValueFloat;

void setup() {
  tValueFloat = tValue.getTempValue();
	//ustawienie pinów
	pinMode(LED,OUTPUT);
	digitalWrite(LED,LOW);
	Serial.begin(9600);
	
	//ustawienie ekranu
	  lcdDisplay.begin(16,2);
	  lcdDisplay.setCursor(0,0);
	  lcdDisplay.print("Temperatura: ");
	  displayTemp(tValue.getTempValue(),lcdDisplay,0);
	  
	  lcdDisplay.setCursor(0,1);
	  lcdDisplay.print("Ustaw temp.");
	  displayTemp(tDesired.value(),lcdDisplay,1);
}

RegulacjaPID reg;
void loop() 
{
  
	displayTemp(tDesired.value(), lcdDisplay, 1);
	displayTemp(tValueFloat, lcdDisplay, 0);
	
	tempSimul(reg, tValueFloat, tDesired.value());
}


