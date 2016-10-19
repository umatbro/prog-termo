#include <LiquidCrystal.h>
#include "fun.h"

const int LED = 9;
const int buttonPlus = 11;
const int buttonMinus = 10;
const int buttonAccept = 12;
int temperature = 14;
int tempDesired = 0;
Temperature t1(20,40);

LiquidCrystal lcdDisplay(2,3,4,5,6,7);


void mrugaj();


void setup() {
  lcdDisplay.begin(16,2);
  lcdDisplay.setCursor(0,0);
  lcdDisplay.print("Temperatura: ");
  lcdDisplay.setCursor(14,0);
  lcdDisplay.print((String)t1.getTempValue());
  
  lcdDisplay.setCursor(0,1);
  lcdDisplay.print("Ustaw temp.");
  lcdDisplay.setCursor(15,1);
  lcdDisplay.print((String)t1.desired +" st.C");
}

void loop() 
{

}
