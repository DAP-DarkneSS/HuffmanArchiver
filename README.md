[Building status](https://build.opensuse.org/package/show/home:DarkSS/HuffmanArchiver) (live master brach), check logs for clang static analyzer & valgrind memcheck output.

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
 * Makefile (design assignment requirement, cmake is default)
   + 'all' target
   + 'clean' target

TUNEDUP
 * compressing
   + predefined symbols list;
 * extracting
   + bytes cache;
   + bitwise operations speed-up;
 * I/O speed-up
   + unlocked std I/O;
   + fgetc → fread (~ kByte);
 * architecture-bit numbers.

NEEDOPTIMIZATION
 * I/O
   + fputc → fwrite;
