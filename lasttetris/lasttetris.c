/****************************************************************************
* @mainpage Tetris : 테트리스 게임 개작
* @version 1.0
* @date 1985년
* @author 알렉세이 파지노프 (러시아)
* @brief 고전 테트리스 게임을 개선하여 고급진 테트리스 게임을 만든다.
* @detail 출처 : 혼자 연구하는 C/C++ (http://www.soen.kr/)
*            가.테트리스 http://www.soen.kr/lecture/ccpp/cpp2/14-3-1.htm
*            나.벽돌의 모양 정의 http://www.soen.kr/lecture/ccpp/cpp2/14-3-2.htm
*            다.게임판 http://www.soen.kr/lecture/ccpp/cpp2/14-3-3.htm
*            라.Turboc.h http://soen.kr/lecture/ccpp/cpp1/6-4-4.htm
*****************************************************************************/

#include "turboc.h"

/* 상수 */
#define LEFT 75		//←키
#define RIGHT 77	//→키
#define UP 72		//↑키
#define DOWN 80		//↓키   
#define ESC 27
#define BX 5		//게임판의 좌상단 좌표 X (외부 벽의 좌상단 좌표 X)
#define BY 1		//게임판의 좌상단 좌표 Y (외부 벽의 좌상단 좌표 Y)
#define BW 10		//게임판의 폭 (외부벽 제외)
#define BH 20		//게임판의 높이 (외부벽 제외)
#define max(a,b) (((a) > (b)) ? (a) : (b))

/* 함수 */
void drawScreen();			//게임의 배경화면인 게임판을 그린다.
void drawBoard();			//게임판의 내부화면을 그린다. (외부벽과 문자열 제외)
void printBrick(bool Show);	//벽돌을 그린다.
bool moveDown();			//벽돌을 좌우, 아래로 움직인다.
bool processKey();			//키를 눌렀을 때 동작을 정의한다.
int getAround(int x, int y, int b, int r);	//벽돌의 주변을 점검하여 빈공간이 있는지 체크한다.
void testFull();
int leftkey=1;
int righttkey=1;
int upkey=1;
int bongkey;			//게임판이 벽돌로 다 찼는지 체크한다.
int sp=20;
int re=0;
int trot;
int nx, ny; //이동중인 벽돌의 배열상의 현재 좌표 (화면상의 좌표 BX+nx*2, BY+ny)
int brick; //이동중인 벽돌의 번호
int rot; //이동중인 벽돌의 회전 모양 번호
int tnum=0;
int turnNumber=0;
void speed(){
	sp+=80;//속도를 바꾸는 함수
}

void revers(){
	re+=1;//좌우 방향키를 바꾸는 함수
}
int gop=1;
void doublepoint(){
	gop=2;
}
void mipoint(){
	gop=-1;
}
int scorenum=0;
void score(){
	gotoXY(58,3);
	printf("%d",scorenum);
	scorenum += 100*gop;
}
void bong(){
	bongkey=rand()% 3;
	switch (bongkey)
	{
	case 0:leftkey=0;
		break;
	case 1:righttkey=0;
		break;
	case 2:upkey=0;
		break;
	}
	
}
void Nbong(){
	switch (bongkey)
	{
	case 0:leftkey=1;
		break;
	case 1:righttkey=1;
		break;
	case 2:upkey=1;
		break;
	}
	
}


/* 구조체 */
typedef struct point 
{
	int x, y;
} point;


