//
// Created by Dustin Burda on 11/15/23.
//

#ifndef COMPRESSIONSERVICE_LZW_H
#define COMPRESSIONSERVICE_LZW_H

#include "Compress.h"
#include "util.h"
#include <iostream>
#include <unordered_map>
#include <map>

class LZW : public Compressor<std::vector<uint8_t>, IntegerList> {
public:
    // Singleton
    static LZW& get_instance() {
        static LZW instance;
        return instance;
    }

    //Can't copy
    LZW(const LZW& other) = delete;
    LZW& operator=(const LZW& other) = delete;
    //Can't delete
    LZW(const LZW&& other) = delete;
    LZW& operator=(const LZW&& other) = delete;

   //  ~LZW() override { }

   std::optional<uint8_t> set_word_size(uint8_t w_size) {
       if(w_size < 8)
           return std::nullopt;

       word_size_ = w_size;
       return w_size;
   }

    void decompress_file(std::string& ifilename, std::string& ofilename) {
        std::ifstream ifile { ifilename, std::ios::binary };

        std::size_t filesize = std::filesystem::file_size(ifilename);
        std::vector<uint8_t> vec_bytes;
        vec_bytes.resize(filesize);

        ifile.read((char *)vec_bytes.data(), filesize);

//        IntegerList compressed_integers;
//        compress(vec_bytes, compressed_integers);
//
//        BitSet compressed_bits;
//        vinteger_to_vbit(compressed_integers, compressed_bits, word_size_);
//
//        std::vector<uint8_t> compressed_bytes;
//        vbit_to_vbyte(compressed_bits, compressed_bytes);
//
//        std::ofstream ofile { ofilename, std::ios::binary };
//
//        ofile.write((char *)&word_size_, 1);
//        write_bytes_file(compressed_bytes, ofile);
    }



   void write_bytes_file(const std::vector<uint8_t>& compressed_bytes, std::ofstream& ofile) {
       std::for_each(compressed_bytes.begin(), compressed_bytes.end(), [&ofile](auto byte) {
           ofile.write((char *)&byte, 1);
       });
   }

   void compress_file(std::string& ifilename, std::string& ofilename) {
       std::ifstream ifile { ifilename, std::ios::binary };

       std::size_t filesize = std::filesystem::file_size(ifilename);
       std::vector<uint8_t> vec_bytes;
       vec_bytes.resize(filesize);

       ifile.read((char *)vec_bytes.data(), filesize);

       IntegerList compressed_integers;
       compress(vec_bytes, compressed_integers);

       BitSet compressed_bits;
       vinteger_to_vbit(compressed_integers, compressed_bits, word_size_);

       std::vector<uint8_t> compressed_bytes;
       vbit_to_vbyte(compressed_bits, compressed_bytes);

       std::ofstream ofile { ofilename, std::ios::binary };

       ofile.write((char *)&word_size_, 1);
       write_bytes_file(compressed_bytes, ofile);
   }



    void compress(const std::vector<uint8_t> & src, IntegerList& compressed) override {
        std::cout << "Compressing...\n";
        std::map<std::vector<uint8_t>, int> dict;
        build_compress_dict(dict);

        int code = 256;

        // std::string prefix { "" };
        std::vector<uint8_t> prefix;
        for(auto elem : src) {
            prefix.push_back(elem);

            if(dict.count(prefix) > 0) {
                continue;
            } else {
                if(dict.size() < (1 << word_size_)) {
                    dict[prefix] = code;
                    code++;
                }

                prefix.pop_back();
                compressed.push_back(dict[prefix]);

                prefix.clear();
                prefix.push_back(elem);
            }
        }

        if(prefix.size()) {
            compressed.push_back(dict[prefix]);
        }

    }

    void decompress(const IntegerList& compressed, std::vector<uint8_t>& decompressed) override {
        std::cout << "Decompressing... \n";

        std::map<int, std::vector<uint8_t>> dict;
        build_decompress_dict(dict);

        int new_code = 256;

        std::vector<uint8_t> old_s = dict[compressed[0]];
        decompressed.insert(decompressed.end(), old_s.begin(), old_s.end()); // Concatenate first decompressed byte to decompressed bytes
        for(int i = 1; i < compressed.size(); i++) {
            auto curr_code = compressed[i];

            std::vector<uint8_t> new_s;
            if(dict.count(curr_code) > 0)
                new_s = dict[curr_code];
            else {
                new_s = old_s;
                new_s.push_back(old_s[0]);
            }

            decompressed.insert(decompressed.end(), new_s.begin(), new_s.end());

            if(dict.size() < (1 << word_size_)) {
                dict[new_code] = old_s;
                dict[new_code].push_back(new_s[0]);

                new_code++;
            }

            old_s = new_s;
        }

    }

private:
    LZW() : word_size_{8} {}

    void build_compress_dict(std::map<std::vector<uint8_t>, int> & dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == false; i++) {
            std::vector<uint8_t> character {i};
            dict[character] = i;

            if(i == 255)
                wrap = true;
        }
    }

    void build_decompress_dict(std::map<int, std::vector<uint8_t>> & dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == false; i++) {
            std::vector<uint8_t> character {i};
            dict[i] = character;

            if(i == 255)
                wrap = true;
        }
    }

    void build_compress_dictOLD(std::unordered_map<std::string, int> & dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == false; i++) {
            dict[std::string(1, i)] = i;

            if(i == 255)
                wrap = true;
        }
    }

    void build_decompress_dictOLD(std::unordered_map<int, std::string> & dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == false; i++) {
            dict[i] = std::string(1, i);

            if(i == 255)
                wrap = true;
        }
    }




private:
    uint8_t word_size_;
};


#endif //COMPRESSIONSERVICE_LZW_H
