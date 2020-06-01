#ifndef BITSBUFFER_HPP
#define BITSBUFFER_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

class BitsInputBuffer {

    std::istream& file;
    uint64_t buffer = 0;
    uint32_t bufferCount = 0;
    void ReadBits(uint32_t len) {
        uint8_t byte = 0;
        while (bufferCount < len) {
            byte = 0;
            file.read((char*)&byte, 1);
            buffer = (buffer << 8) | (uint64_t)(byte);
            bufferCount += 8;
            file.peek();//to trigger eof
            if (file.eof()) {
                return;
            }
        }
    }

public:
    BitsInputBuffer(std::istream& inputFile):
        file(inputFile)
    {}

    void Flush() {
        buffer = 0;
        bufferCount = 0;
    }
    bool Empty() {
        file.peek();
        return (bufferCount == 0) && file.eof();
    }

    uint32_t GetBits(uint32_t len) {
        uint32_t symbol = 0;

        if (bufferCount < len){
            ReadBits(len);
        }
        if (bufferCount < len) {
            //input data ended
            //return what have
            uint32_t a = buffer;
            file.peek();
            buffer = 0;
            bufferCount = 0;
            return a;
        }
        //get len bits from begin of bufferCount
        bufferCount -= len;
        symbol = (uint32_t)(buffer >> bufferCount);
        //delete first len bits stored
        if (bufferCount == 0) {
            //shifts for full size do not work
            //left shift count >= width of type [-Wshift-count-overflow]
            symbol = buffer;
            buffer = 0;
        } else {
            buffer = buffer << (64 - bufferCount);
            buffer = buffer >> (64 - bufferCount);
        }
        return symbol;
    }

};

class BitsOutputBuffer {

    std::ostream& file;
    uint64_t buffer = 0;
    uint32_t bufferCount = 0;
    void WriteBits() {
        uint8_t byte = 0;
        while (bufferCount >= 8) {
            //8 bits from begin of buffer
            bufferCount -= 8;
            byte = (uint8_t) (buffer >> (bufferCount));
            //delete first 8 bits stored
            if (bufferCount == 0) {
                //shifts for full size do not work
                //left shift count >= width of type [-Wshift-count-overflow]
                buffer = 0;
            }
            else {
                buffer = buffer << (64 - bufferCount);
                buffer = buffer >> (64 - bufferCount);
            }
            file.write((char*)&byte, 1);
        }
    }

public:
    BitsOutputBuffer(std::ostream& outputFile): file(outputFile) {}

    //for LZW special
    void AddToBuffer(uint32_t code, uint32_t len) {
        buffer = (buffer << len) | (uint64_t) code;
        bufferCount += len;
        if (bufferCount >= 24)
            WriteBits();
    }

    void AddToBuffer(std::vector<bool>& code) {
        for (bool k : code) {
            //put in code
            if (k)
                buffer = (buffer << 1) | 1;
            else
                buffer = (buffer << 1);
            ++bufferCount;
            if (bufferCount >= 32)
                WriteBits();
        }
    }

    void AddToBuffer(uint8_t code) {
        buffer = (buffer << 8) | (uint64_t) code;
        bufferCount += 8;
        WriteBits();
    }

    void AddToBuffer(uint16_t code) {
        buffer = (buffer << 16) | (uint64_t) code;
        bufferCount += 16;
        WriteBits();
    }

    void AddToBuffer(bool code) {
        if (code)
            buffer = (buffer << 1) | 1;
        else
            buffer = (buffer << 1);

        ++bufferCount;
        if (bufferCount >= 24) {
            WriteBits();
        }
    }

    void Flush() {
        WriteBits();
        uint8_t byte = 0;
        //fill with 0 empty space
        if (bufferCount == 0)
            //no incomplete bytes
            //so no filler needed
            return;
        byte = (uint8_t) (buffer << (8 - bufferCount));
        file.write((char*) &byte, 1);
        buffer = 0;
        bufferCount = 0;
    }

    virtual ~BitsOutputBuffer() {}

};

#endif