/* 전역 변수 */
//shape[brick][rot][tile], shape[벽돌번호][회전번호][타일번호]
//기준점(0,0)으로부터의 상대좌표를 지정하는 방식
point shape[][4][4] =
{               
	{ 
		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} },
 		{ {0,0},{1,0},{2,0},{-1,0} }, { {0,0},{0,1},{0,-1},{0,-2} } 
	},	//0번 ─
	{ 
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} },
		{ {0,0},{1,0},{0,1},{1,1} }, { {0,0},{1,0},{0,1},{1,1} }
	},	//1번 ㅁ
	{ 
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} },
		{ {0,0},{-1,0},{0,-1},{1,-1} }, { {0,0},{0,1},{-1,0},{-1,-1} }
	},	//2번 ┘┌
	{ 
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} },
		{ {0,0},{-1,-1},{0,-1},{1,0} }, { {0,0},{-1,0},{-1,1},{0,-1} }
	},	//3번 ┐└
	{ 
		{ {0,0},{-1,0},{1,0},{-1,-1} }, { {0,0},{0,-1},{0,1},{-1,1} },
		{ {0,0},{-1,0},{1,0},{1,1} }, { {0,0},{0,-1},{0,1},{1,-1} }
	},	//4번 └─
	{ 
		{ {0,0},{1,0},{-1,0},{1,-1} }, { {0,0},{0,1},{0,-1},{-1,-1} },
		{ {0,0},{1,0},{-1,0},{-1,1} }, { {0,0},{0,-1},{0,1},{1,1} }
	},	//5번 ─┘
	{ 
		{ {0,0},{-1,0},{1,0},{0,1} }, { {0,0},{0,-1},{0,1},{1,0} },
		{ {0,0},{-1,0},{1,0},{0,-1} }, { {0,0},{-1,0},{0,-1},{0,1} }
	}	//6번 ┬
};


enum { EMPTY, BRICK, WALL }; //0-EMPTY-공백, 1-BRICK-다른 벽돌, 2-WALL-벽

char *arTile[] = { ". ","■","□" }; //공간, 벽돌, 벽 모양을 정의하는 문자열 배열, 타일 1개는 2문자로 모양 표시

int board[BW+2][BH+2]; //전체 게임판의 상태


/* main함수 */



/**
* @brief drawScreen 게임의 배경인 게임판을 그린다.
* @detail board[x][y]에 EMPTY(0), BRICK(1), WALL(2)로 설정된 것에 맞게
*         arTile[]을 통해 ". "(0), "■"(1), "□"(2)로 화면에 그린다.
*/

void drawScreen()
{
	int x, y;
	
	for (x=0 ; x<BW+2 ; x++) 
	{
		for (y=0 ; y<BH+2 ; y++) 
		{
			gotoXY(BX+x*2, BY+y);
			puts(arTile[board[x][y]]);
		}
	}

	gotoXY(50, 3);
	printf("score : ");
	gotoXY(58,3);
	printf("0");
	gotoXY(50,4);
	printf("your choice : ");


}



/**
* @brief printBrick 벽돌을 그린다.
* @param bool Show true이면 벽돌을 그리고, false이면 벽돌을 삭제한다.
*/
void printBrick(bool Show)
{
	int i;

	for (i = 0; i<4; i++)
	{
		gotoXY(BX + (shape[brick][rot][i].x + nx) * 2, BY + shape[brick][rot][i].y + ny);
		puts(arTile[Show ? BRICK : EMPTY]);
	}

}


/**
* @brief moveDown 벽돌을 한 칸 씩 아래로 내린다.
* @return bool 다른 벽돌에 닿으면 true를 반환한다.
*/
bool moveDown()
{
	if (getAround(nx, ny + 1, brick, rot) != EMPTY)
	{
		testFull();
		return true;
	}
	printBrick(false);
	ny++;
	printBrick(true);
	return false;
}


