/* Copyright (C) 2015 Dmitriy A. Perlow <dap.darkness@gmail.com>

This file is part of Huffman Archiver for EPAM linux cource.

HuffmanArchiver is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2,
or (at your option) any later version.

HuffmanArchiver is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General
Public License along with HuffmanArchiver. If not, see
<http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html> */

#define _BSD_SOURCE
// ^ HACK vs. warning: implicit declaration of *_unlocked

#include <limits.h> // error: ‘CHAR_BIT’ undeclared
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

void writeInBytes
(
    unsigned int Number,
    size_t BitsCount,
    FILE *OutputFile
)
{
    int OutputByte = 0;
    for
    (
        size_t i = (BitsCount - CHAR_BIT);
        i >= CHAR_BIT;
        i -= CHAR_BIT
    )
    {
        OutputByte = Number >> i;
        putc_unlocked (OutputByte, OutputFile);
        Number -= (OutputByte << i);
    }
    putc_unlocked (Number, OutputFile);
}

void compress (char InputFileName[], char OutputFileName[])
{
    const int MaxSymbolsCount = 1 << CHAR_BIT;
    struct SymbolWeightStruct *SymbolWeightPtr = malloc
    (
        MaxSymbolsCount * 2 * SymbolWeightSingleSize
    );
    int SymbolWeightCount = 0;
    FILE *InputFile;
    FILE *OutputFile;
    unsigned long int BitsStream = 0;
    int OutputByte = 0;
    int BitsStreamCount = 0;
    size_t MaxWeightBits = 0;
    int TheLastSymbolIndex = 0;
    unsigned char InputCache [IO_BYTES];
    size_t InputCacheCount;

    for (int i = 0; i < MaxSymbolsCount; i++)
    {
        SymbolWeightPtr [i].Symbol           =  i;
        SymbolWeightPtr [i].Weight           =  0;
        SymbolWeightPtr [i].LeftBranchIndex  = -1;
        SymbolWeightPtr [i].RightBranchIndex = -1;
        SymbolWeightPtr [i].ParentIndex      = -1;
    }

    InputFile = fopen (InputFileName, "rb");
    while
    (
        (
            InputCacheCount = fread_unlocked
            (
                InputCache, 1, IO_BYTES, InputFile
            )
        ) > 0
    )
    {
        for (size_t i = 0; i < InputCacheCount; i++)
        {
            SymbolWeightPtr [InputCache[i]].Weight++;
        }
    }
    fclose (InputFile);

    SymbolWeightCount = makeHuffman (SymbolWeightPtr, MaxSymbolsCount);

    for
    (
        size_t i = SymbolWeightPtr [SymbolWeightCount - 1].Weight;
        i > 0;
        i >>= CHAR_BIT
    )
    {
        MaxWeightBits += CHAR_BIT;
    }
    OutputFile = fopen (OutputFileName, "w");
    putc_unlocked (MaxWeightBits, OutputFile);
    writeInBytes
    (
        SymbolWeightPtr [SymbolWeightCount - 1].Weight,
        MaxWeightBits,
        OutputFile
    );
    for
    (
        int i = 0;
        SymbolWeightPtr [i].Symbol != -1;
        i++
    )
    {
        if (SymbolWeightPtr [i].Weight != 0)
        {
            putc_unlocked
            (
                i, OutputFile
            );
            writeInBytes
            (
                SymbolWeightPtr [i].Weight,
                MaxWeightBits,
                OutputFile
            );
            TheLastSymbolIndex = i;
        }
    }
    putc_unlocked (TheLastSymbolIndex, OutputFile);
// A HACK to mark the end of characters occurrence statistic ^

    InputFile = fopen (InputFileName, "rb");
    while
    (
        (
            InputCacheCount = fread_unlocked
            (
                InputCache, 1, IO_BYTES, InputFile
            )
        ) > 0
    )
    {
        for (size_t i = 0; i < InputCacheCount; i++)
        {
            BitsStream =
                (
                    (BitsStream << SymbolWeightPtr [InputCache[i]].CodeBitsCount)
                    + SymbolWeightPtr [InputCache[i]].Code
                );
            BitsStreamCount += SymbolWeightPtr [InputCache[i]].CodeBitsCount;
            while (BitsStreamCount >= CHAR_BIT)
            {
                OutputByte = BitsStream >> (BitsStreamCount - CHAR_BIT);
                BitsStream -= ((unsigned long int) OutputByte) <<
                              (BitsStreamCount - CHAR_BIT);
                BitsStreamCount -= CHAR_BIT;
                putc_unlocked (OutputByte, OutputFile);
            }
        }
    }
    fclose (InputFile);
    OutputByte = BitsStream << (CHAR_BIT - BitsStreamCount);
    putc_unlocked (OutputByte, OutputFile);
    fclose (OutputFile);

    free (SymbolWeightPtr);
}
