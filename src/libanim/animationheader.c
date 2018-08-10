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

#include "animationheader.h"
#include <string.h>
#include <libiff/io.h>
#include <libiff/util.h>
#include <libiff/error.h>
#include "anim.h"

#define CHUNKID "ANHD"

ANIM_AnimationHeader *ANIM_createAnimationHeader(void)
{
    ANIM_AnimationHeader *animationHeader = (ANIM_AnimationHeader*)IFF_allocateChunk(CHUNKID, sizeof(ANIM_AnimationHeader));

    if(animationHeader != NULL)
    {
        animationHeader->chunkSize = 2 * sizeof(IFF_UByte) + 2 * sizeof(IFF_UWord) + 2 * sizeof(IFF_Word) + 2 * sizeof(IFF_ULong) + 2 * sizeof(IFF_UByte) + sizeof(IFF_ULong) + 16 * sizeof(IFF_UByte);
        animationHeader->pad0 = '\0';
        memset(animationHeader->pad, '\0', 16 * sizeof(IFF_UByte));
    }

    return animationHeader;
}

IFF_Chunk *ANIM_readAnimationHeader(FILE *file, const IFF_Long chunkSize)
{
    ANIM_AnimationHeader *animationHeader = ANIM_createAnimationHeader();

    if(animationHeader != NULL)
    {
        IFF_UByte byte;

        if(!IFF_readUByte(file, &byte, CHUNKID, "operation"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        animationHeader->operation = byte;

        if(!IFF_readUByte(file, &animationHeader->mask, CHUNKID, "mask"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readUWord(file, &animationHeader->w, CHUNKID, "w"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readUWord(file, &animationHeader->h, CHUNKID, "h"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readWord(file, &animationHeader->x, CHUNKID, "x"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readWord(file, &animationHeader->y, CHUNKID, "y"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readULong(file, &animationHeader->abstime, CHUNKID, "abstime"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readULong(file, &animationHeader->reltime, CHUNKID, "reltime"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readUByte(file, &animationHeader->interleave, CHUNKID, "interleave"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readUByte(file, &animationHeader->pad0, CHUNKID, "pad0"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(!IFF_readULong(file, &animationHeader->bits, CHUNKID, "bits"))
        {
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }

        if(fread(animationHeader->pad, sizeof(IFF_UByte), ANIM_HEADER_NUM_OF_PADDING_BYTES, file) < ANIM_HEADER_NUM_OF_PADDING_BYTES)
        {
            IFF_error("Error reading padding bytes of chunk: '" CHUNKID "'\n");
            ANIM_free((IFF_Chunk*)animationHeader);
            return NULL;
        }
    }

    return (IFF_Chunk*)animationHeader;
}

int ANIM_writeAnimationHeader(FILE *file, const IFF_Chunk *chunk)
{
    const ANIM_AnimationHeader *animationHeader = (const ANIM_AnimationHeader*)chunk;

    if(!IFF_writeUByte(file, animationHeader->operation, CHUNKID, "operation"))
        return FALSE;

    if(!IFF_writeUByte(file, animationHeader->mask, CHUNKID, "mask"))
        return FALSE;

    if(!IFF_writeUWord(file, animationHeader->w, CHUNKID, "w"))
        return FALSE;

    if(!IFF_writeUWord(file, animationHeader->h, CHUNKID, "h"))
        return FALSE;

    if(!IFF_writeWord(file, animationHeader->x, CHUNKID, "x"))
        return FALSE;

    if(!IFF_writeWord(file, animationHeader->y, CHUNKID, "y"))
        return FALSE;

    if(!IFF_writeULong(file, animationHeader->abstime, CHUNKID, "abstime"))
        return FALSE;

    if(!IFF_writeULong(file, animationHeader->reltime, CHUNKID, "reltime"))
        return FALSE;

    if(!IFF_writeUByte(file, animationHeader->interleave, CHUNKID, "interleave"))
        return FALSE;

    if(!IFF_writeUByte(file, animationHeader->pad0, CHUNKID, "pad0"))
        return FALSE;

    if(!IFF_writeULong(file, animationHeader->bits, CHUNKID, "bits"))
        return FALSE;

    if(fwrite(animationHeader->pad, sizeof(IFF_UByte), ANIM_HEADER_NUM_OF_PADDING_BYTES, file) < ANIM_HEADER_NUM_OF_PADDING_BYTES)
        return FALSE;

    return TRUE;
}

int ANIM_checkAnimationHeader(const IFF_Chunk *chunk)
{
    const ANIM_AnimationHeader *animationHeader = (const ANIM_AnimationHeader*)chunk;

    if(animationHeader->operation > ANIM_OPERATION_SHORT_LONG_VERTICAL_DELTA_MODE)
    {
        IFF_error("Invalid 'ANHD'.operation value!\n");
        return FALSE;
    }

    return TRUE;
}

void ANIM_freeAnimationHeader(IFF_Chunk *chunk)
{
}

void ANIM_printAnimationHeader(const IFF_Chunk *chunk, const unsigned int indentLevel)
{
    const ANIM_AnimationHeader *animationHeader = (const ANIM_AnimationHeader*)chunk;

    IFF_printIndent(stdout, indentLevel, "operation = %u;\n", animationHeader->operation);
    IFF_printIndent(stdout, indentLevel, "mask = %u;\n", animationHeader->mask);
    IFF_printIndent(stdout, indentLevel, "w = %u;\n", animationHeader->w);
    IFF_printIndent(stdout, indentLevel, "h = %u;\n", animationHeader->h);
    IFF_printIndent(stdout, indentLevel, "x = %d;\n", animationHeader->x);
    IFF_printIndent(stdout, indentLevel, "y = %d;\n", animationHeader->y);
    IFF_printIndent(stdout, indentLevel, "abstime = %u;\n", animationHeader->abstime);
    IFF_printIndent(stdout, indentLevel, "reltime = %u;\n", animationHeader->reltime);
    IFF_printIndent(stdout, indentLevel, "interleave = %u;\n", animationHeader->interleave);
    IFF_printIndent(stdout, indentLevel, "pad0 = %u;\n", animationHeader->pad0);
    IFF_printIndent(stdout, indentLevel, "bits = %u;\n", animationHeader->bits);
}

int ANIM_compareAnimationHeader(const IFF_Chunk *chunk1, const IFF_Chunk *chunk2)
{
    const ANIM_AnimationHeader *animationHeader1 = (const ANIM_AnimationHeader*)chunk1;
    const ANIM_AnimationHeader *animationHeader2 = (const ANIM_AnimationHeader*)chunk2;

    if(animationHeader1->operation != animationHeader2->operation)
        return FALSE;

    if(animationHeader1->mask != animationHeader2->mask)
        return FALSE;

    if(animationHeader1->w != animationHeader2->w)
        return FALSE;

    if(animationHeader1->h != animationHeader2->h)
        return FALSE;

    if(animationHeader1->x != animationHeader2->x)
        return FALSE;

    if(animationHeader1->y != animationHeader2->y)
        return FALSE;

    if(animationHeader1->abstime != animationHeader2->abstime)
        return FALSE;

    if(animationHeader1->reltime != animationHeader2->reltime)
        return FALSE;

    if(animationHeader1->interleave != animationHeader2->interleave)
        return FALSE;

    if(animationHeader1->bits != animationHeader2->bits)
        return FALSE;

    return TRUE;
}
