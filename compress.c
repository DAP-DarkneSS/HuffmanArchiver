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
    struct SymbolWeightStruct *SymbolWeightPtr = malloc (0);
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

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc (InputFile)) != EOF)
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
            SymbolWeightCount++;
        }
    }
    fclose (InputFile);

    SymbolWeightPtr = realloc
    (
        SymbolWeightPtr,
        (SymbolWeightCount * 2 * SymbolWeightSingleSize)
    );
    SymbolWeightCount = makeHuffman (SymbolWeightPtr, SymbolWeightCount);

    OutputFile = fopen (OutputFileName, "w");
// TODO Put coding header ^

    InputFile = fopen (InputFileName, "r");
    while ((TempChar = getc (InputFile)) != EOF)
    {
        for
            (
                SymbolWeightIndex = 0;
                (
                    (SymbolWeightIndex < SymbolWeightCount) &&
                    (SymbolWeightPtr [SymbolWeightIndex].Symbol != TempChar)
                );
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
            putc (OutputByte, OutputFile);
        }
    }
    fclose (InputFile);
    OutputByte = BitsStream << (CHAR_BIT - BitsStreamCount);
    putc (OutputByte, OutputFile);
    fclose (OutputFile);

    free (SymbolWeightPtr);
}
