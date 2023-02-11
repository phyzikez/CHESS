#include <Windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

///////////// DESK /////////////
// The frame of the desk
char s1 = 201;
char s2 = 200;
char s3 = 187;
char s4 = 188;
char s5 = 205;
char s6 = 186;
char s7 = 209;
char s8 = 207;
char s9 = 199;
char s10 = 182;
char s11 = 197;
char s12 = 179;
char s13 = 196;
char fr = 176;

// Who's move
string moveState = "white";
// The end of game
bool endGame = false;

// Is figures moved (for castling)
bool movedWQ_d1 = false;
bool movedWR_a1 = false;
bool movedWR_h1 = false;
bool movedBQ_d8 = false;
bool movedBR_a8 = false;
bool movedBR_h8 = false;

void showMenu();

// Pair with names of figures and their position
vector<vector<pair<string, string>>> desk;
void fillDesk() {
	// Row 1
	vector<pair<string, string>> row1;
	row1.emplace_back("a1", "WR");
	row1.emplace_back("b1", "WN");
	row1.emplace_back("c1", "WB");
	row1.emplace_back("d1", "WQ");
	row1.emplace_back("e1", "WK");
	row1.emplace_back("f1", "WB");
	row1.emplace_back("g1", "WN");
	row1.emplace_back("h1", "WR");
	desk.push_back(row1);

	// Row 2
	vector<pair<string, string>> row2;
	row2.emplace_back("a2", "WP");
	row2.emplace_back("b2", "WP");
	row2.emplace_back("c2", "WP");
	row2.emplace_back("d2", "WP");
	row2.emplace_back("e2", "WP");
	row2.emplace_back("f2", "WP");
	row2.emplace_back("g2", "WP");
	row2.emplace_back("h2", "WP");
	desk.push_back(row2);

	// Rows 3-6
	for (int i = 3; i < 7; i++) {
		vector<pair<string, string>> rowMiddle;
		for (char j = 'a'; j < 'i'; j++) {
			rowMiddle.emplace_back(j + to_string(i), "  ");
		}
		desk.push_back(rowMiddle);
	}

	// Row 7
	vector<pair<string, string>> row7;
	row7.emplace_back("a7", "BP");
	row7.emplace_back("b7", "BP");
	row7.emplace_back("c7", "BP");
	row7.emplace_back("d7", "BP");
	row7.emplace_back("e7", "BP");
	row7.emplace_back("f7", "BP");
	row7.emplace_back("g7", "BP");
	row7.emplace_back("h7", "BP");
	desk.push_back(row7);

	// Row 8
	vector<pair<string, string>> row8;
	row8.emplace_back("a8", "BR");
	row8.emplace_back("b8", "BN");
	row8.emplace_back("c8", "BB");
	row8.emplace_back("d8", "BQ");
	row8.emplace_back("e8", "BK");
	row8.emplace_back("f8", "BB");
	row8.emplace_back("g8", "BN");
	row8.emplace_back("h8", "BR");
	desk.push_back(row8);
}

// Show the desk
void showDesk() {
	cout << endl;
	cout << "     a" << "   b" << "   c" << "   d" << "   e" << "   f" << "   g" << "   h" << endl;
	// Upper row
	cout << "   " << s1;
	cout << s5 << s5;
	for (int i = 0; i < 7; i++) {
		cout << s5 << s7 << s5 << s5;
	}
	cout << s5 << s3;
	cout << endl;
	// Main block
	for (int t = 0; t < 7; t++) {
		// Row with letters
		cout << " " << t + 1 << " ";
		cout << s6;
		for (int i = 0; i < 7; i++) {
			cout << desk[t][i].second << " " << s12;
		}
		cout << desk[t][7].second << " " << s6 << " " << t + 1;
		// Row without letters
		cout << endl;
		cout << "   " << s9;
		for (int i = 0; i < 7; i++) {
			cout << s13 << s13 << s13 << s11;
		}
		cout << s13 << s13 << s13 << s10;
		cout << endl;
	}
	// Row with letters
	cout << " 8 " << s6;
	for (int i = 0; i < 7; i++) {
		cout << desk[7][i].second << " " << s12;
	}
	cout << desk[7][7].second << " " << s6 << " 8";
	// Bottom row
	cout << endl;
	cout << "   " << s2;
	cout << s5 << s5;
	for (int i = 0; i < 7; i++) {
		cout << s5 << s8 << s5 << s5;
	}
	cout << s5 << s4;
	cout << endl;
	cout << "     a" << "   b" << "   c" << "   d" << "   e" << "   f" << "   g" << "   h" << endl << endl;
}

