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
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

void extract (char InputFileName[], char OutputFileName[])
{
    struct SymbolWeightStruct *SymbolWeightPtr = malloc (0);
    size_t SymbolWeightCount = 0;
    FILE *InputFile;
    FILE *OutputFile;
    int TempChar = EOF;
    size_t SymbolWeightIndex = 0;
    size_t MaxWeightBits = 0;
    int TempBit = 0;

    InputFile = fopen (InputFileName, "r");
    MaxWeightBits = getc_unlocked (InputFile);

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
        TempChar = getc_unlocked (InputFile); // The first weight byte.
        SymbolWeightPtr [SymbolWeightCount].Weight           = TempChar;
        SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  = -1;
        SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = -1;
        SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
        for (size_t i = CHAR_BIT; i < MaxWeightBits; i += CHAR_BIT)
        {
            TempChar = getc_unlocked (InputFile); // The next weight byte.
            SymbolWeightPtr [SymbolWeightCount].Weight     <<= CHAR_BIT;
            SymbolWeightPtr [SymbolWeightCount].Weight      += TempChar;
        }
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
        for (int i = (CHAR_BIT - 1); i >= 0; i--)
        {
            TempBit = TempChar >> i;
            TempChar -= (TempBit >> i);
            if (TempBit == 0)
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
                putc_unlocked
                (
                    SymbolWeightPtr [SymbolWeightIndex].Symbol,
                    OutputFile
                );
                SymbolWeightIndex = SymbolWeightCount - 1;
            }
        }
    }

    fclose (OutputFile);
    fclose (InputFile);
    free (SymbolWeightPtr);
}
