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

void extract (char InputFileName[], char OutFileName[])
{
    struct SymbolWeightStruct *SymbolWeightPtr = malloc (0);
    size_t SymbolWeightCount = 0;
    FILE *InputFile;
    int TempChar = EOF;
    size_t MaxWeightBits = 0;

    InputFile = fopen (InputFileName, "r");
    MaxWeightBits = getc (InputFile);

    TempChar = getc (InputFile); // The first unique symbol.
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
        TempChar = getc (InputFile); // The first weight byte.
        SymbolWeightPtr [SymbolWeightCount].Weight           = TempChar;
        SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  = -1;
        SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = -1;
        SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
        for (size_t i = 1; i < MaxWeightBits; i += CHAR_BIT)
        {
            TempChar = getc (InputFile); // The next weight byte.
            SymbolWeightPtr [SymbolWeightCount].Weight     <<= CHAR_BIT;
            SymbolWeightPtr [SymbolWeightCount].Weight      += TempChar;
        }
        SymbolWeightCount++;
        TempChar = getc (InputFile); // The next symbol.
    }
    while (TempChar != SymbolWeightPtr [SymbolWeightCount - 1].Symbol);

    SymbolWeightPtr = realloc
    (
        SymbolWeightPtr,
        (SymbolWeightCount * 2 * SymbolWeightSingleSize)
    );
    SymbolWeightCount = makeHuffman (SymbolWeightPtr, SymbolWeightCount);

    fclose (InputFile);
    free (SymbolWeightPtr);
}
