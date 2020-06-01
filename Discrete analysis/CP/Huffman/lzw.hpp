#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include <bitset>
#include "bitsBuffer.hpp"
class TrieNode {

    std::map<uint8_t, TrieNode*> path;
    uint32_t code;

public:
    friend class Trie;
    TrieNode( uint32_t num):
        code(num) {
    }
    void Clear() {
        for (std::pair<uint8_t, TrieNode*> elem : path)
            delete elem.second;
        path.clear();
    }
    virtual ~TrieNode() {
        for (std::pair<uint8_t, TrieNode*> elem : path)
            delete elem.second;
    }
    
};

class Trie {
    
    TrieNode* currentNode;
    std::vector<TrieNode*> path;

public:
    
    uint32_t maxCode;//next code to give//also used in calculating of codeLen
    uint32_t codeLen;//current lenght of code in bits
    uint32_t nextIncrease;//next value of maxCode that will increase codeLen
    Trie() :
        path(0xff + 1),//255
        maxCode(1),
        nextIncrease(2),
        codeLen(1),
        currentNode(nullptr) {
        uint8_t sym = 0x0;
        for (uint32_t i = 0; i <= 0xff; ++sym, ++i) {
            //conversion from char to string
            path[sym] = new TrieNode(maxCode);
            if (maxCode == nextIncrease) {
                nextIncrease *= 2;
                ++codeLen;
            }
            ++maxCode;
        }
    }

    void Clear() {
        //root array stays in place
        maxCode = 0xff + 2;
        nextIncrease = 512;
        codeLen = 9;
        currentNode = nullptr;
        for (TrieNode* node : path){
            node->Clear();
        }
    }

    uint32_t GetCode(std::string& str) {
        TrieNode* currNode = path[(uint8_t)str[0]];
        for (int i = 1; i < str.length(); ++i)  {
            currNode = currNode->path[(uint8_t)str[i]];
        }
        return currNode->code;
    }

    TrieNode* CheckWord(uint8_t sym) {
        if (currentNode) {
            if (currentNode->path.count(sym)) {
                currentNode = currentNode->path[sym];
            } else {
                currentNode = nullptr;
            }
        } else {
            currentNode = this->path[sym];
        }
        return currentNode;
    }

    void AddWord(std::string & str) {
        TrieNode* currNode = path[(uint8_t)str[0]];
        for (int i = 1; i < str.length() - 1; ++i)  {
            currNode = currNode->path[(uint8_t)str[i]];
        }
        currNode->path[(uint8_t)str[str.length() - 1]] = new TrieNode(maxCode);
        if (maxCode == nextIncrease) {
            nextIncrease *= 2;
            ++codeLen;
        }
        ++maxCode;
    }
    virtual ~Trie() {
        for (TrieNode* node : path){
            delete node;
        }
    }

};

int32_t GetCodeLen(const uint32_t maxCode) {
    return (uint32_t)ceil(log2(maxCode));
}

void EncodeLZW(std::istream& inputData, std::ostream& outputData, uint32_t maxCodeLen) {
    //LZW ENCODING
    Trie trie;
    uint32_t exitCode = 0;
    std::string prev;
    std::string full;
    uint8_t curr;
    BitsOutputBuffer buffer(outputData);
    //write in file maxCodeLen
    buffer.AddToBuffer(maxCodeLen, 32);
    inputData.peek();//check for eof
    while (!inputData.eof()) {
        inputData.read((char*)&curr, 1);
        full += curr;
        if (trie.CheckWord(curr)) {
            prev += curr;
        } else {
            buffer.AddToBuffer(trie.GetCode(prev), trie.codeLen);
            trie.AddWord(full);
            if (trie.codeLen > maxCodeLen) {
                trie.Clear();
            }
            prev = curr;
            full = curr;
            trie.CheckWord(curr);
        }
        inputData.peek();
    }
    if (!(prev.empty())) {
        buffer.AddToBuffer(trie.GetCode(prev), trie.codeLen);
    }
    buffer.AddToBuffer(exitCode, trie.codeLen);
    buffer.Flush();
}

int DecodeLZW(std::istream& encodedData, std::ostream& decodedData) {
    BitsInputBuffer buffer(encodedData);
    uint32_t maxCodeLen = buffer.GetBits(32);
    if (!(maxCodeLen == 12 || maxCodeLen == 16)) {
        return 1;
    }
    std::vector<std::string> dict;
    dict.push_back("");
    uint8_t sym = 0x0;
    for (uint32_t i = 0; i <= 0xff;  ++sym, ++i) {
        //conversion from char to string
        dict.push_back(std::string(1, sym));
    }

    std::string currDecode;
    //get codeLen bits from buffer
    uint32_t exitCode = 0;

    uint32_t code = buffer.GetBits(GetCodeLen(dict.size()));
    std::string prev = dict[code];  //first code processes separatly
    std::string full = prev;
    while (!buffer.Empty()) {
        code = buffer.GetBits(GetCodeLen(dict.size() + 1));

        if (code == exitCode) {
            break;
        }

        if (code == dict.size()) {
            //code that is not in dictionary
            //previous decoded string + its first letter
            currDecode = prev[0];
            full += currDecode;
            dict.push_back(full);
            decodedData.write(prev.data(), prev.length());
            prev = dict.back();
            full = prev;
        } else {
            currDecode = dict[code];
            full += currDecode[0];
            dict.push_back(full);
            decodedData.write(prev.data(), prev.length());
            prev = currDecode;
            full = prev;

        }
        //one more than currently in dict is significant
        //its part of decoding
        if (GetCodeLen(dict.size() + 1) > maxCodeLen) {
            decodedData.write(prev.data(), prev.length());
            dict.resize(0xff + 2);
            prev = dict[buffer.GetBits(GetCodeLen(dict.size() + 1))];
            full = prev;
        }
    }
    decodedData.write(prev.data(), prev.length());

    return 0;
}
