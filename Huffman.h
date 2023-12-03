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
#include <algorithm>


#define DEBUG 0

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

    Node(int freq, uint8_t c) // Leaf Node Constructor
        : freq_{freq}, c_{c}, type_{Node_Type::Leaf}, left_{nullptr}, right_{nullptr} {}

    int freq_;
    std::optional<uint8_t> c_;
    Node_Type type_; // TODO: probably redundant

    huff_node_ptr left_;
    huff_node_ptr right_;
};


static std::function<bool(huff_node_ptr, huff_node_ptr)> compare = [](auto left, auto right) {
    return left->freq_ > right->freq_;
};
using node_pq = std::priority_queue<huff_node_ptr, std::vector<huff_node_ptr>, decltype(compare)>;

class Huffman : public Compressor<std::vector<uint8_t>, BitSet> {
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

    void compress_file(const std::string& ifilename, const std::string& ofilename) {
        std::ifstream ifile{ifilename, std::ios::binary};

        // Open File
        if(ifile.is_open()) {
            std::cout << "File: " << ifilename << " is open!\n";
        } else {
            std::cout << "Failed to open: " << ifilename << "\n";
        }

        // Get File Size
        std::size_t filesize = std::filesystem::file_size(ifilename);
        std::vector<uint8_t> buff;
        buff.resize(filesize);

        // Dump contents of file into vec<u8> buff
//        buff.insert(buff.begin(),
//                    std::istream_iterator<uint8_t>(ifile),
//                    std::istream_iterator<uint8_t>());
        ifile.read((char *)buff.data(), filesize);

        // Compress string into vector<bool>/Bits
        BitSet compressed;
        compress(buff, compressed);

        // Write Header and Compressed Bitset into File
        std::ofstream ofile{ofilename, std::iostream::binary};
        write_codes(ofile, codes_);
        write_content(ofile, compressed);
    }

    void write_uncompressed_content(std::ofstream& ofile, std::vector<uint8_t>& uncompressed_content) {
        std::for_each(uncompressed_content.begin(), uncompressed_content.end(), [&ofile](uint8_t& byte) {
            ofile << byte;
        });
    }

    void read_code(std::ifstream& ifile, std::vector<uint8_t>& code_bytes, uint8_t code_length) {
        int num_bytes = std::ceil(code_length / 8.0);
        code_bytes.reserve(num_bytes);

        for(int i = 0; i < num_bytes; i++) {
            uint8_t curr_byte;
            ifile.read((char*)&curr_byte, 1);
            code_bytes.push_back(curr_byte);
        }

    }

    void read_reverse_codes(std::ifstream& ifile) {
        uint8_t reverse_codes_size = 0;
        ifile.read((char*)&reverse_codes_size, 1);

        for(int i = 0; i < reverse_codes_size; i++) {
            char character;
            uint8_t code_length;
            ifile.read(&character, 1);
            ifile.read((char*)&code_length, 1);
            //ifile >> character >> code_length;

            std::vector<uint8_t> code_bytes;
            read_code(ifile, code_bytes, code_length);

            BitSet code_bits;
            vbyte_to_vbit(code_length, code_bytes, code_bits);

            reverse_codes_[code_bits] = static_cast<uint8_t>(character);
        }
    }

    void read_content(std::ifstream& ifile, uint32_t& content_length, std::vector<uint8_t>& compressed_content_bytes) {
        ifile.read((char*)&content_length, 4);

        int num_bytes = std::ceil(content_length / 8.0);
        compressed_content_bytes.reserve(num_bytes);

        for(int i = 0; i < num_bytes; i++) {
            uint8_t curr_byte = 0;
            ifile.read((char*)&curr_byte, 1);
            compressed_content_bytes.push_back(curr_byte);
        }
    }

