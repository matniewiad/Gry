#include "snake.h"
#include <iostream>
#include <list>
#include <windows.h>
#include <conio.h>
#include <time.h>
using namespace std;


void przesun_kursor(COORD cel)
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cel);		//przenosi kursor na wspolrzedne okreslone przez cel
}

void ramka(int startX, int startY, int szer, int wys)
{
	COORD start;
	start.X = startX;
	start.Y = startY;

	przesun_kursor(start);
	for (int i = 0; i < szer; i++)
		cout << "#";

	przesun_kursor(start);
	for (int i = 0; i < wys; i++)
	{
		przesun_kursor(start);
		cout << "#";
		start.Y += 1;
	}
	start.Y = startY;

	start.X = startX + szer - 1;
	przesun_kursor(start);
	for (int i = 0; i < wys; i++)
	{
		przesun_kursor(start);
		cout << "#" << endl;
		start.Y += 1;
	}

	start.X = startX;
	start.Y = startY + wys - 1;
	przesun_kursor(start);
	for (int i = 0; i < szer; i++)
	{
		cout << "#";
	}
	cout << endl;
}

void inicjalizuj_weza(list<pair<int, int>>&waz, int dlugosc, int startX, int startY, int szer, int wys)
{
	for (int i = 0; i < dlugosc; i++)
	{
		waz.push_back(make_pair(i + (szer / 3) + startX, (wys / 2) + startY));
	}
}

void wyswietl_weza(list<pair<int, int>> waz)
{
	int licznik = 0;
	COORD waz_wspolrzedne;
	for (list<pair<int, int>>::iterator it = waz.begin(); it != waz.end(); it++)
	{
		licznik++;
		waz_wspolrzedne.X = (*it).first;
		waz_wspolrzedne.Y = (*it).second;
		przesun_kursor(waz_wspolrzedne);
		if (licznik != waz.size())
			cout << (char)254;
		else
			cout << "X";
	}
}

void posprzataj_po_starym_wezu(list<pair<int, int>> waz)
{
	COORD waz_wspolrzedne;
	for (list<pair<int, int>>::iterator it = waz.begin(); it != waz.end(); it++)
	{
		waz_wspolrzedne.X = (*it).first;
		waz_wspolrzedne.Y = (*it).second;
		przesun_kursor(waz_wspolrzedne);
		cout << " ";
	}
}

bool dotkniecie_samego_siebie(list<pair<int, int>> &waz)
{
	pair<int, int> glowa;
	glowa.first = waz.back().first;
	glowa.second = waz.back().second;
	int dlugosc = waz.size();
	int licznik = 0;

	for (list<pair<int, int>>::iterator it = waz.begin(); it != waz.end(); it++)
	{
		if (licznik == dlugosc - 1)
			break;
		if (glowa.first == (*it).first && glowa.second == (*it).second)
		{
			return true;
		}
		licznik++;
	}

	return false;
}

bool dotkniecie_ramki(list<pair<int, int>> &waz, int startX, int startY, int szer, int wys)
{
	pair<int, int> glowa;
	glowa.first = waz.back().first;
	glowa.second = waz.back().second;

	if (glowa.first == startX || glowa.first == startX + szer)
	{
		//cout << "skucha" << endl;
		return true;
	}

	if (glowa.second == startY || glowa.second == startY + wys)
	{
		//cout << "skucha" << endl;
		return true;
	}

	return false;
}

void wspolrzedne_weza(list<pair<int, int>> waz)
{
	for (list<pair<int, int>>::iterator it = waz.begin(); it != waz.end(); it++)
	{
		cout << "X: " << (*it).first << "   Y: " << (*it).second << endl;
	}
	cout << "glowa: " << waz.back().first << "  " << waz.back().second << endl;
}

void wstaw_robaka(list<pair<int, int>> &waz, pair<int, int> &robak, int startX, int startY, int szer, int wys, bool& jest_robak)
{
	bool OK = true;

	do															//losuj co najmniej raz
	{
		OK = true;												//bo sie czasami program zawieszal na losowaniu
		robak.first = (rand() % (szer - 1)) + startX + 1;		//odpowiednie manipulacje, zeby robak nie wyladowal na ramce
		robak.second = (rand() % (wys - 1)) + startY + 1;
		//cout << "*";											//pierwsze miejsce do sprawdzenia buga z zawieszeniem sie gry

		for (list<pair<int, int>>::iterator it = waz.begin(); it != waz.end(); it++)
			if (robak.first == (*it).first && robak.second == (*it).second)
				OK = false;										//jesli na wylosowanym jest waz...

	} while (!OK);												//...to losuj dalej	

	COORD r;
	r.X = robak.first;
	r.Y = robak.second;
	przesun_kursor(r);
	cout << (char)207;											//wyswietlanie robaka na planszy
	jest_robak = true;
}

