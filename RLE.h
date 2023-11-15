//
// Created by Dustin Burda on 11/14/23.
//

#ifndef COMPRESSIONSERVICE_RLE_H
#define COMPRESSIONSERVICE_RLE_H

#include "Compress.h"
#include <string>


// TODO, use bitstream class, and compress binary runs

class RLE : public Compressor<std::string> {
public:
    void compress(const std::string& src, std::string& compressed) override {

    }

    void decompress(const std::string& compressed, std::string& src) override {

    }
private:

};


#endif //COMPRESSIONSERVICE_RLE_H
