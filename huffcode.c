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

#include <stdlib.h>
#include "huffcode.h"

int whereIsParentless
(
    struct SymbolWeightStruct *SymbolWeightPtr,
    int MinIndex,
    int MaxIndex
)
{
    int ParentlessIndex = -1;
    for (int i = MinIndex; (i < MaxIndex) && (ParentlessIndex == -1); i++)
    {
        if (SymbolWeightPtr [i].ParentIndex == -1)
        {
            ParentlessIndex = i;
        }
    }
    return (ParentlessIndex);
}

unsigned int makeHuffman
(
    struct SymbolWeightStruct *SymbolWeightPtr,
    unsigned int SymbolWeightCount
)
{
    unsigned int MinIndex1 = 0;
    unsigned int MinIndex2 = 0;
    int MinIndexCandidate = -1;

    while
    (
        ((MinIndexCandidate =
        whereIsParentless (SymbolWeightPtr, 0, SymbolWeightCount))
        != (SymbolWeightCount - 1)) && (MinIndexCandidate != -1)
    )
    {
        MinIndex1 = MinIndexCandidate;
        MinIndex2 = whereIsParentless
                    (SymbolWeightPtr, (MinIndex1 + 1), SymbolWeightCount);
        for
        (
            MinIndexCandidate = whereIsParentless
            (
                SymbolWeightPtr, (MinIndex2 + 1), SymbolWeightCount
            );
            MinIndexCandidate != -1;
            MinIndexCandidate = whereIsParentless
            (
                SymbolWeightPtr, (MinIndexCandidate + 1), SymbolWeightCount
            )
        )
        {
            if
            (
                SymbolWeightPtr [MinIndexCandidate].Weight <
                SymbolWeightPtr [MinIndex2].Weight
            )
            {
                MinIndex2 = MinIndexCandidate;
            }
            else if
            (
                SymbolWeightPtr [MinIndexCandidate].Weight <
                SymbolWeightPtr [MinIndex1].Weight
            )
            {
                MinIndex1 = MinIndex2;
                MinIndex2 = MinIndexCandidate;
            }
        }
        SymbolWeightPtr = realloc
        (
            SymbolWeightPtr,
            (
                (SymbolWeightCount + 1) * SymbolWeightSingleSize
            )
        );
        SymbolWeightPtr [SymbolWeightCount].Symbol           = -1;
        SymbolWeightPtr [SymbolWeightCount].Weight           =
            SymbolWeightPtr [MinIndex1].Weight +
            SymbolWeightPtr [MinIndex2].Weight;
        SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  = MinIndex1;
        SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = MinIndex2;
        SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
        SymbolWeightPtr [SymbolWeightCount].Code             = 0;
        SymbolWeightPtr [SymbolWeightCount].CodeBitsCount    = 0;
        SymbolWeightPtr [MinIndex1].ParentIndex =
        SymbolWeightPtr [MinIndex2].ParentIndex = SymbolWeightCount;
        SymbolWeightCount++;
    }

    for (int i = SymbolWeightCount - 2; i >= 0; i--)
    {
        if (i == SymbolWeightPtr [
                SymbolWeightPtr [i].ParentIndex
            ].LeftBranchIndex)
        {
            SymbolWeightPtr [i].Code = (SymbolWeightPtr [
                SymbolWeightPtr [i].ParentIndex
            ].Code << 1) + 0;
        }
        else
        {
            SymbolWeightPtr [i].Code = (SymbolWeightPtr [
                SymbolWeightPtr [i].ParentIndex
            ].Code << 1) + 1;
        }
        SymbolWeightPtr [i].CodeBitsCount = SymbolWeightPtr [
                SymbolWeightPtr [i].ParentIndex
            ].CodeBitsCount + 1;
    }

    return (SymbolWeightCount);
}
