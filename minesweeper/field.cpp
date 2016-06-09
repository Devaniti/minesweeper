#include "field.h"

#include <time.h>
#include <cstdlib>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#define max(a,b) a>b?a:b


TGameField::TField::TField(int w, int h)
{
	field = new int[w*h];
	this->w = w;
	this->h = h;
}
TGameField::TField::TField() {}
int *TGameField::TField::operator[](int w)
{
	return &field[this->h*w];
}

void TGameField::generate(int x, int y)
{
	srand(time(NULL));
	int x1;
	int y1;
	gameover = false;
	field = TField(w, h);
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			field[i][j] = 0;
	field[x][y] = -1;
	minesleft = 0;
	while (minesleft < mines)
	{
		x1 = rand();
		x1 %= w;
		y1 = rand();
		y1 %= h;
		if (field[x1][y1] != -1)
		{
			field[x1][y1] = -1;
			minesleft++;
		}
	}
	field[x][y] = 0;
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
		{
			if (field[i][j] == -1)
			{
				for (int i1 = max(i - 1, 0); (i1 <= i + 1) && (i1 < w); i1++)
					for (int j1 = max(j - 1, 0); (j1 <= j + 1) && (j1 < h); j1++)
						if (field[i1][j1] != -1) field[i1][j1]++;
			}
		}
	generated = true;
}

bool TGameField::Open(int x, int y)
{
	if (visiblefield[x][y] != -2) return true;
	if (field[x][y] == -1) { visiblefield[x][y] = -1; return false; }
	{
		visiblefield[x][y] = field[x][y]; 
		squaresleft--;
		if (squaresleft - mines == 0) win = true;
	}
	if (visiblefield[x][y] == 0)
		for (int i1 = max(x - 1, 0); (i1 <= x + 1) && (i1 < w); i1++)
			for (int j1 = max(y - 1, 0); (j1 <= y + 1) && (j1 < h); j1++)
				Open(i1, j1);
	return true;
}

void TGameField::ReDraw(System::Windows::Forms::PictureBox^ image)
{
	image->Image = gcnew Bitmap(w * 25, h * 25 + 25);
	Font^ font = gcnew System::Drawing::Font(FontFamily::GenericSansSerif, 12.0F);
	SolidBrush^ brush = gcnew SolidBrush(Color::Black);
	PointF start;
	Graphics^ graphics = Graphics::FromImage(image->Image);
	Pen^ pen = gcnew Pen(Color::Gray);
	for (int i = 0; i <= w; i++)
		graphics->DrawLine(pen, 25 * i, 0, 25 * i, 25 * h);
	for (int j = 0; j <= h; j++)
		graphics->DrawLine(pen, 0, 25*j, 25 * w, 25 * j);
	start.X = 20;
	start.Y = h * 25;
	if (gameover)
		brush->Color = Color::Red;
	if (win)
		brush->Color = Color::Green;
	graphics->DrawString(gameover?Convert::ToString("Gameover"):(win?Convert::ToString("You win!"):Convert::ToString(flagsleft)), font, brush, start);
	brush->Color = Color::Black;
	start.X = 100;
	graphics->DrawString(Convert::ToString(squaresleft-mines), font, brush, start);
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
		{
			start.X = i * 25+3;
			start.Y = j * 25+3;
			switch(visiblefield[i][j])
			{
			case(-3):
				brush->Color = Color::Red;
				break;
			case(-2):
				brush->Color = Color::Gray;
				break;
			case(-1):
				brush->Color = Color::DarkRed;
				break;
			case(1):
				brush->Color = Color::Blue;
				break;
			case(2):
				brush->Color = Color::Green;
				break;
			default:
				brush->Color = Color::Red;
				break;
			}
			if (visiblefield[i][j] != 0)
			{
				if (visiblefield[i][j]==-2)
				graphics->DrawString(Convert::ToString("H"), font, brush, start);else
					if (visiblefield[i][j]==-3)
				graphics->DrawString(Convert::ToString("F"), font, brush, start);else
				graphics->DrawString(Convert::ToString(visiblefield[i][j]), font, brush, start);
			}
		}
}

TGameField::TGameField(int w, int h, int mines)
{
	this->h = h;
	this->w = w;
	this->mines = mines;
	minesleft = mines;
	squaresleft = w*h;
	flagsleft = mines;
	generated = false;
	gameover = false;
	win = false;
	visiblefield = TField(w, h);
	for (int i = 0; i < w; i++)
		for (int j = 0; j < h; j++)
			visiblefield[i][j] = -2;
}

TGameField::TGameField() {};

bool TGameField::Click(int x, int y, System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Button^ button)
{
	if (!gameover&&!win)
	{
		if (!generated)
			generate(x, y);
		if (!Open(x, y))
		{
			gameover = true;
			button->Top = 25 * h + 1;
			button->Visible = true;
			ReDraw(image);
			return false;
		}
		ReDraw(image);
		return true;
	}
	ReDraw(image);
	return false;
}

bool TGameField::SmartClick(int x, int y, System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Button^ button)
{
	if (!gameover&&!win)
	{
		if (visiblefield[x][y] > 0)
		{
			int sum = 0;
			for (int i1 = max(x - 1, 0); (i1 <= x + 1) && (i1 < w); i1++)
				for (int j1 = max(y - 1, 0); (j1 <= y + 1) && (j1 < h); j1++)
					if (visiblefield[i1][j1] == -3) sum++;
			if (sum == visiblefield[x][y])
				for (int i1 = max(x - 1, 0); (i1 <= x + 1) && (i1 < w); i1++)
					for (int j1 = max(y - 1, 0); (j1 <= y + 1) && (j1 < h); j1++)
						if (!Open(i1, j1))
						{
							gameover = true;
							button->Top = 25 * h + 1;
							button->Visible = true;
						}
		}
	}
	ReDraw(image);
	return false;
}

bool TGameField::RClick(int x, int y, System::Windows::Forms::PictureBox^ image)
{
	if (!gameover&&!win)
	{
		if (visiblefield[x][y] == -2)
		{
			visiblefield[x][y] = -3;
			flagsleft--;
		}else
		if (visiblefield[x][y] == -3)
		{
			visiblefield[x][y] = -2;
			flagsleft++;
		}
	}
	ReDraw(image);
	return false;
}

void TGameField::InitGraphics(System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Form^ form)
{
	image->Height = h * 25 + 25;
	image->Width = w * 25;
	form->ClientSize = image->Size;
	ReDraw(image);
}

bool TGameField::GameOver()
{
	return gameover;
}
