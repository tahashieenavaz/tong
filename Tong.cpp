#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include "alphabet.h"
#define ConsoleWidth 57 // ODD Number
#define ConsoleHeight 44
#define NeedleSize 5 // ODD Nuber 
#define SST 17

int get_header();
int get_footer();
void MoveHumanNeedle( int y );
void MovePCNeedle( int y );
void DrawBorders();
void DoNeedles();
void ChangeBallPosition( int x,int y );
void DrawBall();
void MoveBallByAngle( int Angle );
void gotoxy(int x, int y);
bool CheckNeedleGrablance(int Player);
bool GuessTheWay();
void dST(bool half);
void IndexPage();

bool BallSentForTheFirstTime = false;
long int TheWayPCActsTemp;


struct Pos{
	int x,y;
};
struct Position{
	long int hy,by;
};
struct Direction{
	int up,left;
};
struct Direction BallDir;



struct Position Needle[2];// 0=> User , 1=>PC
struct Pos BallPos;
struct Pos LastBallPos;

int main( void ){
	
	srand(time(NULL));
	// function __construct(){

	    HANDLE in,out;
	    DWORD info;
	    CONSOLE_SCREEN_BUFFER_INFO scrinfo;
	    INPUT_RECORD input;
	    COORD zerozero={0,0},paintareapos={9,2},textpos={25,1};
	    int i,j;
	    in=GetStdHandle(STD_INPUT_HANDLE);
	    out=GetStdHandle(STD_OUTPUT_HANDLE);
	    GetConsoleScreenBufferInfo(in,&scrinfo);
	    
	// }
	
    get_header(); // Calls Other Functions

	while(true){ // Begin The Game
	    SetConsoleMode(in,ENABLE_PROCESSED_INPUT|ENABLE_MOUSE_INPUT);
	    ReadConsoleInput(in,&input,1,&info);
	    //if(input.EventType==MOUSE_EVENT){
	        //if(input.Event.MouseEvent.dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED){
				if( input.Event.MouseEvent.dwMousePosition.X < ConsoleWidth && input.Event.MouseEvent.dwMousePosition.X > ConsoleWidth/2 && input.Event.MouseEvent.dwMousePosition.Y<= ConsoleHeight ){
					MoveHumanNeedle(input.Event.MouseEvent.dwMousePosition.Y);
				}
	        //}
	    //}
	    
	    
	    MovePCNeedle( (BallPos.y + ( (Needle[0].by + Needle[0].hy)/2 ))/2 );
	    
	    // Move The Ball
	    if(!BallSentForTheFirstTime){
	    	//Send The Ball For The First Time
	    	int SendBallX = ConsoleWidth/2,SendBallY = ConsoleHeight/2;
	    	BallPos.x = SendBallX;
	    	BallPos.y = SendBallY;
			DrawBall();
	    	BallSentForTheFirstTime = true;
	    	if( input.Event.MouseEvent.dwMousePosition.X < ConsoleWidth && input.Event.MouseEvent.dwMousePosition.X > ConsoleWidth/2 && input.Event.MouseEvent.dwMousePosition.Y<= ConsoleHeight )
				MoveBallByAngle(45);
		}else{
	    	int SendBallX = ConsoleWidth/2,SendBallY = ConsoleHeight/2;
	    	DrawBall();
	    	if( input.Event.MouseEvent.dwMousePosition.X < ConsoleWidth && input.Event.MouseEvent.dwMousePosition.X > ConsoleWidth/2 && input.Event.MouseEvent.dwMousePosition.Y<= ConsoleHeight )
				MoveBallByAngle(45);
		}
	    
	}
	
	get_footer();
	
}
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!SetConsoleCursorPosition(hConsole, coord)){}

}
int get_header(){
	SMALL_RECT windowSize = {0 , 0 , 75 , 50}; //change the values
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
	SetConsoleTitle("Pong Game By Taha Shieenavaz");
	IndexPage();
	/* User Needle Setup */
	Needle[1].hy = 2 ;
	Needle[1].by = 2+NeedleSize ;
	
	Needle[0].hy = ConsoleWidth-2 ;
	Needle[0].by = Needle[0].hy + NeedleSize ;
	DrawBorders();
	DoNeedles();
	BallDir.left = -1;
	BallDir.up =1;
}

