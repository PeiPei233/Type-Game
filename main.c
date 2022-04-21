#include<stdio.h>
#include<time.h>

#include "libgraphics.h"
#include "extraimgui.h"

double height, width;	//the size of window
double speed = 1;	//the speed of game
int tottime = 0;	//tottime: the total time of game
int isplaying = 0;
double score = 0;
char currentLetter;	//current dropping letter
int thistime;	//the dropping time of current letter

typedef struct {
	double score;
	int time;
	time_t date;
}Ranking;	//ranking data
int cntrank = 0;	//the number of ranking
Ranking rank[100];
int isShowRanking = 0;
int currentRanking = 0;	//the ranking of the game played this time
int isExitGame = 0;
int isShowingHelp = 0;

char inputAns[1010] = "You have entered:";	//the answer characteristic user input
char *colorTable[] = {
	"Black", "Dark Gray", "Gray", "Light Gray", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Magenta", "Cyan"
};	//0~12
int currentColor = 10;	//current input area color

void display();

void MouseEventProcess(int x, int y, int button, int event) {
	uiGetMouse(x, y, button, event);
	display();
}

void CharEventProcess(char ch) {
	uiGetChar(ch);
	// printf("CALL:%c\n", ch);
	display();
}

void KeyboardEventProcess(int key, int event) {
	uiGetKeyboard(key, event);
	display();
}

#define REFRESH_INTERVAL 1
#define COLOR_CHANGE 2

void TimerEventProcess(int timerID) {
	if (timerID == REFRESH_INTERVAL && isplaying) {	//dropping
		tottime+=40;
		thistime+=40;
		display();
	}
	if (timerID == COLOR_CHANGE && isplaying) {	//color change
		currentColor = RandomInteger(0, 12);
		display();
	}
}

void Main() {

	SetWindowTitle("Type Game");

	InitGraphics();
	// InitConsole();
	
	height = GetWindowHeight();
	width = GetWindowWidth();

	//Get ranking data from rank.txt
	memset(rank, 0, sizeof(rank));
	FILE *fp = fopen("rank.txt", "rb");
	fread(&cntrank, sizeof(int), 1, fp);
	fread(rank, sizeof(rank[0]), sizeof(rank) / sizeof(rank[0]), fp);
	fclose(fp);
	printf("CNT:%d\n", cntrank);

	registerCharEvent(CharEventProcess);
	registerKeyboardEvent(KeyboardEventProcess);
	registerMouseEvent(MouseEventProcess);
	registerTimerEvent(TimerEventProcess);

	SetFont("Consolas");
	SetPointSize(15);
	startTimer(REFRESH_INTERVAL, 40);
	startTimer(COLOR_CHANGE, 10000);

	Randomize();

	display();

}

void ExitPlaying() {
	isShowRanking = 1;
	isExitGame = 1;
	Ranking t;	//temporary
	t.score = score;
	t.time = tottime;
	time(&t.date);
	if (cntrank == 0) {
		cntrank = 1;
		rank[cntrank] = t;
	} else {
		int i;
		int p = cntrank + 1;
		for (i = 1; i <= cntrank; i++) {
			if (rank[i].score < score) {	//insert current rank into the ranking list
				p = i;
				break;
			} else if (rank[i].score == score && rank[i].time > tottime) {
				p = i;
				break;
			}
		}
		if (p == cntrank + 1) {
			cntrank++;
			rank[cntrank] = t;
		} else {
			for (i = cntrank + 1; i >= p + 1; i--) {
				rank[i] = rank[i - 1];
			}
			rank[p] = t;
			cntrank++;
		}
		currentRanking = p;
	}
	//save the ranking list
	FILE *fp = fopen("rank.txt", "wb");
	fwrite(&cntrank, sizeof(int), 1, fp);
	fwrite(rank, sizeof(rank[0]), sizeof(rank) / sizeof(rank[0]), fp);
	fclose(fp);

	//show the ranking list when exit
	display();
}

void DrawMenu() {

	static char * menuListSettings[] = {"Settings",
		"Start      | Ctrl-S",
		"Pause      | Ctrl-Z",
		"Speed Up   | Ctrl-A",
		"Slow Down  | Ctrl-D",
		"Exit       | Ctrl-W"
	};
	static char * menuListAbout[] = {"About",
		"Help",
		"Ranking"
	};
	double fH = GetFontHeight();
	double x = 0;
	double y = height;
	double h = fH * 1.5;
	double w = TextStringWidth(menuListSettings[0]) * 2;
	double wlist = TextStringWidth(menuListSettings[4]) *  1.2;
	int selection;
	selection = menuList(GenUIID(0), x, y - h, w, wlist, h, menuListSettings, sizeof(menuListSettings) / sizeof(menuListSettings[0]));
	switch (selection) {
		case 1:		//Start
			isplaying = 1;
			thistime = 0;
			currentLetter = RandomInteger(0, 25) + 'A';
			break;
		case 2:		//Pause
			isplaying = 0;
			break;
		case 3:		//Speed Up
			speed += 0.1;
			break;
		case 4:		//Slow Down
			if (speed > 0.1) speed -= 0.1;
			else speed = 0.1;
			break;
		case 5:		//Exit
			ExitPlaying();
			break;
	}

	selection = menuList(GenUIID(0), x + w, y - h, w, wlist, h, menuListAbout, sizeof(menuListAbout) / sizeof(menuListAbout[0]));
	switch (selection) {
	case 1:		//Help
		isShowingHelp = 1;
		isShowRanking = 0;
		break;
	
	case 2:		//Exit
		isShowRanking = 1;
		isShowingHelp = 0;
		break;
	}
}

