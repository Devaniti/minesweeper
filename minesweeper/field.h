#pragma once
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
	TField visiblefield;
	int h, w, mines;
	int minesleft, squaresleft;
	bool generated;
	bool gameover;
	void generate(int x, int y);
	bool Open(int x, int y);
public:
	TGameField(int h, int w, int mines);
	bool Click(int x, int y);
	void InitGraphics(System::Windows::Forms::PictureBox^ image, System::Windows::Forms::Form^ form);
	void Paint(System::Windows::Forms::PictureBox^ image);
	bool GameOver();
};