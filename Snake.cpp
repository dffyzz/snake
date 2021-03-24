#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
using namespace std;

const int ScreenWidth = 40;
const int ScreenHeight = 10;

int snakex[(ScreenHeight - 2) * ((ScreenWidth / 2) - 2)];
int snakey[(ScreenHeight - 2) * ((ScreenWidth / 2) - 2)];

enum moove {
	w = 1,
	a,
	s,
	d
};

void apple(int& apple_x, int& apple_y, int& x, int& y, int& score) {
	bool flag = false;
	do {
		flag = false;
		apple_x = (rand() % ((ScreenWidth / 2) - 2) + 1) * 2;
		apple_y = rand() % (ScreenHeight - 2) + 1;
		for (int i = 0; i < score; i ++) {
			if ((snakex[i] == apple_x) && (snakey[i] == apple_y)) { flag = true; }
		}
		if ((apple_x == x) && (apple_y == y)) { flag = true; }
	} while (flag);
}

void setup(wchar_t* screen) {
	for (int i = 0; i < ScreenWidth; i++) {
		for (int j = 0; j < ScreenHeight; j++) {
			if ((i == 0) || (j == 0) || (i == ScreenWidth - 1) || (j == ScreenHeight - 1) || (i == 1) || (i == ScreenWidth - 2)) {
				screen[j * ScreenWidth + i] = 0x2592;		//wall
			}
			else {
				screen[j * ScreenWidth + i] = ' ';			//void
			}
		}
	}
}

int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle(L" Snake by Dffyzz");
	COORD bufferSize = { ScreenHeight, ScreenWidth };
	SetConsoleScreenBufferSize(hConsole, bufferSize);
	HWND hWindowConsole = GetConsoleWindow();
	RECT r;
	GetWindowRect(hWindowConsole, &r);
	MoveWindow(hWindowConsole, r.left, r.top, (ScreenWidth * 725 / 100 + 30), (ScreenHeight * 16 + 40), TRUE);
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];
	bool game = true;
	int x = ScreenWidth / 2;
	int y = ScreenHeight / 2;
	int press = 0, last_press = 0;
	int apple_x = 0, apple_y = 0;
	int score = 0;
	int speed = 0;
	srand(time(0));

	setup(screen);
	wstring sett;
	sett += L"choose speed";
	sett += L"1 - slow    ";
	sett += L"2 - normal  ";
	sett += L"3 - quickly ";
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 12; j++) {
			screen[(ScreenWidth / 3 + ScreenWidth * (ScreenHeight / 3)) + i * ScreenWidth + j] = sett[i * 12 + j];
		}
	}
	WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);

	while (1) {			//	speed
		int sped = _getch();
		if (sped == '1') { speed = 150; break; }
		if (sped == '2') { speed = 100; break; }
		if (sped == '3') { speed = 50; break; }
	}

	setup(screen);
	apple(apple_x, apple_y, x, y, score);
	screen[apple_x + apple_y * ScreenWidth] = '<';
	screen[apple_x + apple_y * ScreenWidth + 1] = '>';
	
	WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);

	while (game) {

		
		if ((GetAsyncKeyState((unsigned short)'W') & 0x0001) || (GetAsyncKeyState(VK_UP) & 0x0001)) { last_press = press; press = moove::w; }
		else {
			if ((GetAsyncKeyState((unsigned short)'A') & 0x0001) || (GetAsyncKeyState(VK_LEFT) & 0x0001)) { last_press = press; press = moove::a; }
			else {
				if ((GetAsyncKeyState((unsigned short)'S') & 0x0001) || (GetAsyncKeyState(VK_DOWN) & 0x0001)) { last_press = press; press = moove::s; }
				else { 
					if ((GetAsyncKeyState((unsigned short)'D') & 0x0001) || (GetAsyncKeyState(VK_RIGHT) & 0x0001)) { last_press = press; press = moove::d; } 
				}
			}
		}


		if ((last_press == moove::w) && (press == moove::s)) { press = moove::w; }		//	anti
		if ((last_press == moove::s) && (press == moove::w)) { press = moove::s; }
		if ((last_press == moove::d) && (press == moove::a)) { press = moove::d; }
		if ((last_press == moove::a) && (press == moove::d)) { press = moove::a; }

		screen[y * ScreenWidth + x] = ' ';		// -head
		screen[y * ScreenWidth + x + 1] = ' ';

		for (int i = score; i >= 0; i--) {		// -tail
			if ((snakey[i] != 0) && (snakex[i] != 0)) {
				screen[snakey[i] * ScreenWidth + snakex[i]] = ' ';
				screen[snakey[i] * ScreenWidth + snakex[i] + 1] = ' ';
			}
			if (i != 0) {
				snakex[i] = snakex[i - 1];
				snakey[i] = snakey[i - 1];
			}
			else {
				snakex[0] = x;
				snakey[0] = y;
			}
		}

		for (int i = 0; i < score; i++) {			// +tail
			screen[snakey[i] * ScreenWidth + snakex[i]] = 0x2588;
			screen[snakey[i] * ScreenWidth + snakex[i] + 1] = 0x2588;
		}

		if (press == moove::w) { y--; }			//moove head
		if (press == moove::s) { y++; }
		if (press == moove::d) { x += 2; }
		if (press == moove::a) { x -= 2; }

		if (y == ScreenHeight - 1) { y = 1; }		//tp
		if (y == 0) { y = ScreenHeight - 2; }
		if (x == ScreenWidth - 2) { x = 2; }
		if (x == 0) { x = ScreenWidth - 4; }

		for (int i = 2; i < score; i++) {			//collision
			if ((snakex[i] == x) && (snakey[i]) == y) { game = false; }
		}

		screen[y * ScreenWidth + x] = 0x2588;		// +head
		screen[y * ScreenWidth + x + 1] = 0x2588;

		if ((x == apple_x) && (y == apple_y)) {		//eat
			score++;
			apple(apple_x, apple_y, x, y, score);
			screen[apple_x + apple_y * ScreenWidth] = '<';
			screen[apple_x + apple_y * ScreenWidth + 1] = '>';
		}

		WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);
		Sleep(speed);
	}

	setup(screen);
	wstring lose = L"you score: ";
	lose += to_wstring(score);
	int k = 0;
	while (score / 10 > 0) { score /= 10; k++; }

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < (12 + k); j++) {
			if (lose[i * (12 + k) + j] != ' ')
				screen[(ScreenWidth / 3 + ScreenWidth * (ScreenHeight / 3)) + i * ScreenWidth + j] = lose[i * (12 + k) + j];
		}
	}
	WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth* ScreenHeight, { 0,0 }, & dwBytesWritten);
	while (_getch() != 13) {}
	return 0;
}