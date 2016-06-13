#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdlib.h>

#include "vector2.h"

typedef struct {
	vector2_t origin;
	vector2_t size;
} rect_t;

typedef struct {
	size_t width;
	size_t height;
	unsigned char *image;
} image_t;

#endif
