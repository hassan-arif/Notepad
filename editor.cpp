#include <fstream>
#include <iostream>
#include <stdlib.h>

# include "myconsole.h"
# include "editor.h"
# include "DLinkedList.h"

using namespace std;


//-------------------------------Color Codes------------------------------//
// Color Codes Hexadecimal values for both Fore Ground and Back Ground //ForeGround|BackGround
int B_BLACK = 0x00;	int B_Royal_Blue = 0x10;	int B_Dark_Green = 0x20;
int B_Tale_Blue = 0x30;	int B_Dark_Red = 0x40;	int B_Purple = 0x50;
int B_Tale_Green = 0x60;	int B_Light_Grey = 0x70; int B_Dark_Gray = 0x80;
int B_Light_Blue = 0x90; int B_Light_Green = 0xA0;	int B_Sky_Blue = 0xB0;
int B_Red = 0xC0;	int B_Pink = 0xD0;	int B_Yellow = 0xE0;
int B_White = 0xF0;

int F_BLACK = 0x00;	int F_Royal_Blue = 0x01; int F_Dark_Green = 0x02;
int F_Tale_Blue = 0x03;	int F_Dark_Red = 0x04;	int F_Purple = 0x05;
int F_Tale_Green = 0x06;	int F_Light_Grey = 0x07; int F_Dark_Gray = 0x08;
int F_Light_Blue = 0x09; int F_Light_Green = 0x0A;	int F_Sky_Blue = 0x0B;
int F_Red = 0x0C;	int F_Pink = 0x0D;	int F_Yellow = 0x0E;
int F_White = 0x0F;

//-------------------------------POINT CLASS------------------------------//
//initialize MaxPoint
point point::MaxPoint(MAX_X, MAX_Y);
bool point::incrementX()
{
	bool returnValue = true;
	if (x < MaxPoint.x)
		x++;
	else if (y < MaxPoint.y)
	{
		x = 0;
		y = y + 1;
	}
	else
		returnValue = false;

	return returnValue;
}

bool point::incrementY()
{
	bool returnValue = true;
	if (y < MaxPoint.y)
		y++;
	else
		returnValue = false;

	return returnValue;

}
bool point::decrementX()
{
	bool returnValue = true;
	if (x > 0)
		x--;
	else if (y > 0)
	{
		y--;
		x = MaxPoint.x;
	}
	else
		returnValue = false;

	return returnValue;
}
bool point::decrementY()
{
	bool returnValue = true;
	if (y > 0)
		y--;
	else
		returnValue = false;

	return returnValue;

}

//-------------------------------END OF POINT CLASS------------------------------------------------//


//--------------------------The function Where you will write code--------------------------------//

