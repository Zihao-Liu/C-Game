#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
using namespace std;

#define INIT_WIDTH 10
#define INIT_HEIGHT 20

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
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++)
					cout << B[i][j];
				cout << endl;
			}
		}
	}
	void Show(int y = 1, int x = 0) {
		//system("cls");
		pos.X = 0;
		pos.Y = y - 2 >= 0 ? y - 2 : 0;
		SetConsoleCursorPosition(hout, pos);
		for (int i = pos.Y; i < height; i++) {
			for (int j = 0; j < width; j++)
				cout << B[i][j];
			cout << endl;
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
	virtual bool Rotate(GameBoard& G, int y, int x) { return true; };
	virtual bool Fall(GameBoard& G, int& y, int& x) { return true; };
	//virtual void FastFall() = 0;
	virtual bool Init(GameBoard& G, int& y, int& x) { return true; };
	virtual bool MoveLeft(GameBoard& G, int& y, int& x) { return true; };
	virtual bool MoveRight(GameBoard& G, int& y, int& x) { return true; };
};

/*正方形方块，只有一种形式
	* * * *
	* # # *
	* # # *
	* * * *
*/
class Square :public Block {
public:
	Square() {

	}

	bool Rotate(GameBoard& G, int y, int x) {
		return true;
	}

	bool Init(GameBoard& G, int& y, int& x) {
		y = 0, x = 4;
		if (G.GetBoard()[1][5] == '#' || G.GetBoard()[1][6] == '#' || G.GetBoard()[2][5] == '#' || G.GetBoard()[2][6] == '#')
			return false;
		else {
			G.SetBoard(2, 5, '#');
			G.SetBoard(2, 6, '#');
			G.SetBoard(1, 5, '#');
			G.SetBoard(1, 6, '#');
			return true;
		}
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
		else
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
		else
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
		else
			return false;
	}

};

/*直形方块，有两种形式
	1. * # * *	 2. * * * *
	   * # * *		# # # #
	   * # * *		* * * *
	   * # * *		* * * *
*/
class Straight :public Block {
private:
	int shape;
public:
	Straight() {

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
			else
				return false;
		}
	}

	bool Init(GameBoard& G, int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 2;
		if (shape == 0) {
			y = 1;
			x = 4;
			if (G.GetBoard()[1][5] == '#' || G.GetBoard()[2][5] == '#' || G.GetBoard()[3][5] == '#' || G.GetBoard()[4][5] == '#')
				return false;
			else {
				G.SetBoard(1, 5, '#');
				G.SetBoard(2, 5, '#');
				G.SetBoard(3, 5, '#');
				G.SetBoard(4, 5, '#');
				return true;
			}
		}
		else {
			y = 0;
			x = 4;
			if (G.GetBoard()[1][4] == '#' || G.GetBoard()[1][5] == '#' || G.GetBoard()[1][6] == '#' || G.GetBoard()[1][7] == '#')
				return false;
			else {
				G.SetBoard(1, 4, '#');
				G.SetBoard(1, 5, '#');
				G.SetBoard(1, 6, '#');
				G.SetBoard(1, 7, '#');
				return true;
			}
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
			else
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
			else
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
			else
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
			else
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
		x = 4;
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
	virtual bool MoveLeft(GameBoard& G, int& y, int& x) {
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
	virtual bool MoveRight(GameBoard& G, int& y, int& x) {
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


int main() {
	GameBoard G(INIT_WIDTH, INIT_HEIGHT);
	G.InitBorder();
	G.Show();
	//char S[4][4];
	Block *A = new Block();
	int currentBlock;
	int x = 4, y = 0;
	while (true) {
		srand((unsigned)time(0));
		currentBlock = rand() % 3;
		switch (currentBlock) {
		case 0: A = new Square();
			break;
		case 1: A = new Straight();
			break;
		case 2: A = new Tpoly();
			break;
		default:break;
		}

		if (A->Init(G, y, x) == false)
			break;
		G.Show();
		while (A->Fall(G, y, x) == true) {

			G.Show(y, x);
			srand((unsigned)time(0));
			int a = rand() % 4;
			if (a == 1)
				A->MoveLeft(G, y, x);
			else if (a == 2)
				A->MoveRight(G, y, x);
			else if (a == 3)
				A->Rotate(G, y, x);
			else {
				A->Fall(G, y, x);
			}
			G.Show(y, x);
			Sleep(200);
		}
		G.Eliminate(y, x);
		delete(A);
	}

	cout << "stop";
	system("pause");

}