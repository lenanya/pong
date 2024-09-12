#include <raylib.h>
#include <stdio.h>

#define W_W 1500 //window width
#define W_H 1000 //window height
#define W_TITLE "Pong" //window title
#define W_BACKGROUND (Color){0x19, 0x19, 0x19, 0xFF} //background color
#define P_COLOR (Color){0xFF, 0xFF, 0xFF, 0xFF} // Player and Ball color
#define B_SPEED 13
#define P_SPEED 30
#define W_FPS 60
#define KEY_W 87
#define KEY_S 83

typedef struct Player {
	Vector2 position;
	Vector2 size;
	Color color;
	int score;
} player;

typedef struct Ball {
	Vector2 position;
	Vector2 size;
	Vector2 speed;
	Color color;
} ball;

void drawBall(ball b);
void drawPlayer(player p);
int gameFrame();
void movePlayer(player* p, int direction);
player createPlayer(int number);
ball createBall();
int moveBall(ball *b, player p1, player p2);
player p1;
player p2;
ball b;

int main(int argc, char *argv[]) {
	InitWindow(W_W, W_H, W_TITLE);
	SetTargetFPS(W_FPS);
	int running = 0;
	p1 = createPlayer(1);
	p2 = createPlayer(2);
	b = createBall();
	while (!WindowShouldClose()) {
		int ret = gameFrame(running);
		if (ret == 1) {
			break; 
		} else if (ret == 2) {
			running = 1;
		}
	}
	CloseWindow();
	return 0;
}

int gameFrame(int running) {
	BeginDrawing();
	ClearBackground(W_BACKGROUND);
	if (IsKeyReleased(KEY_SPACE)) {
		// start game thingy
		if (running == 0) return 2;
	}
	if (running == 1) {
		if (IsKeyDown(KEY_W)) {
			movePlayer(&p1, -1);
		} else if (IsKeyDown(KEY_S)) {
			movePlayer(&p1, 1);
		}
		if (IsKeyDown(KEY_UP)) {
			movePlayer(&p2, -1);
		} else if (IsKeyDown(KEY_DOWN)) {
			movePlayer(&p2, 1);
		}
		moveBall(&b, p1, p2);
	}
	drawPlayer(p1);
	drawPlayer(p2);
	drawBall(b);
	EndDrawing();
	return 0;
}

void drawBall(ball b) {
	DrawRectangleV(b.position, b.size, b.color);
}

void drawPlayer(player p) {
	DrawRectangleV(p.position, p.size, p.color);
}

player createPlayer(int number) {
	int playerX;
	if (number == 1) {
		playerX = W_W * 0.05;
	} else if (number == 2) {
		playerX = W_W * 0.95;
	}
	int playerY = W_H * 0.4;
	int playerW = W_W * 0.015;
	int playerH = W_H * 0.15;
	player p = {
		{playerX, playerY},
		{playerW, playerH},
		P_COLOR
	};
	return p;
}

ball createBall() {
	int ballX = W_W * 0.475;
	int ballSize = W_H * 0.05;
	int ballY = W_H * 0.5 - ballSize / 2;
	ball b = {
		{ballX, ballY},
		{ballSize, ballSize},
		{B_SPEED * 0.75, B_SPEED},
		P_COLOR
	};
	return b;
}

int moveBall(ball *b, player p1, player p2) {
	Vector2	newPosition = {
		b->position.x + b->speed.x,
		b->position.y + b->speed.y
	};
	if (newPosition.x <= 0) {
		ball newBall = createBall();
		b->position = newBall.position;
		b->speed = newBall.speed;
		return 1;
	} else if (newPosition.x + b->size.x >= W_W) {
		ball newBall = createBall();
		b->position = newBall.position;
		b->speed = newBall.speed;
		return 2;
	}
	if (newPosition.y <= 0 || newPosition.y + b->size.y >= W_H) {
		b->speed.y *= -1;
	}
	if (newPosition.x <= p1.position.x + p1.size.x && newPosition.x + b->size.x >= p1.position.x) {
		if (newPosition.y >= p1.position.y && newPosition.y <= p1.position.y + p1.size.y) {
			b->speed.x *= -1;
			b->position.x = p1.position.x + p1.size.x + 1;
			return 0;
		}
	}
	if (newPosition.x +b->size.x >= p2.position.x && newPosition.x <= p2.position.x + p2.size.x) {
		if (newPosition.y >= p2.position.y && newPosition.y <= p2.position.y + p2.size.y) {
			b->speed.x *= -1;
			b->position.x = p2.position.x - b->size.x - 1;
			return 0;
		}
	}
	b->position = newPosition;
	return 0;
}

void movePlayer(player *p, int direction) {
	int move = P_SPEED * direction;
	int newY;
	if (p->position.y + p->size.y + move >= W_H) {
		newY = W_H - p->size.y;
	} else if (p->position.y + move <= 0) {
		newY = 0;
	} else {
		newY = p->position.y + move;
	}
	Vector2 newPosition = {p->position.x, newY};
	p->position = newPosition;
}
