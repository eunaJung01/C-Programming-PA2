#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define PLAYER_MARK "��"
#define FLAG_MARK "��"
#define TREASURE_MARK "��"

#define START_X 2				// ����� ���� ��ġ (x)
#define START_Y 9				// ����� ���� ��ġ (y)

#define LEVEL_1_X 30			// �ʱ� ȭ�� ���� ũ��
#define LEVEL_1_Y 15			// �ʱ� ȭ�� ���� ũ��

#define LEVEL_2_X 50			// �߱� ȭ�� ���� ũ��
#define LEVEL_2_Y 17			// �߱� ȭ�� ���� ũ��

#define LEVEL_3_X 70			// ��� ȭ�� ���� ũ��
#define LEVEL_3_Y 20			// ��� ȭ�� ���� ũ��

#define FLAG_1 5				// �ʱ� ��� ����
#define FLAG_2 15				// �߱� ��� ����
#define FLAG_3 25				// ��� ��� ����

#define MOVE_LIMIT 80			// ����Ű �̵� Ƚ��
#define SCORE 50				// �⺻ ����

void gotoxy(int x, int y);
void menu(void);
void start_info(void);
void print_limit(void);
void game_screen_1(int* x, int* y, int flag[FLAG_3][2]);
void game_screen_2(int* x, int* y, int flag[FLAG_3][2]);
void game_screen_3(int* x, int* y, int flag[FLAG_3][2]);
void cursor(int* x, int* y, int* move, int* score, int flag[FLAG_3][2], int* check);
void flag_(int** x, int** y, int flag[FLAG_3][2], int** score, int** move);
void win(int** score, int** check);
void menu_re_win(int* level_re);
void move_limit(int** check);
void score_limit(int** check);
void menu_re_lose(int* level_re);

int main(void)
{
	int level, score = SCORE, move = 0, x = START_X, y = START_Y, flag[FLAG_3][2], check = 0, level_re_win, level_re_lose;
	// flag : ����, ����� ��ġ�� ������ 2���� �迭
	// level : ���̵� ����
	// score : ����
	// move : �̵� Ƚ��
	// x, y : ����� ��ġ
	// check : ���� ã�� ���� or ���� (���п� ���� ���� ������ �� ������ �ٽ� �� ������ �ٲ�� ����.)
	// level_re_win, level_re_lose : ��� ������ ������ ������ or ������ ������

	menu();
	scanf("%d", &level);

	if (level == 4)
		printf("����.\n");

	else
	{
		system("cls");
		start_info();
		switch (level)
		{
		case 1:
			game_screen_1(&x, &y, flag);
			break;
		case 2:
			game_screen_2(&x, &y, flag);
			break;
		case 3:
			game_screen_3(&x, &y, flag);
			break;
		}
		print_limit();
		cursor(&x, &y, &move, &score, flag, &level, &check);

		while (1)
		{
			system("cls");
			level_re_win = 0;
			level_re_lose = 0;

			switch (check)						// ���� ã�� ���� or ����
			{
			case 0:
				menu_re_lose(&level_re_win);
				break;
			case 1:
				menu_re_win(&level_re_lose);
				break;
			}

			x = START_X;						// �� �ʱ�ȭ
			y = START_Y;
			move = MOVE_LIMIT;
			system("cls");

			if (level_re_lose == 1)				// ���� ã�� ���� -> �ٽ� ����
			{
				score = SCORE;					// ���� �ʱ�ȭ
				start_info();
				switch (level)
				{
				case 1:
					game_screen_1(&x, &y, flag);
					break;
				case 2:
					game_screen_2(&x, &y, flag);
					break;
				case 3:
					game_screen_3(&x, &y, flag);
					break;
				}
				print_limit();
				cursor(&x, &y, &move, &score, flag, &level, &check);
			}
			else if (level_re_lose == 2)		// ���� ã�� ���� -> �׸�
			{
				system("cls");
				printf("����.");
				break;
			}


			if (level_re_win == 1)				// ���� ã�� ���� -> ���� �ܰ�� ����
			{
				start_info();
				switch (level)
				{
				case 1:
					game_screen_2(&x, &y, flag);
					break;
				case 2:
				case 3:
					game_screen_3(&x, &y, flag);
					break;
				}
				print_limit();
				cursor(&x, &y, &move, &score, flag, &level, &check);
			}
			else if (level_re_win == 2)			// ���� ã�� ���� -> �׸�
			{
				printf("����.\n");
				break;
			}
		}
	}
}

