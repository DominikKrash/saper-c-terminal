#include <iostream>
#include <time.h>
#include <Windows.h>
using namespace std;
/////////////Stale
//////////////////
const int BOARD_WIDTH = 11;       //rozmiary
const int BOARD_HEIGHT = 11;	  //tablicy
const int HIS_SIZE = 100; //rozmair histori ruchow 
const int BUFER = 20; //wpisywanie danych
const int BOMBS = 60; //ilosc bomb

//////////////////
//////////////////
//struktury
struct Point{
	int x = -1;
	int y =  -1;
};
struct History{
	char text[HIS_SIZE][BUFER];
	int dl = 0;
};
//funkcje
//sprawdzamy czy gra jest juz wygrana
void GameEndCheck(int bombs[][BOARD_WIDTH - 2], bool &gs,bool &win){
	int counter = 0;
	for (int i = 0; i < BOARD_HEIGHT - 2; i++){
		for (int j = 0; j < BOARD_WIDTH - 2; j++){
			if (bombs[i][j] != -1) counter++;
		}
	}
	if (counter == ((BOARD_HEIGHT - 2)*(BOARD_WIDTH - 2) - BOMBS)){
		win = TRUE;
		gs = !gs;
	}
}
//gs games status a = y b = x
//tutaj wchodzi sterowanie
void BombStatus(int a,int b, int map[][BOARD_WIDTH - 2], int bombs[][BOARD_WIDTH - 2], bool &gs,bool check[][BOARD_WIDTH-2],bool &win){ 
	if (map[a][b] == 1){
		check[a][b] = TRUE;
		win = FALSE;
		gs = !gs;
		return;
	}
	int counter = 0;
	for (int i = -1; i < 2; i++){
		for (int j = -1; j < 2; j++){
			if (a + i < 9 && b + j < 9 && a + i >= 0 && b + j >= 0){
				if (i == 0 && j == 0)continue;
				if (map[a + i][b + j] == 1) counter++;
			}
		}
	}
	bombs[a][b] = counter;
	check[a][b] = TRUE;
	if (counter == 0){
		for (int i = -1; i < 2; i++){
			for (int j = -1; j < 2; j++){
				if (a + i < 9 && b + j < 9 && a + i >= 0 && b + j >= 0){
					if ((i == 0 && j == 0) || check[a+i][b+j] == TRUE) continue;
					BombStatus(a + i, b + j, map, bombs, gs,check,win);
				}
			}
		}
	}
}


void PutBomb(int map[][BOARD_WIDTH - 2],bool check[][BOARD_WIDTH-2], Point p) {
	int x, y;
	x = rand() % (BOARD_WIDTH - 2);
	y = rand() % (BOARD_HEIGHT - 2);
	while (map[y][x] == 1 || (p.x == x && p.y == y)) {
		x = rand() % (BOARD_WIDTH - 2);
		y = rand() % (BOARD_HEIGHT - 2);
	}
	map[y][x] = 1;

}
void RandomizeBombs(int map[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2], Point p) {
	for (int i = 0; i < BOMBS; i++) {
		PutBomb(map,check ,p);
	}
}

void DrawLine() {
	cout << " ";
	for (int i = 0; i < BOARD_WIDTH; i++) {
		if (i == 0 || i == BOARD_WIDTH - 1) {
			cout << "+";
		}
		else {
			cout << "-";
		}
	}
	cout << endl;
}

void DrawTop() {
	char znak = 65;
	cout << "  ";
	for (int i = 0; i < BOARD_WIDTH - 2; i++, znak++) {
		cout << znak;
	}
	cout << endl;
	DrawLine();
}

void DrawMidlle(int map[][BOARD_WIDTH - 2], int bombs[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2]) {
	for (int j = 0; j <BOARD_HEIGHT - 2; j++) {
		cout << j;
		cout << "|";
		for (int i = 0; i < BOARD_WIDTH - 2; i++) {
		if (bombs[j][i] == -1 && check[j][i] == 0) {
		cout << " ";
		}
		else if (bombs[j][i] != -1 && check[j][i] == 1) {
		cout << bombs[j][i];
		}
		else if (bombs[j][i] == -1 && check[j][i] == 1){
			cout << "*";
		}
		else {
		cout << " ";
		}

	
		}
		cout << "|";
		cout << endl;
		}
}
//sterowanie
void ShowBombs(int bombs[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2]){
	for (int j = 0; j < BOARD_HEIGHT - 2; j++) {
		for (int i = 0; i < BOARD_WIDTH - 2; i++) {
			if (bombs[j][i] == 1) check[j][i] = TRUE;
		}
	}
}
void Control(char input[], Point &p, int bombs[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2],bool &chit,History &h) {
	cin >> input;
	
	if (input[0] < 57 && input[0] >47) { //jesli pierwsza to liczba
		strcpy_s(h.text[h.dl++], input);
		p.y = (int(input[0]) - 48);
		p.x = (toupper(input[1]) - 65);
		
	}
	else if (!strcmp(input, "CZITER") || !strcmp(input, "cziter")){
		chit = TRUE;
		ShowBombs(bombs, check);
	}
	else if (toupper(input[0]) >64 && toupper(input[0]) < 74){
		strcpy_s(h.text[h.dl++], input);
		p.y = (int(input[1] - 48));
		p.x = (toupper(input[0]) - 65);
	}
}
void DrawCheat(bool czit){
	if (czit == TRUE){
		cout << endl << "TY OSZUKUJESZ!!!!" << endl;
	}
}
void DrawHistory(History h){
	for (int i = 0; i < h.dl; i++){
		cout << h.text[i] << endl;;
	}
}