// Check valid move
bool validMoveCheck(string figure, string fromTo, string whereTo);

// Check valid castling
bool validCheckCastling(string castleCell) {
	bool CC = true;
	if (castleCell == "h8") {
		// Check some white figure move for long castling
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'W') {
					string tempWhiteFigure = desk[i][j].second;
					string tempWhitePos = desk[i][j].first;
					if (validMoveCheck(tempWhiteFigure, tempWhitePos, "d8") ||
						validMoveCheck(tempWhiteFigure, tempWhitePos, "e8") ||
						validMoveCheck(tempWhiteFigure, tempWhitePos, "f8") ||
						validMoveCheck(tempWhiteFigure, tempWhitePos, "g8")) {
						CC = false;
					}
				}
			}
		}
	}
	if (castleCell == "a8") {
		// Check some white figure move for short castling
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'W') {
					string tempWhiteFigure = desk[i][j].second;
					string tempWhitePos = desk[i][j].first;
					if (validMoveCheck(tempWhiteFigure, tempWhitePos, "d8") ||
						validMoveCheck(tempWhiteFigure, tempWhitePos, "c8") ||
						validMoveCheck(tempWhiteFigure, tempWhitePos, "b8")) {
						CC = false;
					}
				}
			}
		}
	}
	if (castleCell == "h1") {
		// Check some black figure move for long castling
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'B') {
					string tempBlackFigure = desk[i][j].second;
					string tempBlackPos = desk[i][j].first;
					if (validMoveCheck(tempBlackFigure, tempBlackPos, "d1") ||
						validMoveCheck(tempBlackFigure, tempBlackPos, "e1") ||
						validMoveCheck(tempBlackFigure, tempBlackPos, "f1") ||
						validMoveCheck(tempBlackFigure, tempBlackPos, "g1")) {
						CC = false;
					}
				}
			}
		}
	}
	if (castleCell == "h1") {
		// Check some black figure move for short castling
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'B') {
					string tempBlackFigure = desk[i][j].second;
					string tempBlackPos = desk[i][j].first;
					if (validMoveCheck(tempBlackFigure, tempBlackPos, "d1") ||
						validMoveCheck(tempBlackFigure, tempBlackPos, "c1") ||
						validMoveCheck(tempBlackFigure, tempBlackPos, "b1")) {
						CC = false;
					}
				}
			}
		}
	}
	return CC;
}

void move() {
	cout << "Let's move:";
};

// Castling figures
void castling() {
	cout << "You are doing castling..." << endl;
	string posRook;
	if (moveState == "white") {
		do {
			cout << "Choose [a1] or [h1]:\t";
			cin >> posRook;
		} while (posRook != "a1" && posRook != "h1");
	}
	else {
		do {
			cout << "Choose [a8] or [h8]:\t";
			cin >> posRook;
		} while (posRook != "a8" && posRook != "h8");
	}
	// Castling for black
	if (movedBQ_d8 == false && moveState == "black") {
		if (posRook == "h8" && movedBR_h8 == false) {
			if (desk[7][4].second[0] == ' ' && desk[7][5].second[0] == ' ' && desk[7][6].second[0] == ' ') {
				for (int i = 3; i <= 7; i++) {
					if (validCheckCastling("h8")) {
						desk[7][3].second = "  ";
						desk[7][4].second = "  ";
						desk[7][5].second = "BR";
						desk[7][6].second = "BQ";
						desk[7][7].second = "  ";
						movedBQ_d8 = true;
						movedBR_h8 = true;
						moveState = "white";
						showMenu();
					}
				}
			}
		}
		else if (posRook == "a8" && movedBR_a8 == false) {
			if (desk[7][1].second[0] == ' ' && desk[7][2].second[0] == ' ') {
				for (int i = 0; i <= 3; i++) {
					if (validCheckCastling("a8")) {
						desk[7][0].second = "  ";
						desk[7][1].second = "BQ";
						desk[7][2].second = "BR";
						desk[7][3].second = "  ";
						movedBQ_d8 = true;
						movedBR_a8 = true;
						moveState = "white";
						showMenu();
					}
				}
			}
		}
	}
	// Castling for white
	if (movedWQ_d1 == false && moveState == "white") {
		if (posRook == "h1" && movedWR_h1 == false) {
			if (desk[0][4].second[0] == ' ' && desk[0][5].second[0] == ' ' && desk[0][6].second[0] == ' ') {
				for (int i = 3; i <= 7; i++) {
					if (validCheckCastling("h1")) {
						desk[0][3].second = "  ";
						desk[0][4].second = "  ";
						desk[0][5].second = "WR";
						desk[0][6].second = "WQ";
						desk[0][7].second = "  ";
						movedWQ_d1 = true;
						movedWR_h1 = true;
						moveState = "black";
						showMenu();
					}
				}
			}
		}
		else if (posRook == "a1" && movedWR_a1 == false) {
			if (desk[0][1].second[0] == ' ' && desk[0][2].second[0] == ' ') {
				for (int i = 0; i <= 3; i++) {
					if (validCheckCastling("a1")) {
						desk[0][0].second = "  ";
						desk[0][1].second = "WQ";
						desk[0][2].second = "WR";
						desk[0][3].second = "  ";
						movedWQ_d1 = true;
						movedWR_a1 = true;
						moveState = "black";
						showMenu();
					}
				}
			}
		}
	}
	cout << "Can't do castling!" << endl;
};