void DrawButtons() {
	double fH = GetFontHeight();
	double h = fH * 2;
	double w = width / 9;
	double x = w * 2;
	double y = height / 7 - h;	

	if (button(GenUIID(0), x, y, w, h, "Start")) {
		printf("1");
		isplaying = 1;
		thistime = 0;
		currentLetter = RandomInteger(0, 25) + 'A';
	}
	if (button(GenUIID(0), x + w, y, w, h, "Pause")) {
		printf("2");
		isplaying = 0;
	}
	if (button(GenUIID(0), x + 2 * w, y, w, h, "Speed Up")) {
		printf("3");
		speed += 0.1;
	}
	if (button(GenUIID(0), x + 3 * w, y, w, h, "Slow Down")) {
		printf("4");
		if (speed > 0.1) speed -= 0.1;
		else speed = 0.1;
	}
	if (button(GenUIID(0), x + 4 * w, y, w, h, "Exit")) {
		ExitPlaying();
	}

}

void DrawScorePart() {
	double x = width / 16;
	double y = height * 9.5 / 16;
	double w = width * 6.5 / 16;
	double h = height * 5 / 16;
	char s[100];
	drawRectangle(x, y, w, h, 0);
	SetPointSize(18);
	MovePen(x + w / 3, y + h / 2);
	sprintf(s, "Score:%6.1lf", score);
	DrawTextString(s);
	MovePen(x + w / 3, y + h / 3);
	sprintf(s, "Time:%6ds", tottime / 1000);
	DrawTextString(s);
	MovePen(x + w / 3, y + h * 2 / 3);
	sprintf(s, "Speed:%5.0lf%%", speed * 100);
	DrawTextString(s);
	SetPointSize(15);
}

void DrawLetterPart() {
	double x = width * 8.5 / 16;
	double y = height * 3 / 16;
	double w = width * 6 / 16;
	double h = height * 11.5 / 16;
	SetPenColor(colorTable[currentColor]);
	SetPenSize(4);
	drawRectangle(x, y, w, h, 0);
	SetPenSize(1);
	SetPenColor("Blue");
	SetPointSize(30);
	double fH = GetFontHeight();
	if (thistime * speed * h / 1600 > h - fH) {	//update current letter when dropping to the floor
		thistime = 0;
		currentLetter = RandomInteger(0, 25) + 'A';
	}
	MovePen(x + w / 2, y + h - fH - thistime * speed * h / 1600);
	DrawTextString(CharToString(currentLetter));
	SetPointSize(15);
}

void DrawInputPart() {
	double x = width / 16;
	double y = height * 3 / 16;
	double w = width * 6.5 / 16;
	double h = height * 5.5 / 16;
	drawRectangle(x, y, w, h, 0);
	double fH = GetFontHeight();
	double th = fH * 1.5;
	double tw = w * 3 / 4;
	static char input[100] = "Enter the Letter Here:\n";
	if (textbox(GenUIID(0), x + w / 8, y + h * 3 / 4, tw, th, input, sizeof(input))) {
		// sscanf(input, "%s", inputans);
		char ans = input[strlen(input) - 1];
		if (isplaying) inputAns[strlen(inputAns)] = ans;
		printf("IN:%c\n", ans);
		sprintf(input, "Enter the Letter Here:");
		if ((ans == currentLetter || ans + 'A' - 'a' == currentLetter) && isplaying) {
			score += speed;
			currentLetter = RandomInteger(0, 25) + 'A';
			thistime = 0;
		}
	}
	SetPenColor("Gray");
	DrawTextStringWithinArea(inputAns, x + w / 10, y + h * 3 / 4, w * 8 / 10, h * 3 / 4, th);
	SetPenColor("Blue");
}

