//
//  drw_font_hershey.c
//  drw
//
//  Created by vs on 1/11/18.
//  Copyright © 2018 ruminant. All rights reserved.
//

#include "drw_font_hershey.h"

#include "../../drw.h"

#include <hershey/futural.h>
//#include <r4/src/r4_config.h>
#include <stdbool.h>

void drw_font_hershey_draw(const char* text)
{

#ifdef RPLATFORM_DARWIN
#ifdef __APPLE__
	drw_push();
	char c    = (char)0;
	bool done = false;
	int  i    = 0;
	while (!done)
	{
		c = text[i];
		//printf("%c", c);
		if (c == '\0')
		{

			done = true;
		}
		else
		{
			//int w = futural_width;

			//int h = futural_height;
			//done = true;
			int idx = (int)(c);
			idx -= 32; //hershey fonts don't have the 32 nonprintables
			//idx -= 1; //not sure what this is about
			//const char* local     = &futural;
			int	 corrected = -1 + idx;
			int	 sz	= futural_size[corrected];
			const char* data      = futural[corrected];
			const char  w	 = futural_width[corrected];
			//printf("width is %d\n", w);

			for (int i2 = 0; i2 < sz; i2 += 4)
			{
				const char a  = data[i2];
				const char b  = data[i2 + 1];
				const char c  = data[i2 + 2];
				const char d  = data[i2 + 3];
				int	a2 = (int)a;
				int	b2 = (int)b;
				int	c2 = (int)c;
				int	d2 = (int)d;

				//printf("%c %c", a, b);
				int arr[4];
				arr[0] = a2;
				arr[1] = b2;
				arr[2] = c2;
				arr[3] = d2;

				glVertexPointer(2, GL_INT, 0, &arr);
				glDrawArrays(GL_LINES, 0, 4);
			}

			//todo: bring this back
			// glColor4f(1,0,1,1);
			//glTranslated(16,0,0);
			//glVertexPointer(2, GL_INT, 0, &data);
			//glDrawArrays(GL_LINES, 0, sz);

			//printf("\n");
			i++;
			drw_translate(w, 0, 0);
		}
	}
	//for ( int i = 0 ;)
	////float arr[8];
	drw_pop();

#endif
#endif

	//glVertexPointer(2, GL_FLOAT, 0, &arr);
	//glDrawArrays(GL_LINES, 0, 4);
	//: glDrawArrays(GL_LINE_LOOP, 0, 4);
}
