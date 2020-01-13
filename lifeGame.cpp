#include <iostream>
#include <ctime>
#include <unistd.h>
using namespace std;

const int DEAD = 0;
const int ALIVE = 1;

#define max_row 40
#define max_col 40

//cell
class cell {
public:
	cell() {
		status = next_status = DEAD;
	}

	~cell() {}

	int getStat() {
		return status;
	}

	void setStat(int stat) {
		status = stat;
	}

	int getNext() {
		return next_status;
	}

	void setNext(int stat) {
		next_status = stat;
	}

	void copy() {
		status = next_status;
	}

private:
	int status;
	int next_status;
};

//Life
class Life {
public:
	Life() {}

	Life(int c[][max_col]) {
		for (int i = 0; i < max_row; ++i) {
			for (int j = 0; j < max_col; ++j) {
				m[i][j].setStat(c[i][j]);
			}
		}
	}

	void run() {
		while (1) {
			printM();
			for (int i = 0; i < max_row; ++i) {
				for (int j = 0; j < max_col; ++j) {
					switch (numsOfNei(i, j)) {
					case 0:
					case 1:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
						m[i][j].setNext(DEAD);
						break;
					case 3:
						m[i][j].setNext(ALIVE);
						break;
					case 2:
						m[i][j].setNext(m[i][j].getStat());
						break;
					}
				}
			}
			/*
			cout << "Press 'N' to continue." << endl;
			char c;
			cin >> c;
			if (toupper(c) != 'N') break;
			*/
			usleep(100000);     //set the interval to 0.1s
			++round;
			printM();
			copyToNext();
		}
	}

private:
    //calculate the number of neighbors
	int numsOfNei(int x, int y) {
		int count = 0;
		for (int i = x - 1; i <= x + 1; ++i) {
			for (int j = y - 1; j <= y + 1; ++j) {
				if (i < 0 || j < 0 || i >= max_row || j >= max_col)
					continue;
				if ((i != x || j != y) && m[i][j].getStat() == ALIVE)
					++count;
			}
		}
		return count;
	}
    //print the matrix in the screen
	void printM() {
		system("clear");
		cout << "Round " << round << ":" << endl;
		for (int i = 0; i < max_row; ++i) {
			for (int j = 0; j < max_col; ++j) {
			    if ((i == 0 || j == 0 || i == max_row-1 || j == max_col-1) 
			        && (m[i][j].getStat() == DEAD))
			        cout << "- ";
				else if (m[i][j].getStat() == DEAD)
					cout << "  ";
				else cout << "# ";
			}
			cout << endl;
		}
		cout << endl;
	}
	
    //set new status
	void copyToNext() {
		for (int i = 0; i < max_row; ++i) {
			for (int j = 0; j < max_col; ++j) {
				m[i][j].setStat(m[i][j].getNext());
			}
		}
	}

	cell m[max_row][max_col];
	int round = 0;
};

//generate initial status randomly
int (*randomGe())[max_col] {
	int (*cell)[max_col] = new int[max_row][max_col];
	srand((unsigned)time(NULL));
	for (int i = 0; i < max_row; ++i) {
		for (int j = 0; j < max_col; ++j) {
			cell[i][j] = rand() % 2;
		}
	}
	return cell;
}


//the Glider

//the Glider Gun
int(*gliderGun())[max_col] {
	int (*c)[max_col] = new int[max_row][max_col];
	c[6][1] = c[6][2] = c[7][1] = c[7][2] = 1;
	c[5][35] = c[5][36] = c[4][35] = c[4][36] = 1;
	c[6][11] = c[7][11] = c[8][11] = c[9][12] = c[10][13] = c[10][14] = 1;
	c[5][12] = c[4][13] = c[4][14] = 1;
	c[7][15] = c[5][16] = c[9][16] = c[6][17] = c[7][17] = c[8][17] = c[7][18] = 1;
	c[4][21] = c[4][22] = c[5][21] = c[5][22] = c[6][21] = c[6][22] = 1;
	c[3][23] = c[7][23] = c[3][25] = c[2][25] = c[7][25] = c[8][25] = 1;
	return c;
}

//input manually
int (*manualInput())[max_col] {
    cout << "Input the coordinates of alive cells. End with -1 -1." << endl
		<< "0 <= x <= " << max_row - 1 << " and 0 <= y <= " << max_col - 1 << endl;
	int x, y;
	int (*cell)[max_col] = new int[max_row][max_col];
	while (1) {
		cin >> x >> y;
		if (x == -1 && y == -1) break;
		if (x < 0 || x >= max_row || y < 0 || y >= max_col) {
			cout << "Input Error. Continue." << endl;
			continue;
		}
		cell[x][y] = ALIVE;
		cout << "The next one." << endl;
	}
	return cell;
}

//main
int main() {
	int choose = 0;
	int (*initStat)[max_col] = new int[max_row][max_col];
	while(1) {
        cout << "Choose mode:" << endl
            << "1. Randomly generate." << endl
            << "2. Input by myself." << endl
            << "3. The Glider Gun." << endl
            << "0. Quit." << endl;
        cin >> choose;
        switch(choose) {
        case 1: 
            initStat = randomGe();
            break;
        case 2:
            initStat = manualInput();
            break;
        case 3:
            initStat = gliderGun();
            break;
        case 0:
            cout << "Goodbye!" << endl;
            return 0;
        }
        cout << "Enjoy!" << endl;
        sleep(2);
	    Life life(initStat);
	    life.run();
	}
	return 0;
}