/**
* @brief processKey 키입력을 받아 그에 맞는 처리를 한다. 
* @return 이동 중인 벽돌이 다른 벽돌에 닿으면 true를 반환한다.
*/
bool processKey()
{
	int ch;

	if (kbhit()) //눌러진 키가 있으면 true를 반환한다.
	{
		ch = getch(); //<conio.h> getch 값을 입력할 때까지 대기했다가 값이 들어오면 입력버퍼에서 1바이트를 가져온다.
		
		//화살표키(←키→키↑키↓키)는 2byte로 표현되는 확장키코드이다.
		//확장키코드가 입력되었을 경우에는 0 또는 0xE0을 반환한 후, 해당 키 스캔코드를 반환한다.
		if (ch==0xE0 || ch==0) 
		{
			ch = getch();
			
			switch (ch) 
			{
					case LEFT: //←키 입력 : 왼쪽 이동
					if(leftkey==0){
						gotoXY(67,4);
						printf("X");
					}
					else{
						gotoXY(67,4);
						printf("◀");
					}
					
					if (re==0&&getAround(nx-1, ny, brick, rot) == EMPTY && (leftkey==1)|| re==1&&getAround(nx+1,ny,brick,rot)==EMPTY && (leftkey==1)) 
					{
						printBrick(false);
						if(re==0){
							nx--;
						}
						else nx++;
						printBrick(true);
					}
					break;
					case RIGHT:
					if(righttkey==0){
						gotoXY(67,4);
						printf("X");
					}
					else{
						gotoXY(67,4);
						printf("▶");
					}
					if ( re==0&&getAround(nx+1, ny, brick, rot) == EMPTY && (righttkey=1)|| re==1&&getAround(nx-1, ny, brick, rot) == EMPTY && (righttkey=1))
					{
						printBrick(false);
						if(re==0){
							nx++;
						}
						else nx--;
						printBrick(true);
					}
					break;
					case UP:
					if(upkey==0){				
						gotoXY(67,4);
						printf("X");
					}
					else{
						gotoXY(67,4);
						printf("▲");
					}		
					trot = (rot==3 ? 0 : rot+1);
						if ( getAround(nx, ny, brick, trot) == EMPTY && (upkey==1)){
						printBrick(false);
						rot = trot;
						printBrick(true);
						}
					break;
				case DOWN: //↓키 입력 : 아래로 내림
					gotoXY(67,4);
					printf("▼");
					if ( moveDown() ) 
					{
						return true;
					}
					break;
			}

		}
		else {

			switch (ch) 
			{
				case ' ': //space키 입력 : 바닥까지 내림
					while ( moveDown() == false ) 
					{ 
						; 
					}
					return true;

			}

		}

	}

	return false; 

}


/**
* @brief getAround 벽돌의 주변이 공백인지, 다른 벽돌이 있는지 또는 벽인지를 판단한다.
* @param int x 현재 X좌표
* @param int y 현재 Y좌표
* @param int b 벽돌 번호
* @param int r 벽돌의 회전 번호
* @return int 공백이면 EMPTY, 다른 벽돌이 있으면 BRICK, 벽이면 WALL을 반환한다.
*/
int getAround(int x, int y, int b, int r)
{
	int i, k = EMPTY;

	for (i=0 ; i<4 ; i++) 
	{
		k = max(k, board[x + shape[b][r][i].x][y + shape[b][r][i].y]);
	}

	return k;
}

/**
* @brief testFull 게임판이 벽돌로 다 찼는지 체크한다.
* @detail 이동중인 벽돌을 board배열에 기록한다.
*         한줄 모두 벽돌이면 한줄을 지운다.
*/
void testFull()
{
	int i, x, y, ty;

	//이동중인 벽돌을 board 배열에 기록한다.
	for (i=0 ; i<4 ; i++) 
	{
		board[nx + shape[brick][rot][i].x][ny + shape[brick][rot][i].y] = BRICK;
	}

	for (y=1 ; y<BH+1 ; y++) 
	{
		//한줄이 모두 벽돌인지 판단
		for (x=1; x<BW+1; x++) 
		{
			if (board[x][y] != BRICK) 
				break;
		}
		//한줄이 모두 벽돌일 경우, 삭제할 줄 윗줄부터 한칸씩 아래로 복사한다.
		if (x == BW+1) 
		{
			for (ty=y ; ty>1 ; ty--) 
			{
				for (x=1 ; x<BW+1 ; x++) 
				{
					board[x][ty] = board[x][ty-1];
				}
			}
			drawBoard(); //한 줄 삭제 후 게임판 내부화면을 그린다.}
			score();
			delay(200); //0.2초 시간 지연
		}
	}
}
void wave() {
    int x, y=1, ty;

    for (int k = 0; k < 2; k++) {
        for (ty = BH; ty > 1; ty--) {
            for (x = 1; x < BW + 1; x++) {
                board[x][ty] = board[x][ty - 1];
            }
        }

        // 행을 아래로 이동시킨 후, 최상단 행을 비웁니다.
        for (x = 1; x < BW + 1; x++) {
            board[x][1] = EMPTY;
        }

        drawBoard(); // 업데이트된 게임판을 그립니다.
        delay(200); // 0.2초 지연
    }
}
void turn(){
	trot = (rot==3 ? 0 : rot+1);
	if ( getAround(nx, ny, brick, trot) == EMPTY && (upkey==1)){
			printBrick(false);
			rot = trot;
			printBrick(true);
			}
}
int Eventnum = 0;
int checknum = 100;

