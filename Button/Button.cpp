/*
||
|| @file Button.cpp
|| @version 1.6
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making buttons
|| #
||
|| @license
|| | Copyright (c) 2009 Alexander Brevig. All rights reserved.
|| | This code is subject to AlphaLicence.txt
|| | alphabeta.alexanderbrevig.com/AlphaLicense.txt
|| #
||
*/

//include the class definition
#include "Button.h"

/*
|| <<constructor>>
|| @parameter buttonPin sets the pin that this switch is connected to
|| @parameter buttonMode indicates PULLUP or PULLDOWN resistor
*/
Button::Button(uint8_t buttonPin, uint8_t buttonMode){
	this->pin=buttonPin;
    pinMode(pin,INPUT);
	buttonMode==PULLDOWN ? pulldown() : pullup();
    state = 0;
    bitWrite(state,CURRENT,!mode);
}

/*
|| Set pin HIGH as default
*/
void Button::pullup(void){
	mode=PULLUP;
	digitalWrite(pin,HIGH);
}

/*
|| Set pin LOW as default
*/
void Button::pulldown(void){
	mode=PULLDOWN;
	//digitalWrite(pin,LOW);
}

/*
|| Return the bitWrite(state,CURRENT, of the switch
*/
bool Button::isPressed(void){
    bitWrite(state,PREVIOUS,bitRead(state,CURRENT));
    if (digitalRead(pin) == mode){
        bitWrite(state,CURRENT,false);
    } else {
        bitWrite(state,CURRENT,true);
    }
    if (bitRead(state,CURRENT) != bitRead(state,PREVIOUS)){
        bitWrite(state,CHANGED,true);
    }else{
        bitWrite(state,CHANGED,false);
    }
	return bitRead(state,CURRENT);
}

/*
|| Return true if the button has been pressed
*/
bool Button::wasPressed(void){
    if (bitRead(state,CURRENT)){
        return true;
    } else {
        return false;
    }
}

/*
|| Return true if state has been changed
*/
bool Button::stateChanged(void){
    return bitRead(state,CHANGED);
}

/*
|| Return true if the button is pressed, and was not pressed before
*/
bool Button::uniquePress(void){
    return (isPressed() && stateChanged());
}


/*
|| Zwraca czas od początku naciśnięcia przycisku do jego zwolnienia w milisekundach
*/
unsigned long Button::timePressed(void)
{
	static unsigned long sTart = 0;
	static unsigned long sTop = 0;
	if (uniquePress())
		sTart = millis();
	if (!isPressed()) sTart = millis();
	
	sTop = millis();
	if (!isPressed()) return 0;
	else return (sTop - sTart);
}