int mainEditor()
{
	ofstream fout;

	int key = 0;
	bool stop = false;
	point currentPosition(0, 0);
	currentPosition.setMaxPoint(MAX_X, MAX_Y);
	bool specialKey = false;


	char temporary = 0;
	int maxX[51]{}; //will help to identify max entries in each row of texteditor.

	DLinkedList<char> text;

	//generic literator
	DLinkedList<char>::literator current = text.begin();	//stores linked list's node in correspondence with the current coordinates

	DLinkedList<char>::literator last = current;	//maintains tail
	DLinkedList<char>::literator backup;	//helps in deletion of last element (using backspace)
	bool refresh = false;
	DLinkedList<char>::literator* array;	//for f2 function

	//temporary literators
	DLinkedList<char>::literator l1 = text.begin();	//helps if cursor reached to start of list. prevents further deletion.
	DLinkedList<char>::literator l2 = text.end();	//maintains null. 
	int count = 0, size1 = 0, size2 = 0, tempx = 0, tempy = 0;	//for function keys

	while (!stop) {
		key = CheckWhichKeyPressed(specialKey);
		if (key == ESCKEY) {
			stop = true;
			ClearScreen();
			PlaceCursor(0, 0);
		}

		else if (!specialKey && key >= 32 && key <= 126)
		{
			if (currentPosition.getX() < MAX_X) {
				if (maxX[currentPosition.getY()] < 100) {
					if (current != last) refresh = true;

					text.Insert(current, (char)key);
					current++;
					maxX[currentPosition.getY()]++;

					cout << (char)key;
					SetColorAtPoint(currentPosition.getX(), currentPosition.getY(), F_BLACK | B_White);   //ForeGround|BackGround
					currentPosition.incrementX();

					if (!refresh) last = current;
				}
				else {
					int i = currentPosition.getY();
					i++;
					while (maxX[i] >= 100 && i < 51) i++;
					if (i < 51) {
						maxX[i]++;

						if (current != last) refresh = true;

						text.Insert(current, (char)key);
						current++;

						cout << (char)key;
						SetColorAtPoint(currentPosition.getX(), currentPosition.getY(), F_BLACK | B_White);   //ForeGround|BackGround
						currentPosition.incrementX();

						if (!refresh) last = current;
					}
				}

				if (currentPosition.getX() == MAX_X) {
					if (currentPosition.getY() != MAX_Y) {
						currentPosition.incrementY();
						currentPosition.setX(0);
					}
				}
			}
		}
		else if (specialKey && key == LEFTKEY) {
			currentPosition.decrementX();

			if (currentPosition.getX() == MAX_X) {
				currentPosition.setX(maxX[currentPosition.getY()] - 1);
			}

			if (current != l1)
				current--;
		}
		else if (specialKey && key == RIGHTKEY) {
			backup = current;
			current++;
			if (current == l2) current = backup;	//if iterator is already pointing at end
			else {									//else case
				currentPosition.incrementX();

				if (currentPosition.getX() == MAX_X) {
					if (currentPosition.getY() < 50) {
						currentPosition.incrementX();
					}
				}
				else if (currentPosition.getX() == maxX[currentPosition.getY()]) {
					if (currentPosition.getY() < 50 && *current == 13) {
						currentPosition.incrementY();
						currentPosition.setX(0);
					}
					else {
						currentPosition.setX(maxX[currentPosition.getY()]);
					}
				}
			}
		}
		else if (specialKey && key == UPKEY) {
			if (currentPosition.getY() > 0) {
				tempx = currentPosition.getX();
				currentPosition.decrementY();

				if (tempx < maxX[currentPosition.getY()]) {
					count = tempx + (maxX[currentPosition.getY()] - tempx);
					while (count > 0) {
						current--;
						count--;
					}
					currentPosition.setX(tempx);
				}
				else if (tempx >= maxX[currentPosition.getY()]) {
					count = tempx;
					while (count > 0) {
						current--;
						count--;
					}
					current--;
					refresh = true;
					currentPosition.setX(maxX[currentPosition.getY()] - 1);
				}
			}
		}
		else if (specialKey && key == DOWNKEY) {
			if (current != last && currentPosition.getY() < 50) {
				tempx = currentPosition.getX();

				currentPosition.incrementY();

				if (tempx < maxX[currentPosition.getY()]) {
					count = tempx + (maxX[currentPosition.getY() - 1] - tempx - 1);

					while (count >= 0) {
						current++;
						count--;
					}
					refresh = true;
					currentPosition.setX(tempx);
				}
				else if (tempx >= maxX[currentPosition.getY()]) {
					count = (maxX[currentPosition.getY() - 1] - tempx - 1) + maxX[currentPosition.getY()];
					while (count >= 0) {
						current++;
						count--;
					}
					refresh = true;
					currentPosition.setX(maxX[currentPosition.getY()]);
				}

			}
		}
		else if (key == ENTERKEY) {
			if (current != last && maxX[49] == 0) {
				refresh = true;
				int temp;

				int i = currentPosition.getY();
				int j = maxX[i] - currentPosition.getX();
				maxX[i] -= j;
				i++;

				while (i < 50) {
					temp = maxX[i];
					maxX[i] = j;
					j = temp;

					i++;
				}

				text.Insert(current, (char)key);
				current++;
				maxX[currentPosition.getY()]++;
				cout << ('\n');
				currentPosition.setX(maxX[i]);
				currentPosition.incrementY();
			}
			else if (currentPosition.getY() < 50) {
				text.Insert(current, (char)key);
				current++;
				maxX[currentPosition.getY()]++;
				cout << ('\n');
				currentPosition.setX(0);
				currentPosition.incrementY();
				last = current;
			}
		}
		else if (key == DELKEY) {
			if (current != last) {
				backup = current;
				backup++;
				if (backup == last) last = current;
				refresh = true;
				text.Delete(backup, temporary);
			}
		}
		else if (key == BACKSPACE)
		{
			cout << '\0';

			if (!currentPosition.decrementX() && currentPosition.getY() > 0) {
				cout << '\0';
				currentPosition.decrementY();
				currentPosition.setX(MAX_X);
			}
			if (currentPosition.getX() == MAX_X) {
				currentPosition.setX(maxX[currentPosition.getY()] - 1);
			}
			maxX[currentPosition.getY()]--;

			if (current != last) refresh = true;

			backup = current;
			current--;
			if (backup == last) last = current;

			text.Delete(backup, temporary);
			if (current == l2) {
				if (current == last) {
					last = backup;
				}
				current = backup;

			}

			SetColorAtPoint(currentPosition.getX(), currentPosition.getY(), F_BLACK | B_BLACK);

		}
		else if (key == F1KEY || key == F2KEY || key == F3KEY) {
			refresh = true;
			char word1[20];
			char word2[20];

			PlaceCursor(0, MAX_Y + 1);
			cout << "Enter Search Query: ";
			currentPosition.setX(0 + strlen("Enter Search Text : "));

			cin.getline(word1, 20, '\n');
			size1 = strlen(word1);


			if (key == F1KEY) {
				current = text.Search_First(word1, size1);
				if (current == l2) {
					current = text.begin();
					currentPosition.setX(0);
					currentPosition.setY(0);
					refresh = true;
				}
				else {
					refresh = true;
					text.setCoordinates(current, tempx, tempy);
					currentPosition.setX(tempx);
					currentPosition.setY(tempy);
				}

			}
			else if (key == F2KEY) {
				array = text.Search_All(word1, size1, count);
				ClearScreen();
				if (count > 0) {
					PlaceCursor(0, 0);
					text.reprintHighlight(array, count, size1);
					stop = true;
					while (stop) {
						key = CheckWhichKeyPressed(specialKey);
						if (key == ESCKEY)
							stop = false;
					}
				}
				current = text.begin();
				currentPosition.setX(0);
				currentPosition.setY(0);
				count = 0;
			}
			else {
				PlaceCursor(0, MAX_Y + 2);
				cout << "Enter Replaced Text: ";
				currentPosition.setX(0 + strlen("Enter Replaced Text : "));

				cin.getline(word2, 20, '\n');
				size2 = strlen(word2);

				text.Replace_All(word1, size1, word2, size2);

				text.UpdateEntries(maxX);

				last = text.begin();
				backup = last;
				while (last != l2) {
					backup = last;
					last++;
				}
				last = backup;
				current = text.begin();
				currentPosition.setX(0);
				currentPosition.setY(0);
			}

		}
		else if (key == F4KEY) {
			count = 0;
			fout.open("myeditor.txt");
			l1 = text.begin();
			l1++;
			l2 = text.end();
			while (l1 != l2) {
				if (count == 100) {
					fout << endl;
					count = 0;
				}
				if (*l1 == 13) {
					count = 0;
					fout << *l1;
					++l1;
					continue;
				}
				fout << *l1;
				++l1;
				count++;
			}
			fout.close();
			l1 = text.begin();
		}

		if (refresh) {
			PlaceCursor(0, 0);

			ClearScreen();
			text.reprint();
			refresh = false;
		}

		PlaceCursor(currentPosition.getX(), currentPosition.getY());
	}

	return 0;
}

//-------------------------------------------End Main Editor--------------------------//
int main()
{
	mainEditor();
	//cout << __cplusplus << endl;
	return 0;
}