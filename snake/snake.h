#pragma once
#pragma once
#include <iostream>
#include <list>
#include <conio.h>
#include <windows.h>
using namespace std;

//parametry startX i startY w sumie tylko przeszkadzaja i moglyby byc usuniete
//odpowiadaja one za wspolrzedne lewego gornego roga planszy
//mozna zalozyc, ze wspolrzedne te zawsze wynosza (0,0) i uproscic kod w wielu miejscach
//uzytkownik i tak nie ma wplywu na te wspolrzedne

//manipulacja polozeniem kursora
void przesun_kursor(COORD cel);

//rysowanie ramki otaczajacej plansze
void ramka(int startX, int startY, int szer, int wys);

//stworzenie weza. Gra rozpoczyna sie zawsze z wezem w polowie wysokosci i nieco na lewo od pionowej osi symetrii
void inicjalizuj_weza(list<pair<int, int>>&waz, int dlugosc, int startX, int startY, int szer, int wys);

//wyswietlanie weza na planszy
void wyswietl_weza(list<pair<int, int>> waz);

//zmazywanie weza z mapy
void posprzataj_po_starym_wezu(list<pair<int, int>> waz);

//koniec gry poprzez wjechanie w sciane
bool dotkniecie_ramki(list<pair<int, int>> &waz, int startX, int startY, int szer, int wys);

//koniec gry poprzez wjechanie w weza
bool dotkniecie_samego_siebie(list<pair<int, int>> &waz);

//niewykorzystywana funkcja
void wspolrzedne_weza(list<pair<int, int>> waz);

//umieszczanie robaka, zawsze tylko 1 robak na mapie
void wstaw_robaka(list<pair<int, int>> &waz, pair<int, int> &robak, int startX, int startY, int szer, int wys, bool& jest_robak);

bool zjedzono_robaka(list<pair<int, int>> &waz, pair<int, int> robak, bool&jest_robak);

//wlasciwa gra
void snake(int szer, int wys, int poziom_trudnosci);

//sterowanie gracza
void ruch_gracza(list<pair<int, int>> &waz, char& kierunek, char& poprzedni, COORD& glowa, bool& stop, int startX, int startY, int szer, int wys);

void ruch_automatyczny(list<pair<int, int>> &waz, char& kierunek, COORD& glowa, int startX, int startY, int szer, int wys);

//funkcja oglona - MENU
void gra_snake();