// Restart
void restart() {
	cout << "restart";
	Sleep(1000);
	system("cls");
	desk.clear();
	fillDesk();
	showDesk();
	moveState = "white";
	movedWQ_d1 = false;
	movedWR_a1 = false;
	movedWR_h1 = false;
	movedBQ_d8 = false;
	movedBR_a8 = false;
	movedBR_h8 = false;
};

// Menu
void showMenu() {
	system("cls");
	// Show game desk
	showDesk();
	// Who moves now
	cout << "Now " << moveState << " play:" << endl;
	// Show menu
	for (int i = 0; i < 20; i++) cout << fr; cout << " MENU "; for (int i = 0; i < 20; i++) cout << fr; cout << endl;
	cout << fr << "m - move\t" << fr << " c - castling\t" << fr << " r - restart" << fr << endl;
	for (int i = 0; i < 46; i++) cout << fr; cout << endl;
	// Make choice
	string menuChoise;
	bool cor = false;
	do {
		cin >> menuChoise;
		if (menuChoise[0] == 'm') {
			move();
			cor = true;
		}
		else if (menuChoise[0] == 'c') {
			castling();
			cor = true;
		}
		else if (menuChoise[0] == 'r') {
			cout << "If you wish to restart, press [y]?" << endl;
			string yes;
			cin >> yes;
			if (yes[0] == 'y') {
				restart();
				cout << "White begins:";
				cor = true;
			}
			else {
				cor = false;
				cout << "Please, make a correct choise!" << endl;
			}
		}
		else {
			cout << "Your choise is incorrect! Please, make a correct choise!" << endl;
		}
	} while (!cor);
}

string figure;
string fromTo;
string whereTo;

// Enter params for moving
void enterMove() {
	cout << endl;
	do {
		cout << "Enter correct figure's name:\t\t";
		cin >> figure;
	} while (figure != "WR" && figure != "WN" && figure != "WB" && figure != "WQ" && figure != "WK" && figure != "WP" &&
		figure != "BR" && figure != "BN" && figure != "BB" && figure != "BQ" && figure != "BK" && figure != "BP"
		);
	do {
		cout << "Enter correct figure's position:\t";
		cin >> fromTo;
	} while (
		(fromTo[0] != 'a' && fromTo[0] != 'b' && fromTo[0] != 'c' && fromTo[0] != 'd' &&
			fromTo[0] != 'e' && fromTo[0] != 'f' && fromTo[0] != 'g' && fromTo[0] != 'h') ||
			(fromTo[1] != '1' && fromTo[1] != '2' && fromTo[1] != '3' && fromTo[1] != '4' &&
				fromTo[1] != '5' && fromTo[1] != '6' && fromTo[1] != '7' && fromTo[1] != '8') ||
				(fromTo[2] != '\0')
		);
	do {
		cout << "Enter correct place you wish to go:\t";
		cin >> whereTo;
	} while (
		(whereTo[0] != 'a' && whereTo[0] != 'b' && whereTo[0] != 'c' && whereTo[0] != 'd' &&
			whereTo[0] != 'e' && whereTo[0] != 'f' && whereTo[0] != 'g' && whereTo[0] != 'h') ||
			(whereTo[1] != '1' && whereTo[1] != '2' && whereTo[1] != '3' && whereTo[1] != '4' &&
				whereTo[1] != '5' && whereTo[1] != '6' && whereTo[1] != '7' && whereTo[1] != '8') ||
				(whereTo[2] != '\0')
		);
}

int validNum = 0;

