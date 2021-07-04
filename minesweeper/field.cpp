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
    field = new int[w * h];
    this->w = w;
    this->h = h;
}
TGameField::TField::TField() {}
int* TGameField::TField::operator[](int w)
{
    return &field[this->h * w];
}

void TGameField::generate(int x, int y)
{
    srand(static_cast<unsigned int>(time(NULL)));
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

void TGameField::ImageDraw(System::Windows::Forms::PictureBox^ image, int x, int y, int cell)
{
    Graphics^ graphics = Graphics::FromImage(image->Image);
    Image^ c;
    switch (cell)
    {
    case(-3):
        c = tm3;
        break;
    case(-2):
        c = tm2;
        break;
    case(-1):
        c = tm1;
        break;
    case(0):
        c = t0;
        break;
    case(1):
        c = t1;
        break;
    case(2):
        c = t2;
        break;
    case(3):
        c = t3;
        break;
    case(4):
        c = t4;
        break;
    case(5):
        c = t5;
        break;
    case(6):
        c = t6;
        break;
    case(7):
        c = t7;
        break;
    case(8):
        c = t8;
        break;
    }
    graphics->DrawImage(c, x, y, 25, 25);
}

void TGameField::ReDraw(System::Windows::Forms::PictureBox^ image)
{
    image->Image = gcnew Bitmap(w * 25, h * 25 + 25);
    Font^ font = gcnew System::Drawing::Font(FontFamily::GenericSansSerif, 12.0F);
    SolidBrush^ brush = gcnew SolidBrush(Color::Black);
    PointF start;
    Graphics^ graphics = Graphics::FromImage(image->Image);
    start.X = 20;
    start.Y = static_cast<float>(h * 25);
    if (gameover)
        brush->Color = Color::Red;
    if (win)
        brush->Color = Color::Green;
    graphics->DrawString(gameover ? Convert::ToString("Gameover") : (win ? Convert::ToString("You win!") : Convert::ToString(flagsleft)), font, brush, start);
    brush->Color = Color::Black;
    start.X = 100;
    graphics->DrawString(Convert::ToString(squaresleft - mines), font, brush, start);
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
        {
            ImageDraw(image, 25 * i, 25 * j, visiblefield[i][j]);
        }
}

TGameField::TGameField(int w, int h, int mines)
{
    this->h = h;
    this->w = w;
    this->mines = mines;
    minesleft = mines;
    squaresleft = w * h;
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
    if (!gameover && !win)
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
    if (!gameover && !win)
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
    if (!gameover && !win)
    {
        if (visiblefield[x][y] == -2)
        {
            visiblefield[x][y] = -3;
            flagsleft--;
        }
        else
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
    tm3 = Image::FromFile("-3.bmp");
    tm2 = Image::FromFile("-2.bmp");
    tm1 = Image::FromFile("-1.bmp");
    t0 = Image::FromFile("0.bmp");
    t1 = Image::FromFile("1.bmp");
    t2 = Image::FromFile("2.bmp");
    t3 = Image::FromFile("3.bmp");
    t4 = Image::FromFile("4.bmp");
    t5 = Image::FromFile("5.bmp");
    t6 = Image::FromFile("6.bmp");
    t7 = Image::FromFile("7.bmp");
    t8 = Image::FromFile("8.bmp");
    image->Height = h * 25 + 25;
    image->Width = w * 25;
    form->ClientSize = image->Size;
    ReDraw(image);
}

bool TGameField::GameOver()
{
    return gameover;
}
