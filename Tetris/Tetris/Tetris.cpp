#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
using namespace std;

#define INIT_WIDTH 20
#define INIT_HEIGHT 30
#define LEFT_MARGIN 65
#define TOP_MARGIN 5
#define MIDDLE 9

class GameBoard {
private:
	vector<vector<char>> B;
	int width;
	int height;
	HANDLE hout;
	COORD pos;
public:
	GameBoard(int width = INIT_WIDTH, int height = INIT_HEIGHT) {
		this->width = width + 2;
		this->height = height + 2;
		hout = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursor_info = { 1,0 };
		SetConsoleCursorInfo(hout, &cursor_info);
		pos.X = LEFT_MARGIN + (this->width+10)/2;
		pos.Y = TOP_MARGIN / 2;
		SetConsoleCursorPosition(hout, pos);
		cout << "俄 罗 斯 方 块";
		vector<char> A(this->width, ' ');
		for (int i = 0; i < this->height; i++) {
			B.push_back(A);
		}
		A.clear();
	}
	void InitBorder() {
		for (int i = 0; i < width; i++) {
			B[0][i] = '*';
			B[height - 1][i] = '*';
		}
		for (int i = 0; i < height; i++) {
			B[i][0] = '*';
			B[i][width - 1] = '*';
		}

	}
	void Eliminate(int y, int x) {
		int i = y;
		int flag;
		bool flag2 = false;
		for (i; i <= y + 3 && i < height - 1; i++) {
			flag = 0;
			for (int j = 1; j < width - 1; j++) {
				if (B[i][j] != '#') {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				flag2 = true;
				for (int k = i; k > 1; k--) {
					for (int j = 1; j < width - 1; j++)
						B[k][j] = B[k - 1][j];
				}
				i--;
			}
		}
		if (flag2) {
			pos.Y = TOP_MARGIN;
			pos.X = LEFT_MARGIN;
			SetConsoleCursorPosition(hout, pos);
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
					cout << B[i][j]<<' ';
				cout << endl;
			}
		}
	}
	void Show(int y = 1, int x = 0) {
		//system("cls");
		pos.Y = y - 2 >= 0 ? y - 2 : 0;
		for (int i = pos.Y; i < height; i++) {
			pos.Y = i + TOP_MARGIN;
			pos.X = LEFT_MARGIN;
			SetConsoleCursorPosition(hout, pos);
			for (int j = 0; j < width; j++)
				cout << B[i][j]<<' ';
		}

	}

	int GetWidth() {
		return width;
	}
	int GetHeight() {
		return height;
	}
	void SetBoard(int y, int x, char val) {
		B[y][x] = val;
	}
	vector<vector<char>> GetBoard() {
		return B;
	}
};

class Block {
private:
	int shape;
public:
	Block() {
		shape = 0;
	}

	//virtual void FastFall() = 0;
	virtual bool Rotate(GameBoard& G, int y, int x) { return true; };
	virtual bool Fall(GameBoard& G, int& y, int& x) { return true; };
	virtual bool Init(GameBoard& G, int& y, int& x) { return true; };
	virtual bool MoveLeft(GameBoard& G, int& y, int& x) { return true; };
	virtual bool MoveRight(GameBoard& G, int& y, int& x) { return true; };
};

/* 正方形方块，只有一种形式
	* * * *
	* # # *
	* # # *
	* * * *
*/
class Square :public Block {
public:
	Square() { }

	bool Init(GameBoard& G, int& y, int& x) {
		y = 0, x = MIDDLE;
		if (G.GetBoard()[y + 1][x + 1] == ' ' && G.GetBoard()[y + 1][x + 2] == ' ' && G.GetBoard()[y + 2][x + 1] == ' ' && G.GetBoard()[y + 2][x + 2] == ' ') {

			G.SetBoard(y + 1, x + 1,  '#');
			G.SetBoard(y + 2, x + 1,  '#');
			G.SetBoard(y + 1, x + 2, '#');
			G.SetBoard(y + 2, x + 2,  '#');

			return true;
		}
		return false;
	}

