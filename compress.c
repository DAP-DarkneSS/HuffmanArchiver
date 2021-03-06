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

#define _DEFAULT_SOURCE
// ^ HACK vs. warning: implicit declaration of *_unlocked.

#include <limits.h> // error: ‘CHAR_BIT’ undeclared
#include <stdint.h> // error: ‘uint64_t’ undeclared
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

void writeInBytes
(
    uint_fast32_t Number,
    uint_fast32_t BitsCount,
    FILE *OutputFile
)
{
    int_fast32_t OutputByte = 0;
    for
    (
        uint_fast32_t i = (BitsCount - CHAR_BIT);
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
    const int_fast32_t MaxSymbolsCount = 1 << CHAR_BIT;
    struct SymbolWeightStruct *SymbolWeightPtr = malloc
    (
        MaxSymbolsCount * 2 * SymbolWeightSingleSize
    );
    int_fast32_t SymbolWeightCount = 0;
    FILE *InputFile;
    FILE *OutputFile;
    uint_fast64_t BitsStream = 0;
// Not uint_fast32_t to not be overflowed at 32-bit PC.
    int_fast32_t OutputByte = 0;
    int_fast32_t BitsStreamCount = 0;
    uint_fast32_t MaxWeightBits = 0;
    int_fast32_t TheLastSymbolIndex = 0;
    unsigned char InputCache [IO_BYTES];
    uint_fast32_t InputCacheCount;

    for (int_fast32_t i = 0; i < MaxSymbolsCount; i++)
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
        for (uint_fast32_t i = 0; i < InputCacheCount; i++)
        {
            SymbolWeightPtr [InputCache[i]].Weight++;
        }
    }
    fclose (InputFile);

    SymbolWeightCount = makeHuffman (SymbolWeightPtr, MaxSymbolsCount);

    for
    (
        uint_fast32_t i = SymbolWeightPtr [SymbolWeightCount - 1].Weight;
        i > 0;
        i >>= CHAR_BIT
    )
    {
        MaxWeightBits += CHAR_BIT;
    }
    if (MaxWeightBits == 0)
    {
        MaxWeightBits = 8; // Zero file HACK.
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
        int_fast32_t i = 0;
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
        for (uint_fast32_t i = 0; i < InputCacheCount; i++)
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
                BitsStream -= ((uint_fast64_t) OutputByte) <<
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