// Is the color of picked figure is valid
void ownColorChoise(string figure, string fromTo, string whereTo) {
	int temp = 0;
	if ((moveState == "white" && figure[0] == 'W') || (moveState == "black" && figure[0] == 'B')) {
		++validNum;
		++temp;
	}
	if (temp == 0) {
		//cout << "Pick your color team!" << endl;
	}
}

// Check name of figure in current position
void enableFigureFrom(string figure, string fromTo, string whereTo) {
	int temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((desk[i][j].second == figure) && (desk[i][j].first == fromTo)) {
				++validNum;
				++temp;
			}
		}
	}
	if (temp == 0) {
		//cout << "ERROR 2. Can't find this figure on desk! Please, reenter!" << endl;
	}
}

// Check outside the field
void limitsFromWhere(string figure, string fromTo, string whereTo) {
	if (fromTo[0] >= 97 && fromTo[0] <= 104 && fromTo[1] >= 49 && fromTo[1] <= 56 && whereTo[0] >= 97 && whereTo[0] <= 104 && whereTo[1] >= 49 && whereTo[1] <= 56) {
		++validNum;
	}
	else {
		//cout << "Desk has only 64 cells!" << endl;
	}
}

// Check "friend"-color of figure in position
void notRelationsWhere(string figure, string fromTo, string whereTo) {
	int temp = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((desk[i][j].first == whereTo) && (desk[i][j].second[0] != figure[0])) {
				++validNum;
				++temp;
			}
		}
	}
	if (temp == 0) {
		//cout << "You can't go there, there has already chills your Ally!" << endl;
	}
}

