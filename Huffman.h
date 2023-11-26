//
// Created by Dustin Burda on 11/10/23.
//

#ifndef COMPRESSIONSERVICE_HUFFMAN_H
#define COMPRESSIONSERVICE_HUFFMAN_H

#include "Compress.h"
#include "util.h"

#include <memory>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <optional>
#include <sstream>


// TODO: Create proper bitstream class instead of std::vector<Bool>
// TODO: Test

struct Node;
using huff_node_ptr = std::shared_ptr<Node>;

enum class Node_Type {
    Leaf,
    Internal
};

struct Node {
    Node() = default;
    Node(int freq, huff_node_ptr left, huff_node_ptr right) // Internal Node Constructor
        : freq_ {freq}, c_ {std::nullopt}, type_{Node_Type::Internal}, left_{left}, right_{right} {}

    Node(int freq, char c) // Leaf Node Constructor
        : freq_{freq}, c_{c}, type_{Node_Type::Leaf}, left_{nullptr}, right_{nullptr} {}

    int freq_;
    std::optional<char> c_;
    Node_Type type_; // TODO: probably redundant

    huff_node_ptr left_;
    huff_node_ptr right_;
};


static std::function<bool(huff_node_ptr, huff_node_ptr)> compare = [](auto left, auto right) {
    return left->freq_ > right->freq_;
};
using node_pq = std::priority_queue<huff_node_ptr, std::vector<huff_node_ptr>, decltype(compare)>;

class Huffman : public Compressor<BitSet> {
public:

    //Singleton
    static Huffman& get_instance() {
        static Huffman instance;
        return instance;
    }

    //Can't copy
    Huffman(const Huffman& other) = delete;
    Huffman& operator=(const Huffman& other) = delete;
    //Can't move
    Huffman(Huffman&& other) = delete;
    Huffman& operator=(const Huffman&& other) = delete;

    // ~Huffman() override {};

    void compress_file(const std::string& ifilename, std::string& ofilename) {
        std::ifstream ifile{ifilename};

        if(ifile.is_open()) {
            std::cout << "File: " << ifilename << " is open!\n";
        } else {
            std::cout << "Failed to open: " << ifilename << "\n";
        }

        std::stringstream buff;
        buff << ifile.rdbuf();

        BitSet compressed;
        compress(buff.str(), compressed);

        std::ofstream ofile{ofilename};
        write_codes(ofile, codes_);
        write_content(ofile, compressed);
    }

    void compress(const std::string& src, BitSet& compressed) override {
        std::cout << "Compressing...\n";

        build_huffman_tree(src);

        build_codes(root_, {});

        for(auto elem : src) {
            auto& code = codes_[elem];
            // TODO replace with memcpy calls
            compressed.insert(compressed.end(), code.begin(), code.end());
        }
    }

    void decompress(const BitSet& compressed, std::string& src) override {
        std::cout << "Decompressing...\n";

        auto curr = root_;

        for(auto bit : compressed) {
            curr = (bit == 0) ? curr->left_
                              : curr->right_;

            if(curr->type_ == Node_Type::Leaf) {
                auto char_elem = curr->c_.value();
                src.push_back(char_elem);

                curr = root_;
            }
        }
    }

    void reset() {
        root_ = nullptr;
        codes_.clear();
    }


private:
    Huffman() = default;

    void write_codes(std::ofstream& ofile, std::unordered_map<char, BitSet>& codes) {
        assert(codes.size() < 256);
        ofile << static_cast<uint8_t>(codes.size()); // write num elements in map

        for(const auto& [elem, code] : codes) {
            std::vector<uint8_t> bytes;
            vbit_to_vbyte(code, bytes);
            ofile << elem // char
                  << static_cast<uint8_t>(code.size()); // length of bit pattern in bits
            std::for_each(bytes.begin(), bytes.end(), [&ofile](auto byte) {
                ofile << byte;
            });
        }
    }

    void write_content(std::ofstream& ofile, BitSet& compressed) {
        ofile << static_cast<uint32_t>(compressed.size());

        std::vector<uint8_t> bytes;
        vbit_to_vbyte(compressed, bytes);

        std::for_each(bytes.begin(), bytes.end(), [&ofile](auto byte){
            ofile << byte;
        });
    }

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

    void build_huffman_tree(const std::string& src) {
        std::unordered_map<char, int> histogram;
        build_histogram(src, histogram);

        std::priority_queue<huff_node_ptr, std::vector<huff_node_ptr>, decltype(compare)> pq(compare);
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
        root_ = pq.top();
        pq.pop();
    }

    void build_codes(huff_node_ptr curr, const BitSet& code) {
        if(curr == nullptr)
            return;

        if(curr->type_ == Node_Type::Leaf) {
            auto char_elem = curr->c_.value();
            codes_[char_elem] = code;
        }


        auto code_left = code;
        code_left.push_back(0);

        auto code_right = code;
        code_right.push_back(1);

        build_codes(curr->left_, code_left);
        build_codes(curr->right_, code_right);
    }

private:
    huff_node_ptr root_;
    std::unordered_map<char, BitSet> codes_;

};


#endif //COMPRESSIONSERVICE_HUFFMAN_H