void Event(int a){
	if(a==1){
			switch (checknum)
			{
			case 0: turnNumber = 0;
				break;
			case 1: re-=1;
				break;
			case 2: gop=+1;
				break;
			case 3: gop=1;
				break;
			case 4: sp=80;
				break;
			case 5: Nbong();
				break;
			}
		}

	if(a == 2){
		
		srand(time(NULL));
		int randnum=rand()% 6;
		Eventnum -= 2;
		checknum = randnum;
		
		switch(randnum){
		case 0:
			turnNumber =1; //계속 회전 할것인지
			break;
		case 1:
			revers(); //방향키 좌우 반전
			break;
		case 2:
			doublepoint(); 
			break;
		case 3:
			mipoint(); 
			break;
		case 4:
			speed(); //속도 변경
			break;
		case 5:
			bong(); //랜덤으로 화살표키 한개중 하나 막음
			break;
		default:
			wave(); //맨아랫줄 삭제  
			break;
		}
	}
}

/**
* @brief drawBoard 게임판의 내부를 그린다.
* @detail 외부벽과 문자열을 제외한 쌓여 있는 벽돌만 그린다.
*         board[x][y]에 EMPTY(0), BRICK(1), WALL(2)로 설정된 것에 맞게
*         arTile[]을 통해 ". "(0), "■"(1), "□"(2)로 화면에 그린다.
*/
void drawBoard()
{
	
	int x, y;

	for (x=1 ; x<BW+1 ; x++) 
	{
		for (y=1 ; y<BH+1 ; y++) 
		{
			gotoXY(BX+x*2, BY+y);
			puts(arTile[board[x][y]]);
		}
	}
	Eventnum++;
	Event(Eventnum);
}

int main()

{
	int nStay; //벽돌이 내려가는 속도
	int x, y;
	int nFrame;
	/*게임 초기화*/
	score();
	setCursorType(NOCURSOR); //커서 숨김
	srand((unsigned)time(NULL)); //난수생성기 초기화
	clearScreen(); //화면 삭제

	//보드 배열의 바깥쪽을 벽으로 초기화
	for (x=0 ; x<BW+2 ; x++) 
	{
		for (y=0 ; y<BH+2 ; y++) 
		{
			board[x][y] = ( y==0 || y==BH+1 || x==0 || x==BW+1 ) ? WALL : EMPTY;
		}
	}

	drawScreen(); //화면 전체 그리기

	nFrame = 10;//벽돌이 떨어지는 속도변수

	while(1)

	{
		/*새 벽돌 생성*/
		brick = rand() % ( sizeof(shape) / sizeof(shape[0]) ); // 0~6번 벽돌 선택하기
		nx = BW/2; //벽돌의 초기 X좌표
		ny = 2; //벽돌의 초기 Y좌표
		rot = 0; //벽돌의 초기 회전모양 번호
		printBrick(true); //벽돌 그리기 함수 호출

		/*게임 끝 점검*/
		if (getAround(nx, ny, brick, rot) != EMPTY) //벽돌 주변이 비었는지 체크
			break;
		nStay = nFrame;

		while(1)
		{
			/*벽돌내림*/
			if (--nStay == 0) 
			{
				nStay = nFrame;
				if (moveDown()) //바닥에 닿으면 true
					break;
			}
			/*키입력 처리*/
			if (processKey()) //다른 벽돌에 닿으면 true
				break;
			/*시간 지연*/
			if(turnNumber==1){
				turn();
			}
			delay(1000/sp);  //0.05초씩 지연 (1000=1초)
		}
	}
	/*게임 끝 처리*/
	clearScreen();
	gotoXY(30, 12); 
	puts("G A M E  O V E R");
	setCursorType(NORMALCURSOR);
	
}
