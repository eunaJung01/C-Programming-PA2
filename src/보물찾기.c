#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#define PLAYER_MARK "◆"
#define FLAG_MARK "▶"
#define TREASURE_MARK "▦"

#define START_X 2				// 사용자 시작 위치 (x)
#define START_Y 9				// 사용자 시작 위치 (y)

#define LEVEL_1_X 30			// 초급 화면 가로 크기
#define LEVEL_1_Y 15			// 초급 화면 세로 크기

#define LEVEL_2_X 50			// 중급 화면 가로 크기
#define LEVEL_2_Y 17			// 중급 화면 세로 크기

#define LEVEL_3_X 70			// 고급 화면 가로 크기
#define LEVEL_3_Y 20			// 고급 화면 세로 크기

#define FLAG_1 5				// 초급 깃발 개수
#define FLAG_2 15				// 중급 깃발 개수
#define FLAG_3 25				// 고급 깃발 개수

#define MOVE_LIMIT 80			// 방향키 이동 횟수
#define SCORE 50				// 기본 점수

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
	// flag : 보물, 깃발의 위치를 저장할 2차원 배열
	// level : 난이도 선택
	// score : 점수
	// move : 이동 횟수
	// x, y : 사용자 위치
	// check : 보물 찾기 성공 or 실패 (승패에 따라서 다음 레벨로 갈 것인지 다시 할 것인지 바뀌기 때문.)
	// level_re_win, level_re_lose : 계속 게임을 진행할 것인지 or 종료할 것인지

	menu();
	scanf("%d", &level);

	if (level == 4)
		printf("종료.\n");

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

			switch (check)						// 보물 찾기 성공 or 실패
			{
			case 0:
				menu_re_lose(&level_re_win);
				break;
			case 1:
				menu_re_win(&level_re_lose);
				break;
			}

			x = START_X;						// 값 초기화
			y = START_Y;
			move = MOVE_LIMIT;
			system("cls");

			if (level_re_lose == 1)				// 보물 찾기 실패 -> 다시 ㄱㄱ
			{
				score = SCORE;					// 점수 초기화
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
			else if (level_re_lose == 2)		// 보물 찾기 실패 -> 그만
			{
				system("cls");
				printf("종료.");
				break;
			}


			if (level_re_win == 1)				// 보물 찾기 성공 -> 다음 단계로 ㄱㄱ
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
			else if (level_re_win == 2)			// 보물 찾기 성공 -> 그만
			{
				printf("종료.\n");
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

void menu(void)										// 게임 실행 시 난이도 선택 (출력)
{
	printf("난이도를 선택하세요 (초급 : 1 / 중급 : 2 / 고급 : 3)\n종료하려면 4 입력 : ");
}

void start_info(void)
{
	printf("방향키 이동 80번 안에 보물을 찾으시오.\n점수가 음수가 되면 게임 종료\n(2초 후 게임 시작)");
	Sleep(2000);
	system("cls");
}

void print_limit(void)
{
	gotoxy(0, 20);
	printf("남은 방향키 이동 횟수 : %d\n", MOVE_LIMIT);
	printf("점수 : %d", SCORE);
}

void game_screen_1(int* x, int* y, int flag[FLAG_3][2])							// 게임 화면 출력 (초급)
{
	int i, u, j, h, f, x_f, y_f;
	int game[LEVEL_1_Y][LEVEL_1_X];

	for (i = 0; i < LEVEL_1_X; i++)					// 벽 (사용자가 벽을 벗어나지 않는다고 가정..)
	{
		game[0][i] = 1;								// 위
		game[LEVEL_1_Y - 1][i] = 1;					// 아래 벽
	}
	for (u = 0; u < LEVEL_1_Y; u++)
	{
		game[u][0] = 1;								// 왼쪽 벽
		game[u][LEVEL_1_X - 1] = 1;					// 오른쪽 벽
	}

	for (j = 0; j < LEVEL_1_Y; j++)					// 벽 출력
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

void game_screen_2(int* x, int* y, int flag[FLAG_3][2])							// 게임 화면 출력 (중급)
{
	int i, u, j, h, f, x_f, y_f;
	int game[LEVEL_2_Y][LEVEL_2_X];

	for (i = 0; i < LEVEL_2_X; i++)					// 벽
	{
		game[0][i] = 1;								// 위
		game[LEVEL_2_Y - 1][i] = 1;					// 아래 벽
	}
	for (u = 0; u < LEVEL_2_Y; u++)
	{
		game[u][0] = 1;								// 왼쪽 벽
		game[u][LEVEL_2_X - 1] = 1;					// 오른쪽 벽

	}

	for (j = 0; j < LEVEL_2_Y; j++)					// 벽 출력
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

void game_screen_3(int* x, int* y, int flag[FLAG_3][2])							// 게임 화면 출력 (중급)
{
	int i, u, j, h, f, x_f, y_f;
	int game[LEVEL_3_Y][LEVEL_3_X];

	for (i = 0; i < LEVEL_3_X; i++)					// 벽
	{
		game[0][i] = 1;								// 위
		game[LEVEL_3_Y - 1][i] = 1;					// 아래 벽
	}
	for (u = 0; u < LEVEL_3_Y; u++)
	{
		game[u][0] = 1;								// 왼쪽 벽
		game[u][LEVEL_3_X - 1] = 1;					// 오른쪽 벽

	}

	for (j = 0; j < LEVEL_3_Y; j++)					// 벽 출력
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

void cursor(int* x, int* y, int* move, int* score, int flag[FLAG_3][2], int* check)				// 방향키 이동하기 -> 깃발
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

void flag_(int** x, int** y, int flag[FLAG_3][2], int** score, int** move)				// 깃발
{
	int i, r;

	for (i = 1; i < FLAG_3; i++)
	{
		if ((**x == flag[i][0]) && (**y == flag[i][1]))
		{
			r = rand() % 7;
			switch (r)
			{
			case 0:		// 보물 위치 보여주기
				gotoxy(0, 22 + i);
				printf("보물 위치 보여주기");
				gotoxy(flag[0][0], flag[0][1]);
				printf(TREASURE_MARK);
				gotoxy(**x, **y);
				break;

			case 1:		// 방향키 이동 횟수 10 증가
				**move -= 10;
				gotoxy(0, 22 + i);
				printf("방향키 이동 횟수 10 증가 !");
				gotoxy(**x, **y);
				break;

			case 2:		// 방향키 이동 횟수 20 증가
				**move -= 20;
				gotoxy(0, 22 + i);
				printf("방향키 이동 횟수 20 증가 !");
				gotoxy(**x, **y);
				break;

			case 3:		// 방향키 이동 횟수 10 감소
				**move += 10;
				gotoxy(0, 22 + i);
				printf("방향키 이동 횟수 10 감소 ㅠㅠ");
				gotoxy(**x, **y);
				break;

			case 4:		// 점수 +10
				**score += 10;
				gotoxy(0, 22 + i);
				printf("점수 +10 !");
				gotoxy(9, 21);
				printf("\b\b %d", **score);
				gotoxy(**x, **y);
				break;

			case 5:		// 점수 -10
				**score -= 10;
				gotoxy(0, 22 + i);
				printf("점수 -10 ㅠㅠ");
				gotoxy(9, 21);
				printf("\b\b  %d", **score);
				gotoxy(**x, **y);
				break;

			case 6:		// 점수 -20
				**score -= 20;
				gotoxy(0, 22 + i);
				printf("점수 -20 ㅠㅠ");
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
	printf("보물 찾음.\n");
	printf("점수 +10 : %d\n", **score);
	Sleep(2000);
	**check = 1;
}

void menu_re_win(int* level_re_win)
{
	printf("다음 난이도로 ㄱㄱ? (레츠고 : 1 / 게임 종료 : 2) ");
	scanf("%d", &(*level_re_win));
}

void move_limit(int** check)
{
	system("cls");
	printf("방향키 횟수 초과로 실패\n");
	Sleep(2000);
	**check = 0;
}

void score_limit(int** check)
{
	system("cls");
	printf("점수 폭망으로 실패\n");
	Sleep(2000);
	**check = 0;
}

void menu_re_lose(int* level_re_lose)
{
	printf("다시 하시겠습니까? (레츠고 : 1 / 게임 종료 : 2) ");
	scanf("%d", &(*level_re_lose));
}