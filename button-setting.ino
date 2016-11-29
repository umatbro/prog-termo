
#include <LiquidCrystal.h>
#include "fun.h"
#include "Button/Button.h"


LiquidCrystal lcdDisplay(2,3,4,5,6,7);

Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(12);
Temperature tDesired(20);

//funkcje
//void lcd16RollString (LiquidCrystal lcd, String napis, int rzad);
//void displayTemp(float value, LiquidCrystal lcd, int row); //wyświetl wartość value temperatury w wierszu row wyświetlacza lcd


void setup() {
	//ustawienie pinów
	
	//ustawienie ekranu
	lcdDisplay.begin(16,2);
	lcdDisplay.setCursor(0,0);
	lcdDisplay.print("USTAW TEMPERATURĘ");
	  
	lcdDisplay.setCursor(0,1);
	lcdDisplay.print("-/+");
	displayTemp(tDesired.value(),lcdDisplay,1);
}



void loop() 
{
	if(buttonPlus.uniquePress())
	{
		displayTemp(tDesired.set( tDesired.value() + 0.5 ), lcdDisplay, 1);
	}
	if (buttonMinus.uniquePress())
	{
		displayTemp(tDesired.set( tDesired.value() - 0.5 ), lcdDisplay, 1);
	}
}


