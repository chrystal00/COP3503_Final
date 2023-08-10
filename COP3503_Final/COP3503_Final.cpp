#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream> 
using namespace std;


// Tile class
class Tile {
private:
	int x;
	int y;
	int num;
	string str;
	bool flag;
	bool mine;
	bool isRevealed;

public:
	Tile();
	Tile(int x, int y);
	int getX();
	int getY();
	bool getFlag();
	void setFlag();
	void setFlagX(bool a);
	bool getMine();
	void setMine(bool a);
	bool getClicked();
	void setClicked();
	void setNum(vector<vector<Tile>>& x, int a, int b);
	int getNum();
};
bool inBounds(vector<vector<Tile>>& x, int a, int b);

Tile::Tile() {}

Tile::Tile(int x, int y) {
	this->x = x;
	this->y = y;
	flag = false;
	mine = false;
	isRevealed = false;
}

int Tile::getX() {
	return x;
}

int Tile::getY() {
	return y;
}

bool Tile::getFlag() {
	return flag;
}

void Tile::setFlag() {
	flag = !flag;
}

void Tile::setFlagX(bool a) {
	flag = a;
}

bool Tile::getMine() {
	return mine;
}

void Tile::setMine(bool a) {
	mine = a;
}

bool Tile::getClicked() {
	return isRevealed;
}

void Tile::setClicked() {
	isRevealed = true;
}


void Tile::setNum(vector<vector<Tile>>& x, int i, int j) {
	int count = 0;
	if (x[i][j].getMine())
		count = -1;
	else {
		if (inBounds(x, i - 1, j - 1) && x[i - 1][j - 1].getMine())
			count++;
		if (inBounds(x, i - 1, j) && x[i - 1][j].getMine())
			count++;
		if (inBounds(x, i - 1, j + 1) && x[i - 1][j + 1].getMine())
			count++;
		if (inBounds(x, i, j - 1) && x[i][j - 1].getMine())
			count++;
		if (inBounds(x, i, j + 1) && x[i][j + 1].getMine())
			count++;
		if (inBounds(x, i + 1, j - 1) && x[i + 1][j - 1].getMine())
			count++;
		if (inBounds(x, i + 1, j) && x[i + 1][j].getMine())
			count++;
		if (inBounds(x, i + 1, j + 1) && x[i + 1][j + 1].getMine())
			count++;
	}
	num = count;
}

int Tile::getNum() {
	return num;
}

bool inBounds(vector<vector<Tile>>& vec, int x, int y) {
	return x >= 0 && y >= 0 && x < vec.size() && y < vec[0].size();
}

// Mine randomization for the board (numbers between 0 - 399)
void randomMines(vector<int>& x) {
	srand(time(NULL));
	while (x.size() < 50) {
		int a = rand() % 400;
		x.push_back(a);
		sort(x.begin(), x.end());
		x.erase(unique(x.begin(), x.end()), x.end());
	}
}

void floodfill(vector<vector<Tile>>& vec, int r, int c) {
	int a = 0;
	if (inBounds(vec, r, c) && vec[r][c].getNum() == 0
		&& !vec[r][c].getClicked() && !vec[r][c].getFlag()) {
		vec[r][c].setClicked();
		floodfill(vec, r - 1, c - 1);
		floodfill(vec, r - 1, c);
		floodfill(vec, r - 1, c + 1);
		floodfill(vec, r, c - 1);
		floodfill(vec, r, c + 1);
		floodfill(vec, r + 1, c - 1);
		floodfill(vec, r + 1, c);
		floodfill(vec, r + 1, c + 1);
	}
	else if (inBounds(vec, r, c) && vec[r][c].getNum() != 0 && vec[r][c].getNum() != -1)
	{
		vec[r][c].setClicked();
	}
}

// Sets up the different board options (1, 2, or 3)
void changeBoard(vector<vector<Tile>>& board, int n) {
	// TODO eventually change so its not hard coded
	string str = "";
	if (n == 1)
		str = "boards/testboard1.brd";
	if (n == 2)
		str = "boards/testboard2.brd";
	if (n == 3)
		str = "boards/testboard3.brd";

	fstream file(str, ios_base::in);
	string line;
	vector<vector<string>> arr;
	while (getline(file, line)) {
		vector<string> a;
		for (int i = 0; i < line.length(); i++)
			a.push_back(line.substr(i, 1));
		arr.push_back(a);
	}
	board.clear();
	for (int i = 0; i < 512; i += 32) {
		vector<Tile> a;
		for (int j = 0; j < 800; j += 32) {
			Tile x = Tile(j, i);
			a.push_back(x);
		}
		board.push_back(a);
	}
	for (int i = 0; i < arr.size(); i++) {
		for (int j = 0; j < arr[0].size(); j++) {
			if (arr[i][j].compare("1") == 0)
				board[i][j].setMine(true);
		}
	}
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].size(); j++) {
			board[i][j].setNum(board, i, j);
		}
	}
}

