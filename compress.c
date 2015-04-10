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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "huffcode.h"

void compress (char InputFileName[], char OutputFileName[])
{
    struct SymbolWeightStruct *SymbolWeightPtr = malloc (0);
    unsigned int SymbolWeightCount = 0;
    FILE *InputFile;
    int TempChar = EOF;
    bool UnKnownSymbol = true;

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc (InputFile)) != EOF)
    {
        UnKnownSymbol = true;
        for (unsigned int i = 0; (i < SymbolWeightCount) && (UnKnownSymbol); i++)
        {
            if (SymbolWeightPtr [i].Symbol == TempChar)
            {
                (SymbolWeightPtr [i].Weight)++;
                UnKnownSymbol = false;
            }
        }
        if (UnKnownSymbol)
        {
            SymbolWeightPtr = realloc
            (
                SymbolWeightPtr,
                (
                    (SymbolWeightCount + 1) * SymbolWeightSingleSize
                )
            );
            SymbolWeightPtr [SymbolWeightCount].Symbol           = TempChar;
            SymbolWeightPtr [SymbolWeightCount].Weight           = 1;
            SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  = -1;
            SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = -1;
            SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
            SymbolWeightPtr [SymbolWeightCount].Code             = 0;
            SymbolWeightCount++;
        }
    }
    fclose (InputFile);

    SymbolWeightCount = makeHuffman (SymbolWeightPtr, SymbolWeightCount);

    for (unsigned int i = 0; (i < SymbolWeightCount) && (SymbolWeightPtr [i].LeftBranchIndex == -1); i++)
    {
        printf("%c = %i\n", SymbolWeightPtr [i].Symbol, SymbolWeightPtr [i].Code);
    }
    free (SymbolWeightPtr);
}
