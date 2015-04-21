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

#define IO_BYTES (1 << 13)

#include <stdint.h> // error: ‘int_fast32_t’ undeclared

struct SymbolWeightStruct
{
    int_fast32_t  Symbol;
    uint_fast32_t Weight;
    int_fast32_t  LeftBranchIndex;
    int_fast32_t  RightBranchIndex;
    int_fast32_t  ParentIndex;
    uint_fast32_t Code;
    uint_fast32_t CodeBitsCount;
}; /// Core struct for this implementation.

static const int_fast32_t SymbolWeightSingleSize =
    sizeof (struct SymbolWeightStruct); /// Core struct size.

int_fast32_t makeHuffman
(
    struct SymbolWeightStruct *SymbolWeightPtr,
    int_fast32_t SymbolWeightCount
); /// Huffman coding.
