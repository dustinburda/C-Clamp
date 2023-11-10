//
// Created by Dustin Burda on 11/10/23.
//

#ifndef COMPRESSIONSERVICE_COMPRESS_H
#define COMPRESSIONSERVICE_COMPRESS_H

#include <fstream>

class Compressor {
    virtual void compress(std::string& src) = 0;
    virtual void decompress(std::string& src) = 0;
};

#endif //COMPRESSIONSERVICE_COMPRESS_H
