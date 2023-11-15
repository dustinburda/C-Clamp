#include <iostream>
#include "Huffman.h"

int main() {
    Huffman& single_instance = Huffman::instance();

    BitSet bits;
    std::string s {"Compress this with Huffman!"};

    single_instance.compress(s, bits);

    std:: cout << s << "\n";

    std::string result = "";
    single_instance.decompress(bits, result);

    std::cout << result << "\n";

}
