#include<stdio.h>
#include"conio2.h"

/* Uwaga: w docelowym programie nale�y zadeklarowa� odpowiednie
   sta�e, na przyk�ad po to, aby wyeliminowa� z programu
   warto�ci numeryczne umieszczajac w ich miejsce 
   dobrze dobrane identyfikatory */

int main() {
	int zn = 0, x = 40, y = 12, attr = 7, back = 0, zero = 0;
	char txt[32];
	// je�eli program jest kompilowany w czystym j�zyku C
	// nale�y wywo�a� poni�sz� lini�
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle ustawia tytu� okienka
	settitle("imie nazwisko nr_indeksu");
	do {
		// ustawiamy czarny kolor t�a, z kt�rego b�dziemy
		// korzystali rysuj�c po ekranie
		// prosz� zerkn�� do conio2.h na list� dost�pnych kolor�w
		textbackground(BLACK);
		// czy�cimy ekran: wype�niamy spacjami z ustawionym kolrem t�a
		clrscr();
		// ustawiamy kolor tekstu na jasnoszary (7 == LIGHTGRAY)
		textcolor(7);
		// przesuwamy kursor do kolumny 55 i wiersza 1
		// wiersze i kolumny liczymy od 1
		gotoxy(55, 1);
		// wy�wietlamy na ekranie, w miejscu kursora napis
		// kursor przesuwa si� w prawo o d�ugo�� napisu
		cputs("q = wyjscie");
		gotoxy(55, 2);
		cputs("strzalki = poruszanie");
		gotoxy(55, 3);
		cputs("spacja = zmiana koloru");
		gotoxy(55, 4);
		cputs("enter = zmiana koloru tla");
		// wypisujemy na ekranie kod ostatnio naci�ni�tego klawisza
		if(zero) sprintf(txt, "kod klawisza: 0x00 0x%02x", zn);
		else sprintf(txt, "kod klawisza: 0x%02x", zn);
		
		gotoxy(55, 5);
		cputs(txt);

		// rysujemy na ekranie kolorow� gwiazdk�
		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		// putch rysuje jeden znak i przesuwa kursor w prawo
		putch('*');

		// czekamy na naci�ni�cie klawisza i odczytujemy jego kod,
		// wi�kszo�� kod�w klawiszy odpowiada ich znakom, np.
		// a to 'a', 2 to '2', + to '+', ale niekt�re klawisze
		// specjalne, np. strza�ki s� zg�aszane jako dwa kolejne znaki,
		// z kt�rych pierwszy jest zerem, np. strza�ka w g�r�
		// to zero i 'H'
		zero = 0;
		zn = getch();
		// nie chcemy, aby klawisz 'H' dzia�a� jako strza�ka w g�r�
		// wi�c sprawdzamy, czy pierwszym znakiem by�o zero
		if(zn == 0) {
			zero = 1;		// je�eli tak, to czytamy
			zn = getch();		// kolejny znak wiedz�c ju�,
			if(zn == 0x48) y--;	// �e b�dzie to znak specjalny
			else if(zn == 0x50) y++;
			else if(zn == 0x4b) x--;
			else if(zn == 0x4d) x++;
		} else if(zn == ' ') attr = (attr + 1) % 16;
		else if(zn == 0x0d) back = (back + 1) % 16;
	} while (zn != 'q');

	return 0;
	}