	bool Rotate(GameBoard& G, int y, int x) {
		return true;
	}

	bool Fall(GameBoard& G, int& y, int& x) {
		if (G.GetBoard()[y + 3][x + 1] == ' '&& G.GetBoard()[y + 3][x + 2] == ' ') {
			G.SetBoard(y + 1, x + 1, ' ');
			G.SetBoard(y + 1, x + 2, ' ');
			G.SetBoard(y + 3, x + 1, '#');
			G.SetBoard(y + 3, x + 2, '#');
			y++;
			return true;
		}
		return false;
	}

	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (G.GetBoard()[y + 1][x] == ' '&& G.GetBoard()[y + 2][x] == ' ') {
			G.SetBoard(y + 1, x + 2, ' ');
			G.SetBoard(y + 2, x + 2, ' ');
			G.SetBoard(y + 1, x, '#');
			G.SetBoard(y + 2, x, '#');
			x--;
			return true;
		}
		return false;
	}

	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (G.GetBoard()[y + 1][x + 3] == ' '&& G.GetBoard()[y + 2][x + 3] == ' ') {
			G.SetBoard(y + 1, x + 1, ' ');
			G.SetBoard(y + 2, x + 1, ' ');
			G.SetBoard(y + 1, x + 3, '#');
			G.SetBoard(y + 2, x + 3, '#');
			x++;
			return true;
		}
		return false;
	}

};

/* 直形方块，有两种形式
	1. * # * *	 2. * * * *
	   * # * *		# # # #
	   * # * *		* * * *
	   * # * *		* * * *
*/
class Straight :public Block {
private:
	int shape;
public:
	Straight() { }

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 2;
		if (shape == 0) {
			y = 1;
			x = MIDDLE;
			if (G.GetBoard()[y][x + 1] == ' ' && G.GetBoard()[y + 1][x + 1] == ' ' && G.GetBoard()[y + 2][x + 1] == ' ' && G.GetBoard()[y + 3][x + 1] == ' ') {

				G.SetBoard(y, x + 1, '#');
				G.SetBoard(y+1, x + 1, '#');
				G.SetBoard(y+2, x + 1, '#');
				G.SetBoard(y+3, x + 1, '#');
				return true;
			}
			return false;
		}
		else {
			y = 0;
			x = MIDDLE;
			if (G.GetBoard()[y + 1][x] == ' ' && G.GetBoard()[y + 1][x + 1] == ' ' && G.GetBoard()[y + 1][x + 2] == ' ' && G.GetBoard()[y + 1][x + 3] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 1, x + 3, '#');
				return true;
			}
			return false;
		}

	}

	bool Rotate(GameBoard& G, int y, int x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 1][x + 3] == ' ') {
				shape = 1;
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 1, x + 3, '#');
				return true;
			}
			else
				return false;
		}
		else {
			if (G.GetBoard()[y][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 3][x + 1] == ' ') {
				shape = 0;
				G.SetBoard(y, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 1, x, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				return true;
			}
			return false;
		}
	}



	bool Fall(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 4][x + 1] == ' ') {
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 4, x + 1, '#');
				y++;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y + 1, x, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				y++;
				return true;
			}
			return false;
		}

	}

	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y][x] == ' '&&G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y, x, '#');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 3, x, '#');
				x--;
				return true;
			}
			else
				return false;
		}
		else {
			if (G.GetBoard()[y + 1][x - 1] == ' ') {
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 1, x - 1, '#');
				x--;
				return true;
			}
			return false;
		}

	}

	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y][x + 2] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y, x + 2, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				x++;
				return true;
			}
			else
				return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 3] == ' ') {
				G.SetBoard(y + 1, x - 1, ' ');
				G.SetBoard(y + 1, x + 3, '#');
				x++;
				return true;
			}
			return false;
		}
	}

};

