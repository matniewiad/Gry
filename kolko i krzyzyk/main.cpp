#include <iostream>
using namespace std;

//wyswietlanie planszy 3x3 w konsoli
void wyswietl_plansze(char t[])
{
	for(int i = 1 ; i <= 9 ; i++)		//bardziej intuicyjne pozycje niz przy indeksowaniu od 0
	{
	cout << " " << t[i] << " ";

    if(i!=3 && i!=6 && i!=9)       
		cout << "|";							// Po elementach 1,2,4,5,7,8 rysujemy |
    else if(i == 3 || i == 6) 
		cout << endl << "-----------" << endl;  // Po elementach 3 i 6 poprzeczka
    else if(i == 9) 
		cout << endl << endl;						    // Po elemencie 9 koniec wiersza
	}
}

//funkcja sprawdzajaca, czy ktorys zawodnik wygral
bool wygrana(char t[], char znak_gracza, bool czy_wyswietlac) //czy_wyswietlac - bo nie wyswietlamy w symulacji w minimaxie, true-tak, false-nie
{
	bool czy_wygrana = false;					//przyjmujemy, ze nie ma wygranej i w dalszej czesci funkcji sprawdzamy odpowiednie warunki

	for(int i = 1 ; i < 9 ; i+=3)				//sprawdzamy wiersze, i+=3 bo wiersze maja indeksy 1, 4, 7 
	{
		if( (t[i] == znak_gracza)  &&  (t[i+1] == znak_gracza)  && (t[i+2] == znak_gracza)  )
		{
			czy_wygrana = true;
		}
	}

	for(int i = 1 ; i < 4 ; i++)				//sprawdzamy kolumny, i<4 bo kolumny maja indeksy 1,2,3
	{
		if( (t[i] == znak_gracza)  &&  (t[i+3] == znak_gracza)  && (t[i+6] == znak_gracza)  )
		{
			czy_wygrana = true;
		}
	}

	//sprawdzamy przekatne
	if( (t[1] == znak_gracza)  &&  (t[5] == znak_gracza)  && (t[9] == znak_gracza)  )
	{
		czy_wygrana = true;
	}

	if( (t[3] == znak_gracza)  &&  (t[5] == znak_gracza)  && (t[7] == znak_gracza)  )
	{
		czy_wygrana = true;
	}

	if(czy_wygrana)
	{
		if(czy_wyswietlac)
		{
			wyswietl_plansze(t);		//wygrana ktoregos z zawodnikow, wyswietlamy plansze
			cout << "wygrywa zawodnik o znaku " << znak_gracza << endl;
		}
		return true;
	}
	return false;
}

//funkcja sprawdzajaca, czy wystapil remis
bool remis(char t[], bool czy_wyswietlac)	//czy_wyswietlac - bo nie wyswietlamy w symulacji w minimaxie, true-tak, false-nie
{
	for(int i = 1 ; i <= 9 ; i++) //jesli jest wolne pole mozna grac dalej, zwracamy false
	{
		if(t[i] == ' ')
		{
			return false;
		}
	}

	if(czy_wyswietlac)
	{
		wyswietl_plansze(t);
		cout << "remis" << endl;
	}
	return true;
}

//algorytm minimaks
int minimax(char t[], char znak_gracza)
{
	int ruch; //wartosc danego ruchu; -1, 0 lub 1
	int pkt_odniesienia_ruch; //zapamietywana najbardziej korzystna wartosc ruchu w danym obiegu

	if(wygrana(t, znak_gracza, false)) //sprawdzamy, czy dany gracz wygrywa
	{
		if(znak_gracza == 'X')
			return 1;
		else 
			return -1;
	}

	if(remis(t, false))  //sprawdzamy, czy jest remis
	{
		return 0;
	}

	//zmieniamy gracza i symulujemy jego zagrania, oceniamy je
	if(znak_gracza == 'X')
		znak_gracza = 'O';
	else
		znak_gracza = 'X';

	//naprzemiennie analizujemy ruchy graczy
	//pkt_odniesienia_ruch ustawiamy na minimum dla gracza i na maksimum dla przeciwnika
	if(znak_gracza == 'X')
		pkt_odniesienia_ruch = -100;  //liczmymy maksimum, zmienna ustawiamy na mala wartosc
	else
		pkt_odniesienia_ruch = 100; //liczmymy minimum, zmienna ustawiamy na duza wartosc

	//przegladamy cala plansze, szukamy wolnych pol

	for(int i = 1 ; i <= 9 ; i++)
	{
		if(t[i] == ' ')
		{
			t[i] = znak_gracza;					//symulujemy ruch
			ruch = minimax(t, znak_gracza);		//rekurencyjnie wywolujemy funckje ze zmienionym graczem
			t[i] = ' ';							//cofamy ruch
			if(  (znak_gracza == 'O') && (ruch < pkt_odniesienia_ruch)  ||  (znak_gracza == 'X') && (ruch > pkt_odniesienia_ruch)  )
			{
				pkt_odniesienia_ruch = ruch;	//po kazdym wykonaniu zmieniamy punkt odniesienia
			}
		}
	}
	return pkt_odniesienia_ruch;
}

