#include <iostream>
#include "Huffman.h"

int main() {
    Huffman& single_instance = Huffman::instance();

    single_instance.compress("Hello");
    single_instance.decompress("Hello");

}
