#include <iostream>
#include <conio.h>		//strzalki
#include <time.h>		//rand
#include <iomanip>		//wyswietlanie
#include <string>		//pobieranie rozmiaru planszy
using namespace std;

void gra_2048();

int** stworz_plansze(int n);
void usun_plansze(int**plansza, int n);
void wyswietl_plansze(int**plansza, int n);
int strzalki();			//1-lewo, 2-prawo, 3-gora, 4-dol, 5-esc
void ruch(int**plansza, int n, bool&grac_dalej);
void ruch_prawo(int**plansza, int n);
void ruch_lewo(int**plansza, int n);
void ruch_gora(int**plansza, int n);
void ruch_dol(int**plansza, int n);
void zakoncz(int**plansza, int n, bool&grac_dalej);
void dodaj_rand_kafelek(int**plansza, int n);
bool wygrana(int**plansza, int n);
bool przegrana(int**plansza, int n);
bool ruch_zabroniony_lewo(int**plansza, int n);
bool ruch_zabroniony_prawo(int**plansza, int n);
bool ruch_zabroniony_gora(int**plansza, int n);
bool ruch_zabroniony_dol(int**plansza, int n);


int main()
{
	cout << "instrukcja: " << endl;
	cout << "strzalki gora/dol i lewo/prawo -> wykonywanie ruchu" << endl;
	cout << "esc -> przerwanie gry i rozpoczecie nowej" << endl << endl;

	while (1)
		gra_2048();

	system("pause");
	return 0;
}


//GRA - funkcja glowna
void gra_2048()
{
	cout << "NOWA GRA" << endl;
	int n = 0;
	cout << "podaj rozmiar planszy (od 2 do 10)" << endl;
	bool poprawny_wybor_rozmiaru = false;
	string wybor_rozmiaru = "0";
	char rozmiar;
	while (!poprawny_wybor_rozmiaru)
	{
		getline(cin, wybor_rozmiaru);
		rozmiar = wybor_rozmiaru[0];		//jesli wybor bedzie poprawny to rozmiarem bedzie pierwszy (i jedyny) znak
		if (wybor_rozmiaru.size() > 1)		//wprowadzono wiecej niz jeden znak -> na pewno nie jest to poprawny rozmiar
			cout << "zly rozmiar, podaj inny" << endl;
		else if ((int)rozmiar >= 50 && (int)rozmiar <= 57)	//poprawny rozmiar planszy
		{
			poprawny_wybor_rozmiaru = true;
			n = (int)rozmiar - 48;
		}
		else								//wprowadzono litere/znak lub cos co nie jest liczba z zakresu 2-9
			cout << "zly rozmiar, podaj inny" << endl;
	}

	bool grac_dalej = true;					//z gry mozna wyjsc naciskajac esc
	int**plansza = stworz_plansze(n);		//tworzy plansze i wypelnia 0
	dodaj_rand_kafelek(plansza, n);			//zeby po rozpoczeciu gry byly 2 kafelki

	while (grac_dalej)						//wlasciwa czesc gry.
	{
		dodaj_rand_kafelek(plansza, n);		//po kazdym ruchu dodawany jest kafelek w losowym miejscu, 80% - 2, 20% - 4
		wyswietl_plansze(plansza, n);

		if (przegrana(plansza, n))
		{
			cout << "GAME OVER" << endl << endl;
			break;
		}
		else if (wygrana(plansza, n))
		{
			cout << "WIN" << endl << endl;
			break;
		}
		ruch(plansza, n, grac_dalej);
	}
	usun_plansze(plansza, n);		//po skonczeniu gry plansza zostaje usunieta
}

