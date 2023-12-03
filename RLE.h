//
// Created by Dustin Burda on 11/14/23.
//

#ifndef COMPRESSIONSERVICE_RLE_H
#define COMPRESSIONSERVICE_RLE_H

#include "Compress.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>

// TODO, use bitstream class, and compress binary runs


class RLE : public Compressor<std::string, std::string> {
public:
    // Singleton
    static RLE& get_instance() {
        static RLE instance;
        return instance;
    }

    // Can't move
    RLE(const RLE& other) = delete;
    RLE& operator=(const RLE& other) = delete;
    // Can't delete
    RLE(const RLE&& other) = delete;
    RLE& operator=(const RLE&& other) = delete;

    // ~RLE() override { }

    void apply_function_to_file(const std::string& ifilename, const std::string& ofilename, std::function<void(const std::string&, std::string&)> func) {
        std::ifstream ifile { ifilename };

        std::stringstream ss;
        ss << ifile.rdbuf(); // possibly bad

        std::string file_content;
        func(ss.str(), file_content);

        std::ofstream ofile { ofilename };
        ofile << file_content; // possibly bad
    }

    void compress(const std::string& src, std::string& compressed) override {
        std::cout << "Compressing... \n";

        std::optional<char> curr = std::nullopt;
        int count = 0;

        for(auto elem : src) {
            if(elem != curr) {
                if(curr != std::nullopt) {
                    write_char_quantity(compressed, curr, count);
                }
                    curr = elem;
                    count = 1;
            }
            else
                count++;
        }
        write_char_quantity(compressed, curr, count);
    }

    // https://stackoverflow.com/questions/7582546/using-generic-stdfunction-objects-with-member-functions-in-one-class
    void compress_file(const std::string& ifilename, const std::string& ofilename) {
        std::function<void(const std::string&, std::string&)> compress_func = std::bind(&RLE::compress, this, std::placeholders::_1, std::placeholders::_2);
        apply_function_to_file(ifilename, ofilename, compress_func);
    }


    void decompress(const std::string& compressed, std::string& src) override {
        std::cout << "Decompressing... \n";

        int pos { 0 };

        while(pos < compressed.size()) {
            char elem = parse_char(compressed, pos);
            int quantity = parse_quantity(compressed, pos);

            src += std::string(quantity, elem);
        }

    }

    void decompress_file(const std::string& ifilename, const std::string& ofilename) {
        std::function<void(const std::string&, std::string&)> decompress_func = std::bind(&RLE::decompress, this, std::placeholders::_1, std::placeholders::_2);
        apply_function_to_file(ifilename, ofilename, decompress_func);
    }

private:

    void write_char_quantity(std::string& compressed, std::optional<char> curr, int count) {
        compressed += count >= 2 ? std::string(2, curr.value())
                                 : std::string(1, curr.value());
        if(count >= 2) compressed += std::to_string(count);
    }

    char parse_char(const std::string& src, int& pos) {
        char ret = src[pos];
        if(pos + 1 < src.size() && src[pos] == src[pos + 1]) {
            pos++;
        }
        pos++;
        return ret;
    }

    int parse_quantity(const std::string& src, int& pos) {
        size_t length = src.size();

        int quantity = 0;
        while(pos < length && std::isdigit(src[pos])) {
            quantity = quantity * 10 + (src[pos] - '0');
            pos++;
        }

        return quantity == 0 ? 1 : quantity;
    }

    RLE() = default;

};


#endif //COMPRESSIONSERVICE_RLE_H
