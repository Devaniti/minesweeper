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

class TGameField
{
	class TField
	{
		int* field;
		int h, w;
	public:
		TField(int h, int w)
		{
			delete[] field;
			field = new int[h*w];
			this->h = h;
			this->w = w;
		}
		TField() {}
		int *operator[](int h)
		{
			return &field[this->h*h];
		}
	};
	TField field;
	TField visiblefield;
	int h, w, mines;
	int minesleft, squaresleft;
	bool generated;
	bool gameover;
	void generate(int x, int y)
	{
		srand(time(NULL));
		gameover = false;
		field = TField(h, w);
		visiblefield = TField(h, w);
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				visiblefield[i][j] = -2;
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
	}
	bool Open(int x, int y)
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
public:
	TGameField(int h, int w, int mines)
	{
		this->h = h;
		this->w = w;
		this->mines = mines;
		generated = false;
	}
	bool Click(int x, int y)
	{
		if (!gameover)
		{
			if (!generated)
				generate(x, y);
			if (!Open(x, y))
			{
				gameover = true;
				return false;
			}
			return true;
		}
		return false;
	}
	void InitGraphics(System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Form^ form)
	{
		image->Width = w * 25;
		image->Height = h * 25;
		form->ClientSize = image->Size;
	}
	void Paint(System::Windows::Forms::PictureBox^ image)
	{
		
	}
	bool GameOver()
	{
		return gameover;
	}
};