//funkcja symulujaca ruch komputera, zwraca indeks wybranego pola
int ruch_komputera(char t[])
{
	int wykonany_ruch;			//ruch wykonany przez komuter
	int ruch;					//jw.
	int pkt_odniesienia_ruch;	//jw.

	pkt_odniesienia_ruch = -100; //liczmymy maksimum, zmienna ustawiamy na mala wartosc

	//przegladamy plansze i szukamy wolnych miejsc
	for(int i = 1 ; i <= 9 ; i++)
	{
		if(t[i] == ' ')
		{
			t[i] = 'X';					//symulujemy ruch
			ruch = minimax(t, 'X');		//wykonujemy algorytm minimaks
			//cout << "wartosc ruchu komputera: " << ruch << endl;
			t[i] = ' ';					//cofamy ruch
			if(ruch > pkt_odniesienia_ruch)		//jesli jest lepszy ruch, to zastepujemy nim wczesniejszy wybor
			{
				pkt_odniesienia_ruch = ruch;
				wykonany_ruch = i;
			}
		}
	}
	return wykonany_ruch;
}

//wykonywanie ruchu przez gracza i komputer
void wykonaj_ruch(char t[], char &znak_gracza, int tryb_gry)
{
	int wybor;							//pole wybrane przez gracza

	wyswietl_plansze(t);							//aktualny stan gry

	if(tryb_gry == 1)		//gracz vs gracz
	{
		cout << "wybor gracza o znaku " << znak_gracza << " : ";
		cin >> wybor;
		cout << endl << endl;

		if(  (t[wybor] == ' ')  &&  (wybor > 0)  &&  (wybor < 10)  )
		{
			t[wybor] = znak_gracza;
		}
	}
	else if(tryb_gry == 2)		//gracz vs komputer
	{
		if(znak_gracza == 'O')
		{
			cout << "wybor gracza o znaku " << znak_gracza << " : ";
			cin >> wybor;
		}
		else
		{
			wybor = ruch_komputera(t);
			cout << "wybor komputera: " << wybor << endl;
		}

		if(  (t[wybor] == ' ')  &&  (wybor > 0)  &&  (wybor < 10)  )
		{
			t[wybor] = znak_gracza;
		}
	}
	
	//zamiana gracza
	if(  (znak_gracza == 'X')  &&  (wybor > 0)  &&  (wybor < 10)  )
	{
		znak_gracza = 'O';
	}
	else if(  (znak_gracza == 'O')  &&  (wybor > 0)  &&  (wybor < 10)  )
	{
		znak_gracza = 'X';
	}
}


int main()
{
	char t[10];			// plansza, 10 elementow dla latwiejszego poruszania sie
	char gracz;			// znak gracza, ktory rozpoczyna
	int tryb_gry;		// 1 - gracz vs gracz, 2 - gracz vs komputer
	bool esc = false;	// do wychodzenia z petli
	char wybor_esc;

	cout << "gra w kolko i krzyzyk" << endl << "wybor pola poprzez klawisze 1-9" << endl << endl;
	while(!esc)
	{
		gracz = 'O'; //kto zaczyna, 'O' - pierwszy gracz, 'X' - komputer

		//wypelnienie planszy pustymi polami
		for(int i = 1 ; i <= 9 ; i++)
		{
			t[i] = ' ';
		}

		cout << "wybierz tryb gry" << endl << "1: gracz vs gracz" << endl << "2: gracz vs komputer" << endl;
		cin >> tryb_gry;

		while(  (!wygrana(t, 'X', true))  &&  (!wygrana(t, 'O', true))  &&  (!remis(t, true))  )  //dopoki nie ma wygranej lub remisu wykonywane sa ruchy
		{
			wykonaj_ruch(t, gracz, tryb_gry);
		}

		cout << "czy chesz grac dalej? (T/N)" << endl;
		cin >> wybor_esc;
		if(wybor_esc != 'T')
		{
			esc = true;
		}
	}

	system("pause");
	return 0;
}