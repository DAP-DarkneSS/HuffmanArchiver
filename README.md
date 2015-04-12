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
   + output file writing, header:
     * numbers bits count;
     * symbols count;
     * symbols occurrence statistic;
     * end of statistic mark;
   + output file writing: content;
 * Huffman coding
   + Huffman tree raising;
   + encoding;
 * extracting
   + input file reading, header:
     * numbers bits count;
     * symbols count;
     * symbols occurrence statistic;
     * end of statistic mark;
   + input file reading: content;
   + output file writing;

TUNEDUP
 * compressing
   + predefined symbols list;

BUGZONE
 * 

NEEDOPTIMIZATION
 * extracting
   - 093: if (SymbolWeightPtr [SymbolWeightIndex].LeftBranchIndex == -1)

TODO
 * Makefile
