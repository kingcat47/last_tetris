/****************************************************************************
* @version 1.0
* @date - 2018/4/16
* @author sohi
* @brief ���־� C++ ȯ�濡�� �ͺ� C ��Ÿ���� �Լ��� �����Ѵ�.
* @detail ��ó : ȥ�� �����ϴ� C/C++ (http://www.soen.kr/)
*            ��.Turboc.h http://soen.kr/lecture/ccpp/cpp1/6-4-4.htm
*****************************************************************************/

#ifndef TURBOC_HEADER //TURBOC_HEADER ��ũ�ΰ� ���ǵǾ� ���� ������ �Ʒ� ���� ������
#define TURBOC_HEADER
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

	typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } cursor_type;

	void clearScreen();			// ȭ���� ��� �����. //tc->clrscr();
	void gotoXY(int x, int y);	// Ŀ���� x,y��ǥ�� �̵���Ų��.
	int whereX();				// Ŀ���� x ��ǥ�� �����Ѵ�.
	int whereY();				// Ŀ���� y ��ǥ�� �����Ѵ�.
	void setCursorType(cursor_type c);	// Ŀ���� ����ų� �ٽ� ǥ���Ѵ�.

	#define delay(n) Sleep(n)							// n/1000�ʸ�ŭ �ð� ����
	#define randomize() srand((unsigned)time(NULL))		// ���� �߻��� clearScreen
	                     //<stdlib.h> srand�Լ� : ���� �߻��⸦ �ʱ�ȭ �Ѵ�.
	#define random(n) (rand() % (n))					//0~n������ ���� �߻�
						 //<stdlib.h> rand�Լ� : ����ǥ���� ������ �����Ѵ�.

	// �� ��ũ�ΰ� ���ǵǾ� ������ �Լ��� ������ �����ϰ� ���Ǵ� ���� �ʴ´�.
	#ifndef TURBOC_PROTOTYPE_ONLY
		/**
		* @brief clearScreen ȭ���� ����� Ŀ���� (1,1)�� �ű��.
		*/
		void clearScreen()
		{
			system("cls");
		}

		/**
		* @brief gotoXY �ܼ�ȭ�鿡�� Ŀ���� (x,y)�� �̵���Ų��.
		* @param int �ܼ�ȭ�鿡���� ���� ��ġ�� ����
		* @param int �ܼ�ȭ�鿡���� ���� ��ġ�� ����
		*/
		void gotoXY(int x, int y)
		{
			COORD Cur;
			Cur.X = x;
			Cur.Y = y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
		}

		/**
		* @brief whereX Ŀ���� X ��ǥ�� �����Ѵ�.
		* @return int ���� X ��ǥ
		*/
		int whereX()
		{
			CONSOLE_SCREEN_BUFFER_INFO BufInfo;

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
			return BufInfo.dwCursorPosition.X;
		}

		/**
		* @brief whereY Ŀ���� Y ��ǥ�� �����Ѵ�.
		* @return int ���� Y ��ǥ
		*/
		int whereY()
		{
			CONSOLE_SCREEN_BUFFER_INFO BufInfo;

			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &BufInfo);
			return BufInfo.dwCursorPosition.Y;
		}

		/**
		* @brief setCursorType Ŀ���� ����� �����Ѵ�.
		* @param cursor_type NOCURSOR(0), SOLIDCURSOR(1), NORMALCURSOR(2)
		*/ 
		void setCursorType(cursor_type c)
		{
			CONSOLE_CURSOR_INFO CurInfo;

			switch (c) {
			case NOCURSOR:
				CurInfo.dwSize = 1;
				CurInfo.bVisible = FALSE;
				break;
			case SOLIDCURSOR:
				CurInfo.dwSize = 100;
				CurInfo.bVisible = TRUE;
				break;
			case NORMALCURSOR:
				CurInfo.dwSize = 20;
				CurInfo.bVisible = TRUE;
				break;
			}
			SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
		}

	#endif // TURBOC_PROTOTYPE_ONLY
#endif // TURBOC_HEADER