// Check valid horizonal move
bool validHorisontal(string fromTo, string whereTo) {
	int num = fromTo[1];
	int sym;
	if (fromTo[0] < whereTo[0]) {
		for (sym = fromTo[0] - 97 + 1; sym < whereTo[0] - 97; sym++) {
			if (desk[num - 1 - 48][sym].second[0] != ' ') {
				return false;
			}
		}
	}
	else if (fromTo[0] > whereTo[0]) {
		for (sym = fromTo[0] - 97 - 1; sym > whereTo[0] - 97; sym--) {
			if (desk[num - 1 - 48][sym].second[0] != ' ') {
				return false;
			}
		}
	}
	return true;
}
// Check valid vertical move
bool validVertical(string fromTo, string whereTo) {
	int sym = fromTo[0];
	int num;
	if (fromTo[1] < whereTo[1]) {
		for (num = fromTo[1] - 49 + 1; num < whereTo[1] - 49; num++) {
			if (desk[num][sym - 97].second[0] != ' ') {
				return false;
			}
		}
	}
	else if (fromTo[1] > whereTo[1]) {
		for (num = fromTo[1] - 49 - 1; num > whereTo[1] - 49; num--) {
			if (desk[num][sym - 97].second[0] != ' ') {
				return false;
			}
		}
	}
	return true;
}
// Check valid diagonal move
bool validDiagonal(string fromTo, string whereTo) {
	int sym;
	int num;
	if (fromTo[0] < whereTo[0]) {
		// leftTop --> rightBottom
		if (fromTo[1] < whereTo[1]) {
			for (num = fromTo[1] - 49 + 1, sym = fromTo[0] - 97 + 1; num < whereTo[1] - 49, sym < whereTo[0] - 97; num++, sym++) {
				if (num < 0) num++;
				if (num > 7) num--;
				if (desk[num][sym].second[0] != ' ') {
					return false;
				}
			}
		}
		// leftBottom --> rightTop
		else if (fromTo[1] > whereTo[1]) {
			for (num = fromTo[1] - 49 - 1, sym = fromTo[0] - 97 + 1; num > whereTo[1] - 49, sym < whereTo[0] - 97; num--, sym++) {
				if (num < 0) num++;
				if (num > 7) num--;
				if (desk[num][sym].second[0] != ' ') {
					return false;
				}
			}
		}
	}
	if (fromTo[0] > whereTo[0]) {
		// rightTop --> leftBottom
		if (fromTo[1] < whereTo[1]) {
			for (num = fromTo[1] - 49 + 1, sym = fromTo[0] - 97 - 1; num < whereTo[1] - 49, sym > whereTo[0] - 97; num++, sym--) {
				if (num < 0) num++;
				if (num > 7) num--;
				if (desk[num][sym].second[0] != ' ') {
					return false;
				}
			}
		}
		// rightBottom --> leftTop
		else if (fromTo[1] > whereTo[1]) {
			for (num = fromTo[1] - 49 - 1, sym = fromTo[0] - 97 - 1; num > whereTo[1] - 49, sym > whereTo[0] - 97; num--, sym--) {
				if (num < 0) num++;
				if (num > 7) num--;
				if (desk[num][sym].second[0] != ' ') {
					return false;
				}
			}
		}
	}
	return true;
}
// Is valid figure's trajectory
void trueTrajectory(string figure, string fromTo, string whereTo) {
	// Knight
	if (figure == "WN" || figure == "BN") {
		if (
			((fromTo[0] == whereTo[0] + 2) && (fromTo[1] == whereTo[1] + 1)) ||
			((fromTo[0] == whereTo[0] + 2) && (fromTo[1] == whereTo[1] - 1)) ||
			((fromTo[0] == whereTo[0] - 2) && (fromTo[1] == whereTo[1] + 1)) ||
			((fromTo[0] == whereTo[0] - 2) && (fromTo[1] == whereTo[1] - 1)) ||
			((fromTo[0] == whereTo[0] + 1) && (fromTo[1] == whereTo[1] + 2)) ||
			((fromTo[0] == whereTo[0] + 1) && (fromTo[1] == whereTo[1] - 2)) ||
			((fromTo[0] == whereTo[0] - 1) && (fromTo[1] == whereTo[1] + 2)) ||
			((fromTo[0] == whereTo[0] - 1) && (fromTo[1] == whereTo[1] - 2))
			) {
			++validNum;
		}
		else {
			//cout << "This is not Knight style!" << endl;
		}
	}
	// Rook
	if (figure == "WR" || figure == "BR") {
		int temp = 0;
		if (fromTo[0] == whereTo[0] || fromTo[1] == whereTo[1]) {
			++validNum;
			++temp;
		}
		if (fromTo[1] == whereTo[1]) {
			if (validHorisontal(fromTo, whereTo) == false) {
				--validNum;
			}
		}
		else if (fromTo[0] == whereTo[0]) {
			if (validVertical(fromTo, whereTo) == false) {
				--validNum;
			}
		}
		if (temp == 0) {
			//cout << "This is not ROOK style!" << endl;
		}
	}
	// Bishop
	if (figure == "WB" || figure == "BB") {
		//int temp = 0;
		for (int i = 1; i < 8; i++) {
			if (
				((fromTo[0] == whereTo[0] + i) && (fromTo[1] == whereTo[1] + i)) ||
				((fromTo[0] == whereTo[0] + i) && (fromTo[1] == whereTo[1] - i)) ||
				((fromTo[0] == whereTo[0] - i) && (fromTo[1] == whereTo[1] + i)) ||
				((fromTo[0] == whereTo[0] - i) && (fromTo[1] == whereTo[1] - i))
				) {
				++validNum;
				//++temp;
			}
		}
		if (validDiagonal(fromTo, whereTo) == false) {
			--validNum;
		}
		//if (temp == 0) {
			//cout << "This is not Bishop style!" << endl;
		//}
	}
	// Queen
	if (figure == "WK" || figure == "BK") {
		if (fromTo[0] == whereTo[0] || fromTo[1] == whereTo[1]) { ++validNum; }
		if (fromTo[1] == whereTo[1]) {
			if (validHorisontal(fromTo, whereTo) == false) {
				--validNum;
			}
		}
		else if (fromTo[0] == whereTo[0]) {
			if (validVertical(fromTo, whereTo) == false) {
				--validNum;
			}
		}
		for (int i = 1; i < 8; i++) {
			if (
				((fromTo[0] == whereTo[0] + i) && (fromTo[1] == whereTo[1] + i)) ||
				((fromTo[0] == whereTo[0] + i) && (fromTo[1] == whereTo[1] - i)) ||
				((fromTo[0] == whereTo[0] - i) && (fromTo[1] == whereTo[1] + i)) ||
				((fromTo[0] == whereTo[0] - i) && (fromTo[1] == whereTo[1] - i))
				) {
				++validNum;
			}
		}
		if (validDiagonal(fromTo, whereTo) == false) {
			--validNum;
		}
	}
	// King
	if (figure == "WQ" || figure == "BQ") {
		int temp = 0;
		if (
			((fromTo[0] == whereTo[0] + 1) && (fromTo[1] == whereTo[1] + 1)) ||
			((fromTo[0] == whereTo[0] + 1) && (fromTo[1] == whereTo[1] - 1)) ||
			((fromTo[0] == whereTo[0] - 1) && (fromTo[1] == whereTo[1] + 1)) ||
			((fromTo[0] == whereTo[0] - 1) && (fromTo[1] == whereTo[1] - 1)) ||
			((fromTo[0] == whereTo[0]) && (fromTo[1] == whereTo[1] + 1)) ||
			((fromTo[0] == whereTo[0]) && (fromTo[1] == whereTo[1] - 1)) ||
			((fromTo[0] == whereTo[0] + 1) && (fromTo[1] == whereTo[1])) ||
			((fromTo[0] == whereTo[0] - 1) && (fromTo[1] == whereTo[1]))
			) {
			++validNum;
			++temp;
		}
		if (temp == 0) {
			//cout << "This is not KING style!" << endl;
		}
	}
	// Pawn
	if (figure == "WP" || figure == "BP") {
		int temp = 0;
		// Check pwan kick enemy diagonally
		string posPawn = desk[fromTo[1] - 49][fromTo[0] - 97].first;
		if (figure == "WP") {
			if (fromTo[0] - 97 == 0) {
				string tempPos = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 + 1].first;
				string tempVal = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 + 1].second;
				if (whereTo == tempPos && tempVal[0] != figure[0] && tempVal[0] != ' ') {
					++validNum;
					++temp;
				}
			}
			else if (fromTo[0] - 97 == 7) {
				string tempPos = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 - 1].first;
				string tempVal = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 - 1].second;
				if (whereTo == tempPos && tempVal[0] != figure[0] && tempVal[0] != ' ') {
					++validNum;
					++temp;
				}
			}
			else if (fromTo[0] - 97 > 0 && fromTo[0] - 97 < 7) {
				string tempPos1 = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 + 1].first;
				string tempVal1 = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 + 1].second;
				string tempPos2 = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 - 1].first;
				string tempVal2 = desk[fromTo[1] - 49 + 1][fromTo[0] - 97 - 1].second;
				if (whereTo == tempPos1 && tempVal1[0] != figure[0] && tempVal1[0] != ' ') {
					++validNum;
					++temp;
				}
				if (whereTo == tempPos2 && tempVal2[0] != figure[0] && tempVal2[0] != ' ') {
					++validNum;
					++temp;
				}
			}
		}
		else if (figure == "BP") {
			if (fromTo[0] - 97 == 0) {
				string tempPos = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 + 1].first;
				string tempVal = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 + 1].second;
				if (whereTo == tempPos && tempVal[0] != figure[0] && tempVal[0] != ' ') {
					++validNum;
					++temp;
				}
			}
			else if (fromTo[0] - 97 == 7) {
				string tempPos = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 - 1].first;
				string tempVal = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 - 1].second;
				if (whereTo == tempPos && tempVal[0] != figure[0] && tempVal[0] != ' ') {
					++validNum;
					++temp;
				}
			}
			else if (fromTo[0] - 97 > 0 && fromTo[0] - 97 < 7) {
				string tempPos1 = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 + 1].first;
				string tempVal1 = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 + 1].second;
				string tempPos2 = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 - 1].first;
				string tempVal2 = desk[fromTo[1] - 49 - 1][fromTo[0] - 97 - 1].second;
				if (whereTo == tempPos1 && tempVal1[0] != figure[0] && tempVal1[0] != ' ') {
					++validNum;
					++temp;
				}
				if (whereTo == tempPos2 && tempVal2[0] != figure[0] && tempVal2[0] != ' ') {
					++validNum;
					++temp;
				}
			}
		}
		// Standard pawn move 
		if (whereTo[0] == fromTo[0] && desk[whereTo[1] - 49][whereTo[0] - 97].second[0] == ' ') {
			// White spawn
			if (figure == "WP") {
				if (whereTo[1] > fromTo[1]) {
					if ((fromTo[1] == '2' && (whereTo[1] == '3' || whereTo[1] == '4')) || (whereTo[1] == fromTo[1] + 1)) {
						++validNum;
						++temp;
					}
				}
			}
			// Black spawn
			if (figure == "BP") {
				if (whereTo[1] < fromTo[1]) {
					if ((fromTo[1] == '7' && (whereTo[1] == '6' || whereTo[1] == '5')) || (whereTo[1] == fromTo[1] - 1)) {
						++validNum;
						++temp;
					}
				}
			}
		}
		if (temp == 0) {
			//cout << "This is not PAWN style!" << endl;
		}
	}
}

