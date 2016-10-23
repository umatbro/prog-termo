#include <LiquidCrystal.h>
#include <IRremote.h>

/*
TEMPERATURA
*/

class Temperature
{
	private:
		float value; //obecna wartość temperatury
	
	public:
		float 	desired; //pożądana temperatury
				Temperature(float ,float); //konstruktor
		float 	increaseDesired(float howMuch); //zwiększa oczekiwaną (desidred) wartośc o howMuch i zwraca wynik dodawania
		float 	decreaseDesired(float howMuch); //zmniejsza oczekiwaną wartość (desired) o howMuch i zwraca wynik odejmowania
		float 	getTempValue();
		float 	tempSimulation(bool heating); //symulacja temperatury
		bool 	isHeatingReq();
};


Temperature::Temperature(float tv=14, float td=0)
{
	value = tv;
	desired = td;
}

float Temperature::increaseDesired(float howMuch)
{
	if (desired>= 50) return 50;
	else return desired += howMuch;
}

float Temperature::decreaseDesired(float howMuch)
{
	if (desired<=0) return 0;
	else return desired -= howMuch;
}

float Temperature::getTempValue()
{
	return value;
}


float Temperature::tempSimulation(bool heating)
{
	if (heating == true)
	{
		digitalWrite(9,HIGH);
		delay(500);
		value += 0.5;
		return value;
	}
	else 
	{
		digitalWrite(9,LOW);
		delay(500);
		value -= 0.5;
		return value;
	}
}

bool Temperature::isHeatingReq()
{
	if(value < desired + 5) return true;
	else if (value > desired - 3) return false;
}


/*
PRZYCISK
*/
class Button
{
	private:
	int pin;
	
	public:
	Button(int pn);
	bool isPushed(); //wciśnięcie przycisku; wciśnięty przycisk zwraca wartość true
};

Button::Button(int pn)
{
	pin = pn;
	pinMode(pin,INPUT_PULLUP);
}

bool Button::isPushed()
{
	if(digitalRead(pin) == LOW) return true;
	else return false;
}




/*
		PILOT
*/

class RemoteButton
{
	private:
		int hexCode1;
		int hexCode2;
		
	public:
		RemoteButton(int);
};


RemoteButton::RemoteButton(int h1)
{
	hexCode1 = h1;
	hexCode2 = hexCode1 + 0x10000;
}




/*
FUNKCJE
*/


//przewijanie napisu po ekranie gdy ilość znaków większa od 16 
 void lcd16RollString (LiquidCrystal lcd, String napis, int rzad)
  {
    if(napis.length()>16){
    for(int i =0;i<=(napis.length()-16);i++)
    {
      //lcd.clear();
      lcd.setCursor(0,rzad);
      for(int j=i;j<=i+15;j++)
      {
        if(j<napis.length()){lcd.print(napis[j]);}
      }
      delay(1000);
    }}
    else{lcd.setCursor(0,rzad);
    for(int i=0;i<napis.length();i++){
    lcd.print(napis[i]);}
    }
  }
  
void displayTemp(float value, LiquidCrystal lcd, int row)
{
	if (value < 10) //liczby jednocyfrowe
	{
		lcd.setCursor(12,row);
		lcd.print(" ");
		lcd.setCursor(13,row);
		lcd.print((String)value);
	}
	else if (value >= 10 && value <100 ) //liczby dwucyfrowe
	{
		lcd.setCursor(12,row);
		lcd.print((String)value);
	}
	else
	{
		lcd.setCursor(13,row);
		lcd.print("err");
	}
}