// Resetting board 
void reset(vector<vector<Tile>>& board) {
	board.clear();
	for (int i = 0; i < 512; i += 32) {
		vector<Tile> a;
		for (int j = 0; j < 800; j += 32) {
			Tile x = Tile(j, i);
			a.push_back(x);
		}
		board.push_back(a);
	}
	vector<int> nums;
	randomMines(nums);
	for (int i = 0; i < 50; i++) {
		int a = nums[i] / 25;
		int b = nums[i] % 25;
		board[a][b].setMine(true);
	}
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].size(); j++) {
			board[i][j].setNum(board, i, j);
		}
	}
}

// Check if player won
bool checkWin(vector<vector<Tile>>& board) {
	int count = 0; int numMines = 0;
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board[0].size(); j++) {
			if (board[i][j].getClicked())
				count++;
			if (board[i][j].getMine())
				numMines++;
		}
	}

	// If won returns true, otherwise false 
	if (count == (400 - numMines))
		return true;
	return false;
}


// SFML library information 
int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Chrystal's Minesweeper"); // TODO eventually change this hard code

	sf::Texture texture;
	texture.loadFromFile("images/tile_hidden.png");
	sf::Sprite hiddentile;
	hiddentile.setTexture(texture);
	sf::Texture tiletext;
	tiletext.loadFromFile("images/tile_revealed.png");
	sf::Sprite tile;
	tile.setTexture(tiletext);
	sf::Texture flagtext;
	flagtext.loadFromFile("images/flag.png");
	sf::Sprite flag;
	flag.setTexture(flagtext);
	sf::Texture minetext;
	minetext.loadFromFile("images/mine.png");
	sf::Sprite mine;
	mine.setTexture(minetext);
	sf::Texture num1text;
	num1text.loadFromFile("images/number_1.png");
	sf::Sprite num1;
	num1.setTexture(num1text);
	sf::Texture num2text;
	num2text.loadFromFile("images/number_2.png");
	sf::Sprite num2;
	num2.setTexture(num2text);
	sf::Texture num3text;
	num3text.loadFromFile("images/number_3.png");
	sf::Sprite num3;
	num3.setTexture(num3text);
	sf::Texture num4text;
	num4text.loadFromFile("images/number_4.png");
	sf::Sprite num4;
	num4.setTexture(num4text);
	sf::Texture num5text;
	num5text.loadFromFile("images/number_5.png");
	sf::Sprite num5;
	num5.setTexture(num5text);
	sf::Texture num6text;
	num6text.loadFromFile("images/number_6.png");
	sf::Sprite num6;
	num6.setTexture(num6text);
	sf::Texture num7text;
	num7text.loadFromFile("images/number_7.png");
	sf::Sprite num7;
	num7.setTexture(num7text);
	sf::Texture num8text;
	num8text.loadFromFile("images/number_8.png");
	sf::Sprite num8;
	num8.setTexture(num8text);
	sf::Texture happytext;
	happytext.loadFromFile("images/face_happy.png");
	sf::Sprite happy;
	happy.setTexture(happytext);
	sf::Texture debugtext;
	debugtext.loadFromFile("images/debug.png");
	sf::Sprite debug;
	debug.setTexture(debugtext);
	sf::Texture riptext;
	riptext.loadFromFile("images/face_lose.png");
	sf::Sprite rip;
	rip.setTexture(riptext);
	sf::Texture wintext;
	wintext.loadFromFile("images/face_win.png");
	sf::Sprite win;
	win.setTexture(wintext);

	// Test 1
	sf::Texture test1text;
	test1text.loadFromFile("images/test_1.png");
	sf::Sprite test1;
	test1.setTexture(test1text);

	// Test 2
	sf::Texture test2text;
	test2text.loadFromFile("images/test_2.png");
	sf::Sprite test2;
	test2.setTexture(test2text);

	// Test 3
	sf::Texture test3text;
	test3text.loadFromFile("images/test_3.png");
	sf::Sprite test3;
	test3.setTexture(test3text);


	vector<vector<Tile>> board;
	reset(board);

	bool p = false, lose = false, showMines = false;
	while (window.isOpen()) {
		sf::Event event;
		window.clear();

		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[0].size(); j++) {
				int x = board[i][j].getX();
				int y = board[i][j].getY();
				hiddentile.setPosition(x, y);
				window.draw(hiddentile); // drawing hidden tiles 
			}

		}

		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Right) { // Right mouse clicks
					p = !p;
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;

					int i = (x + 1) / 32;
					int j = (y + 1) / 32;
					if (!board[j][i].getClicked())
						board[j][i].setFlag();

				}
			}


			if (event.type == sf::Event::MouseButtonReleased) {
				if (event.mouseButton.button == sf::Mouse::Left) { // Left mouse clicks 
					int x = event.mouseButton.x;
					int y = event.mouseButton.y;

					int i = (x + 1) / 32;
					int j = (y + 1) / 32;
					if (inBounds(board, j, i) && board[j][i].getFlag())
						break;
					if (!lose)
						floodfill(board, j, i);

					if (inBounds(board, j, i) && board[j][i].getMine()) {
						showMines = true;
						lose = true;
					}
					if (x >= 496 && x <= 560 && y >= 512 && y <= 576) // debug button
						showMines = !showMines;

					if (x >= 560 && x <= 624 && y >= 512 && y <= 576) {
						changeBoard(board, 1);
						showMines = false;
					}

					if (x >= 624 && x <= 688 && y >= 512 && y <= 576) {
						changeBoard(board, 2);
						showMines = false;
					}
					if (x >= 688 && x <= 752 && y >= 512 && y <= 576) {
						changeBoard(board, 3);
						showMines = false;
					}
					if (x >= 368 && x <= 432 && y >= 512 && y <= 576) { // smiley button 
						if (lose) {
							lose = false;
							showMines = false;
						}
						reset(board);
					}
				}
			}
		}

		int countFlag = 0; int countMines = 0;
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[0].size(); j++) {
				int x = board[i][j].getX();
				int y = board[i][j].getY();
				if (lose)
					board[i][j].setFlagX(false);
				if (board[i][j].getFlag()) {
					flag.setPosition(x, y);
					window.draw(flag);
					countFlag++;
				}
				if (board[i][j].getMine()) {
					mine.setPosition(x, y);
					if (showMines) {
						tile.setPosition(x, y);
						window.draw(tile);
						window.draw(mine);
					}

					countMines++;

				}
			}
		}
		// Positions for drawing the numbers on the board 
		for (int i = 0; i < board.size(); i++) {
			for (int j = 0; j < board[0].size(); j++) {
				if (board[i][j].getClicked()) {
					int num = board[i][j].getNum();
					int x = board[i][j].getX();
					int y = board[i][j].getY();
					tile.setPosition(x, y);
					window.draw(tile);
					if (num == 1) {
						num1.setPosition(x, y);
						window.draw(num1);
					}
					if (num == 2) {
						num2.setPosition(x, y);
						window.draw(num2);
					}
					if (num == 3) {
						num3.setPosition(x, y);
						window.draw(num3);
					}
					if (num == 4) {
						num4.setPosition(x, y);
						window.draw(num4);
					}
					if (num == 5) {
						num5.setPosition(x, y);
						window.draw(num5);
					}
					if (num == 6) {
						num6.setPosition(x, y);
						window.draw(num6);
					}
					if (num == 7) {
						num7.setPosition(x, y);
						window.draw(num7);
					}
					if (num == 8) {
						num8.setPosition(x, y);
						window.draw(num8);
					}
				}
			}
		}
		// Mine counter 
		int count = countMines - countFlag;



		if (count < 0)
		{
			count = count * -1;
			int d = 21 * 10;

			int a = abs(count / 100);
			int b = abs((count % 100) / 10);
			int c = abs(count % 10);

			sf::Texture digitstext;

			sf::Sprite digits;
			digitstext.loadFromFile("images/digits.png", sf::IntRect(d, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(0, 512);
			window.draw(digits);
			digitstext.loadFromFile("images/digits.png", sf::IntRect(21 * a, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(21, 512);
			window.draw(digits);
			digitstext.loadFromFile("images/digits.png", sf::IntRect(21 * b, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(42, 512);
			window.draw(digits);
			digitstext.loadFromFile("images/digits.png", sf::IntRect(21 * c, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(63, 512);
			window.draw(digits);
		}
		else
		{
			int a = abs(count / 100);
			int b = abs((count % 100) / 10);
			int c = abs(count % 10);

			sf::Texture digitstext;

			sf::Sprite digits;
			digitstext.loadFromFile("images/digits.png", sf::IntRect(21 * a, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(21, 512);
			window.draw(digits);
			digitstext.loadFromFile("images/digits.png", sf::IntRect(21 * b, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(42, 512);
			window.draw(digits);
			digitstext.loadFromFile("images/digits.png", sf::IntRect(21 * c, 0, 21, 32));
			digits.setTexture(digitstext);
			digits.setPosition(63, 512);
			window.draw(digits);
		}




		// Sets position of icons 
		happy.setPosition(368, 512); // numColumns * 32 / 2
		window.draw(happy);
		debug.setPosition(496, 512);
		window.draw(debug);
		test1.setPosition(560, 512);
		window.draw(test1);
		test2.setPosition(624, 512);
		window.draw(test2);
		test3.setPosition(688, 512);
		window.draw(test3);



		if (lose) {
			rip.setPosition(368, 512); // numColumns * 32 / 2
			window.draw(rip);
		}

		if (!lose && checkWin(board)) {
			win.setPosition(368, 512); // numColumns * 32 / 2
			window.draw(win);
			for (int i = 0; i < board.size(); i++) {
				for (int j = 0; j < board[0].size(); j++) {
					if (board[i][j].getMine()) {
						showMines = false;
						board[i][j].setFlagX(true);
					}
				}
			}
		}

		window.display();
	}

	return 0;
}