//******************************************
int** stworz_plansze(int n)
{
	int**plansza = new int*[n];
	for (int i = 0; i < n; i++)
		plansza[i] = new int[n];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			plansza[i][j] = 0;
	}

	return plansza;
}
void usun_plansze(int**plansza, int n)
{
	for (int i = 0; i < n; i++)
		delete[]plansza[i];
	delete[]plansza;
}
void wyswietl_plansze(int**plansza, int n)
{
	system("CLS");		//czyszczenie ekranu
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//cout << "	" << plansza[i][j] << "	";
			if (plansza[i][j] != 0)
				cout << setw(4) << plansza[i][j] << "  ";
			else
				cout << setw(4) << "      ";

			if (j != (n - 1))		//ostatnie pole
				cout << "|";
		}
		if (i != (n - 1))	//ostatnia linia, 2*
		{
			cout << endl;
			for (int k = 0; k < n; k++)
			{
				cout << "------";		//poprzeczka po kazdym wierszu, poza ostatnim 2*
				if (k != (n - 1))
					cout << "+";
			}
		}
		cout << endl;
	}
	cout << endl;
}
void ruch(int**plansza, int n, bool&grac_dalej)
{
	int wybor;
	wybor = strzalki();

	if (wybor == 1 && !ruch_zabroniony_lewo(plansza, n))
		ruch_lewo(plansza, n);
	else if (wybor == 2 && !ruch_zabroniony_prawo(plansza, n))
		ruch_prawo(plansza, n);
	else if (wybor == 3 && !ruch_zabroniony_gora(plansza, n))
		ruch_gora(plansza, n);
	else if (wybor == 4 && !ruch_zabroniony_dol(plansza, n))
		ruch_dol(plansza, n);
	else if (wybor == 5)
		zakoncz(plansza, n, grac_dalej);
	else
	{
		cout << "ruch zabroniony, wykonaj inny" << endl;
		ruch(plansza, n, grac_dalej);
	}
}

//dodawanie rand kafelka na poczatku kazdej tury
void dodaj_rand_kafelek(int**plansza, int n)
{
	bool czy_jest_puste_pole = false;		//zakladamy, ze nie ma
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (plansza[i][j] == 0)
				czy_jest_puste_pole = true;		//sprawdzamy, ze jednak jest puste pole

	if (czy_jest_puste_pole == false)
	{
		cout << "brak pustych pol, nie mozna dodac rand kafelka" << endl;
		cout << "cos jest nie tak! warunki na win/lose nie zostaly spelnione i nie mozna dodac kafelka <- to nie powinno miec prawa sie nigdy wyswietlic!" << endl;
		system("pause");
		return;
	}

	int x = 0, y = 0;
	int dwa_czy_cztery = rand() % 100;
	while (1)
	{
		x = rand() % n;
		y = rand() % n;
		if (plansza[x][y] == 0)
		{
			if (dwa_czy_cztery > 80)
				plansza[x][y] = 4;
			//plansza[x][y] = rand();
			else
				plansza[x][y] = 2;
			//plansza[x][y] = rand();

			break;			//ew. return;
		}
	}
}

//wybory ze strzalek
int strzalki()		//1-lewo, 2-prawo, 3-gora, 4-dol, 5-esc
{
	bool koniec = false;
	while (koniec == false)
	{
		unsigned char znak = _getch();
		switch (znak)
		{
		case 0:		//klawisze specjalne (czasem zero czasem 224 - zale¿ne od pc'ta chyba)
		case 224:	//klawisze specjalne
			znak = _getch();
			switch (znak)
			{
			case 72: //strza³ka w górê
				cout << "gora" << endl;
				return 3;
				break;
			case 80: //strza³ka w dó³
				cout << "dol" << endl;
				return 4;
				break;
			case 75: //strza³ka w lewo
				cout << "lewo" << endl;
				return 1;
				break;
			case 77: //strza³ka w prawo
				cout << "prawo" << endl;
				return 2;
				break;
			}
			znak = 0;
			break;
		case 27: //ESC
			cout << "esc" << endl;
			return 5;
			koniec = true;
			break;
		}
	}
	return(99);
}