/* T形方块，有四种形式
	1. * * * *   2. * * * *   3. * * * *   4. * * * *
	   * # * *		* # * *		 * * * *	  * # * *
	   # # # *		* # # *		 # # # *	  # # * *
	   * * * *		* # * *		 * # * *	  * # * *
*/
class Tpoly :public Block {
private:
	int shape;
public:
	Tpoly() {

	}

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 4;
		x = MIDDLE;
		if (shape == 0) {
			y = 0;
			if (G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 1][x + 1] == ' ') {
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 1, x + 1, '#');
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			y = -1;
			if (G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 1] == ' ') {
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 1, '#');
				return true;
			}
			return false;
		}
		else {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 2][x] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 2, x, '#');
				return true;
			}
			return false;
		}
	}

	bool Rotate(GameBoard& G, int y, int x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 3][x + 1] == ' ') {
				shape = 1;
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 2, x, ' ');
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 2][x] == ' ') {
				shape = 2;
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x + 1] == ' ') {
				shape = 3;
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 2, ' ');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 2][x + 2] == ' ') {
				shape = 0;
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 1, ' ');
				return true;
			}
			return false;
		}
	}

	bool Fall(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 3][x] == ' '&&G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 4][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 4, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 3][x] == ' '&&G.GetBoard()[y + 4][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 4, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 4][x + 1] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y + 4, x + 1, '#');
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x, ' ');
				y++;
				return true;
			}
			return false;
		}
	}
	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 2][x - 1] == ' '&&G.GetBoard()[y + 1][x] == ' ') {
				G.SetBoard(y + 2, x - 1, '#');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 2][x - 1] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y + 2, x - 1, '#');
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x - 1] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x - 1, '#');
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				x--;
				return true;
			}
			return false;
		}
	}
	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 1][x + 2] == ' ') {
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
	}

};

/* 左偏L形方块，有四种形式
	1. * * # *   2. * * * *   3. * * * *   4. * * * *
	   * * # *		* # * *		 * # # *	  * # # #
	   * # # *		* # # #		 * # * *	  * * * #
	   * * * *		* * * *		 * # * *	  * * * *

*/
class LeftL :public Block {
private:
	int shape;
public:
	LeftL() {

	}

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 4;
		x = MIDDLE;
		if (shape == 0) {
			y = 1;
			if (G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y][x + 2] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y, x + 2, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 2, '#');
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 2, x + 2, '#');
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 1][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 3, x + 1, '#');
				return true;
			}
			return false;
		}
		else {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 3, '#');
				return true;
			}
			return false;
		}
	}

	bool Rotate(GameBoard& G, int y, int x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 1][x + 1] == ' ') {
				shape = 1;
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y, x + 2, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 3][x + 1] == ' ') {
				shape = 2;
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				shape = 3;
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y][x + 2] == ' ') {
				shape = 0;
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y, x + 2, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				return true;
			}
			return false;
		}
	}

	bool Fall(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y, x + 2, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' '&&G.GetBoard()[y + 3][x + 3] == ' ') {
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 3, x + 3, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 4][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 4, x + 1, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 3] == ' ') {
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 3, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				y++;
				return true;
			}
			return false;
		}
	}

	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y][x + 1] == ' '&&G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x] == ' ') {
				G.SetBoard(y, x + 1, '#');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y, x + 2, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 3, x, '#');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				x--;
				return true;
			}
			return false;
		}
	}
	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y][x + 3] == ' '&&G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y, x + 3, '#');
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y, x + 2, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 4] == ' ') {
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 4, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 1][x + 4] == ' '&&G.GetBoard()[y + 2][x + 4] == ' ') {
				G.SetBoard(y + 1, x + 4, '#');
				G.SetBoard(y + 2, x + 4, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				x++;
				return true;
			}
			return false;
		}
	}

};

