#include <iostream>
#include "Huffman.h"
#include "LZW.h"

int main() {

    //=================================================
    //|    HUFFMAN COMPRESSION TESTS
    //================================================
    Huffman& huffman_instance = Huffman::get_instance();

    BitSet bits;
    std::string s1 {"Compress this with Huffman!"};

    huffman_instance.compress(s1, bits);
    std::cout <<  "Original: " << s1 << "\n";


    std::string result1 = "";
    huffman_instance.decompress(bits, result1);

    std::cout << "Decompressed: " << result1 << "\n\n\n";
    //=================================================
    //|    LZW COMPRESSION TESTS
    //================================================

    LZW& lzw_instance = LZW::get_instance();

    IntegerList ints;
    std::string s2 {"Compress this with LZW!"};

    lzw_instance.compress(s2, ints);
    std::cout <<  "Original: " << s2 << "\n";

    std::string result2 = "";
    lzw_instance.decompress(ints, result2);

    std::cout << "Decompressed: " << result2 << "\n\n\n";
    //=================================================
    //|    RLE COMPRESSION TESTS
    //================================================



}