//rysowanie planszy
void Draw(int map[][BOARD_WIDTH - 2], int bombs[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2],bool czit,History h) {
	DrawTop();
	DrawMidlle(map, bombs,check);
	DrawLine();
	DrawHistory(h);
	DrawCheat(czit);
}

//zapelniamy tablice
void SET(int map[][BOARD_WIDTH - 2],int p) {
	for (int i = 0; i<BOARD_HEIGHT - 2; i++) {
		for (int j = 0; j < BOARD_WIDTH - 2; j++) {
			map[i][j] = p;
		}
	}
}
//trzeba przeciazyc
void SET(bool map[][BOARD_WIDTH - 2], bool p) {
	for (int i = 0; i<BOARD_HEIGHT - 2; i++) {
		for (int j = 0; j < BOARD_WIDTH - 2; j++) {
			map[i][j] = p;
		}
	}
}
void SET(char map[][BOARD_WIDTH - 2], char p) {
	for (int i = 0; i<BOARD_HEIGHT - 2; i++) {
		for (int j = 0; j < BOARD_WIDTH - 2; j++) {
			map[i][j] = p;
		}
	}
}
//wypisujemy po przegranej
void DrawFail(int map[][BOARD_WIDTH - 2], int bombs[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2], bool &gra, bool &program, bool czit, History h){
	char button;
	while (1){
		system("cls");
		Draw(map,bombs,check,czit,h);
		cout << endl;
		cout << "Niestety przegrales:( czy chcesz kontynuowac? t/n" << endl;
		cin >> button;
		if (toupper(button) == 'T'){
			gra = TRUE;
			return;
		}
		else if (toupper(button) == 'N'){
			program = FALSE;
			return;
		}
	}

}
//wypisujemy po wygranej
void DrawWin(int map[][BOARD_WIDTH - 2], int bombs[][BOARD_WIDTH - 2], bool check[][BOARD_WIDTH - 2], bool &gra, bool &program, bool czit,History h){
	SET(check, TRUE);
	char button;
	while (1){
		system("cls");
		cout << endl;
		Draw(map, bombs, check,czit,h);
		cout << "Wygrales mistrzuniu !!!kontynuowac? t/n" << endl;
		cin >> button;
		if (toupper(button) == 'T'){
			gra = TRUE;
			return;
		}
		else if (toupper(button) == 'N'){
			program = FALSE;
			return;
		}
	}
}
//intro*
void intro(bool &gra,bool &program,bool &skip){
	char button;
	while (1){
		system("cls");
		cout << "Student Dominik wita w grze 'saper'!" << endl;
		cout << "Gra polega na znalezieniu wszystkich miejsc wolnych od bomb" << endl;
		cout << "cyfra na polu oznacza ilosc bomb ktore otaczaja wybrane pole (maks 8)" << endl;
		cout << "Miejsce bomb wybieramy poprzez podanie docelowych wspolrzednych np A9" << endl;
		cout << "Wspolrzedne mozemy podac w obojetnej kolejnosci to znaczy a9 = 9a i wielkosc liter tez nie ma znaczenia" << endl;
		cout << "Zycze wszystkim graczom wspanialej rozgrywki!!!" << endl;
		cout << "Czy chcesz gre? t/n" << endl;
		cin >> button;
		if (toupper(button) == 'T'){
			skip = TRUE;
			return;
		}
		else if (toupper(button) == 'N'){
			program = FALSE;
			gra = !gra;
			return;
		}
	}
}
/////////////////////////
///////////MAIN//////////
////////////////////////
int main()
{
	srand(time(NULL));
	int board[BOARD_HEIGHT - 2][BOARD_WIDTH - 2]; // tablica do rysowania 
	int boardBombs[BOARD_HEIGHT - 2][BOARD_WIDTH - 2]; //miejsce z bombami
	bool checked[BOARD_HEIGHT - 2][BOARD_WIDTH - 2]; // czy mozna wypisac, tutaj sa miejsca ktore wczytalismy 
	Point point; //bedziemy tu zapisywac wczytane koordynaty
	History history;
	char input[BUFER];
	bool gameOVER = TRUE;//dzialanie gry
	bool gameStart = TRUE; // czy to pierwsz tura
	bool win; // czy wygralismy
	bool program = TRUE; // dzialanie programu
	bool skip = FALSE; // jesli odrazu wylaczymy gre nie wypiszemy win/los
	bool cheat = FALSE;

	while (program){///////////////////////
		SET(board, -1);//////////////////////tutaj inicjalizujemy gre na poczatkowy stan
		SET(boardBombs, 0);///////////////////
		SET(checked, FALSE);
		gameStart = TRUE;
		cheat = FALSE;
		history.dl = 0;
		intro(gameOVER, program,skip);
		system("cls");
		while (gameOVER) {

			Draw(boardBombs, board, checked,cheat,history);
			Control(input, point,boardBombs,checked,cheat,history);

			if (gameStart) { //wrzucamy bomby na mape po wybraniu miejsca, tylko raz sie to dzieje
				RandomizeBombs(boardBombs, checked, point);
				gameStart = FALSE;
			}

			if (!gameStart){ // tutaj sprawdzamy ilosc bomb po wczytaniu inputu
				BombStatus(point.y, point.x, boardBombs, board, gameOVER, checked, win);
			}
			GameEndCheck(board, gameOVER, win);
			system("cls");
		}
		if (skip){//jesli nie pominelismy gry
			if (win == FALSE) DrawFail(boardBombs, board, checked, gameOVER, program,cheat,history);
			else DrawWin(boardBombs, board, checked, gameOVER, program,cheat,history);
		}
	}
}
