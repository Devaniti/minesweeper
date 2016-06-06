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


		TGameField::TField::TField(int h, int w)
		{
			field = new int[h*w];
			this->h = h;
			this->w = w;
		}
		TGameField::TField::TField() {}
		int *TGameField::TField::operator[](int h)
		{
			return &field[this->h*h];
		}
	void TGameField::generate(int x, int y)
	{
		srand(time(NULL));
		gameover = false;
		field = TField(h, w);
		visiblefield = TField(h, w);
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				visiblefield[i][j] = -2;
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				field[i][j] = 0;
		field[x][y] = -1;
		minesleft = 0;
		while (minesleft < mines)
		{
			int x1 = rand() % h, y1 = rand() % w;
			if (field[x1][y1] != -1)
			{
				field[x1][y1] = -1;
				minesleft++;
			}
		}
		field[x][y] = 0;
		for (int i = 0; i < h; i++)
			for (int j = 0; j < h; j++)
			{
				if (field[i][j] == -1)
				{
					for (int i1 = max(i - 1, 0); (i1 <= i + 1) && (i1 < h); i1++)
						for (int j1 = max(j - 1, 0); (j1 <= j + 1) && (j1 < w); j1++)
							if (field[i1][j1] != -1) field[i1][j1]++;
				}
			}
		generated = true;
	}
	bool TGameField::Open(int x, int y)
	{
		if (visiblefield[x][y] != -2) return true;
		if (field[x][y] == -1) { visiblefield[x][y] = -1; return false; }
		visiblefield[x][y] = field[x][y];
		if (visiblefield[x][y] == 0)
			for (int i1 = max(x - 1, 0); (i1 <= x + 1) && (i1 < h); i1++)
				for (int j1 = max(y - 1, 0); (j1 <= y + 1) && (j1 < w); j1++)
					Open(i1, j1);
		return true;
	}
	void TGameField::ReDraw(System::Windows::Forms::PictureBox^ image)
	{
		image->Image = gcnew Bitmap(w * 25, h * 25);
		Font^ font = gcnew System::Drawing::Font(FontFamily::GenericSansSerif, 12.0F);
		SolidBrush^ brush = gcnew SolidBrush(Color::Gray);
		PointF start;
		Graphics^ graphics = Graphics::FromImage(image->Image);

		if (generated)
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
				{
					start.X = j * 25;
					start.Y = i * 25;
					switch(visiblefield[i][j])
					{
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
						graphics->DrawString(visiblefield[i][j] == -2 ? Convert::ToString("H") : Convert::ToString(visiblefield[i][j]), font, brush, start);
				}
		else
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++)
				{
					start.X = j * 25;
					start.Y = i * 25;
					graphics->DrawString(Convert::ToString("H"), font, brush, start);
				}
	}
	TGameField::TGameField(int h, int w, int mines)
	{
		this->h = h;
		this->w = w;
		this->mines = mines;
		generated = false;
		gameover = false;
	}
	TGameField::TGameField() {};
	bool TGameField::Click(int x, int y, System::Windows::Forms::PictureBox^ image)
	{
		if (!gameover)
		{
			if (!generated)
				generate(x, y);
			if (!Open(x, y))
			{
				gameover = true;
				ReDraw(image);
				return false;
			}
			ReDraw(image);
			return true;
		}
		ReDraw(image);
		return false;
	}
	void TGameField::InitGraphics(System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Form^ form)
	{
		image->Height = w * 25;
		image->Width = h * 25;
		form->ClientSize = image->Size;
		ReDraw(image);
	}
	bool TGameField::GameOver()
	{
		return gameover;
	}