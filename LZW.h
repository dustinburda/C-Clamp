//
// Created by Dustin Burda on 11/15/23.
//

#ifndef COMPRESSIONSERVICE_LZW_H
#define COMPRESSIONSERVICE_LZW_H

#include "Compress.h"
#include <iostream>
#include <unordered_map>

class LZW : public Compressor<IntegerList> {
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

    void compress(const std::string& src, IntegerList& compressed) override {
        std::cout << "Compressing...\n";
        std::unordered_map<std::string, int> dict;
        build_compress_dict(dict);

        int code = 256;

        std::string prefix { "" };
        for(auto elem : src) {
            prefix.push_back(elem);

            if(dict.count(prefix) > 0) {
                continue;
            } else {
                dict[prefix] = code;
                code++;

                prefix.pop_back();
                compressed.push_back(dict[prefix]);

                prefix = elem;
            }
        }

        if(prefix.size()) {
            compressed.push_back(dict[prefix]);
        }

    }

    void decompress(const IntegerList& compressed, std::string& src) override {
        std::cout << "Decompressing... \n";

        std::unordered_map<int, std::string> dict;
        build_decompress_dict(dict);

        int new_code = 256;

        std::string old_s = dict[compressed[0]];
        src += old_s;
        for(int i = 1; i < compressed.size(); i++) {
            auto curr_code = compressed[i];

            std::string new_s { "" };
            if(dict.count(curr_code) > 0)
                new_s = dict[curr_code];
            else
                new_s = old_s + std::string(1, old_s[0]);

            src += new_s;

            dict[new_code] = old_s +std::string(1, new_s[0]);
            new_code++;

            old_s = new_s;
        }

    }

private:

    LZW() = default;


    void build_compress_dict(std::unordered_map<std::string, int> & dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == false; i++) {
            dict[std::string(1, i)] = i;

            if(i == 255)
                wrap = true;
        }
    }

    void build_decompress_dict(std::unordered_map<int, std::string> & dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == false; i++) {
            dict[i] = std::string(1, i);

            if(i == 255)
                wrap = true;
        }
    }

};


#endif //COMPRESSIONSERVICE_LZW_H
