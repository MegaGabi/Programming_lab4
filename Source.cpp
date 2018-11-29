#include <iostream>
#include <fstream>
#include <ctime>
#include <conio.h>

using namespace std;

const int BuildingsCount = 25;

int CountriesCount;

char Countries[][4] = 
{
	"UA",
	"EU",
	"US",
	"RU"
};

int indexByValueCountry(char* Value)
{
	for (int i = 0; i < CountriesCount; i++)
	{
		if (!strcmp(Countries[i], Value))
		{
			return i;
		}
	}

	return -1;
}

struct Product
{
	char*	Provider;		//поставщик
	char*	ProductName;	//наименование продукта
	double	Cost;			//цена
	double	Count;			//кол-во
	char*	Country;		//страна поставщик

	struct Date { int day, month, year; } DeliveryDate; //дата поставки

	void Show(ostream& out)
	{
		out		<< "Provider: " << Provider
				<< ", ProductName: " << ProductName
				<< ", Cost: " << Cost
				<< ", Count: " << Count
				<< ", Country: " << Country
				<< ", Delivery Date: " << DeliveryDate.day
				<< "/" << DeliveryDate.month
				<< "/" << DeliveryDate.year
				<< ";";
	}
};

int GetRandomNumber(int smallest, int biggest)
{
	if ((biggest - smallest) == 0)
	{
		int buf = biggest;
		biggest = smallest;
		smallest = buf;

		if (!biggest && !smallest)//если оба значения равны нулю
			return 0;

		if (biggest == smallest)//если оба значения одинаковы
			return biggest;
	}

	return smallest + rand() % (biggest - smallest);
}

double GetRandomDouble(int smallest, int biggest, int digitsAfterComma)
{
	if (digitsAfterComma <= 0)
		digitsAfterComma = 1;

	double toDivide = pow(10, digitsAfterComma);
	return GetRandomNumber(smallest, biggest - 1) + GetRandomNumber(0, toDivide) / toDivide;
}

char* RandomCharacterString(int size)//создает строку случайных символов размером size
{
	char* res = new char[size + 1];
	for (int i = 0; i < size; i++)
	{
		res[i] = GetRandomNumber('A', 'Z'); //запись случайного символа между А и Z в строку
	}
	res[size] = '\0';


	return res;
}

char* GenerateRandomProvider()
{
	int res_size = 12;	//размер массива на выходе

	char* res = new char[res_size];
	strcpy_s(res, res_size, "OOO\"");

	strcat_s(res, res_size, RandomCharacterString(res_size - strlen(res) - 2));

	res[res_size - 2] = '\"';	//закрыть кавычку после ООО
	res[res_size - 1] = '\0';	//нуль терминатор

	return res;
}

char* GenerateRandomProductName()
{
	int res_size = 12;

	char* res = new char[res_size];

	strcpy_s(res, res_size, RandomCharacterString(res_size - 1));

	res[res_size - 1] = '\0';	//нуль терминатор

	return res;
}

void ProductsCostSumByCountry(double *&Sum, Product* Products, int ProductsCount, int year)
{
	for (int i = 0; i < ProductsCount; i++)
	{
		if (Products[i].DeliveryDate.year == year)
		{
			int CountryIndex = indexByValueCountry(Products[i].Country);

			if (CountryIndex == -1)
				cout << "Ошибка! Неверный индекс!";

			Sum[CountryIndex] += Products[i].Cost;
		}
	}
}

void main()
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	CountriesCount = sizeof(Countries) / sizeof(Countries[0]);

	int ProductsCount = 100;

	Product* Products = new Product[ProductsCount];

	ofstream fout("output.txt");

	for (int i = 0; i < ProductsCount; i++)
	{
		Products[i] = Product();
		Products[i].Provider = GenerateRandomProvider();
		Products[i].ProductName = GenerateRandomProductName();
		Products[i].Cost = GetRandomDouble(0, 1000, 2);
		Products[i].Count = GetRandomDouble(0, 1000, 3);
		Products[i].Country = Countries[GetRandomNumber(0, CountriesCount)];

		Products[i].DeliveryDate.day = GetRandomNumber(1, 31);
		Products[i].DeliveryDate.month = GetRandomNumber(1, 12);
		Products[i].DeliveryDate.year = GetRandomNumber(2010, 2017);

		Products[i].Show(cout); cout << endl;
		Products[i].Show(fout); fout << endl;
	}
	cout << endl;
	fout << endl;

	double *Sums = new double[CountriesCount] {0};
	
	int year = 2016;
	ProductsCostSumByCountry(Sums, Products, ProductsCount, year);

	cout << "За " << year << " год было продано:\n";
	fout << "За " << year << " год было продано:\n";
	for (int i = 0; i < CountriesCount; i++)
	{
		cout << "Страна " << Countries[i] << " всего продала на сумму: " << Sums[i] << endl;
		fout << "Страна " << Countries[i] << " всего продала на сумму: " << Sums[i] << endl;
	}

	_getch();

	fout.close();

	delete[] Products;
	delete[] Sums;
}