// Check complete move valid
bool validMove(string figure, string fromTo, string whereTo) {
	ownColorChoise(figure, fromTo, whereTo);	// 1
	enableFigureFrom(figure, fromTo, whereTo);	// 2
	limitsFromWhere(figure, fromTo, whereTo);	// 3
	notRelationsWhere(figure, fromTo, whereTo); // 4
	trueTrajectory(figure, fromTo, whereTo);	// 5
	if (validNum == 5) {
		validNum = 0;
		return true;
	}
	validNum = 0;
	//cout << "No valid move!" << endl;
	return false;
}

string oldFigure;
// Make move
void makeMove(string figure, string fromTo, string whereTo) {
	for (auto& i : desk) {
		for (auto& j : i) {
			if (j.first == fromTo && j.second == figure) {
				j.second = "  ";
			}
			if (j.first == whereTo) {
				oldFigure = j.second;
				j.second = figure;
				// 1. Check Rook and King for further castling
				if (figure == "WR") {
					if (fromTo == "a1") {
						movedWR_a1 = true;
					}
					else if (fromTo == "h1") {
						movedWR_h1 = true;
					}
				}
				else if (figure == "BR") {
					if (fromTo == "a8") {
						movedBR_a8 = true;
					}
					else if (fromTo == "h8") {
						movedBR_h8 = true;
					}
				}
				else if (figure == "WQ") {
					movedWQ_d1 = true;
				}
				else if (figure == "BQ") {
					movedBQ_d8 = true;
				}
				// 2. Check spawn promotion
				else if (figure == "BP" || figure == "WP") {
					bool promo = false;
					string promoWhite;
					// White:
					if (figure == "WP" && whereTo[1] == '8' && fromTo[1] == '7') {
						cout << "Write FIGURE to Promotion:" << endl;
						cout << "WR | WN | WB | WK" << endl;
						do {
							cin >> promoWhite;
							if (promoWhite == "WR" || promoWhite == "WN" || promoWhite == "WB" || promoWhite == "WK") {
								promo = true;
							}
						} while (promo == false);
						desk[7][int(whereTo[0] - 97)].second = promoWhite;
					}
					// Black:
					string promoBlack;
					if (figure == "BP" && whereTo[1] == '1' && fromTo[1] == '2') {
						cout << "Write FIGURE to Promotion:" << endl;
						cout << "BR | BN | BB | BK" << endl;
						do {
							cin >> promoBlack;
							if (promoBlack == "BR" || promoBlack == "BN" || promoBlack == "BB" || promoBlack == "BK") {
								promo = true;
							}
						} while (promo == false);
						desk[0][int(whereTo[0] - 97)].second = promoBlack;
					}
				}
			}
		}
	}
}

