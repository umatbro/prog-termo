
#include <LiquidCrystal.h>
#include "fun.h"
#include "Button/Button.h"


LiquidCrystal lcdDisplay(2,3,4,5,6,7);

Button buttonPlus(8,PULLUP);
Button buttonMinus(9,PULLUP);
Button buttonAccept(12);
Button *flag;
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
		flag = &buttonPlus;
	}
	if (buttonMinus.uniquePress())
	{
		displayTemp(tDesired.set( tDesired.value() - 0.5 ), lcdDisplay, 1);
		flag = &buttonMinus;
	}
	unsigned long czas = flag -> timePressed();
	if(czas>1000 && czas%200 < 10)
	{
		if(flag == &buttonPlus) tDesired.set( tDesired.value() + 0.5 );
		if(flag == &buttonMinus) tDesired.set( tDesired.value() - 0.5 );
		displayTemp(tDesired.value(), lcdDisplay, 1);
	}
	

	Serial.println(flag -> timePressed());
}


