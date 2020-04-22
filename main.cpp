#include<stdio.h>
#include"conio2.h"

/* Uwaga: w docelowym programie nale¿y zadeklarowaæ odpowiednie
   sta³e, na przyk³ad po to, aby wyeliminowaæ z programu
   wartoœci numeryczne umieszczajac w ich miejsce 
   dobrze dobrane identyfikatory */

int main() {
	int zn = 0, x = 40, y = 12, attr = 7, back = 0, zero = 0;
	char txt[32];
	// je¿eli program jest kompilowany w czystym jêzyku C
	// nale¿y wywo³aæ poni¿sz¹ liniê
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle ustawia tytu³ okienka
	settitle("imie nazwisko nr_indeksu");
	do {
		// ustawiamy czarny kolor t³a, z którego bêdziemy
		// korzystali rysuj¹c po ekranie
		// proszê zerkn¹æ do conio2.h na listê dostêpnych kolorów
		textbackground(BLACK);
		// czyœcimy ekran: wype³niamy spacjami z ustawionym kolrem t³a
		clrscr();
		// ustawiamy kolor tekstu na jasnoszary (7 == LIGHTGRAY)
		textcolor(7);
		// przesuwamy kursor do kolumny 55 i wiersza 1
		// wiersze i kolumny liczymy od 1
		gotoxy(55, 1);
		// wyœwietlamy na ekranie, w miejscu kursora napis
		// kursor przesuwa siê w prawo o d³ugoœæ napisu
		cputs("q = wyjscie");
		gotoxy(55, 2);
		cputs("strzalki = poruszanie");
		gotoxy(55, 3);
		cputs("spacja = zmiana koloru");
		gotoxy(55, 4);
		cputs("enter = zmiana koloru tla");
		// wypisujemy na ekranie kod ostatnio naciœniêtego klawisza
		if(zero) sprintf(txt, "kod klawisza: 0x00 0x%02x", zn);
		else sprintf(txt, "kod klawisza: 0x%02x", zn);
		
		gotoxy(55, 5);
		cputs(txt);

		// rysujemy na ekranie kolorow¹ gwiazdkê
		gotoxy(x, y);
		textcolor(attr);
		textbackground(back);
		// putch rysuje jeden znak i przesuwa kursor w prawo
		putch('*');

		// czekamy na naciœniêcie klawisza i odczytujemy jego kod,
		// wiêkszoœæ kodów klawiszy odpowiada ich znakom, np.
		// a to 'a', 2 to '2', + to '+', ale niektóre klawisze
		// specjalne, np. strza³ki s¹ zg³aszane jako dwa kolejne znaki,
		// z których pierwszy jest zerem, np. strza³ka w górê
		// to zero i 'H'
		zero = 0;
		zn = getch();
		// nie chcemy, aby klawisz 'H' dzia³a³ jako strza³ka w górê
		// wiêc sprawdzamy, czy pierwszym znakiem by³o zero
		if(zn == 0) {
			zero = 1;		// je¿eli tak, to czytamy
			zn = getch();		// kolejny znak wiedz¹c ju¿,
			if(zn == 0x48) y--;	// ¿e bêdzie to znak specjalny
			else if(zn == 0x50) y++;
			else if(zn == 0x4b) x--;
			else if(zn == 0x4d) x++;
		} else if(zn == ' ') attr = (attr + 1) % 16;
		else if(zn == 0x0d) back = (back + 1) % 16;
	} while (zn != 'q');

	return 0;
	}
