#include "Screen.h"

int Screen::height = 0;
int Screen::width = 0;

void Screen::InitializeScreen(const int _width, const int _height)
{
	width = _width;
	height = _height;
}