//
// Created by Dustin Burda on 11/10/23.
//

#ifndef COMPRESSIONSERVICE_HUFFMAN_H
#define COMPRESSIONSERVICE_HUFFMAN_H

#include "Compress.h"

#include <memory>


class Huffman : public Compressor {


    void compress(std::string& src) override {

    }

    void decompress(std::string& src) override {

    }


private:
    struct Node {
        using huff_node_ptr = std::shared_ptr<Node>;

        int freq;
        huff_node_ptr left;
        huff_node_ptr right;

        bool operator<(huff_node_ptr other) {
            return this->freq < other->freq;
        }
    };

    std::shared_ptr<Node> root;
};


#endif //COMPRESSIONSERVICE_HUFFMAN_H
