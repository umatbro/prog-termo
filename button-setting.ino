
#include <LiquidCrystal.h>
#include "fun.h"
#include "Button/Button.h"


LiquidCrystal lcdDisplay(2,3,4,5,6,7);

Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(12);
Button *recentlyPressed; //flaga recently pressed
Temperature tDesired(20);

//funkcje
//void lcd16RollString (LiquidCrystal lcd, String napis, int rzad);
//void displayTemp(float value, LiquidCrystal lcd, int row); //wyświetl wartość value temperatury w wierszu row wyświetlacza lcd


void setup() {
	Serial.begin(9600);
	//ustawienie pinów
	
	//ustawienie ekranu
	lcdDisplay.begin(16,2);
	lcdDisplay.setCursor(0,0);
	lcdDisplay.print("USTAW   |  START");
	  
	lcdDisplay.setCursor(0,1);
	lcdDisplay.print("-/+");
	displayTemp(tDesired.value(),lcdDisplay,1);
}



void loop() 
{
	if(buttonPlus.uniquePress())
	{
		displayTemp(tDesired.set( tDesired.value() + 0.5 ), lcdDisplay, 1);
		recentlyPressed = &buttonPlus;
	}
	if (buttonMinus.uniquePress())
	{
		displayTemp(tDesired.set( tDesired.value() - 0.5 ), lcdDisplay, 1);
		recentlyPressed = &buttonMinus;
	}
	unsigned long czas = recentlyPressed -> timePressed();
	if(czas>1000 && czas%200 < 10)
	{
		if(recentlyPressed == &buttonPlus) tDesired.set( tDesired.value() + 0.5 );
		if(recentlyPressed == &buttonMinus) tDesired.set( tDesired.value() - 0.5 );
		displayTemp(tDesired.value(), lcdDisplay, 1);
	}
	

	Serial.println(recentlyPressed -> timePressed());
}


