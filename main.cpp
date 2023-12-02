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
//    std::string s1 {"This is the 100th Etext file presented by Project Gutenberg, and\n"
//                    "is presented in cooperation with World Library, Inc., from their\n"
//                    "Library of the Future and Shakespeare CDROMS.  Project Gutenberg\n"
//                    "often releases Etexts that are NOT placed in the Public Domain!!\n"
//                    "\n"
//                    "Shakespeare\n"
//                    "\n"
//                    "*This Etext has certain copyright implications you should read!*\n"
//                    "\n"
//                    "<<THIS ELECTRONIC VERSION OF THE COMPLETE WORKS OF WILLIAM\n"
//                    "SHAKESPEARE IS COPYRIGHT 1990-1993 BY WORLD LIBRARY, INC., AND IS\n"
//                    "PROVIDED BY PROJECT GUTENBERG ETEXT OF ILLINOIS BENEDICTINE COLLEGE\n"
//                    "WITH PERMISSION.  ELECTRONIC AND MACHINE READABLE COPIES MAY BE\n"
//                    "DISTRIBUTED SO LONG AS SUCH COPIES (1) ARE FOR YOUR OR OTHERS\n"
//                    "PERSONAL USE ONLY, AND (2) ARE NOT DISTRIBUTED OR USED\n"
//                    "COMMERCIALLY.  PROHIBITED COMMERCIAL DISTRIBUTION INCLUDES BY ANY\n"
//                    "SERVICE THAT CHARGES FOR DOWNLOAD TIME OR FOR MEMBERSHIP.>>\n"
//                    "\n"
//                    "*Project Gutenberg is proud to cooperate with The World Library*\n"
//                    "in the presentation of The Complete Works of William Shakespeare\n"
//                    "for your reading for education and entertainment.  HOWEVER, THIS\n"
//                    "IS NEITHER SHAREWARE NOR PUBLIC DOMAIN. . .AND UNDER THE LIBRARY\n"
//                    "OF THE FUTURE CONDITIONS OF THIS PRESENTATION. . .NO CHARGES MAY\n"
//                    "BE MADE FOR *ANY* ACCESS TO THIS MATERIAL.  YOU ARE ENCOURAGED!!\n"
//                    "TO GIVE IT AWAY TO ANYONE YOU LIKE, BUT NO CHARGES ARE ALLOWED!!\n"
//                    "\n"
//                    "\n"
//                    "**Welcome To The World of Free Plain Vanilla Electronic Texts**"};
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

        huffman_instance.decompress_file("/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compressed_file.txt",
                                         "/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/uncompressed_file.txt");

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
