#include "stdafx.h"
#include <cstdio>
#include <cassert>
#include <memory>
#include "display.h"

#define W 200
#define H 200
static unsigned short frame_buffer[W*H];

Display::Display()
{
	memset(frame_buffer, 0, sizeof(frame_buffer));
}

Display::~Display()
{
	unsigned short *pix = frame_buffer;
	for (int y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			if (x > 0) {
				printf(" ");
			}
			printf("%04X", *pix++);
		}
		printf("\n");
	}
}

bool Display::connect(const char *display_name)
{
	return true;
}

void Display::get_size(int &width, int &height)
{
	width = W;
	height = H;
}

void Display::draw_raster(int x, int y,
	const unsigned short *pixels, int width)
{
	memcpy(&frame_buffer[y*W + x], pixels, width*sizeof(unsigned short));
}
