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
    float k_i = 2;
    float k_d = 0;

    //zmienne pomocnicze
    float p,i,d,r;
    float u; //uchyb regulacji
    static float u_p = 0; //uchyb regulacji w poprzednim wywołaniu
    static float su = 0; //suma minionych uchybów regulacji
    u = w_zad - wy_o; // aktualny uchyb regulacji

// wyznaczenie skladnika proporcjonalnego
    p = k * u;

// wyznaczenie sk�adnika ca�kowego
su = su + u; //najpierw trzeba wyliczyć sumę wszystkich uchybów;
i = k_i * su;

// wyznaczenie składnika D
d = k_d * (u-u_p);
u_p = u; //zapamitaj chwilową wartość uchybu dla przyszłych obliczeń

r = p + i + d; //sygnał wyjściowy regulatora

//ograniczenia regulowanej wartości
if (r < 0) r = 0; //czyli moc nie może być ujemna
if (r > 100) r = 90;

    return r;
}
};
	
	
	


/*
	------------------------------------------------------------------
							METODY
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

extern void tempSimul(RegulacjaPID regulacja, float& tValue, float tDesired)
{
	//while(true)
	//{
	int wspolczynnik;
	wspolczynnik =(int)(regulacja.regulator(tDesired,tValue));
	tValue += (wspolczynnik/10)*0.5;
	
	//Serial.print("Set: ");
	Serial.print(tDesired);
	//Serial.print("\tTemp: ");
	Serial.print("\t");
	Serial.println(tValue);
	randomSeed(analogRead(A5));
	float randTemp = (random(0,700))/100;
	tValue -= randTemp;
	analogWrite(11,map(wspolczynnik,0,100,0,255));
	delay(200);
	//}
}