int get_footer(){
	
}
void DrawBorders(){
	int CurrentLine = 0,CurrentColumn = 0;
	
	while( CurrentColumn < ConsoleWidth ){
		gotoxy(CurrentColumn, 0);
		printf("%c",219);
		CurrentColumn++;
		dST(true);
	}
	
	while( CurrentLine < ConsoleHeight ){
		gotoxy(ConsoleWidth, CurrentLine);
		printf("%c",219);
		CurrentLine++;
		dST(true);
	}
	CurrentColumn = ConsoleWidth;
	while( CurrentColumn >= 0 ){
		gotoxy(CurrentColumn, ConsoleHeight);
		printf("%c",219);
		CurrentColumn--;
		dST(true);
	}
	CurrentLine = ConsoleHeight;
	while( CurrentLine >= 0 ){
		gotoxy(0, CurrentLine);
		printf("%c",219);
		CurrentLine--;
		dST(true);
	}
	
	CurrentLine = 0;
	while( CurrentLine < ConsoleHeight ){
		gotoxy(ConsoleWidth/2, CurrentLine);
		printf("%c",219);
		CurrentLine++;
		dST(true);
	}
}
void DoNeedles(){
	// PC Needle
	int CurrentLine=2;
	while( CurrentLine < NeedleSize ){
		gotoxy(2,CurrentLine);
		printf("%c",219);
		CurrentLine++;
	}
	
	// User Needle
	CurrentLine=2;
	while( CurrentLine < NeedleSize ){
		gotoxy(ConsoleWidth-2,CurrentLine);
		printf("%c",219);
		CurrentLine++;
	}
}
void MoveHumanNeedle( int y ){
	int CurrentLine = Needle[0].hy,HalfNeedle = NeedleSize/2;
	if( y+HalfNeedle >= ConsoleHeight || y-HalfNeedle <= 0 )
		return;
	while( CurrentLine < Needle[0].by ){
		gotoxy(ConsoleWidth-2,CurrentLine);
		printf(" ");
		CurrentLine++;	
	}
	Needle[0].hy = y - HalfNeedle;
	Needle[0].by = Needle[0].hy+NeedleSize;
	int Startup = y-1,Startback = y+1;
	gotoxy(ConsoleWidth-2,y);
	printf("%c",219);
	for( int i=1;i<HalfNeedle;i++ ){
		gotoxy(ConsoleWidth-2,Startup);
		printf("%c",219);
		Startup--;
	}
	for( int i=1;i<HalfNeedle;i++ ){
		gotoxy(ConsoleWidth-2,Startback);
		printf("%c",219);
		Startback++;
	}
}

void MovePCNeedle( int y ){
	int CurrentLine = Needle[1].hy,HalfNeedle = NeedleSize/2;
	if( y+HalfNeedle >= ConsoleHeight || y-HalfNeedle <= 0 )
		return;
	while( CurrentLine < Needle[1].by ){
		gotoxy(2,CurrentLine);
		printf(" ");
		CurrentLine++;	
	}
	Needle[1].hy = y - HalfNeedle;
	Needle[1].by = Needle[1].hy+NeedleSize;
	int Startup = y-1,Startback = y+1;
	gotoxy(2,y);
	printf("%c",219);
	for( int i=1;i<HalfNeedle;i++ ){
		gotoxy(2,Startup);
		printf("%c",219);
		Startup--;
	}
	for( int i=1;i<HalfNeedle;i++ ){
		gotoxy(2,Startback);
		printf("%c",219);
		Startback++;
	}
}