bool zjedzono_robaka(list<pair<int, int>> &waz, pair<int, int> robak, bool&jest_robak)
{
	if (waz.back().first == robak.first && waz.back().second == robak.second)
	{
		jest_robak = false;
		return true;
	}
	return false;
}

void ruch_gracza(list<pair<int, int>> &waz, char& kierunek, char& poprzedni, COORD& glowa, bool& stop, int startX, int startY, int szer, int wys)
{
	kierunek = _getch();

	if (kierunek != 'w' && kierunek != 's' && kierunek != 'a' && kierunek != 'd' && kierunek != VK_ESCAPE)
		kierunek = poprzedni;						//naciskanie innych przyciskow nie powoduje zatrzymania, ani innych nieporzadanych akcji

													//brzydko, ale dziala
	switch (kierunek)
	{
	case 'w':
		if (poprzedni != 's')						//jesli ruch wykonywany jest w innym kierunku niz dol (przeciwny do wcisnietego klawisza)
		{											//to zachowuj sie normalnie
			if (glowa.Y > startY)
				glowa.Y -= 1;
			poprzedni = 'w';						//jesli klawisz zostal wcisniety zgodnie z prawem to zostaje zapamietany jako poprzedni
		}
		else										//ale jesli wcisniety klawisz jest przeciwny do aktualnego ruchu
		{											//to zachowuj sie tak, jakby wcisnieto klawisz zgodny z kierunkiem ruchu
			if (glowa.Y < startY + wys)
				glowa.Y += 1;
			kierunek = 's';							//chroni przed nacisnieciem klawisza przeciwnego do aktualnego ruchu
		}
		break;

	case 's':
		if (poprzedni != 'w')
		{
			if (glowa.Y < startY + wys)
				glowa.Y += 1;
			poprzedni = 's';
		}
		else
		{
			if (glowa.Y > startY)
				glowa.Y -= 1;
			kierunek = 'w';
		}
		break;

	case 'a':
		if (poprzedni != 'd')
		{
			if (glowa.X > startX)
				glowa.X -= 1;
			poprzedni = 'a';
		}
		else
		{
			if (glowa.X < startX + szer)
				glowa.X += 1;
			kierunek = 'd';
		}
		break;

	case 'd':
		if (poprzedni != 'a')
		{
			if (glowa.X < startX + szer)
				glowa.X += 1;
			poprzedni = 'd';
		}
		else
		{
			if (glowa.X > startX)
				glowa.X -= 1;
			kierunek = 'a';
		}
		break;

	case VK_ESCAPE:								//esc - przerywanie gry
		stop = true;
		cin.ignore();
		getchar();
		break;
	}

	przesun_kursor(glowa);
	waz.push_back(make_pair(glowa.X, glowa.Y));
}

void ruch_automatyczny(list<pair<int, int>> &waz, char& kierunek, COORD& glowa, int startX, int startY, int szer, int wys)
{
	switch (kierunek)
	{
	case 'w':
		if (glowa.Y > startY)
			glowa.Y -= 1;
		break;

	case 's':
		if (glowa.Y < startY + wys)
			glowa.Y += 1;
		break;

	case 'a':
		if (glowa.X > startX)
			glowa.X -= 1;
		break;

	case 'd':
		if (glowa.X < startX + szer)
			glowa.X += 1;
		break;
	}
	przesun_kursor(glowa);
	waz.push_back(make_pair(glowa.X, glowa.Y));
}