void DrawRanking() {
	double x = width / 10;
	double y = height * 2 / 16;
	double w = width * 8 / 10;
	double h = height * 8 / 10;
	drawRectangle(x, y, w, h, 0);
	
	SetPointSize(35);
	double fH = GetFontHeight();
	double fW = TextStringWidth("Ranking List");
	MovePen(x + w / 2 - fW / 2, y + h - fH * 1.4);
	DrawTextString("Ranking List");
	SetPointSize(15);

	SetPointSize(20);
	fH = GetFontHeight();
	MovePen(x, y + h - fH * 1.7 * 3);
	DrawTextString("    Ranking      Score          Time                Date");
	int i;
	char s[110];
	if (currentRanking <= 10) {
		for (i = 1; i <= min(10, cntrank); i++) {
			MovePen(x, y + h - fH * 1.7 * (i + 2.8));
			struct tm *pt = NULL;
			pt = localtime(&rank[i].date);
			sprintf(s, "%8d%13.1lf%13ds%14d-%02d-%02d %02d:%02d:%02d", i, rank[i].score, rank[i].time/1000, (pt->tm_year) + 1900, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
			if (i == currentRanking) SetPenColor("Red");
			DrawTextString(s);
			if (i == currentRanking) SetPenColor("Blue");
		}
	} else {
		for (i = 1; i <= min(9, cntrank); i++) {
			MovePen(x, y + h - fH * 1.7 * (i + 2.8));
			struct tm *pt = NULL;
			pt = localtime(&rank[i].date);
			sprintf(s, "%8d%13.1lf%13ds%14d-%02d-%02d %02d:%02d:%02d", i, rank[i].score, rank[i].time/1000, (pt->tm_year) + 1900, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
			// printf("%s\n", s);
			DrawTextString(s);
		}
		i = currentRanking;
		MovePen(x, y + h - fH * 1.7 * (10 + 2.8));
		struct tm *pt = NULL;
		pt = localtime(&rank[i].date);
		sprintf(s, "%8d%13.1lf%13ds%14d-%02d-%02d %02d:%02d:%02d", i, rank[i].score, rank[i].time/1000, (pt->tm_year) + 1900, pt->tm_mon, pt->tm_mday, pt->tm_hour, pt->tm_min, pt->tm_sec);
		if (i == currentRanking) SetPenColor("Red");
		DrawTextString(s);
		if (i == currentRanking) SetPenColor("Blue");
	}
	SetPointSize(15);

	fH = GetFontHeight();
	double bh = fH * 2;
	double bw = width / 9;
	y = y - fH * 3;
	x = (width - bw) / 2;
	
	if (isExitGame) {
		if (button(GenUIID(0), x, y, bw, bh, "Exit")) {
			ExitGraphics();
		}
	} else {
		if (button(GenUIID(0), x, y, bw, bh, "Back")) {
			isShowRanking = 0;
		}
	}

}

void DrawHelp() {
	double x = width / 18;
	double y = height * 2 / 16;
	double w = width * 16 / 18;
	double h = height * 8 / 10;
	drawRectangle(x, y, w, h, 0);
	
	SetPointSize(35);
	double fH = GetFontHeight();
	double fW = TextStringWidth("Help");
	MovePen(x + w / 2 - fW / 2, y + h - fH * 1.4);
	DrawTextString("Help");
	SetPointSize(15);

	SetPointSize(18);
	fH = GetFontHeight();
	SetPenColor("Black");
	DrawTextStringWithinArea("\
    Welcome! Thank you for playing this simple game programmed by PeiPei!\nYou can stars this project at https://github.com/PeiPei233/Type_Game\n\
  Rules:\n\
    1. Get Start: Press Start button / Setting > Start / Ctrl+S\n\
    2. Get Pause: Press Pause button / Setting > Pause / Ctrl+Z\n\
    3. Speed Up:  Press Speed button / Setting > Speed / Ctrl+A\n\
    4. Slow Down: Press Slow  button / Setting > Slow  / Ctrl+D\n\
    5. Enter your answer in the textbox. If your answer is correct, your\nscore will add by your speed.\n\
  Attention:\n\
    1. Every time you exit, your typing score and time will be recorded and you will see the Ranking List with your rank this time.\n\
    2. There exist some known bugs, and we do not want to fix them.\
	", x + w / 18, y + h - fH * 1.6 * 2, w * 16 / 18, h - 1.6 * fH * 2, fH * 1.5);


	SetPointSize(15);
	fH = GetFontHeight();
	double bh = fH * 2;
	double bw = width / 9;
	y = y - fH * 3;
	x = (width - bw) / 2;

	if (button(GenUIID(0), x, y, bw, bh, "Back")) {
		isShowingHelp = 0;
	}

}

void display() {
	DisplayClear();
	if (isExitGame) {
		DrawRanking();
	} else if (isShowRanking) {
		DrawRanking();
		DrawMenu();
	} else if(isShowingHelp)  {
		DrawHelp();
		DrawMenu();
	} else {
		DrawButtons();
		DrawScorePart();
		DrawLetterPart();
		DrawInputPart();
		DrawMenu();
	}
}