/* 左偏L形方块，有四种形式
	1. * # * *   2. * * * *   3. * * * *   4. * * * *
	   * # * *		* # # #		 * # # *	  * * * #
	   * # # *		* # * *		 * * # *	  * # # #
	   * * * *		* * * *		 * * # *	  * * * *

*/
class RightL :public Block {
private:
	int shape;
public:
	RightL() {

	}

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 4;
		x = MIDDLE;
		if (shape == 0) {
			y = 1;
			if (G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y][x + 1] == ' '&&G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' ') {
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y, x + 1, '#');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 1] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 1, '#');
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			y = 0;
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				return true;
			}
			return false;
		}
		else {
			y = 0;
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				return true;
			}
			return false;
		}
	}

	bool Rotate(GameBoard& G, int y, int x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 1][x + 2] == ' ') {
				shape = 1;
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				shape = 2;
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 2][x + 1] == ' ') {
				shape = 3;
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 3, x + 2, ' ');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y][x + 1] == ' '&&G.GetBoard()[y + 1][x + 1] == ' ') {
				shape = 0;
				G.SetBoard(y, x + 1, '#');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				return true;
			}
			return false;
		}
	}

	bool Fall(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 4][x + 2] == ' ') {
				G.SetBoard(y + 4, x + 2, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				y++;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' '&&G.GetBoard()[y + 3][x + 3] == ' ') {
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 3, x + 3, '#');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 1, x + 3, ' ');
				y++;
				return true;
			}
			return false;
		}
	}

	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y][x] == ' '&&G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' ') {
				G.SetBoard(y, x, '#');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 3][x + 1] == ' ') {
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 2, ' ');
				x--;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 1][x + 2] == ' ') {
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				x--;
				return true;
			}
			return false;
		}
	}

	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y][x + 2] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y, x + 2, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 1) {
			if (G.GetBoard()[y + 1][x + 4] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 4, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 2) {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 3][x + 3] == ' ') {
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 3, x + 3, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 2, ' ');
				x++;
				return true;
			}
			return false;
		}
		else if (shape == 3) {
			if (G.GetBoard()[y + 1][x + 4] == ' '&&G.GetBoard()[y + 2][x + 4] == ' ') {
				G.SetBoard(y + 1, x + 4, '#');
				G.SetBoard(y + 2, x + 4, '#');
				G.SetBoard(y + 1, x + 3, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				x++;
				return true;
			}
			return false;
		}
	}

};

/* Z形方块，有两种形式
	1. * * * *   2. * * * * 
	   * # # *		* * # *	
	   * * # #		* # # *	
	   * * * *		* # * *		

*/

class Zpoly : public Block {
private:
	int shape;
public:
	Zpoly() {};

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 2;
		x = MIDDLE;
		y = 0;
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 1] == ' ') {
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 1, '#');
				return true;
			}
			return false;
		}
	}

	bool Rotate(GameBoard& G, int y, int x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 3][x + 1] == ' ') {
				shape = 1;
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 3] == ' ') {
				shape = 0;
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 3, '#');
				return true;
			}
			return false;
		}
	}
	bool Fall(GameBoard& G, int& y, int& x) { 
		if (shape == 0) {
			if (G.GetBoard()[y+2][x+1] == ' '&&G.GetBoard()[y+3][x+2] == ' '&&G.GetBoard()[y+3][x+3] == ' ') {
				G.SetBoard(y+1,x+1,' ');
				G.SetBoard(y+1,x+2,' ');
				G.SetBoard(y+2,x+3,' ');
				G.SetBoard(y+2,x+1,'#');
				G.SetBoard(y+3,x+2,'#');
				G.SetBoard(y+3,x+3,'#');
				y++;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 4][x + 1] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 4, x + 1, '#');
				G.SetBoard(y + 3, x + 2, '#');
				y++;
				return true;
			}
			return false;
		}
	}

	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' ') {
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 3, ' ');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x + 1, '#');
				x--;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x ] == ' '&&G.GetBoard()[y + 3][x ] == ' ') {
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x , '#');
				G.SetBoard(y + 3, x , '#');
				x--;
				return true;
			}
			return false;
		}
	}

	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x+3] == ' '&&G.GetBoard()[y + 2][x + 4] == ' ') {
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 1, x+3, '#');
				G.SetBoard(y + 2, x + 4, '#');
				x++;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x+3] == ' '&&G.GetBoard()[y + 3][x+2] == ' ') {
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x+3, '#');
				G.SetBoard(y + 3, x+2, '#');
				x++;
				return true;
			}
			return false;
		}
	}
};

