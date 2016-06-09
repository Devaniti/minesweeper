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
	TField visiblefield;//-2 - not opened; -3 - flag
	int h, w, mines;
	int minesleft, squaresleft, flagsleft;
	bool generated;
	bool gameover,win;
	void generate(int x, int y);
	bool Open(int x, int y);
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