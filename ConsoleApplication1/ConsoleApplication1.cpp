// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "Display.h"
#include <iostream>
#include <string> 

typedef unsigned short ushort;

using namespace std;

ushort normalize(ushort col, ushort max);
ushort getRed(ushort col);
ushort getGreen(ushort col);
ushort getBlue(ushort col);
ushort blendColour(int x, int y, int width, int height, ushort tl, ushort tr, ushort bl, ushort br, ushort maxvalue);
int distance(int x1, int y1, int x2, int y2);

int main(int argc, char *argv[])
{
	//Check arguments first
	ushort tl, tr, bl, br;
	string auxs;
	int auxint;
	if (argc >= 2){
		auxs = argv[1];
		if (auxs.compare("display") != 0){
			cout << "\nFirst argument should be \'display\'." << endl;
			system("pause");
			return -1;
		}
	}
	if (argc == 4){
		//2 colours
		for (int i = 2; i < 4; i++){
			auxs = argv[i];
			if (auxs.compare(0, 2, "0x") == 0){
				auxint = stoi(auxs, nullptr,16);
			}
			else{
				auxint = stoi(auxs, nullptr, 10);
			}
			if (i == 2){
				tl = bl = auxint;
			}
			else{
				tr = br = auxint;
			}
		}
	}
	else if (argc == 6){
		//4 colours
		for (int i = 2; i < 6; i++){
			auxs = argv[i];
			if (auxs.compare(0, 2, "0x") == 0){
				auxint = stoi(auxs, nullptr, 16);
			}
			else{
				auxint = stoi(auxs, nullptr, 10);
			}
			switch (i){
			case 2:tl = auxint;
				break;
			case 3:tr = auxint;
				break;
			case 4:bl = auxint;
				break;
			case 5:br = auxint;
				break;

			}
		}
			
	}
	else{
		cout << "\nIncorrect number of arguments." << endl;
		system("pause");
		return -1;
	}

	//Get a console handle
	HWND console = GetConsoleWindow();
	//Get a handle to device context
	HDC dc = GetDC(console);

	//Display
	Display * display = new Display();
	int wSizeWidth;
	int wSizeHeight;

	display->get_size(wSizeWidth,wSizeHeight);

	
	//PRINT COLOURS IN CONSOLE
	COLORREF pixcolour = 0;
	ushort nRed, nGreen, nBlue;
	
	//Get corner components
	//reds
	ushort redtl = getRed(tl);
	ushort redtr = getRed(tr);
	ushort redbl = getRed(bl);
	ushort redbr = getRed(br);
	//greens
	ushort greentl = getGreen(tl);
	ushort greentr = getGreen(tr);
	ushort greenbl = getGreen(bl);
	ushort greenbr = getGreen(br);
	//blues
	ushort bluetl = getBlue(tl);
	ushort bluetr = getBlue(tr);
	ushort bluebl = getBlue(bl);
	ushort bluebr = getBlue(br);

	ushort auxcolour;

	//Draw pixels
	for (int i = 0; i < wSizeWidth; i++)
	{
		for (int j = 0; j < wSizeHeight; j++)
		{
			
			nRed = blendColour(i, j, wSizeHeight, wSizeHeight, redtl, redtr, redbl, redbr,31);
			nBlue = blendColour(i, j, wSizeHeight, wSizeHeight, bluetl, bluetr, bluebl, bluebr,31);
			nGreen = blendColour(i, j, wSizeHeight, wSizeHeight, greentl, greentr, greenbl, greenbr,63);
			auxcolour = (nRed << 11) | (nGreen << 5) | (nBlue);
			display->draw_raster(i, j, &auxcolour, 1);
			nRed = normalize(nRed, 31);
			nGreen = normalize(nGreen, 63);
			nBlue = normalize(nBlue, 31);
			pixcolour = RGB(nRed,nGreen,nBlue);
			SetPixel(dc, i, j, pixcolour);
			
		}
	}

	
	cin.get();
	delete display;
	ReleaseDC(console, dc);
	cin.get();
	return 0;
}

ushort normalize(ushort col, ushort max){
	//cout << (col * 255) / max << endl;
	return (col * 255) / max;
}

ushort getRed(ushort col){
	return (col >> 11);
}

ushort getGreen(ushort col){
	//delete red bits
	col = col & 0x07FF;
	return (col >> 5);
}
ushort getBlue(ushort col){
	//delete red and green
	col = col & 0x001F;
	return col;
}

/*
This function calculates the colour value of a pixel doing a simple interpolation based on percantege of the
distance the point is from the corners.
*/
ushort blendColour(int x, int y, int width, int height, ushort tl, ushort tr, ushort bl, ushort br,ushort maxvalue){

	int w = width - 1;
	int h = height - 1;
	double px = (1.0 * x) / w;
	double py = (1.0 * y) / h;

	//Top blend combination
	double topblend = ((tl*(1-px))+(tr*px));
	//Left blend combination
	double leftblend = ((tl*(1-py)) + (bl*(py)));
	//Right blend combination
	double rightblend = ((tr*(1-py)) + (br*py));
	//Bottom blend combination
	double bottomblend = ((bl*(1-px)) + (br*px));
	
	//horizontal blend
	double hblend = (leftblend*(1-px)) + (rightblend*px);
	//vertical blend
	double vblend = (topblend*(1 - py)) + (bottomblend*py);

	ushort result = (ushort)(hblend+vblend)/2;
	return result;
}


int distance(int x1, int y1, int x2, int y2){

	return (int) sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}