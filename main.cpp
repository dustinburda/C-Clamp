#include <iostream>
#include "Huffman.h"
#include "LZW.h"
#include "RLE.h"
#include "quotes.h"



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

    auto start = std::chrono::steady_clock::now();



        huffman_instance.compress_file("/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compress_this.txt",
                                       "/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compressed_file.txt");
auto compression_end = std::chrono::steady_clock::now();

        huffman_instance.decompress_file("/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compressed_file.txt",
                                         "/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/uncompressed_file.txt");

    auto end = std::chrono::steady_clock::now();

    std::cout << "Compression Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(compression_end - start).count() << std::endl;
    std::cout << "Decompression Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - compression_end).count() << std::endl;
//    //=================================================
//    //|    LZW COMPRESSION TESTS
//    //================================================

//    LZW& lzw_instance = LZW::get_instance();
//    lzw_instance.set_word_size(10);
//
//    IntegerList ints;
//
////    std::string s2 = "gegefykfymjkykky"; // g   e    ge   f    y    k    fy   m   j   k   yk  ky
////                                         // 103 101  256  102  121  107  259  109 106 107 260 265
////                                      //        256  257  258  259  260  261  262 263 264 265 266
////                                     // {ge: 256, eg: 257, gef: 258, fy: 259, yk: 260, kf: 261, fym: 262, mj: 263, jk:264, ky: 265, ykk: 266}
//
//    std::string s2 = SHAKESPEARE;
//    std::vector<uint8_t> bytes {s2.begin(), s2.end()};
//
//    lzw_instance.compress(bytes, ints);
//
//
//    std::vector<uint8_t> result_bytes;
//    lzw_instance.decompress(ints, result_bytes);
//
//    std::string result_string {result_bytes.begin(), result_bytes.end()};
//
//    std::cout << "Decompressed: " << result_string << "\n\n\n";
//    assert(s2 == result_string);


    //=================================================
    //|    RLE COMPRESSION TESTS
    //================================================

//    RLE& rle_instance = RLE::get_instance();
//
//        rle_instance.compress_file("/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compress_thisRLE.txt",
//                                       "/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compressed_fileRLE.txt");
//
//        rle_instance.decompress_file("/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/compressed_fileRLE.txt",
//                                         "/Users/dustin/Desktop/Programming/C++ Programming/CompressionService/cmake-build-debug/uncompressed_fileRLE.txt");

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
