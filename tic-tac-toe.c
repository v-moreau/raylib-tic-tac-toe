#include <math.h>
#include "dependencies/raylib.h"

const int screenWidth = 480;
const int screenHeight = 480;
const int speed = 5;

const int thick = 5;
const int marginExt = 20;
const int marginInt = 35;
const int radius = (screenWidth - 2 * marginExt) / 6 - marginInt;

enum coup
{
	v = 0,
	x = 1,
	o = -1
};

int length(int i)
{
	return marginExt + i * 2 * (marginInt + radius);
}

int getIndex(int x)
{
	if (length(0) <= x && x <= length(1))
	{
		return 0;
	}
	if (length(1) <= x && x <= length(2))
	{
		return 1;
	}
	if (length(2) <= x && x <= length(3))
	{
		return 2;
	}
	return -1;
}

void drawMove(int i, int j, enum coup joueur, Color c)
{
	if (joueur == o)
	{
		DrawCircle(length(i) + marginInt + radius, length(j) + marginInt + radius, radius, c);
		DrawCircle(length(i) + marginInt + radius, length(j) + marginInt + radius, radius - thick, RAYWHITE);
	}
	else if (joueur == x)
	{
		// Backslash
		Vector2 coorUpperLeft = {length(i) + marginInt, length(j) + marginInt};
		Vector2 coorLowerRight = {coorUpperLeft.x + 2 * radius, coorUpperLeft.y + 2 * radius};
		DrawLineEx(coorUpperLeft, coorLowerRight, thick, c);
		// Slash
		Vector2 coorUpperRight = {coorUpperLeft.x, coorUpperLeft.y + 2 * radius};
		Vector2 coorLowerLeft = {coorUpperLeft.x + 2 * radius, coorUpperLeft.y};
		DrawLineEx(coorUpperRight, coorLowerLeft, thick, c);
	}
}

void drawAllMoves(enum coup move[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			drawMove(i, j, move[i][j], MAROON);
		}
	}
}

void drawLines()
{
	for (int j = 1; j < 3; j++)
	{
		Vector2 coorUp = {marginExt, length(j)};
		Vector2 coorDown = {screenWidth - marginExt, length(j)};
		DrawLineEx(coorUp, coorDown, thick, DARKGRAY);

		Vector2 coorLeft = {coorUp.y, coorUp.x};
		Vector2 coorRight = {coorDown.y, coorDown.x};
		DrawLineEx(coorLeft, coorRight, thick, DARKGRAY);
	}
}

bool won(enum coup move[3][3], enum coup joueur)
{
	for (int i = 0; i < 3; i++)
	{
		if ((move[i][0] == move[i][1] && move[i][1] == move[i][2] && move[i][2] == joueur) ||
			(move[0][i] == move[1][i] && move[1][i] == move[2][i] && move[2][i] == joueur))
		{
			return true;
		}
	}
	return (move[0][0] == move[1][1] && move[1][1] == move[2][2] && move[2][2] == joueur) ||
		   (move[0][2] == move[1][1] && move[1][1] == move[2][0] && move[2][0] == joueur);
}

bool full(enum coup move[3][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (move[i][j] == v)
			{
				return false;
			}
		}
	}
	return true;
}

void jouer()
{
	enum coup joueur = x;
	enum coup move[3][3] = {{v, v, v}, {v, v, v}, {v, v, v}};

	bool clickAfterFinish = false;

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		drawLines();
		drawAllMoves(move);

		if (!won(move, -joueur) && !full(move))
		{
			int i = getIndex(GetMouseX());
			int j = getIndex(GetMouseY());
			if (i != -1 && j != -1 && move[i][j] == v)
			{
				drawMove(i, j, joueur, GRAY);
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				{
					move[i][j] = joueur;
					joueur = -joueur;
				}
			}
		}
		else
		{
			DrawRectangle(0, 0, screenWidth, screenHeight, CLITERAL(Color){255, 255, 255, 200});

			const char *resultat;
			if (won(move, x))
			{
				resultat = "X won!";
			}
			else if (won(move, o))
			{
				resultat = "O won!";
			}
			else
			{
				resultat = "It's a tie!";
			}
			int textWidth = MeasureText(resultat, 75);
			DrawText(resultat, (screenWidth - textWidth) / 2, length(1) + 10, 75, DARKGRAY);

			char suivant[] = "Click to start a new game.";
			textWidth = MeasureText(suivant, 20);
			DrawText(suivant, (screenWidth - textWidth) / 2, length(1) + 3 * marginInt, 20, DARKGRAY);

			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && clickAfterFinish)
			{
				return jouer();
			}

			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			{
				clickAfterFinish = true;
			}
		}
		EndDrawing();
	}
}

int main(void)
{
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(screenWidth, screenHeight, "Tic-tac-toe");
	SetTargetFPS(60);

	jouer();

	CloseWindow();
	return 0;
}