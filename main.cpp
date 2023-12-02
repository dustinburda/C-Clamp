#include <iostream>
#include "Huffman.h"
#include "LZW.h"
#include "RLE.h"


void string_to_bytes(const std::string& src, std::vector<uint8_t>& bytes) {
    bytes.resize(src.size());
    std::memcpy(bytes.data(), src.data(), src.size());
}

void bytes_to_string(const std::vector<uint8_t>& bytes, std::string& src) {
    src.resize(bytes.size());
    std::memcpy(src.data(), bytes.data(), bytes.size());
}

int main() {

    //TODO: Create bitstream class
    // Operate on files/uint8_t instead of strings

    //=================================================
    //|    HUFFMAN COMPRESSION TESTS
    //================================================
    Huffman& huffman_instance = Huffman::get_instance();

//    BitSet bits;
//    std::string s1 {"aaabaacbaa"};
//    std::vector<uint8_t> bytes;
//    string_to_bytes(s1, bytes);
//
//
//    huffman_instance.compress(bytes, bits);
//    std::cout <<  "Original: " << s1 << "\n";
//
//
//    std::vector<uint8_t> result1;
//    huffman_instance.decompress(bits, result1);
//
//    std::string s_result1;
//    bytes_to_string(result1, s_result1);
//
//    std::cout << "Decompressed: " << s_result1 << "\n\n\n";
//    assert(s1 == s_result1);
        huffman_instance.compress_file("/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compress_this.txt",
                                       "/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compressed_file.txt");

//    //=================================================
//    //|    LZW COMPRESSION TESTS
//    //================================================

//    LZW& lzw_instance = LZW::get_instance();
//
//    IntegerList ints;
//    std::string s2 {"Compress this with LZW!"};
//
//    lzw_instance.compress(s2, ints);
//    std::cout <<  "Original: " << s2 << "\n";
//
//    std::string result2 = "";
//    lzw_instance.decompress(ints, result2);
//
//    std::cout << "Decompressed: " << result2 << "\n\n\n";


    //=================================================
    //|    RLE COMPRESSION TESTS
    //================================================

//    RLE& rle_instance = RLE::get_instance();
//
//    std::string compressed;
//    std::string s3 { "eddddaaaacccccgcccf" };
//
//    rle_instance.compress(s3, compressed);
//    std::cout << "Original: " << s3 << "\n";
//
//    std::string result3 = "";
//    rle_instance.decompress(compressed, result3);
//
//    std::cout << "Decompressed: " << result3 << "\n\n\n";

}
