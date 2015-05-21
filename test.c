#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char *argv[])
{
	double width, height, angle, radian;
	double w, h, w1, w2; /* output width and height */

	if (argc != 4) {
		printf ("Usage:/%s <width> <height> <angle> of the original image, and the angle want to rotate, then I'll reply with the reduced size of image width and height.\n", argv[0]);
		exit (1);
	}

	width = atof (argv[1]);
	height = atof (argv[2]);
	angle = atof (argv[3]);
	radian = angle * M_PI / 180;

	w1 = width / ((height / width) * sin (radian) + cos (radian));
	//printf ("%f.\n", ((height / width) * sin (radian) + cos (radian)));
	w2 = height / (sin (radian) + (height / width) * cos (radian));
	//printf ("%f.\n", (sin (radian) + (height / width) * cos (radian)));
	w = w1 < w2 ? w1 : w2;
	h = w * height / width;

	printf ("Rotated: w %.0f h %.0f \nOriginal: width %.0f height %.0f radian %f.\n", w, h, width, height, radian);
	return 0;


}