    void decompress_file(const std::string& ifilename, const std::string& ofilename) {
        std::ifstream ifile{ifilename, std::ios::binary};

        // Open Compressed File
        if(ifile.is_open()) {
            std::cout << "File: " << ifilename << " is open!\n";
        } else {
            std::cout << "Failed to open: " << ifilename << "\n";
        }

        //Read Codes from ifile
        read_reverse_codes(ifile);

        //Read Length + Content
        uint32_t content_length;
        std::vector<uint8_t> compressed_content_bytes;
        read_content(ifile, content_length, compressed_content_bytes);

        // Dump compressed content into vector<uint8_t>
        std::vector<bool> compressed_content_bits;
        vbyte_to_vbit(content_length, compressed_content_bytes, compressed_content_bits);

        // Uncompressed content
        std::vector<uint8_t> uncompressed_content;
        decompress(compressed_content_bits, uncompressed_content);

        // Write uncompressed content into output file
        std::ofstream ofile{ofilename, std::iostream::binary};
        write_uncompressed_content(ofile, uncompressed_content);
    }

    void compress(const std::vector<uint8_t>& src, BitSet& compressed) override {
        std::cout << "Compressing...\n";

        build_huffman_tree(src);

        build_codes(root_, {});

        for(auto elem : src) {
            auto& code = codes_[elem];
            // TODO replace with memcpy calls
            compressed.insert(compressed.end(), code.begin(), code.end());
        }
    }

    void decompress(const BitSet& compressed, std::vector<uint8_t>& src) override {
        std::cout << "Decompressing...\n";
        // TODO: Rewrite this routine using reverse_codes_

        BitSet curr_code;

        for(auto bit : compressed) {
            curr_code.push_back(bit);

            if(reverse_codes_.count(curr_code) > 0) {
                auto character = reverse_codes_[curr_code];
                src.push_back(character);

                curr_code = {};
            }


        }

#if DEBUG
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
#endif // DEBUG
    }

    void reset() {
        root_ = nullptr;
        codes_.clear();
    }


private:
    Huffman() = default;

    void write_codes(std::ofstream& ofile, std::unordered_map<uint8_t, BitSet>& codes) {
        assert(codes.size() < 256);
        ofile << static_cast<uint8_t>(codes.size()); // write num elements in map

        for(const auto& [elem, code] : codes) {
            std::vector<uint8_t> bytes;
            uint8_t code_size = code.size();
            vbit_to_vbyte(code, bytes);
//            ofile << elem // char
//                  << static_cast<uint8_t>(code.size()); // length of bit pattern in bits
            ofile.write((char*)&elem, 1);
            ofile.write((char*)&code_size, 1);

            std::for_each(bytes.begin(), bytes.end(), [&ofile](auto byte) {
               ofile.write((char*)&byte, 1);
            });
        }
    }

    void write_content(std::ofstream& ofile, BitSet& compressed) {
        uint32_t compressed_size = static_cast<uint32_t>(compressed.size());
        ofile.write((char*)&compressed_size, sizeof(compressed_size));

        std::vector<uint8_t> bytes;
        vbit_to_vbyte(compressed, bytes);

        std::for_each(bytes.begin(), bytes.end(), [&ofile](auto byte){
            ofile.write((char*)&byte, 1);
        });
    }

    void build_histogram(const std::vector<uint8_t>& src, std::unordered_map<uint8_t, int>& histogram) {
        for(auto c : src) {
            histogram[c]++;
        }
    }

    void populate_pq(std::unordered_map<uint8_t, int>& histogram, node_pq& pq) {
        for(auto& [elem, freq] : histogram) {
            auto node = std::make_shared<Node>(freq, elem);

            pq.push(node);
        }
    }

    void build_huffman_tree(const std::vector<uint8_t>& src) {
        std::unordered_map<uint8_t, int> histogram;
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
    std::unordered_map<uint8_t, BitSet> codes_;
    std::unordered_map<BitSet, uint8_t> reverse_codes_;
};


#endif //COMPRESSIONSERVICE_HUFFMAN_H