void ruch_lewo(int**plansza, int n)
{
	int ktore_pole = 0;
	int *new_line = new int[n];

	for (int i = 0; i < n; i++)		//kolejne linie
	{
		for (int j = 0; j < n; j++)		//new_line jest czyszczone po kazdej linii
			new_line[j] = 0;

		for (int j = 0; j < n; j++)	//kolejne pola w linii
		{
			if (plansza[i][j] != 0)
			{
				new_line[ktore_pole] = plansza[i][j];
				for (int k = j + 1; k < n; k++)
				{
					if (plansza[i][j] != plansza[i][k] && plansza[i][k] != 0)		//jesli ktores z kolejnych pol (roznych od 0) jest rozne od pola j to przerywamy (kafelki nie moga sie polaczyc)
						break;
					else if (plansza[i][k] == plansza[i][j])		//jesli dwa (niekoniecznie bezposrednio kolo siebie, moga byc zera pomiedzy) sasiednie kafelki sa takie same, to zlaczamy je
					{
						new_line[ktore_pole] *= 2;
						j = k;		//j przeskakuje na pole k
						break;		//break, bo tylko raz w obrebie jednego pola mozna zlaczyc kafelki	
					}
				}
				ktore_pole++;
			}
		}
		for (int j = 0; j < n; j++)		//przepisanie new_line do plansza
			plansza[i][j] = new_line[j];
		ktore_pole = 0;
	}

	delete[]new_line;
}
void ruch_prawo(int**plansza, int n)
{
	int ktore_pole = n - 1;
	int *new_line = new int[n];

	for (int i = 0; i < n; i++)		//kolejne linie
	{
		for (int j = 0; j < n; j++)		//new_line jest czyszczone po kazdej linii
			new_line[j] = 0;

		for (int j = n - 1; j >= 0; j--)	//kolejne pola w linii (od konca, od prawej strony)
		{
			if (plansza[i][j] != 0)
			{
				new_line[ktore_pole] = plansza[i][j];
				for (int k = j - 1; k >= 0; k--)
				{
					if (plansza[i][j] != plansza[i][k] && plansza[i][k] != 0)		//jesli ktores z kolejnych pol (roznych od 0) jest rozne od pola j to przerywamy (kafelki nie moga sie polaczyc)
						break;
					else if (plansza[i][k] == plansza[i][j])		//jesli dwa (niekoniecznie bezposrednio kolo siebie, moga byc zera pomiedzy) sasiednie kafelki sa takie same, to zlaczamy je
					{
						new_line[ktore_pole] *= 2;
						j = k;		//j przeskakuje na pole k
						break;		//break, bo tylko raz w obrebie jednego pola mozna zlaczyc kafelki	
					}
				}
				ktore_pole--;
			}
		}
		for (int j = 0; j < n; j++)		//przepisanie new_line do plansza
			plansza[i][j] = new_line[j];
		ktore_pole = n - 1;
	}

	delete[]new_line;
}
void ruch_dol(int**plansza, int n)
{
	int ktore_pole = n - 1;
	int *new_line = new int[n];

	for (int j = 0; j < n; j++)		//kolejne kolumny
	{
		for (int i = 0; i < n; i++)		//new_line jest czyszczone po kazdej linii
			new_line[i] = 0;

		for (int i = n - 1; i >= 0; i--)		//kolejne pola w kolumnie
		{
			if (plansza[i][j] != 0)
			{
				new_line[ktore_pole] = plansza[i][j];
				for (int k = i - 1; k >= 0; k--)
				{
					if (plansza[i][j] != plansza[k][j] && plansza[k][j] != 0)		//jesli ktores z kolejnych pol (roznych od 0) jest rozne od pola j to przerywamy (kafelki nie moga sie polaczyc)
						break;
					else if (plansza[k][j] == plansza[i][j])		//jesli dwa (niekoniecznie bezposrednio kolo siebie, moga byc zera pomiedzy) sasiednie kafelki sa takie same, to zlaczamy je
					{
						new_line[ktore_pole] *= 2;
						i = k;		//i przeskakuje na pole k
						break;		//break, bo tylko raz w obrebie jednego pola mozna zlaczyc kafelki	
					}
				}
				ktore_pole--;
			}
		}
		for (int i = 0; i < n; i++)		//przepisanie new_line do plansza
			plansza[i][j] = new_line[i];
		ktore_pole = n - 1;
	}

	delete[]new_line;
}
void ruch_gora(int**plansza, int n)
{
	int ktore_pole = 0;
	int *new_line = new int[n];

	for (int j = 0; j < n; j++)		//kolejne kolumny
	{
		for (int i = 0; i < n; i++)		//new_line jest czyszczone po kazdej linii
			new_line[i] = 0;

		for (int i = 0; i < n; i++)		//kolejne pola w kolumnie
		{
			if (plansza[i][j] != 0)
			{
				new_line[ktore_pole] = plansza[i][j];
				for (int k = i + 1; k < n; k++)
				{
					if (plansza[i][j] != plansza[k][j] && plansza[k][j] != 0)		//jesli ktores z kolejnych pol (roznych od 0) jest rozne od pola j to przerywamy (kafelki nie moga sie polaczyc)
						break;
					else if (plansza[k][j] == plansza[i][j])		//jesli dwa (niekoniecznie bezposrednio kolo siebie, moga byc zera pomiedzy) sasiednie kafelki sa takie same, to zlaczamy je
					{
						new_line[ktore_pole] *= 2;
						i = k;		//i przeskakuje na pole k
						break;		//break, bo tylko raz w obrebie jednego pola mozna zlaczyc kafelki	
					}
				}
				ktore_pole++;
			}
		}
		for (int i = 0; i < n; i++)		//przepisanie new_line do plansza
			plansza[i][j] = new_line[i];
		ktore_pole = 0;
	}

	delete[]new_line;
}
void zakoncz(int**plansza, int n, bool&grac_dalej)
{
	cout << "przerwales gre" << endl;
	grac_dalej = false;
}

