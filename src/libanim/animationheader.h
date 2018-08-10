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

#ifndef __LIBANIM_ANIMATIONHEADER_H
#define __LIBANIM_ANIMATIONHEADER_H

#define ANIM_LONG_DATA_BIT         0x80000000
#define ANIM_XOR_BIT               0x40000000
#define ANIM_ONE_INFO_LIST_BIT     0x20000000
#define ANIM_RLC_BIT               0x10000000
#define ANIM_VERTICAL_BIT          0x08000000
#define ANIM_LONG_INFO_OFFSETS_BIT 0x04000000

#define ANIM_HEADER_NUM_OF_PADDING_BYTES 16

#include <stdio.h>
#include <libiff/ifftypes.h>
#include <libiff/group.h>
#include <libiff/chunk.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ANIM_OPERATION_SET_DIRECT = 0,
    ANIM_OPERATION_XOR_MODE = 1,
    ANIM_OPERATION_LONG_DELTA_MODE = 2,
    ANIM_OPERATION_SHORT_DELTA_MODE = 3,
    ANIM_OPERATION_GENERALIZED_SHORT_LONG_DELTA_MODE = 4,
    ANIM_OPERATION_BYTE_VERTICAL_DELTA_MODE = 5,
    ANIM_OPERATION_SHORT_LONG_VERTICAL_DELTA_MODE = 7
}
ANIM_Operation;

typedef struct
{
    IFF_Group *parent;

    IFF_ID chunkId;
    IFF_Long chunkSize;

    ANIM_Operation operation;
    IFF_UByte mask;
    IFF_UWord w, h;
    IFF_Word x, y;
    IFF_ULong abstime;
    IFF_ULong reltime;
    IFF_UByte interleave;
    IFF_UByte pad0;
    IFF_ULong bits;
    IFF_UByte pad[ANIM_HEADER_NUM_OF_PADDING_BYTES];
}
ANIM_AnimationHeader;

ANIM_AnimationHeader *ANIM_createAnimationHeader(void);

IFF_Chunk *ANIM_readAnimationHeader(FILE *file, const IFF_Long chunkSize);

int ANIM_writeAnimationHeader(FILE *file, const IFF_Chunk *chunk);

int ANIM_checkAnimationHeader(const IFF_Chunk *chunk);

void ANIM_freeAnimationHeader(IFF_Chunk *chunk);

void ANIM_printAnimationHeader(const IFF_Chunk *chunk, const unsigned int indentLevel);

int ANIM_compareAnimationHeader(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2);

#ifdef __cplusplus
}
#endif

#endif
