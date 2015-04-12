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

#include <limits.h> // error: ‘CHAR_BIT’ undeclared
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

void compress (char InputFileName[], char OutputFileName[])
{
    const size_t MaxSymbolsCount = 1 << CHAR_BIT;
    struct SymbolWeightStruct *SymbolWeightPtr = malloc
    (
        MaxSymbolsCount * 2 * SymbolWeightSingleSize
    );
    size_t SymbolWeightCount = 0;
    FILE *InputFile;
    FILE *OutputFile;
    int TempChar = EOF;
    bool UnKnownSymbol = true;
    int BitsDataCount = 0;
    unsigned long int BitsStream = 0;
    int OutputByte = 0;
    int BitsStreamCount = 0;
    size_t SymbolWeightIndex = 0;
    size_t MaxWeight = 0;
    size_t MaxWeightBits = 0;
    size_t TempWeight = 0;

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc_unlocked (InputFile)) != EOF)
    {
        UnKnownSymbol = true;
        for (size_t i = 0; (i < SymbolWeightCount) && (UnKnownSymbol); i++)
        {
            if (SymbolWeightPtr [i].Symbol == TempChar)
            {
                (SymbolWeightPtr [i].Weight)++;
                UnKnownSymbol = false;
            }
        }
        if (UnKnownSymbol)
        {
            SymbolWeightPtr [SymbolWeightCount].Symbol           = TempChar;
            SymbolWeightPtr [SymbolWeightCount].Weight           = 1;
            SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  = -1;
            SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = -1;
            SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
            SymbolWeightCount++;
        }
    }
    fclose (InputFile);

    SymbolWeightCount = makeHuffman (SymbolWeightPtr, SymbolWeightCount);

    for
    (
        SymbolWeightIndex = 0;
        (
            (SymbolWeightIndex < SymbolWeightCount) &&
            (SymbolWeightPtr [SymbolWeightIndex].Symbol != -1)
        );
        SymbolWeightIndex++
    )
    {
        if (SymbolWeightPtr [SymbolWeightIndex].Weight > MaxWeight)
        {
            MaxWeight = SymbolWeightPtr [SymbolWeightIndex].Weight;
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
        SymbolWeightIndex = 0;
        SymbolWeightPtr [SymbolWeightIndex].Symbol != -1;
        SymbolWeightIndex++
    )
    {
        putc_unlocked (SymbolWeightPtr [SymbolWeightIndex].Symbol, OutputFile);
        TempWeight = SymbolWeightPtr [SymbolWeightIndex].Weight;
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
    }
    putc_unlocked (SymbolWeightPtr [SymbolWeightIndex - 1].Symbol, OutputFile);
// A HACK to mark the end of characters occurrence statistic ^

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc_unlocked (InputFile)) != EOF)
    {
        for
            (
                SymbolWeightIndex = 0;
                SymbolWeightPtr [SymbolWeightIndex].Symbol != TempChar;
                SymbolWeightIndex++
            ){}
        BitsDataCount = SymbolWeightPtr [SymbolWeightIndex].CodeBitsCount;
        BitsStream = (BitsStream << BitsDataCount) +
                     SymbolWeightPtr [SymbolWeightIndex].Code;
        BitsStreamCount += BitsDataCount;
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
