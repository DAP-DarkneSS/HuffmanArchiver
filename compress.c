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
# <http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html> */

#include <stdio.h>

void compress (char InputFileName [], char OutputFileName [])
{
    printf ("%s will be compressed into %s.\n", InputFileName, OutputFileName);
    FILE *InputFile;
    InputFile = fopen (InputFileName, "r");
    for ( int TempChar = getc (InputFile);
          TempChar != EOF;
          TempChar = getc (InputFile) )
    {
        printf ("%c", TempChar);
    }
    fclose (InputFile);
}
