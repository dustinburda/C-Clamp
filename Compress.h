//
// Created by Dustin Burda on 11/10/23.
//

#ifndef COMPRESSIONSERVICE_COMPRESS_H
#define COMPRESSIONSERVICE_COMPRESS_H

#include <fstream>

using BitSet = std::vector<bool>;
using IntegerList = std::vector<int>;

template<typename T>
class Compressor {
    virtual void compress(const std::string& src, T& compressed) = 0;
    virtual void decompress(const T& compressed, std::string& src) = 0;
};

#endif //COMPRESSIONSERVICE_COMPRESS_H
