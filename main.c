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
#include <string.h>
#include <unistd.h>

#include "compress.h"
#include "extract.h"

int main (int argc, char * argv[])
{
    bool Compressing, HelpShowing = false;
    int ExitCode = 0;
    for (int i = 0; (i < argc) && (! HelpShowing); i++)
    {
        if ( (strstr (argv [i], "help") != NULL) ||
             (strstr (argv [i], "-h"  ) != NULL) )
// python: if "help" in argv [i]
        {
            HelpShowing = true;
        }
    }
    if (HelpShowing)
    {
        printf ("huff InputFile [ -c|-x ] OutputFile\n");
    }
    else
    {
        if (argc < 4)
// argc[0] == executable file name
        {
            printf ("Too few arguments: %i < 3.\n", (argc - 1));
            ExitCode = 1;
        }
        else
        {
            if (strcmp (argv [2], "-c") == 0)
// python: string1 == string2
            {
                Compressing = true;
            }
            else if (strcmp (argv [2], "-x") == 0)
            {
                Compressing = false;
            }
            else
            {
                printf ("The secong argument isn't an action: %s.\n", argv [2]);
                ExitCode = 2;
            }
            if (access (argv [1], F_OK) == -1)
            {
                printf ("Input file %s isn't accessible.\n", argv [1]);
                ExitCode = 3;
            }
            if (ExitCode == 0)
            {
                if (Compressing)
                {
                    compress (argv [1], argv [3]);
                }
                else
                {
                    extract (argv [1], argv [3]);
                }
            }
            else
            {
                printf ("Try `%s --help`.\n", argv [0]);
            }
        }
    }
    return (ExitCode);
}
