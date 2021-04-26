#include <SFML/Graphics.hpp>
#include <queue>
#include <utility>
using namespace sf;
using namespace std;

#define WIDTH 16
#define HEIGHT 16
#define BOMBS 40

#define BLACK 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define BOMB 9
#define WHITE 10
#define FLAG 11

int isEnd = 0;//졌으면 1 이겼으면 0
int imagePixel = 32;
int board[WIDTH + 2][HEIGHT + 2];
int boardUser[WIDTH + 2][HEIGHT + 2];
queue<pair<pair<int, int>, int>> qpp1;
int dx[4] = { 1,-1,0,0 };
int dy[4] = { 0,0,1,-1 };
void Init_bomb() {
	static int bombs = 0;
	if (bombs < BOMBS) {
		int x = (rand() % WIDTH) + 1;
		int y = (rand() % HEIGHT) + 1;
		if (board[x][y] != BOMB) {
			board[x][y] = BOMB;
			bombs++;
		}
		Init_bomb();
	}
}

void Init_board() {
	Init_bomb();
	for (int i = 1; i < WIDTH + 1; i++) {
		for (int j = 1; j < HEIGHT + 1; j++) {
			if (board[i][j] != BOMB) {
				int count = 0;
				for (int p = i - 1; p <= i + 1; p++) {
					for (int q = j - 1; q <= j + 1; q++) {
						if (board[p][q] == BOMB) {
							count++;
						}
					}
				}
				board[i][j] = count;
			}
		}
	}
}

void Init_boardUser() {
	for (int i = 1; i < WIDTH + 1; i++) {
		for (int j = 1; j < HEIGHT + 1; j++) {
			boardUser[i][j] = WHITE;
		}
	}
}

void Click_right(int _x, int _y) {
	if (boardUser[_x][_y] == WHITE) {
		boardUser[_x][_y] = FLAG;
	}
	else if (boardUser[_x][_y] == FLAG) {
		boardUser[_x][_y] = WHITE;
	}
}

void Check(int _x, int _y) {
	qpp1.push(make_pair(make_pair(_x, _y), 0));

	while (qpp1.empty() == 0) {
		int x = qpp1.front().first.first;
		int y = qpp1.front().first.second;
		int z = qpp1.front().second;
		qpp1.pop();
		boardUser[x][y] = board[x][y];
		for (int i = 0; i < 4; i++) {
			int nx = x + dx[i];
			int ny = y + dy[i];
			if (nx<1 || nx>WIDTH || ny<1 || ny>WIDTH) {
				continue;
			}
			if (z == 1) {
				continue;
			}
			if (boardUser[nx][ny] != WHITE) { //눌리지 않은거면
				continue;
			}
			else if (board[nx][ny] == BLACK) {
				qpp1.push(make_pair(make_pair(nx, ny), 0));
			}
			else {// 그 외 숫자 1개 일 경우
				qpp1.push(make_pair(make_pair(nx, ny), 1));
			}
		}
	}
}

void End_game() {
	for (int i = 1; i < WIDTH + 1; i++) {
		for (int j = 1; j < HEIGHT + 1; j++) {
			boardUser[i][j] = board[i][j];
		}
	}
	isEnd++;
}

void Click_left(int _x, int _y) {
	if (boardUser[_x][_y] == WHITE) {
		if (board[_x][_y] == BOMB) {
			//게임 끝 
			End_game();
		}
		else {
			if (board[_x][_y] == BLACK) {
				//확장 BFS
				Check(_x, _y);
				//boardUser[_x][_y] = board[_x][_y];
			}
			else {
				boardUser[_x][_y] = board[_x][_y];
			}
		}
	}
}


int main(void) {
	RenderWindow app(VideoMode((WIDTH + 2)*imagePixel, (HEIGHT + 2)*imagePixel), "Minesweep!");
	srand(time(NULL));
	Texture texture;
	texture.loadFromFile("images/tiles.jpg");
	Texture win;
	win.loadFromFile("images/win.gif");
	Texture defeat;
	defeat.loadFromFile("images/defeat.gif");
	Sprite sprite(texture);
	Sprite winSprite(win);
	Sprite defeatSprite(defeat);
	Init_board();
	Init_boardUser();
	while (app.isOpen())
	{
		Vector2i pos = Mouse::getPosition(app);
		int x = pos.x / imagePixel;
		int y = pos.y / imagePixel;
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
				app.close();

			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					Click_left(x, y);
				}
				else if (event.key.code == Mouse::Right) {
					Click_right(x, y);
				}
			}
		}
		// 게임이 끝났는지 아닌지 확인
		int count = WIDTH * HEIGHT;
		for (int i = 1; i < WIDTH + 1; i++) {
			for (int j = 1; j < HEIGHT + 1; j++) {
				if (boardUser[i][j] != WHITE && boardUser[i][j] != FLAG && board[i][j] != BOMB) {
					count--;
				}
			}
		}

		app.clear(Color::White);
		for (int i = 1; i < WIDTH + 1; i++) {
			for (int j = 1; j < HEIGHT + 1; j++) {
				sprite.setTextureRect(IntRect(boardUser[i][j] * imagePixel, 0, imagePixel, imagePixel));
				sprite.setPosition(i*imagePixel, j*imagePixel);
				app.draw(sprite);
			}
		}
		if (count <= BOMBS && isEnd == 0) {
			winSprite.setTexture(win);
			app.draw(winSprite);
		}
		app.display();
	}

	return 0;
}