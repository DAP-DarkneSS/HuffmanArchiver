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

#include <stddef.h> // error: unknown type name ‘uint_fast32_t’
#include "huffcode.h"

int_fast32_t whereIsParentless
(
    struct SymbolWeightStruct *SymbolWeightPtr,
    int_fast32_t MinIndex,
    int_fast32_t MaxIndex
)
{
    int_fast32_t ParentlessIndex = -1;
    for
        (
            int_fast32_t i = MinIndex;
            (i < MaxIndex) && (ParentlessIndex == -1);
            i++
        )
    {
        if
            (
                (SymbolWeightPtr [i].Weight      !=  0) &&
                (SymbolWeightPtr [i].ParentIndex == -1)
            )
        {
            ParentlessIndex = i;
        }
    }
    return (ParentlessIndex);
}

int_fast32_t makeHuffman
(
    struct SymbolWeightStruct *SymbolWeightPtr,
    int_fast32_t SymbolWeightCount
)
{
    int_fast32_t MinIndex1 = whereIsParentless
    (
        SymbolWeightPtr,
        0,
        SymbolWeightCount
    );
    int_fast32_t MinIndex2 = whereIsParentless
    (
        SymbolWeightPtr,
        (MinIndex1 + 1),
        SymbolWeightCount
    );
    int_fast32_t MinIndexCandidate = -1;

    if (MinIndex1 == -1)
    {
        SymbolWeightPtr [SymbolWeightCount].Symbol           = -1;
        SymbolWeightPtr [SymbolWeightCount].Weight           = 0;
        SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  =
        SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = -1;
        SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
        SymbolWeightPtr [SymbolWeightCount].Code             = 0;
        SymbolWeightPtr [SymbolWeightCount].CodeBitsCount    = 0;
        SymbolWeightCount++;
    }
    else if (MinIndex2 == -1)
    {
        SymbolWeightPtr [MinIndex1].Code                     = 0;
        SymbolWeightPtr [MinIndex1].CodeBitsCount            = 1;

        SymbolWeightPtr [SymbolWeightCount].Symbol           = -1;
        SymbolWeightPtr [SymbolWeightCount].Weight           =
                SymbolWeightPtr [MinIndex1].Weight;
        SymbolWeightPtr [SymbolWeightCount].LeftBranchIndex  =
        SymbolWeightPtr [SymbolWeightCount].RightBranchIndex = MinIndex1;
        SymbolWeightPtr [SymbolWeightCount].ParentIndex      = -1;
        SymbolWeightPtr [SymbolWeightCount].Code             = 0;
        SymbolWeightPtr [SymbolWeightCount].CodeBitsCount    = 0;
        SymbolWeightCount++;
    }
    else
    {
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
                                        SymbolWeightPtr, (MinIndex2 + 1),
                                        SymbolWeightCount
                                    );
                MinIndexCandidate != -1;
                MinIndexCandidate = whereIsParentless
                                    (
                                        SymbolWeightPtr, (MinIndexCandidate + 1),
                                        SymbolWeightCount
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

        for (int_fast32_t i = SymbolWeightCount - 2; i >= 0; i--)
        {
            if (SymbolWeightPtr [i].Weight != 0)
            {
                if (i == SymbolWeightPtr
                        [
                            SymbolWeightPtr [i].ParentIndex
                        ].LeftBranchIndex)
                {
                    SymbolWeightPtr [i].Code = SymbolWeightPtr
                    [
                        SymbolWeightPtr [i].ParentIndex
                    ].Code << 1;
                }
                else
                {
                    SymbolWeightPtr [i].Code = (SymbolWeightPtr
                    [
                        SymbolWeightPtr [i].ParentIndex
                    ].Code << 1) + 1;
                }
                SymbolWeightPtr [i].CodeBitsCount = SymbolWeightPtr
                    [
                        SymbolWeightPtr [i].ParentIndex
                    ].CodeBitsCount + 1;
            }
        }
    }

    return (SymbolWeightCount);
}
