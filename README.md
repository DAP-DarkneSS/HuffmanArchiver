[Building status](https://build.opensuse.org/project/show/home:DarkSS) (live master brach), check logs for clang static analyzer & valgrind memcheck output.

Development status:
DONE
 * command line arguments with exit code:
   + 1 ← arguments number is less than 3;
   + 2 ← the second argument isn't an action;
   + 3 ← input file isn't accessible;
 * compressing
   + the first input file pass;
   + chars occurrence statistic;
   + the second input file pass;
   + output file writing: header;
 * Huffman coding
   + Huffman tree raising;
   + encoding;
 * extracting
   + input file reading: header;
   + output file writing;

TUNEDUP
 * compressing
   + predefined symbols list;

BUGZONE
 * compressing
   - output file writing: bitstream end;
 * extracting
   - input file reading: bitstream end;

NEEDOPTIMIZATION
 * extracting
   - 093: if (SymbolWeightPtr [SymbolWeightIndex].LeftBranchIndex == -1)

TODO
 * Makefile
