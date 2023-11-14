//
// Created by Dustin Burda on 11/10/23.
//

#ifndef COMPRESSIONSERVICE_HUFFMAN_H
#define COMPRESSIONSERVICE_HUFFMAN_H

#include "Compress.h"

#include <memory>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <optional>


struct Node;
using huff_node_ptr = std::shared_ptr<Node>;
using BitSet = std::vector<bool>;
using node_pq = std::priority_queue<huff_node_ptr, std::vector<huff_node_ptr>>;

enum class Node_Type {
    Leaf,
    Internal
};

struct Node {
    Node() = default;
    Node(int freq, huff_node_ptr left, huff_node_ptr right)
        : freq_ {freq}, c_ {std::nullopt}, type_{Node_Type::Internal}, left_{left}, right_{right} {}

    Node(int freq, char c) : freq_{freq}, c_{c}, type_{Node_Type::Leaf}, left_{nullptr}, right_{nullptr} {}

    int freq_;
    std::optional<char> c_;
    Node_Type type_;
    huff_node_ptr left_;
    huff_node_ptr right_;

    bool operator<(huff_node_ptr other) const {
        return this->freq_ < other->freq_;
    }
};


class Huffman : public Compressor {
public:

    //Singleton
    static Huffman& instance() {
        static Huffman instance;
        return instance;
    }

    //Can't copy
    Huffman(const Huffman& other) = delete;
    Huffman& operator=(const Huffman& other) = delete;
    //Can't move
    Huffman(Huffman&& other) = delete;
    Huffman& operator=(const Huffman&& other) = delete;


    void compress(const std::string& src) override {
        std::cout << "Compressing...\n";
    }

    void decompress(const std::string& src) override {
        std::cout << "Decompressing...\n";
    }


private:
    Huffman() = default;

    void build_histogram(const std::string& src, std::unordered_map<char, int>& histogram) {
        for(auto c : src) {
            histogram[c]++;
        }
    }

    void populate_pq(std::unordered_map<char, int>& histogram, node_pq& pq) {
        for(auto& [elem, freq] : histogram) {
            auto node = std::make_shared<Node>(freq, elem);

            pq.push(node);
        }
    }

    void build_huffman_tree(std::string& src) {
        std::unordered_map<char, int> histogram;
        build_histogram(src, histogram);

        std::priority_queue<huff_node_ptr, std::vector<huff_node_ptr>> pq;
        populate_pq(histogram, pq);

        while(pq.size() > 1) {
            auto left = pq.top();
            pq.pop();

            auto right = pq.top();
            pq.pop();

            auto internal_node = std::make_shared<Node>(left->freq_ + right->freq_, left, right);
            pq.push(internal_node);
        }

        assert(pq.size() == 1);
        root = pq.top();
        pq.pop();
    }

    huff_node_ptr root;
    std::unordered_map<char, BitSet> codes_;

};


#endif //COMPRESSIONSERVICE_HUFFMAN_H
