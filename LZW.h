//
// Created by Dustin Burda on 11/15/23.
//

#ifndef COMPRESSIONSERVICE_LZW_H
#define COMPRESSIONSERVICE_LZW_H

#include "Compress.h"

class LZW : public Compressor<IntegerList> {

    void compress(const std::string& src, IntegerList& compressed) override {
        std::unordered_map<std::string, int> dict;
        build_dict(dict);

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

    }

    void decompress(const IntegerList& compressed, std::string& src) override {

    }

private:

    void build_dict(std::unordered_map<std::string, int>& dict) {
        bool wrap = false;
        for(uint8_t i = 0; wrap == true; i++) {
            dict[std::string(1, i)] = i;
            if(i == 255)
                wrap = true;
        }
    }
};


#endif //COMPRESSIONSERVICE_LZW_H
