#include "fun.h"


/*
	------------------------------------------------------------------
							METODY
	------------------------------------------------------------------
*/


/* ------------------------TEMPERATURE----------------------------- */


Temperature::Temperature(float tv)
{
	val = tv;
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

float Temperature::getTempValue(MAX6675 thermocouple)
{
	return thermocouple.readCelsius();
}
	
float Temperature::value()
{
	return val;
}


float Temperature::set(float setting)
{
	val = setting;
	return val;
}




//--------------------------REGULATOR PID--------------------------------*/
float RegulacjaPID::regulator(float w_zad, float wy_o) 
{
    float k=200;
    float k_i = 0.002;
    float k_d = 20;

    //zmienne pomocnicze
    float p,i,d,r;
    float u; //uchyb regulacji
    static float u_p = 0; //uchyb regulacji w poprzednim wywo�aniu
    static float su = 0; //suma minionych uchyb�w regulacji
    u = w_zad - wy_o; // aktualny uchyb regulacji

	// wyznaczenie skladnika proporcjonalnego
    p = k * u;

	// wyznaczenie składnika ca?kowego
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


/*
	INNE
*/
//przewijanie napisu po ekranie gdy ilo�� znak�w wi�ksza od 16 
extern void lcd16RollString(LiquidCrystal lcd, String napis, int rzad)
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
	
	Serial.print(tDesired);
	Serial.print("\t");
	Serial.println(tValue);
	randomSeed(analogRead(A5));
	float randTemp = (random(0,700))/100;
	tValue -= randTemp;
	analogWrite(11,map(wspolczynnik,0,100,0,255));
	delay(200);
	//}
}


extern boolean stepTimer(int milliseconds)
{
	static int millisecNow;
	static int millisecStart = millis();
	millisecNow = millis();

	if ( (millisecNow - millisecStart > milliseconds) && (millisecNow - millisecStart)%milliseconds < 20 ) 
	{
		millisecStart = millis();
		return true;
	}
	else return false;
}