// Return move back
void cancelMove(string figure, string fromTo, string whereTo) {
	for (auto& i : desk) {
		for (auto& j : i) {
			if (j.first == whereTo && j.second == figure) {
				j.second = oldFigure;
			}
			if (j.first == fromTo) {
				j.second = figure;
			}
		}
	}
}

// Check figure's color for enemy valid move on King position
void checkColorChoise(string figure, string fromTo, string whereTo) {
	int temp = 0;
	if ((moveState == "white" && figure[0] == 'B') || (moveState == "black" && figure[0] == 'W')) {
		++validNum;
		++temp;
	}
	if (temp == 0) {
		//cout << "Pick your color team!" << endl;
	}
}
// 2. Check valid move according to all criteria
bool validMoveCheck(string figure, string fromTo, string whereTo) {
	checkColorChoise(figure, fromTo, whereTo);	// 1
	enableFigureFrom(figure, fromTo, whereTo);	// 2
	limitsFromWhere(figure, fromTo, whereTo);	// 3
	notRelationsWhere(figure, fromTo, whereTo); // 4
	trueTrajectory(figure, fromTo, whereTo);	// 5
	if (validNum == 5) {
		validNum = 0;
		return true;
	}
	validNum = 0;
	//cout << "No valid move!" << endl;
	return false;
}

// Checking "Check"
bool isCheck() {
	bool check = false;
	// 1. "Check" white
	if (moveState == "white") {
		string posWQ;
		// Take current position of white King
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second == "WQ") {
					posWQ = desk[i][j].first;
				}
			}
		}
		// Check any black figure into position of white King
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'B') {
					string tempBlackFigure = desk[i][j].second;
					string tempBlackPos = desk[i][j].first;
					if (validMoveCheck(tempBlackFigure, tempBlackPos, posWQ)) {
						check = true;
					}
				}
			}
		}
	}
	// 2. Check "check" black
	if (moveState == "black") {
		string posBQ;
		// Take current position of black King
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second == "BQ") {
					posBQ = desk[i][j].first;
				}
			}
		}
		// Check any white figure into position of black King
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'W') {
					string tempWhiteFigure = desk[i][j].second;
					string tempWhitePos = desk[i][j].first;
					if (validMoveCheck(tempWhiteFigure, tempWhitePos, posBQ)) {
						check = true;
					}
				}
			}
		}
	}
	return check;
}

