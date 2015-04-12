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

#define _POSIX_C_SOURCE 200809L
// ^ HACK vs. warning: implicit declaration of *c_unlocked

#include <limits.h> // error: ‘CHAR_BIT’ undeclared
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

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
    int TempChar = EOF;
    unsigned long int BitsStream = 0;
    int OutputByte = 0;
    int BitsStreamCount = 0;
    size_t MaxWeight = 0;
    size_t MaxWeightBits = 0;
    size_t TempWeight = 0;
    int TheLastSymbolIndex = 0;

    for (int i = 0; i < MaxSymbolsCount; i++)
    {
        SymbolWeightPtr [i].Symbol           =  i;
        SymbolWeightPtr [i].Weight           =  0;
        SymbolWeightPtr [i].LeftBranchIndex  = -1;
        SymbolWeightPtr [i].RightBranchIndex = -1;
        SymbolWeightPtr [i].ParentIndex      = -1;
    }

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc_unlocked (InputFile)) != EOF)
    {
        SymbolWeightPtr [TempChar].Weight++;
    }
    fclose (InputFile);

    SymbolWeightCount = makeHuffman (SymbolWeightPtr, MaxSymbolsCount);

    for
    (
        int i = 0;
        (
            (i < SymbolWeightCount) &&
            (SymbolWeightPtr [i].Symbol != -1)
        );
        i++
    )
    {
        if (SymbolWeightPtr [i].Weight > MaxWeight)
        {
            MaxWeight = SymbolWeightPtr [i].Weight;
        }
    }
    for
    (
        MaxWeightBits = 0;
        MaxWeight > 0;
        MaxWeight >>= CHAR_BIT
    )
    {
        MaxWeightBits++;
    }
    MaxWeightBits *= CHAR_BIT;
    OutputFile = fopen (OutputFileName, "w");
    putc_unlocked (MaxWeightBits, OutputFile);
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
            TempWeight = SymbolWeightPtr [i].Weight;
            for
            (
                size_t j = (MaxWeightBits - CHAR_BIT);
                j >= CHAR_BIT;
                j -= CHAR_BIT
            )
            {
                OutputByte = TempWeight >> j;
                putc_unlocked (OutputByte, OutputFile);
                TempWeight -= (OutputByte << j);
            }
            putc_unlocked (TempWeight, OutputFile);
            TheLastSymbolIndex = i;
        }
    }
    putc_unlocked (TheLastSymbolIndex, OutputFile);
// A HACK to mark the end of characters occurrence statistic ^

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc_unlocked (InputFile)) != EOF)
    {
        BitsStream =
        (
            (BitsStream << SymbolWeightPtr [TempChar].CodeBitsCount)
            + SymbolWeightPtr [TempChar].Code
        );
        BitsStreamCount += SymbolWeightPtr [TempChar].CodeBitsCount;
        while (BitsStreamCount >= CHAR_BIT)
        {
            OutputByte = BitsStream >> (BitsStreamCount - CHAR_BIT);
            BitsStream -= ((unsigned long int) OutputByte) <<
                          (BitsStreamCount - CHAR_BIT);
            BitsStreamCount -= CHAR_BIT;
            putc_unlocked (OutputByte, OutputFile);
        }
    }
    fclose (InputFile);
    OutputByte = BitsStream << (CHAR_BIT - BitsStreamCount);
    putc_unlocked (OutputByte, OutputFile);
    fclose (OutputFile);

    free (SymbolWeightPtr);
}
