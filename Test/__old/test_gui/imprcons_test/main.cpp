//
// Improved Console - Mover Demo
// =============================
//
// Willkommen bei der Einsteiger-Demo der Improved Console. Diese Demo ist vor
// allem für Programmierneulinge gedacht und enthält daher sehr detaillierte
// Kommentare.
//
// Sinn der Demo ist es, ein kleines Spiel zu entwickeln. Der Benutzer kann
// seine Spielfigur über die Win32-Konsole bewegen. Sobald der Spieler die
// Konsole verlässt, verliert er das Spiel. Zugegebenermaßen, nicht gerade ein
// sehr hochtrabendes Spielprinzip, doch reicht es, um die grundlegende
// Arbeitsweise mit der Improved Console zu erlernen.
//eeeee

#include <iostream>
#include <conio.h> // Für _getch()
#include "ic.hpp" // Includieren der Improved Console
using namespace ic; // Auflösen des Improved Console-Namespace
using namespace ic::shorties; // Für die Verwendung von Shorties (Siehe unten)
using namespace std;

int main ()
{
	// Um Zugriff auf die Konsole zu haben verwenden wir das Objekt "con", als
	// erstes Beispiel setzen wir einen neuen Titel für das Konsolenfenster
	con.setTitle(TEXT("Mover Demo"));

	// Als nächstes zeigen wir einen kleinen Begrüßungsbildschirm. Wir setzen
	// dafür eine neue Textfarbe:
	con.setTextColor(FG_RED); // FG_RED steht für Foreground Red

	// Außerdem wollen wir an eine bestimmte Position schreiben:
	con.setCurPos(10,5); // Der Ursprung liegt an der Position (0/0)
	cout << "Mover Demo";

	con.setCurPos(9,6);
	cout << "============";

	// "con.setCurPos" ist uns faulen Programmierern allerdings zu lang, um
	// es jedesmal auszuschreiben. Deswegen gibt es in der Improved Console
	// sogenannte Shorties. Sie haben die Namen der Funktionen die früher in
	// der <conio.h> verwendet wurden. Benützen wir also für den Rest des
	// Begrüßungsbildschirms Shorties:
	textcolor(FG_YELLOW);
	gotoxy(5,9); cout << "Press <s>/<f> for moving left/right";
	gotoxy(5,10); cout << "Press <e>/<d> for moving up/down";
	gotoxy(5,12); cout << "Press <Enter> to start...";
	cin.get();

	// textcolor() ist das Shorty für con.setTextColor() und gotoxy() steht
	// bekannterweise für con.setCurPos()

	// Sobald der Benutzer <Enter> gedrückt hat beginnt das Spiel, dazu
	// initialisieren wir zuerst das Spielfeld:
	clrscr(BG_WHITE); // Bildschirm mit weißer Farbe löschen
	textcolor(FG_BLUE); // Neue Textfarbe setzen
	bgcolor(BG_WHITE); // Neue Hintergrundfarbe setzen

	// Initialisieren der Spielfigur-Position auf die Mitte des Bildschirms
	int x = con.getWndSizeX() / 2;
	int y = con.getWndSizeY() / 2;

	// Spiel-Schleife beginnen
	while(true)
	{
		// Die jeweils aktuelle Position zeichnen
		gotoxy(x,y); cout << '*';

		// Eine Taste einlesen, und die Spielfigur bewegen
		char c = _getch(); // _getch() ließt ein Zeichen von der Tastatur
		switch(c)
		{
			case 'E': case 'e': --y; break;
			case 'D': case 'd': ++y; break;
			case 'S': case 's': --x; break;
			case 'F': case 'f': ++x; break;
		}

		// Sollte die neue Position nicht mehr innerhalb des Spielfeldes liegen,
		// hat der Spieler verloren
		if(x < 0 || x >= con.getWndSizeX() || y < 0 || y >= con.getWndSizeY())
		{
			textcolor(FG_OCHER);
			gotoxy(5,9);
			cout << "You left the screen ... you lost!";
			break; // Spiel-Schleife verlassen
		}
	}

	return 0;
}

// Wie geht es weiter?
// Man lernt sehr leicht, wenn man sich mit der Improved Console etwas
// beschäftigt! Ganz einfach ein neues Projekt anlegen, die Improved Console
// einbinden und loslegen. Gebt "con." in die main()-Funktion ein und schon
// erhaltet ihr eine Liste von Methoden die ihr benützen könnt. Eine Liste
// aller Shorties erhält ihr mit Eingabe von "ic::shorties::"
//
// Viel Spaß!
