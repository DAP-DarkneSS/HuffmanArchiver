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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

struct BytesCacheStruct
{
             bool UnCached     [2 << CHAR_BIT];
             int  Index        [2 << CHAR_BIT];
    unsigned int  StringLength [2 << CHAR_BIT];
             int  String       [2 << CHAR_BIT] [CHAR_BIT];
};

unsigned int readInBytes
(
    size_t BitsCount,
    FILE *InputFile
)
{
    int TempChar = getc_unlocked (InputFile);
    unsigned int Number = TempChar;
    for (size_t i = CHAR_BIT; i < BitsCount; i += CHAR_BIT)
    {
        TempChar = getc_unlocked (InputFile);
        Number <<= CHAR_BIT;
        Number  += TempChar;
    }
    return (Number);
}

void extract (char InputFileName[], char OutputFileName[])
{
    struct SymbolWeightStruct *SymbolWeightPtr = malloc (0);
    int SymbolWeightCount = 0;
    FILE *InputFile;
    FILE *OutputFile;
    int TempChar = EOF;
    int SymbolWeightIndex = 0;
    size_t MaxWeightBits = 0;
    size_t SymbolsCount = 0;
    struct BytesCacheStruct BytesCache [1 << CHAR_BIT];
    int IndexCache = 0;

    for (size_t i = 0; i < (1 << CHAR_BIT); i++)
    {
        for (size_t j = 0; j < (2 << CHAR_BIT); j++)
        {
            BytesCache [i].UnCached     [j] = true;
            BytesCache [i].StringLength [j] = 0;
        }
    }

    InputFile = fopen (InputFileName, "r");
    MaxWeightBits = getc_unlocked (InputFile);
    SymbolsCount = readInBytes (MaxWeightBits, InputFile);

    TempChar = getc_unlocked (InputFile); // The first unique symbol.
    do
    {
        SymbolWeightPtr = realloc
        (
            SymbolWeightPtr,
            (
                (SymbolWeightCount + 1) * SymbolWeightSingleSize
            )
        );
        SymbolWeightPtr [SymbolWeightCount].Symbol           = TempChar;
        SymbolWeightPtr [SymbolWeightCount].Weight           = readInBytes
        (
            MaxWeightBits,
            InputFile
        );
        SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  = -1;
        SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = -1;
        SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
        SymbolWeightCount++;
        TempChar = getc_unlocked (InputFile); // The next symbol.
    }
    while (TempChar != SymbolWeightPtr [SymbolWeightCount - 1].Symbol);

    SymbolWeightPtr = realloc
    (
        SymbolWeightPtr,
        (SymbolWeightCount * 2 * SymbolWeightSingleSize)
    );
    SymbolWeightCount = makeHuffman (SymbolWeightPtr, SymbolWeightCount);

    SymbolWeightIndex = SymbolWeightCount - 1;
    OutputFile = fopen (OutputFileName, "w");
    while ((TempChar = getc_unlocked (InputFile)) != EOF) // Bit stream bytes.
    {
        if (BytesCache [TempChar].UnCached [SymbolWeightIndex])
        {
            IndexCache = SymbolWeightIndex;
            for (int i = (CHAR_BIT - 1); i >= 0; i--)
            {
                if ((TempChar & (1 << i)) == 0)
                {
                    SymbolWeightIndex = SymbolWeightPtr [SymbolWeightIndex]
                                        .LeftBranchIndex;
                }
                else
                {
                    SymbolWeightIndex = SymbolWeightPtr [SymbolWeightIndex]
                                        .RightBranchIndex;
                }
                if (SymbolWeightPtr [SymbolWeightIndex].LeftBranchIndex == -1)
                {
                    if (SymbolsCount > 0)
                    {
                        BytesCache [TempChar].String [IndexCache] [
                            BytesCache [TempChar].StringLength [IndexCache]
                        ] = SymbolWeightPtr [SymbolWeightIndex].Symbol;
                        BytesCache [TempChar].StringLength [IndexCache]++;
                        putc_unlocked
                        (
                            SymbolWeightPtr [SymbolWeightIndex].Symbol,
                            OutputFile
                        );
                        SymbolsCount--;
                    }
                    SymbolWeightIndex = SymbolWeightCount - 1;
                }
            }
            BytesCache [TempChar].Index    [IndexCache] = SymbolWeightIndex;
            BytesCache [TempChar].UnCached [IndexCache] = false;
        }
        else
        {
            for
            (
                size_t i = 0;
                i < BytesCache [TempChar].StringLength [SymbolWeightIndex];
                i++
            )
            {
                if (SymbolsCount > 0)
                {
                    putc_unlocked
                    (
                        BytesCache [TempChar].String [SymbolWeightIndex] [i],
                        OutputFile
                    );
                    SymbolsCount--;
                }
            }

            SymbolWeightIndex = BytesCache [TempChar].Index [SymbolWeightIndex];
        }
    }

    fclose (OutputFile);
    fclose (InputFile);
    free (SymbolWeightPtr);
}
