#include <LiquidCrystal.h>

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
		float 	tempSimulation(); //symulacja temperatury
		bool 	isHeatingReq();
};


/*
PRZYCISK
*/
class Button
{
	private:
		int 	pin;
	
	public:
				Button(int pn);
		bool 	isPushed(); //wciśnięcie przycisku; wciśnięty przycisk zwraca wartość true
};
	
/*	
	Regulator PID
	
*/
	
class RegulacjaPID 
{
private:
	

public:
	float regulator(float w_zad, float wy_o)
{
    float k=20;
    float k_i = 0;
    float k_d = 10;

    //zmienne pomocnicze
    float p,i,d,r;
    float u; //uchyb regulacji
    static float u_p = 0; //uchyb regulacji w poprzednim wywo�aniu
    static float su = 0; //suma minionych uchyb�w regulacji
    u = w_zad - wy_o; // aktualny uchyb regulacji

// wyznaczenie skladnika proporcjonalnego
    p = k * u;

// wyznaczenie sk�adnika ca�kowego
su = su + u; //najpierw trzeba wyliczy� sum� wszystkich uchyb�w;
i = k_i * su;

// wyznaczenie sk�adnika D
d = k_d * (u-u_p);
u_p = u; //zapami�taj chwilow� warto�� uchybu dla przysz�ych oblicze�

r = p + i + d; //sygna� wyj�ciowy regulatora

//ograniczenia regulowanej wartości
if (r < 0) r = 0; //czyli wyp�yw nie mo�e by� dodatni
if (r > 100) r = 90;

    return r;
}
};
	
	
	


/*
	------------------------------------------------------------------
							FUNKCJE
	------------------------------------------------------------------
*/


/* ------------------------TEMPERATURE----------------------------- */


Temperature::Temperature(float tv=14)
{
	val = tv;
	//desired = td;
	//tempGrowth = true;
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
	return val;
}
	
float Temperature::value()
{
	return val;
}


float Temperature::tempSimulation()
{
	if (tempGrowth == true && val < desired+5)
	{
		digitalWrite(11,HIGH);
		delay(500);
		val += 0.5;
		if(val == desired+5) tempGrowth = false;
		return val;
	}
	else 
	{
		digitalWrite(11,LOW);
		delay(500);
		val -= 0.5;
		if ( val < desired - 5 ) tempGrowth = true;
		return val;
	}
}

bool Temperature::isHeatingReq()
{
	if(val < desired + 5) return true;
	else if (val > desired - 3) return false;
}

	
/*------------------------------BUTTON------------------------------------*/
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
	INNE
*/
//przewijanie napisu po ekranie gdy ilość znaków większa od 16 
extern void lcd16RollString (LiquidCrystal lcd, String napis, int rzad)
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
  
  
//wyświetlanie wartości liczbowej, 4 ostatnie znaki na wyświetlaczu lcd
extern void displayTemp(float value, LiquidCrystal lcd, int row)
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

extern void tempSimul(RegulacjaPID regulacja, float tValue, float tDesired)
{
	while(true)
	{
	int wspolczynnik;
	wspolczynnik =(int)(regulacja.regulator(tDesired,tValue));
	tValue += (wspolczynnik/10)*0.5;
	
	Serial.print("Set: ");
	Serial.print(tDesired);
	Serial.print("\tTemp: ");
	Serial.println(tValue);
	tValue--;
	analogWrite(11,map(wspolczynnik,0,100,0,255));
	delay(200);}
}

