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

BUGZONE
 * compressing
   . output file writing: content;
 * extracting
   . input file reading: content;

TODO
 * Makefile