// Check move is enable
bool isMovable() {
	bool movable = false;
	// Check white move 
	if (moveState == "white") {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'W') {
					string posW = desk[i][j].first;
					string figW = desk[i][j].second;
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							if (validMove(figW, posW, desk[i][j].first)) {
								movable = true;
							}
						}
					}
				}
			}
		}
	}
	// // 1. Check black move
	else if (moveState == "black") {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (desk[i][j].second[0] == 'B') {
					string posB = desk[i][j].first;
					string figB = desk[i][j].second;
					for (int i = 0; i < 8; i++) {
						for (int j = 0; j < 8; j++) {
							if (validMove(figB, posB, desk[i][j].first)) {
								movable = true;
							}
						}
					}
				}
			}
		}
	}
	if (!movable) { cout << moveState << " - YOU CAN NOT MOVE! SLATE-MATE!" << endl; }
	return movable;
}

// Check mate
bool isMate() {
	int act = 0;
	bool mate = true;
	// Check mate white
	if (moveState == "white") {
		// Go through all white figures
		for (int k = 0; k < 8; k++) {
			for (int l = 0; l < 8; l++) {
				if (desk[k][l].second[0] == 'W') {
					string tryWhiteFigure = desk[k][l].second;
					string tryWhitePos = desk[k][l].first;
					// Place checking in any valid position
					for (int m = 0; m < 8; m++) {
						for (int n = 0; n < 8; n++) {
							string tryPos = desk[m][n].first;
							if (validMove(tryWhiteFigure, tryWhitePos, tryPos)) {
								makeMove(tryWhiteFigure, tryWhitePos, tryPos);
								if (!isCheck()) {
									++act;
								}
								cancelMove(tryWhiteFigure, tryWhitePos, tryPos);
							}
						}
					}
				}
			}
		}
	}
	// Check mate black
	if (moveState == "black") {
		// Go through all black figures
		for (int k = 0; k < 8; k++) {
			for (int l = 0; l < 8; l++) {
				if (desk[k][l].second[0] == 'B') {
					string tryBlackFigure = desk[k][l].second;
					string tryBlackPos = desk[k][l].first;
					// Place checking in any valid position
					for (int m = 0; m < 8; m++) {
						for (int n = 0; n < 8; n++) {
							string tryPos = desk[m][n].first;
							if (validMove(tryBlackFigure, tryBlackPos, tryPos)) {
								makeMove(tryBlackFigure, tryBlackPos, tryPos);
								if (!isCheck()) {
									++act;
								}
								cancelMove(tryBlackFigure, tryBlackPos, tryPos);
							}
						}
					}
				}
			}
		}
	}
	if (act > 0) mate = false;
	return mate;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////   MAIN FUNCTION   //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	// Fill entry desk
	fillDesk();

	while (true) {

		if (isCheck() && isMate()) {
			makeMove(figure, fromTo, whereTo);
			showDesk();
			cout << "MATE! ";
			(moveState == "white") ? (cout << "BLACK WIN!" << endl) : (cout << "WHITE WIN!" << endl);
			break;
		}
		else if (!isCheck() && !isMovable()) {
			cout << "SLATE-MATE! ";
			(moveState == "white") ? (cout << "BLACK WIN!" << endl) : (cout << "BLACK WIN!" << endl);
			break;
		}
		else if ((isCheck() && !isMate()) || (!isCheck() && isMovable())) {
			do {
				// Report about "check"
				if (isCheck()) {
					(moveState == "white") ? (cout << "WHITE, you are checked!" << endl) : (cout << "BLACK, you are checked!" << endl);
				}
				// Move
				showMenu();
				do { enterMove(); } while (!validMove(figure, fromTo, whereTo)); // Enter while move is not valid
				makeMove(figure, fromTo, whereTo); // 
				// Cancel move if it's "check"
				if (isCheck()) {
					// Report about "check"
					(moveState == "white") ? (cout << "WHITE, you are checked!" << endl) : (cout << "BLACK, you are checked!" << endl);
					cancelMove(figure, fromTo, whereTo);
					cout << "MOVE WAS CANCELED!" << endl;
					// Reenter move
					showMenu();
					do { enterMove(); } while (!validMove(figure, fromTo, whereTo));
					makeMove(figure, fromTo, whereTo);
				}
			} while (isCheck());
		}

		// Switch move
		(moveState == "white") ? (moveState = "black") : (moveState = "white");
	}

	cout << "THANK YOU FOR GAME!\nENTER 'Y' IF YOU ENJOYED!\n";
	string quit;
	cin >> quit;
}