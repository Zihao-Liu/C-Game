#include <iostream>
#include <vector>
#include <Windows.h>
#include <ctime>
using namespace std;

#define INIT_WIDTH 10
#define INIT_HEIGHT 16

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
		vector<char> A(this->width,' ');
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
	void Show() {
		system("cls");
		for (int i = 0; i < height; i++) {
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
	vector<vector<char>> GetBoard(){
		return B;
	}
};


class Block {
private:
	int Shape;
public:
	virtual void GetShape(char S[4][4],int &y, int& x) {};
	virtual void Rotate(char S[4][4]) {};
	virtual bool Fall(GameBoard& G, int& y, int& x) { return true; };
	//virtual void FastFall() = 0;
	virtual bool Init(GameBoard& G) { return true; };
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
private:
	const char shape[4][4] = { {' ',' ',' ',' '},{' ','#','#',' '},{ ' ','#','#',' ' },{ ' ',' ',' ',' ' } };
public:
	Square() {

	}
	void GetShape(char S[4][4],int& y,int& x) {
		y = 0, x = 4;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				S[i][j] = shape[i][j];
			}
		}
	}
	void Rotate(char S[4][4]) {
		return;
	}

	bool Init(GameBoard& G) {
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
		if (G.GetBoard()[y + 1][x+3] == ' '&& G.GetBoard()[y + 2][x+3] == ' ') {
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
	const char shape1[4][4] = { { ' ','#',' ',' ' },{ ' ','#',' ',' ' },{ ' ','#',' ',' ' },{ ' ','#',' ',' ' } };
	const char shape2[4][4] = { { ' ',' ',' ',' ' },{ '#','#','#','#' },{ ' ',' ',' ',' ' },{ ' ',' ',' ',' ' } };
public:
	Straight() {

	}
	void GetShape(char S[4][4],int& y, int& x) {
		srand((unsigned)time(0));
		shape = rand() % 2;
		if (shape == 0) {
			y = 1, x = 4;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					S[i][j] = shape1[i][j];
				}
			}
		}
		else {
			y = 0, x = 4;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					S[i][j] = shape2[i][j];
				}
			}
		}
	}
	void Rotate(char S[4][4]) {
		return;
	}

	bool Init(GameBoard& G) {
		if (shape == 0) {
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
			if (G.GetBoard()[y + 2][x] == ' '&&G.GetBoard()[y + 2][x + 1] == ' '&&G.GetBoard()[y + 2][x + 2] == ' '&&G.GetBoard()[y + 2][x + 3] == ' '){
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
			if (G.GetBoard()[y][x] == ' '&&G.GetBoard()[y+1][x] == ' '&&G.GetBoard()[y+2][x] == ' '&&G.GetBoard()[y + 3][x] == ' ') {
				G.SetBoard(y, x+1, ' ');
				G.SetBoard(y + 1, x+1, ' ');
				G.SetBoard(y + 2, x+1, ' ');
				G.SetBoard(y + 3, x+1, ' ');
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
			if (G.GetBoard()[y+1][x-1] == ' ') {
				G.SetBoard(y+1, x + 3, ' ');
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
			if (G.GetBoard()[y][x+2] == ' '&&G.GetBoard()[y + 1][x+2] == ' '&&G.GetBoard()[y + 2][x+2] == ' '&&G.GetBoard()[y + 3][x+2] == ' ') {
				G.SetBoard(y, x + 1, ' ');
				G.SetBoard(y + 1, x + 1, ' ');
				G.SetBoard(y + 2, x + 1, ' ');
				G.SetBoard(y + 3, x + 1, ' ');
				G.SetBoard(y, x+2, '#');
				G.SetBoard(y + 1, x+2, '#');
				G.SetBoard(y + 2, x+2, '#');
				G.SetBoard(y + 3, x+2, '#');
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


int main() {
	GameBoard G(INIT_WIDTH, INIT_HEIGHT);
	G.InitBorder();
	G.Show();
	char S[4][4];
	Block *A;
	int currentBlock;
	int x = 4, y = 0;
	while (true) {
		srand((unsigned)time(0));
		currentBlock = rand() % 2;
		switch (currentBlock) {
		case 0: A = new Square();
			break;
		case 1: A = new Straight();
			break;
		default:break;
		}

		A->GetShape(S,y,x);
		if (A->Init(G) == false)
			break;
		G.Show();
		while (A->Fall(G, y, x) == true) {
			srand((unsigned)time(0));
			int a = rand() % 3;
			if (a == 1)
				A->MoveLeft(G, y, x);
			else if (a == 2)
				A->MoveRight(G, y, x);
			G.Show();
			Sleep(100);
		}
		delete(A);
	}
	
	cout << "stop";
	system("pause");
	
}