void DrawBall(){
	
	// Omit Last Ball
	gotoxy(LastBallPos.x,LastBallPos.y);
	printf(" ");	

	// Print New Ball
	gotoxy(BallPos.x,BallPos.y);
	printf("%c",178);


}
void ChangeBallPosition( int x,int y ){
	
	LastBallPos.y = BallPos.y;
	LastBallPos.x = BallPos.x;
	BallPos.x += x;
	BallPos.y += y;
	
}
/*
Directions[ UP 0 OR DOWN 1 , LEFT 0 OR RIGHT 1 ]
[1,0] = DOWN and LEFT
*/
void MoveBallByAngle( int Angle ){
	switch( Angle ){
		case 45:
			if( BallDir.up == 1 ){ // UP
				
				if( BallDir.left == 1 ){ //LEFT -> PC Player
					
					if( BallPos.y - 1 != 0 && BallPos.x - 3 != 0 )
						ChangeBallPosition(-1,-1);
					else if( BallPos.y - 1 != 0 && BallPos.x - 3 == 0 ){
						if( CheckNeedleGrablance(1) ){
							BallDir.left = -1;
							BallDir.up = 1;
							MoveBallByAngle(45);
						}else{
							exit(1);
						}
					}else if(BallPos.y-1 == 0 && BallPos.x - 3 != 0){
						BallDir.up =-1;
						BallDir.left = 1;
						MoveBallByAngle(45);
					}
					
				}else if( BallDir.left == -1){ // RIGHT -> Human Player
					if( BallPos.x != ConsoleWidth - 3 && BallPos.y- 1 != 0 )
						ChangeBallPosition( +1 , -1 );
					else if( BallPos.x == ConsoleWidth-3 && BallPos.y- 1 != 0 ){
						if( CheckNeedleGrablance(0) ){
							BallDir.up = 1;
							BallDir.left = 1;
							MoveBallByAngle(45);
						}else{
							exit(1);
						}
					}else if( BallPos.x != ConsoleWidth - 3 && BallPos.y- 1 == 0  ){
						BallDir.up = -1;
						BallDir.left = -1;
						MoveBallByAngle(45);
					}
				}
				
			}else if(BallDir.up == -1){ // DOWN
			
				if( BallDir.left ==1 ){ //LEFT
					if( BallPos.x - 3 != 0 && BallPos.y - 1 != ConsoleHeight )
						ChangeBallPosition(-1,+1);
					else if( BallPos.x - 3 == 0 && BallPos.y - 1 != ConsoleHeight ){
						if( CheckNeedleGrablance(1) ){
							BallDir.left = -1;
							BallDir.up = -1;
							MoveBallByAngle(45);	
						}else{
							exit(1);
						}

					}else if( BallPos.x - 3 != 0 && BallPos.y - 1 == ConsoleHeight ){
						BallDir.left = 1;
						BallDir.up = 1;
						MoveBallByAngle(45);
					}
				}else if( BallDir.left == -1){ // RIGHT
					if( BallPos.x + 3 != ConsoleWidth && BallPos.y + 1 != ConsoleHeight )
						ChangeBallPosition(+1,+1);
					else if( BallPos.x + 3 != ConsoleWidth && BallPos.y + 1 == ConsoleHeight ){
						BallDir.up = 1;
						BallDir.left = -1;
						MoveBallByAngle(45);
					}else if( BallPos.x + 3 == ConsoleWidth && BallPos.y + 1 != ConsoleHeight){
						if( CheckNeedleGrablance(0) ){
							BallDir.up = -1;
							BallDir.left = 1;
							MoveBallByAngle(45);
						}else{
							exit(1);
						}

					}
				}
				
			}
			break;
	}
	dST(false);
	DrawBall();
}
bool CheckNeedleGrablance( int Player ){ // User = { 0 -> Human , 1 -> PC }
	
	if( BallPos.y <= Needle[Player].by && BallPos.y >= Needle[Player].hy )
		return true;
	else
		return false;
	
}
bool GuessTheWay(){
	
	TheWayPCActsTemp = rand()%96451+rand()%4529;
	
	if( ( TheWayPCActsTemp % 8 ) % 2 == 0 ) // Unpredictable Way That PC Acts
		return true;
	else
		return false;
		
		
}
void dST(bool half){
	
	if( half )
		Sleep(SST/2);
	else
		Sleep(SST);
		
}
void IndexPage(){
	system("cls");
	printstr(12,5,"ping tong!",219,_RED);
	gotoxy(12,13);
	printf("1. Start The Game");
	gotoxy(12,14);
	printf("2. Show Highscores");
	gotoxy(12,15);
	printf("3. Quit");
	char choice= getch();
	switch( choice - 48 ){
		 case 1:
			break;
			
		case 2:
			system("cls");
			printf("Working");
			getch();
			IndexPage();
			break;
		
		case 3:
			exit(1);
			break;
	}
	system("cls"); 
}