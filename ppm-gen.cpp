#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

/*
	Jakub Namyslak, 18.12.2023
	Wiecej na:
	- https://shatterwares.com
	- https://github.com/jakubekgranie	
*/

double countTime(clock_t start, clock_t end) {
	return (double)(end - start) / CLOCKS_PER_SEC * 1000;
}
int main() {


	clock_t start = clock(), startGen, endGen;
	fstream file;
	file.open("bitmap.ppm", ios::out);
	int y, x, mode = 1, b;
	bool genFailed = false;
	y = x = b = 0;
	cout << "---------------------------------\nPodaj wysokosc (h e <0, 255>):\n[H]> ";
	cin >> y;
	if (y < 0 || y > 255) {
		cout << "[H] Nieprawidlowa wysokosc. Wartosc ustawiona na 10.\n";
		y = 10;
	}
	cout << "\nPodaj szerokosc (w e <0, 255>):\n[W]> ";
	cin >> x;
	if (x < 0 || x > 255) {
		cout << "[W] Nieprawidlowa szerokosc. Wartosc ustawiona na 10.\n";
		x = 10;
	}
	cout << "\nWybierz tryb:\n1) Standardowy (B = 0)\n2) Pollosowy (B e <0, 255>)\n3) Losowy (R && G && B e <0, 255>)\n4) Wlasny (B jest wybierane manualnie)\n\n[TRYB]> ";
	cin >> mode;
	if (mode == 4) {
		cout << "\nPodaj B (e <0, 255>).\n[B]> ";
		cin >> b;
		if (b < 0 || b > 255)
			cout << "[B] Nieprawidlowe B. Wartosc ustawiona na 0.\n";
		else
			cout << "[B] B ustawiono pomyslnie na: " << b  << "\n";
	}
	cout << "\n[GEN] Trwa generowanie pliku .ppm. Prosze czekac.\n";
	startGen = clock();
	if (file.good()) {
		file << "# Jakub Namyslak, 2023\n# https://shatterwares.com\n\nP3 # header\n" << y << " " << x << " # wysokosc, szerokosc (rzedy, kolumny)\n255 # przestrzen kolorow\n";
		for (int i = 0; i < y; i++) {
			file << "\n";
			for (int j = 0; j < x; j++) {
				if (mode == 2)
					b = rand() % 255;
				if (mode == 3)
					file << rand() % 255 << "  " << rand() % 255 << "  " << rand() % 255 << "  ";
				else
					file << i << "  " << j << "  " << b << "  ";
			}
		}
		file.close();
		cout << "[GEN] Plik wygenerowany pomyslnie.";
	}
	else
		cout << "[GEN] Wystapil blad dostepu do pliku.";
	endGen = clock();
	cout << "\n\n[T] PELNY CZAS: " << countTime(start, clock()) << "ms\n[T] CZAS GENEROWANIA PLIKU: ";
	if (!genFailed) cout << countTime(startGen, endGen) << "ms";
	else cout << "FAILED";
	cout << "\n---------------------------------";
	return 0;
}