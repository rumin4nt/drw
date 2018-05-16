//
//  r_color.h
//  r4
//
//  Created by Andrew Macfarlane on 1/18/17.
//  Copyright © 2017 vaporstack. All rights reserved.
//

#ifndef r_color_h
#define r_color_h

typedef struct
{
	int r;
	int g;
	int b;
	int a;
} RColor;

typedef struct
{
	float r;
	float g;
	float b;
	float a;
} RColor8;

typedef struct
{
	double r;
	double g;
	double b;
	double a;
} RColor16;

const char* r_color_to_hex(RColor c);
const char* r_color8_to_hex(RColor8 c);
const char* r_color16_to_hex(RColor16 c);

RColor   r_color_hex_to_color(char*);
RColor8  			_hex_to_color8(char*);
RColor16 r_color_hex_to_color16(char*);

RColor8  r_color_rc_to_rc8(RColor);
RColor16 r_color_rc_to_rc16(RColor);
RColor   r_color16_to_rc(RColor16);

#endif /* r_color_h */
