// nv12torgb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdlib.h"

unsigned int yuv2rgb(int Y, int Cr, int Cb)
{
	int Ey, Epb, Epr;
	int Eg, Eb, Er;
	unsigned int result;

	Ey = (Y - 16);
	Epb = (Cb - 128);
	Epr = (Cr - 128);

	Eg = (298 * Ey - 100 * Epb - 208 * Epr) / 256;
	Eb = (298 * Ey + 516 * Epb) / 256;
	Er = (298 * Ey + 408 * Epr) / 256;

	if (Eg > 255)
		Eg = 255;
	if (Eg < 0)
		Eg = 0;

	if (Eb > 255)
		Eb = 255;
	if (Eb < 0)
		Eb = 0;

	if (Er > 255)
		Er = 255;
	if (Er < 0)
		Er = 0;

	result = (Eb << 16) | (Eg << 8) | Er;

	return result;
}

void NV12ToRGB(unsigned char* pOut, unsigned char* pIn, unsigned int Width, unsigned int Height)
{
	unsigned int PX, PY;

	int y[4];
	int u;
	int v;


	int rbg_off;

	int y_off = 0;
	int v_off = Height*Width;
	unsigned int rgb;
	rbg_off = 0;
	int ppy = 0;
	int ppx = 0;
	int off;

	for (PY = 0; PY < Height; PY += 2)
	{

		for (PX = 0; PX < Width; PX += 2)
		{

			// getting the YUV values
			y_off = PY*Width + PX;

			y[0] = pIn[y_off];
			y[1] = pIn[y_off + 1];
			y[2] = pIn[y_off + Width];
			y[3] = pIn[y_off + Width + 1];


			u = pIn[v_off++];

			v = pIn[v_off++];

			off = (PY*Width * 3) + (PX * 3);

			rgb = yuv2rgb(y[0], u, v);
			pOut[off] = (rgb & 0x000000ff);
			pOut[off + 1] = (rgb & 0x0000ff00) >> 8;
			pOut[off + 2] = (rgb & 0x00ff0000) >> 16;


			rgb = yuv2rgb(y[1], u, v);
			pOut[off + 3] = (rgb & 0x000000ff);
			pOut[off + 4] = (rgb & 0x0000ff00) >> 8;
			pOut[off + 5] = (rgb & 0x00ff0000) >> 16;

			off += (Width * 3);

			rgb = yuv2rgb(y[2], u, v);
			pOut[off] = (rgb & 0x000000ff);
			pOut[off + 1] = (rgb & 0x0000ff00) >> 8;
			pOut[off + 2] = (rgb & 0x00ff0000) >> 16;

			rgb = yuv2rgb(y[3], u, v);
			pOut[off + 3] = (rgb & 0x000000ff);
			pOut[off + 4] = (rgb & 0x0000ff00) >> 8;
			pOut[off + 5] = (rgb & 0x00ff0000) >> 16;


		}

	}

}

int main()
{
	FILE *fp;

	fopen_s(&fp,"Z:\\Cases\\Case#00166472 \\screenshot.raw", "r");
	
	if (fp)
	{
		int width = 1280, height = 720;
		unsigned char *pIn = (unsigned char *)malloc(width*height * 3 / 2);
		unsigned char *pOut = (unsigned char *)malloc(width*height*3);
		FILE *out; 
		
		fopen_s(&out, "Z:\\Cases\\Case#00166472 \\screenshot_rgb.raw", "w");

		if (pIn && pOut)
		{
			NV12ToRGB(pOut, pIn, width, height);
		}

		if (out)
		{
			fwrite(pOut, width*height, sizeof(char) * 3, out);
			fclose(out);
		}

		fclose(fp);

	}
    return 0;
}