void gotoxy(int x, int y)
{
	COORD Cur = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void menu(void)										// ���� ���� �� ���̵� ���� (���)
{
	printf("���̵��� �����ϼ��� (�ʱ� : 1 / �߱� : 2 / ��� : 3)\n�����Ϸ��� 4 �Է� : ");
}

void start_info(void)
{
	printf("����Ű �̵� 80�� �ȿ� ������ ã���ÿ�.\n������ ������ �Ǹ� ���� ����\n(2�� �� ���� ����)");
	Sleep(2000);
	system("cls");
}

void print_limit(void)
{
	gotoxy(0, 20);
	printf("���� ����Ű �̵� Ƚ�� : %d\n", MOVE_LIMIT);
	printf("���� : %d", SCORE);
}

void game_screen_1(int* x, int* y, int flag[FLAG_3][2])							// ���� ȭ�� ��� (�ʱ�)
{
	int i, u, j, h, f, x_f, y_f;
	int game[LEVEL_1_Y][LEVEL_1_X];

	for (i = 0; i < LEVEL_1_X; i++)					// �� (����ڰ� ���� ����� �ʴ´ٰ� ����..)
	{
		game[0][i] = 1;								// ��
		game[LEVEL_1_Y - 1][i] = 1;					// �Ʒ� ��
	}
	for (u = 0; u < LEVEL_1_Y; u++)
	{
		game[u][0] = 1;								// ���� ��
		game[u][LEVEL_1_X - 1] = 1;					// ������ ��
	}

	for (j = 0; j < LEVEL_1_Y; j++)					// �� ���
	{
		for (h = 0; h < LEVEL_1_X; h++)
		{
			if (game[j][h] == 1)
				printf("#");
			else
				printf(" ");
		}
		printf("\n");
	}

	srand((unsigned)time(NULL));
	for (f = 0; f < FLAG_1; f++)
	{
		x_f = rand() % (LEVEL_1_X - 3) + 2;
		y_f = rand() % (LEVEL_1_Y - 2) + 1;
		gotoxy(x_f, y_f);
		printf(FLAG_MARK);

		flag[f][0] = x_f;
		flag[f][1] = y_f;
	}
}

void game_screen_2(int* x, int* y, int flag[FLAG_3][2])							// ���� ȭ�� ��� (�߱�)
{
	int i, u, j, h, f, x_f, y_f;
	int game[LEVEL_2_Y][LEVEL_2_X];

	for (i = 0; i < LEVEL_2_X; i++)					// ��
	{
		game[0][i] = 1;								// ��
		game[LEVEL_2_Y - 1][i] = 1;					// �Ʒ� ��
	}
	for (u = 0; u < LEVEL_2_Y; u++)
	{
		game[u][0] = 1;								// ���� ��
		game[u][LEVEL_2_X - 1] = 1;					// ������ ��

	}

	for (j = 0; j < LEVEL_2_Y; j++)					// �� ���
	{
		for (h = 0; h < LEVEL_2_X; h++)
		{
			if (game[j][h] == 1)
				printf("#");
			else
				printf(" ");
		}
		printf("\n");
	}

	srand((unsigned)time(NULL));
	for (f = 0; f < FLAG_2; f++)
	{
		x_f = rand() % (LEVEL_2_X - 3) + 2;
		y_f = rand() % (LEVEL_2_Y - 2) + 1;
		gotoxy(x_f, y_f);
		printf(FLAG_MARK);

		flag[f][0] = x_f;
		flag[f][1] = y_f;
	}
}

void game_screen_3(int* x, int* y, int flag[FLAG_3][2])							// ���� ȭ�� ��� (�߱�)
{
	int i, u, j, h, f, x_f, y_f;
	int game[LEVEL_3_Y][LEVEL_3_X];

	for (i = 0; i < LEVEL_3_X; i++)					// ��
	{
		game[0][i] = 1;								// ��
		game[LEVEL_3_Y - 1][i] = 1;					// �Ʒ� ��
	}
	for (u = 0; u < LEVEL_3_Y; u++)
	{
		game[u][0] = 1;								// ���� ��
		game[u][LEVEL_3_X - 1] = 1;					// ������ ��

	}

	for (j = 0; j < LEVEL_3_Y; j++)					// �� ���
	{
		for (h = 0; h < LEVEL_3_X; h++)
		{
			if (game[j][h] == 1)
				printf("#");
			else
				printf(" ");
		}
		printf("\n");
	}

	srand((unsigned)time(NULL));
	for (f = 0; f < FLAG_3; f++)
	{
		x_f = rand() % (LEVEL_3_X - 3) + 2;
		y_f = rand() % (LEVEL_3_Y - 2) + 1;
		gotoxy(x_f, y_f);
		printf(FLAG_MARK);

		flag[f][0] = x_f;
		flag[f][1] = y_f;
	}
}

void cursor(int* x, int* y, int* move, int* score, int flag[FLAG_3][2], int* check)				// ����Ű �̵��ϱ� -> ���
{
	int ch;

	while (1)
	{
		gotoxy(*x, *y);
		printf(PLAYER_MARK);

		ch = _getch();
		if (ch == 224)
		{
			ch = _getch();

			switch (ch)
			{
			case 72:
				(*y)--;
				break;
			case 80:
				(*y)++;
				break;
			case 75:
				(*x)--;
				break;
			case 77:
				(*x)++;
				break;
			}
		}
		(*move)++;
		printf("\b\b  ");
		gotoxy(26, 20);
		printf("\b\b%d", MOVE_LIMIT - *move);
		gotoxy(*x, *y);

		if (*move == MOVE_LIMIT)
		{
			move_limit(&check);
			break;
		}

		if (*score < 0)
		{
			score_limit(&check);
			break;
		}

		if ((*x == flag[0][0]) && (*y == flag[0][1]))
		{
			win(&score, &check);
			break;
		}

		flag_(&x, &y, flag, &score, &move);
	}
}

void flag_(int** x, int** y, int flag[FLAG_3][2], int** score, int** move)				// ���
{
	int i, r;

	for (i = 1; i < FLAG_3; i++)
	{
		if ((**x == flag[i][0]) && (**y == flag[i][1]))
		{
			r = rand() % 7;
			switch (r)
			{
			case 0:		// ���� ��ġ �����ֱ�
				gotoxy(0, 22 + i);
				printf("���� ��ġ �����ֱ�");
				gotoxy(flag[0][0], flag[0][1]);
				printf(TREASURE_MARK);
				gotoxy(**x, **y);
				break;

			case 1:		// ����Ű �̵� Ƚ�� 10 ����
				**move -= 10;
				gotoxy(0, 22 + i);
				printf("����Ű �̵� Ƚ�� 10 ���� !");
				gotoxy(**x, **y);
				break;

			case 2:		// ����Ű �̵� Ƚ�� 20 ����
				**move -= 20;
				gotoxy(0, 22 + i);
				printf("����Ű �̵� Ƚ�� 20 ���� !");
				gotoxy(**x, **y);
				break;

			case 3:		// ����Ű �̵� Ƚ�� 10 ����
				**move += 10;
				gotoxy(0, 22 + i);
				printf("����Ű �̵� Ƚ�� 10 ���� �Ф�");
				gotoxy(**x, **y);
				break;

			case 4:		// ���� +10
				**score += 10;
				gotoxy(0, 22 + i);
				printf("���� +10 !");
				gotoxy(9, 21);
				printf("\b\b %d", **score);
				gotoxy(**x, **y);
				break;

			case 5:		// ���� -10
				**score -= 10;
				gotoxy(0, 22 + i);
				printf("���� -10 �Ф�");
				gotoxy(9, 21);
				printf("\b\b  %d", **score);
				gotoxy(**x, **y);
				break;

			case 6:		// ���� -20
				**score -= 20;
				gotoxy(0, 22 + i);
				printf("���� -20 �Ф�");
				gotoxy(9, 21);
				printf("\b\b %d", **score);
				gotoxy(**x, **y);
				break;
			}
		}
	}
}

void win(int** score, int** check)
{
	**score += 10;
	printf(TREASURE_MARK);
	printf("!");
	Sleep(1000);

	system("cls");
	printf("���� ã��.\n");
	printf("���� +10 : %d\n", **score);
	Sleep(2000);
	**check = 1;
}

void menu_re_win(int* level_re_win)
{
	printf("���� ���̵��� ����? (������ : 1 / ���� ���� : 2) ");
	scanf("%d", &(*level_re_win));
}

void move_limit(int** check)
{
	system("cls");
	printf("����Ű Ƚ�� �ʰ��� ����\n");
	Sleep(2000);
	**check = 0;
}

void score_limit(int** check)
{
	system("cls");
	printf("���� �������� ����\n");
	Sleep(2000);
	**check = 0;
}

void menu_re_lose(int* level_re_lose)
{
	printf("�ٽ� �Ͻðڽ��ϱ�? (������ : 1 / ���� ���� : 2) ");
	scanf("%d", &(*level_re_lose));
}