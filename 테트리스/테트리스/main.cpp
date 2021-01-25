#include <iostream>
#include <vector>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <algorithm>
using namespace std;

void gotoxy(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int foreground) {
	int color = foreground + 1;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int height = 20;
int width = 10;
int score = 0;

void capStart();
int startPrint();
void loop();
void init();
void deleteLine();
void downLine(int y);

vector<vector<int>> v[7] = {
	{
		{0, 0, 0, 0},
		{ 1, 1, 1, 1 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	},
	{
		{0, 0, 0, 0},
		{0, 2, 2, 0},
		{2, 2, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{0, 0, 0, 0},
		{3, 3, 0, 0},
		{3, 3, 0, 0},
		{0, 0, 0, 0}
	},
	{
		{4, 0, 0},
		{4, 4, 4},
		{0, 0, 0}
	},
	{
		{0, 0, 5},
		{5, 5, 5},
		{0, 0, 0}
	},
	{
		{0, 6, 0},
		{6, 6, 6},
		{0, 0, 0}
	},
	{
		{0, 0, 0, 0},
		{7, 7, 0, 0},
		{0, 7, 7, 0},
		{0, 0, 0, 0}
	}
};

vector<vector<int>> map;

class Matrix {
private:
	vector<vector<int>> board;
	int len, color, xpos, ypos;
public:
	void printBlock() {
		int cnt = 0;
		for (int i = 0; i < len; i++) {
			for (int k = 0; k < len; k++) {
				if (board[i][k]) {
					gotoxy(xpos + k * 2, ypos + i);
					textcolor(color);
					cout << "■";
				}
			}	
		}
	}

	void Lotation() {
		vector<vector<int>> temp;
		temp.resize(len);
		for (int i = 0; i < len; i++) temp[i].resize(len, 0);

		for (int i = 0; i < len; i++) 
			for (int k = 0; k < len; k++) 
				temp[k][len - 1 - i] = board[i][k];

		if (checkMnMxXposYpos(xpos, ypos,temp)) return;
		
		eraseBlock();
		board = temp;
		printBlock();
	}
	
	void storeBlock() {
		int rx = xpos / 2;
		for (int i = ypos; i < ypos + len; i++) {
			for (int k = rx; k < rx + len; k++) {
				if (i < height && k < width && board[i - ypos][k - rx]) {	
					map[i][k] = board[i - ypos][k - rx];	
				}
			}
		}
	}

	bool Down() {
		if (checkMnMxXposYpos(xpos, ypos + 1, board)) {
			storeBlock();
			deleteLine();
			randBlockColor();
			return 0;
		}
		eraseBlock();
		ypos += 1;
		printBlock();
		return 1;
	}

	void Left() {
		if (checkMnMxXposYpos(xpos - 2, ypos, board)) return;
		eraseBlock();
		xpos -= 2;
		printBlock();
	}

	void Right() {
		if (checkMnMxXposYpos(xpos + 2, ypos, board)) return;
		eraseBlock();
		xpos += 2;
		printBlock();
	}

	bool checkMnMxXposYpos(int xpos,int ypos,vector<vector<int>> &board) {
		int mnx = 1e9, mxx = 0, mny = 1e9, mxy = 0;

		int rx = xpos / 2;
		for (int i = ypos; i < ypos + len; i++) {
			for (int k = rx; k < rx + len; k++) {
				if (k >= 0 && i < height && k < width && map[i][k] && board[i - ypos][k - rx]) return 1;
				if (board[i - ypos][k - rx]) {
					mnx = min(mnx, k);
					mxx = max(mxx, k);
					mny = min(mny, i);
					mxy = max(mxy, i);
				}
			}
		}
		if (mnx < 0 || mny < 0 || mxx >= width || mxy >= height) return 1;
		return 0;
	}

	void eraseBlock() {
		for (int i = 0; i < len; i++) {
			for (int k = 0; k < len; k++) {
				if (board[i][k]) {
					gotoxy(xpos + k * 2 + 1, ypos + i);
					cout << " ";
					gotoxy(xpos + k * 2, ypos + i);
					cout << " ";
				}
			}
		}
	}

	int getLength() {
		return len;
	}

	void randBlockColor() {
		int randData = rand() % 7;
		this->xpos = 4;
		this->ypos = 0;
		this->board = v[randData];
		this->len = v[randData].size();
		this->color = randData;
	}
};

void downLine(int y) {
	for (int i = y - 1; i >= 0; i--) {
		for (int k = 0; k < width; k++) {
			map[i + 1][k] = map[i][k];
		}
	}
	for (int k = 0; k < width; k++) map[0][k] = 0;
}

void printScoreLevel() {
	gotoxy(40, 10);
	cout << "level - " << (score * 10) / 100 + 1;
	gotoxy(40, 11);
	cout << "score - " << score * 10;
}

void deleteLine() {
	int line = 0;
	for (int i = height - 1; i >= 0; i--) {
		int cnt = 0;
		for (int k = 0; k < width; k++) {
			if (map[i][k]) cnt++;
		}

		if (cnt == width) { // 한줄이 모두 채워지면
			downLine(i); 
			line++;
		}
	}

	score += line;
	printScoreLevel();

	for (int i = 0; i < height; i++) {
		for (int k = 0; k < width; k++) {
			if (!map[i][k]) {
				gotoxy(k * 2 + 1, i);
				cout << " ";
				gotoxy(k * 2, i);
				cout << " ";
			}
			else {
				gotoxy(k * 2, i);
				textcolor(map[i][k] - 1);
				cout << "■";
			}
		}
	}
}
Matrix *block;

void boardInit(int height, int width) {
	map.resize(height);
	for (int i = 0; i < height; i++) map[i].resize(width,0);
}

void capStart() {
	while (1) {
		init();
		if (startPrint()) return;
		loop();
	}
}

void init() {
	srand(time(NULL));
	boardInit(height, width);
}

int startPrint() {
	system("cls");
	cout << "=========소문자 s 시작 w 종료 =========\n";
	char ch;
	while ((ch = _getch()) != 's') 
		if (ch == 'w') return 1;
	system("cls");
	return 0;
}

void blockMove(char ch) {
	if (ch == 'a') block->Left();
	else if (ch == 'd') block->Right();
	else if (ch == 's') block->Down();
	else if (ch == 32) while (block->Down());
	else if (ch == 'r') block->Lotation();
}

void loop() {
	int blockDownTime = GetTickCount();
	int downSpeed = 1000;

	block = new Matrix();
	block->randBlockColor();
	block->printBlock(); 
	score = 0;
	printScoreLevel();

	while (1) {
		if (_kbhit()) {
			char ch = _getch();
			blockMove(ch);
		}
		int newDownTime = GetTickCount();
		if (newDownTime - blockDownTime >= downSpeed) {
			blockDownTime = newDownTime;
			block->Down();
		}
	}
}

int main() {
	ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	capStart();
}



