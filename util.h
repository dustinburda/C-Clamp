//
// Created by Dustin Burda on 11/25/23.
//

#ifndef COMPRESSIONSERVICE_UTIL_H
#define COMPRESSIONSERVICE_UTIL_H

#include <vector>
#include <cstdlib>

static void integer_to_bits(int integer, std::vector<bool>& bits, uint8_t word_size_) {
    bits.resize(word_size_);
    for(int i = word_size_ - 1; i >= 0; i--) {
        int mask = 1 << i;
        bits[word_size_ - 1 - i] = integer & mask;
    }
}

static void vinteger_to_vbit(IntegerList& v_integer, std::vector<bool>& v_bit, uint8_t word_size_) {
    for(auto& integer : v_integer) {
        std::vector<bool> bits;
        integer_to_bits(integer, bits, word_size_);

        v_bit.insert(v_bit.end(), bits.begin(), bits.end());
    }
}

static void vbit_to_vbyte(const std::vector<bool>& bits, std::vector<uint8_t>& bytes) {
    if(bits.empty())
        return;

    bytes.reserve(std::ceil(bits.size() / 8.0f));

    int padding = 8 - (bits.size() % 8);
    uint8_t byte = 0;


    // TODO: don't shift 0s
    int i = 0;


    // first byte
    if(padding < 8) {
        for(int j = 0; j < 8 - padding; j++) {
            byte |= bits[i + j] << ((8 - padding - 1) - j);
        }
        bytes.push_back(byte);
        i += 8 - padding;
    }


    while(i < bits.size()) {
        byte = 0;
        for(int j = 0; j < 8; j++) {
            byte |= bits[i + j] << (8 - 1 - j);
        }
        bytes.push_back(byte);
        i += 8;
    }
}

// length is in bits
static void vbyte_to_vbit(uint32_t length, std::vector<uint8_t>& bytes, std::vector<bool>& bits) {
    if(length == 0 || std::ceil(length / 8.0f) > bytes.size() ) {
        return;
    }

    // 00000111
    // padding is 5;
    int padding = 8 - (length % 8);
    uint8_t curr_byte;

    if(padding < 8) {
        curr_byte = bytes[0];
        for(int j = 8 - (1 + padding); j >= 0; j--) {
            auto bit = curr_byte & (1 << j);
            bits.push_back(bit);
        }
    }

    int start_index = (padding < 8) ? 1 : 0;
    for(int i = start_index; i < bytes.size(); i++) {
        curr_byte = bytes[i];
        for(int j = 8 - 1; j >= 0; j--) {
            auto bit = curr_byte & (1 << j);
            bits.push_back(bit);
        }
    }
}

#endif //COMPRESSIONSERVICE_UTIL_H
