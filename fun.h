#include <LiquidCrystal.h>

class Temperature
{
	private:
	int value; //obecna wartość temperatury
	
	public:
	int desired; //pożądana temperatury
	Temperature(int ,int); //konstruktor
	int increaseDesired(int howMuch); //zwiększa oczekiwaną (desidred) wartośc o howMuch i zwraca wynik dodawania
	int decreaseDesired(int howMuch); //zmniejsza oczekiwaną wartość (desired) o howMuch i zwraca wynik odejmowania
	int getTempValue();
};


Temperature::Temperature(int tv=14, int td=0)
{
	value = tv;
	desired = td;
}

int Temperature::increaseDesired(int howMuch)
{
	if (desired>= 50) return 50;
	else return desired += howMuch;
}

int Temperature::decreaseDesired(int howMuch)
{
	if (desired<=0) return 0;
	else return desired -= howMuch;
}

int Temperature::getTempValue()
{
	return value;
}