void snake(int szer, int wys, int poziom_trudnosci)
{
	cout << "aby rozpoczac wcisnij enter" << endl;
	getchar();
	system("cls");
	list<pair<int, int>> waz;			//wspolrzedne segmentow weza

	int startX = 1;						//wspolrzedne lewego gornego roga ramki
	int startY = 0;
	int x = szer;
	int y = wys;
	int dlugosc_weza = 4;				//musi byc mniejsza niz szerokosc - x

	inicjalizuj_weza(waz, dlugosc_weza, startX, startY, x, y);
	ramka(startX, startY, x + 1, y + 1);
	wyswietl_weza(waz);

	//******	koniec inicjalizacji weza i planszy. Inicjalizacja sterowanie
	char kierunek = 'd', poprzedni = 'd';
	bool stop = false;					//esc przerywa gre
	bool wykonano_ruch = false;
	COORD glowa;
	COORD koniec_planszy;
	koniec_planszy.X = 0;
	koniec_planszy.Y = startY + y + 2;
	glowa.X = waz.back().first;
	glowa.Y = waz.back().second;
	pair<int, int> robak = make_pair(0, 0);
	pair<int, int> ogon = make_pair(0, 0);
	bool jest_robak = false;

	przesun_kursor(glowa);
	while (!stop)
	{
		if (dotkniecie_ramki(waz, startX, startY, x, y) || dotkniecie_samego_siebie(waz))	//warunki zakonczenia gry
		{
			przesun_kursor(koniec_planszy);
			cout << "GAME OVER                      " << endl;
			cout << "wynik: " << dlugosc_weza << endl;
			cin.ignore();
			getchar();
			break;
		}

		if (zjedzono_robaka(waz, robak, jest_robak))
		{
			dlugosc_weza++;
			waz.push_front(make_pair(ogon.first, ogon.second));		//waz wydluza sie o ogon, czyli to, co zostaloby usuniete, gdyby nie zjedzenie robaka
		}

		if (jest_robak == false)
			wstaw_robaka(waz, robak, startX, startY, x, y, jest_robak);

		if (kierunek == 'w' || kierunek == 's')			//skalowanie predkosci w pionie i w poziomie. pojedyncze pole w konsoli nie jest symetryczne
			Sleep(700 / poziom_trudnosci);
		else
			Sleep(500 / poziom_trudnosci);

		if (_kbhit())									//sygnal z klawiatury
		{
			ruch_gracza(waz, kierunek, poprzedni, glowa, stop, startX, startY, x, y);
			wykonano_ruch = true;
		}

		if (wykonano_ruch)								//bo inaczej po zmianie kierunku wykonuje sie podwojny ruch. Wymyslic inne rozwiazanie!
			wykonano_ruch = false;
		else
		{
			ruch_automatyczny(waz, kierunek, glowa, startX, startY, x, y);
		}

		//aktualizacja pozyscji weza na planszy
		ogon.first = waz.front().first;					//ogon potrzebny do zwiekszania weza po zjedzeniu robaka
		ogon.second = waz.front().second;
		posprzataj_po_starym_wezu(waz);
		waz.pop_front();								//usuwanie ostatniego segmentu, zeby waz nie rosl w nieskonczonosc
		wyswietl_weza(waz);


		przesun_kursor(koniec_planszy);
		cout << "aktualny wynik: " << waz.size();
	}
}

void gra_snake()
{
	int szer = 50, wys = 20;
	int poziom_trudnosci = 5;
	srand(time(NULL));
	bool wyjdz = false;

	while (!wyjdz)
	{
		bool sprawdzanie_cina;
		system("cls");
		cout << "MENU: " << endl;
		cout << "1. Rozpocznij gre" << endl;
		cout << "2. Opis" << endl;
		cout << "3. Wyjdz z gry" << endl;
		char wybor;
		wybor = _getch();
		system("cls");
		switch (wybor)
		{
		case '1':
			cout << "podaj szerokosc planszy <5-100>: ";
			do {
				cin >> szer;
				sprawdzanie_cina = cin.good();
				cin.clear();
				cin.sync();
				cin.ignore();
			} while (sprawdzanie_cina == 0);
			cout << "szerokoscr : " << szer << endl;
			if (szer < 15)
			{
				cout << "Podano zbyt mala szerokosc. Rozgrywka odbedzie sie na planszy o minimalnej szerokosci 15" << endl;
				szer = 15;
			}
			if (szer > 100)
			{
				cout << "Podano zbyt duza szerokosc. Rozgrywka odbedzie sie na planszy o maksymalnej szerokosci 100" << endl;
				szer = 100;
			}

			cout << "podaj wysokosc planszy <5-25>: ";
			do {
				cin >> wys;
				sprawdzanie_cina = cin.good();
				cin.clear();
				cin.sync();
				cin.ignore();
			} while (sprawdzanie_cina == 0);
			cout << "wysokosc : " << wys << endl;
			std::cin.clear();
			std::cin.sync();
			if (wys < 5)
			{
				cout << "Podano zbyt mala wysokosc. Rozgrywka odbedzie sie na planszy o minimalnej wysokosci 5" << endl;
				wys = 5;
			}
			if (wys > 25)
			{
				cout << "Podano zbyt duza wysokosc. Rozgrywka odbedzie sie na planszy o maksymalnej wysokosci 25" << endl;
				wys = 25;
			}

			cout << "podaj poziom trudnosci <1-10>: ";
			do {
				cin >> poziom_trudnosci;
				sprawdzanie_cina = cin.good();
				cin.clear();
				cin.sync();
				cin.ignore();
			} while (sprawdzanie_cina == 0);
			cout << "szerokoscr : " << poziom_trudnosci << endl;
			std::cin.clear();
			std::cin.sync();
			if (poziom_trudnosci < 1)
			{
				cout << "Podano zbyt maly poziom trudnosci. Rozgrywka odbedzie sie z minimalnym poziomem trudnosci 1" << endl;
				poziom_trudnosci = 1;
			}
			if (poziom_trudnosci > 10)
			{
				cout << "Podano zbyt duza wysokosc. Rozgrywka odbedzie sie z maksymalnym poziomem trudnosci 10" << endl;
				poziom_trudnosci = 10;
			}

			snake(szer, wys, poziom_trudnosci);			//GLOWNA FUNKCJA GRY

			break;
		case '2':
			cout << "sterowanie - wsad" << endl << "esc - przerwanie gry" << endl;
			cout << "wykonal MN" << endl;
			getchar();
			break;
		case '3':
			wyjdz = true;
			break;
		}
	}
}