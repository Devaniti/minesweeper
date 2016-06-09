#pragma once
#include <vcclr.h>
class TGameField
{
	class TField
	{
		int* field;
		int h, w;
	public:
		TField(int h, int w);
		TField();
		int *operator[](int h);
	};
	TField field;
	gcroot < System::Drawing::Image^ > tm3,tm2,tm1,t0,t1,t2,t3,t4,t5,t6,t7,t8;
	TField visiblefield;//-2 - not opened; -3 - flag
	int h, w, mines;
	int minesleft, squaresleft, flagsleft;
	bool generated;
	bool gameover,win;
	void generate(int x, int y);
	bool Open(int x, int y);
	void ImageDraw(System::Windows::Forms::PictureBox^ image, int x, int y, int cell);
	void ReDraw(System::Windows::Forms::PictureBox^ image);
public:
	TGameField(int h, int w, int mines);
	TGameField();
	bool Click(int x, int y, System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Button^ button);
	bool SmartClick(int x, int y, System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Button^ button);
	bool RClick(int x, int y, System::Windows::Forms::PictureBox^ image);
	void InitGraphics(System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Form^ form);
	bool GameOver();
};