/* S形方块，有两种形式
	1. * * * *   2. * * * *
	   * # # *		* # * *
	   # # * *		* # # *
	   * * * *		* * # *

*/

class Spoly : public Block {
private:
	int shape;
public:
	Spoly() {};

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 2;
		x = MIDDLE;
		y = 0;
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x , '#');
				G.SetBoard(y + 2, x + 1, '#');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 1] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 1, '#');
				G.SetBoard(y + 2, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				return true;
			}
			return false;
		}
	}

	bool Rotate(GameBoard& G, int y, int x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 3][x + 2] == ' ') {
				shape = 1;
				G.SetBoard(y + 2, x + 2, '#');
				G.SetBoard(y + 3, x + 2, '#');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x, ' ');
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x] == ' ') {
				shape = 0;
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 2, ' ');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x , '#');
				return true;
			}
			return false;
		}
	}
	bool Fall(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 3][x] == ' '&&G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 3, x , '#');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 2, x + 2, '#');
				y++;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 3][x + 1] == ' '&&G.GetBoard()[y + 4][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 1, '#');
				G.SetBoard(y + 4, x + 2, '#');
				y++;
				return true;
			}
			return false;
		}
	}

	bool MoveLeft(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x - 1] == ' ') {
				G.SetBoard(y + 1, x + 2, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x - 1, '#');
				x--;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x] == ' '&&G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 3][x+1] == ' ') {
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 2, ' ');
				G.SetBoard(y + 3, x + 2, ' ');
				G.SetBoard(y + 1, x, '#');
				G.SetBoard(y + 2, x, '#');
				G.SetBoard(y + 3, x+1, '#');
				x--;
				return true;
			}
			return false;
		}
	}

	bool MoveRight(GameBoard& G, int& y, int& x) {
		if (shape == 0) {
			if (G.GetBoard()[y + 1][x + 3] == ' '&&G.GetBoard()[y + 2][x + 2] == ' ') {
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x, ' ');
				G.SetBoard(y + 1, x + 3, '#');
				G.SetBoard(y + 2, x + 2, '#');
				x++;
				return true;
			}
			return false;
		}
		else {
			if (G.GetBoard()[y + 1][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' '&&G.GetBoard()[y + 3][x + 3] == ' ') {
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 2, ' ');
				G.SetBoard(y + 1, x + 2, '#');
				G.SetBoard(y + 2, x + 3, '#');
				G.SetBoard(y + 3, x + 3, '#');
				x++;
				return true;
			}
			return false;
		}
	}
};



int main() {
	system("mode con cols=173 lines=50");
	GameBoard G(INIT_WIDTH, INIT_HEIGHT);
	G.InitBorder();
	G.Show();
	Block *A = new Block();
	int currentBlock;
	int x = MIDDLE, y = 0;
	while (true) {
		srand((unsigned)time(0));
		currentBlock = rand() % 7;
		switch (currentBlock) {
		case 0: A = new Square();
			break;
		case 1: A = new Straight();
			break;
		case 2: A = new Tpoly();
			break;
		case 3: A = new LeftL();
			break;
		case 4: A = new RightL();
			break;
		case 5: A = new Zpoly();
			break;
		case 6:A = new Spoly();
			break;
		default:break;
		}

		if (A->Init(G, y, x) == false)
			break;
		G.Show();
		while (A->Fall(G, y, x) == true) {

			G.Show(y, x);
			srand((unsigned)time(0));
			int a = rand() % 10;
			if (a < 2)
				A->MoveLeft(G, y, x);
			else if (a < 5)
				A->MoveRight(G, y, x);
			else if (a < 9)
				A->Rotate(G, y, x);
			else {
				A->Fall(G, y, x);
			}
			G.Show(y, x);
			Sleep(100);
		}
		G.Eliminate(y, x);
		delete(A);
	}

	cout << "stop";
	system("pause");

}