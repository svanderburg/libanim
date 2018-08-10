/*
 * Copyright (c) 2018 Sander van der Burg
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "anim.h"
#include <libiff/iff.h>
#include <libilbm/ilbm.h>
#include <libilbm/ilbmchunkheaders.h>
#include "animationheader.h"

#define ANIM_NUM_OF_FORM_TYPES ILBM_NUM_OF_FORM_TYPES
#define ANIM_NUM_OF_EXTENSION_CHUNKS (ILBM_NUM_OF_EXTENSION_CHUNKS + 1)

/* An array of all application specific chunks belonging to ILBM. They must be alphabetically sorted. */
static IFF_FormExtension ilbmFormExtension[] = {
    {"ANHD", &ANIM_readAnimationHeader, &ANIM_writeAnimationHeader, &ANIM_checkAnimationHeader, &ANIM_freeAnimationHeader, &ANIM_printAnimationHeader, &ANIM_compareAnimationHeader},
#include <libilbm/ilbmformextensions.h>
};

/* An array of all form types this library should parse. They must be alphabetically sorted. */
static IFF_Extension extension[] = {
#include <libilbm/ilbmextensions.h>
};

IFF_Chunk *ANIM_read(const char *filename)
{
    return IFF_read(filename, extension, ANIM_NUM_OF_FORM_TYPES);
}

IFF_Chunk *ANIM_readFd(FILE *file)
{
    return IFF_readFd(file, extension, ANIM_NUM_OF_FORM_TYPES);
}

int ANIM_writeFd(FILE *file, const IFF_Chunk *chunk)
{
    return IFF_writeFd(file, chunk, extension, ANIM_NUM_OF_FORM_TYPES);
}

int ANIM_write(const char *filename, const IFF_Chunk *chunk)
{
    return IFF_write(filename, chunk, extension, ANIM_NUM_OF_FORM_TYPES);
}

int ANIM_check(const IFF_Chunk *chunk)
{
    return IFF_check(chunk, extension, ANIM_NUM_OF_FORM_TYPES);
}

void ANIM_free(IFF_Chunk *chunk)
{
    IFF_free(chunk, extension, ANIM_NUM_OF_FORM_TYPES);
}

void ANIM_print(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    IFF_print(chunk, 0, extension, ANIM_NUM_OF_FORM_TYPES);
}

int ANIM_compare(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    return IFF_compare(chunk1, chunk2, extension, ANIM_NUM_OF_FORM_TYPES);
}
