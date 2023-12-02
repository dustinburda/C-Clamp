//
// Created by Dustin Burda on 11/10/23.
//

#ifndef COMPRESSIONSERVICE_COMPRESS_H
#define COMPRESSIONSERVICE_COMPRESS_H

#include <fstream>

using BitSet = std::vector<bool>;
using IntegerList = std::vector<int>;

template<typename I, typename O>
class Compressor {
public:
    virtual void compress(const I& src, O& compressed) = 0;
    virtual void decompress(const O& compressed, I& src) = 0;
    // virtual ~Compressor();
};

#endif //COMPRESSIONSERVICE_COMPRESS_H
