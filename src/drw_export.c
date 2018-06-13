//
//  drw_export.c
//  drw
//
//  Created by vs on 6/13/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_export.h"

#include <drw/drw.h>

#ifdef DRW_ENABLE_PNG
#include <libpng/png.h>
#endif

#include <sys/types.h>
//typedef unsigned int uint8_t;

bool drw_export_png(const char* path, int w, int h)
{
	uint8_t *pixels = calloc(w*h*3, sizeof(uint8_t));
	// copy pixels from screen
	//	glBindTexture(GL_TEXTURE_2D, screenTex);
	glBindTexture(GL_TEXTURE_2D, 0);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, w, h);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *)pixels);
	
	// invert pixels (stolen from SOILs source code)
	/*
	 for (int j = 0; j * 2 < h; ++j) {
		int x = j * w * 3;
		int y = (h - 1 - j) * w * 3;
		for (int i = w * 3; i > 0; --i) {
			uint8_t tmp = pixels[x];
			pixels[x] = pixels[y];
			pixels[y] = tmp;
			++x;
			++y;
		}
	}
	*/
	
	// save the image
	int err = drw_export_png_save(path, pixels, w, h);
	if (err)
	{
		printf("Done\n");}
	else
	{	printf("Failed\n");
}
	return err;
}


bool drw_export_png_save(const char *filename, uint8_t *pixels, int w, int h)
{
#ifndef DRW_ENABLE_PNG
	printf("No libpng! cannot save.\n");
	return false;
#else
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
		return false;
	
	png_infop info = png_create_info_struct(png);
	if (!info) {
		png_destroy_write_struct(&png, &info);
		return false;
	}
	
	FILE *fp = fopen(filename, "wb");
	if (!fp) {
		png_destroy_write_struct(&png, &info);
		return false;
	}
	
	png_init_io(png, fp);
	png_set_IHDR(png, info, w, h, 8 /* depth */, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
		     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
	if (!palette) {
		fclose(fp);
		png_destroy_write_struct(&png, &info);
		return false;
	}
	png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
	png_write_info(png, info);
	png_set_packing(png);
	
	png_bytepp rows = (png_bytepp)png_malloc(png, h * sizeof(png_bytep));
	for (int i = 0; i < h; ++i)
	{
		//	orig:
		//	rows[i] = (png_bytep)(pixels + (h - i) * w * 3);
		
		//	fixed:
		rows[i] = (png_bytep)(pixels + (h - i - 1) * w * 3);
	}
	png_write_image(png, rows);
	png_write_end(png, info);
	png_free(png, palette);
	png_destroy_write_struct(&png, &info);
	
	fclose(fp);
	free(rows);
	return true;
	
	
#endif
	
}