//warunki win/lose
bool wygrana(int**plansza, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (plansza[i][j] == 2048)
				return true;

	return false;
}
bool przegrana(int**plansza, int n)
{
	//szukanie pustych pol - jak jest puste pole to mozna grac dalej
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (plansza[i][j] == 0)
				return false;

	//w poziomie
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n - 1; j++)
			if (plansza[i][j] == plansza[i][j + 1])
				return false;

	//w pionie
	for (int i = 0; i < n - 1; i++)
		for (int j = 0; j < n; j++)
			if (plansza[i][j] == plansza[i + 1][j])
				return false;

	//w przeciwynym razie plansza jest zablokowana i nie mozna wykonac ruchu
	return true;
}

//sprawdzanie poprawnosci ruchu
bool ruch_zabroniony_lewo(int**plansza, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n - 1; j++)
			if (plansza[i][j] == plansza[i][j + 1] && plansza[i][j] != 0)	//sasiednie kafelki sa takie same (i nie sa ==0) -> mozna je zlaczyc, ruch nie jest zabroniony
				return false;

	for (int i = 0; i < n; i++)
	{
		for (int j = n - 1; j >= 0; j--)
		{
			if (plansza[i][j] != 0)		//szukanie kafelka !=0 od prawej strony
			{
				for (int k = j - 1; k >= 0; k--)	//poruszanie sie dalej w lewo i szukanie kafelka == 0
					if (plansza[i][k] == 0)		//jesli jest kafelek == 0 tzn. mozna wykonac ruch w lewo -> ruch w lewo nie jest zabroniony
						return false;
			}
		}
	}

	return true;
}
bool ruch_zabroniony_prawo(int**plansza, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n - 1; j++)
			if (plansza[i][j] == plansza[i][j + 1] && plansza[i][j] != 0)		//sasiednie kafelki sa takie same (i nie sa ==0) -> mozna je zlaczyc, ruch nie jest zabroniony
				return false;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (plansza[i][j] != 0)		//szukanie kafelka != 0 od lewej strony
			{
				for (int k = j + 1; k < n; k++)	//poruszanie sie dalej w prawo i szukanie kafelka == 0
					if (plansza[i][k] == 0)		//jesli jest kafelek == 0 tzn. mozna wykonac ruch w prawo -> ruch w prawo nie jest zabroniony
						return false;
			}
		}
	}


	return true;
}
bool ruch_zabroniony_gora(int**plansza, int n)
{
	for (int i = 0; i < n - 1; i++)		//komentarze jak do ruch_zabroniony_lewo/_prawo
		for (int j = 0; j < n; j++)
			if (plansza[i][j] == plansza[i + 1][j] && plansza[i][j] != 0)
				return false;

	for (int j = 0; j < n; j++)
	{
		for (int i = n - 1; i >= 0; i--)
		{
			if (plansza[i][j] != 0)
			{
				for (int k = i - 1; k >= 0; k--)
					if (plansza[k][j] == 0)
						return false;
			}
		}
	}

	return true;
}
bool ruch_zabroniony_dol(int**plansza, int n)
{
	for (int i = 0; i < n - 1; i++)		//komentarze jak do ruch_zabroniony_lewo/_prawo
		for (int j = 0; j < n; j++)
			if (plansza[i][j] == plansza[i + 1][j] && plansza[i][j] != 0)
				return false;

	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < n; i++)
		{
			if (plansza[i][j] != 0)
			{
				for (int k = i + 1; k < n; k++)
					if (plansza[k][j] == 0)
						return false;
			}
		}
	}

	return true;
}