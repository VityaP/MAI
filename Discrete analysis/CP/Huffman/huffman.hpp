#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <bitset>

#include "bitsBuffer.hpp"

class TreeNode {
public:
    TreeNode* left;
    TreeNode* right;
    uint16_t symbol;
    uint32_t freq;
    bool leaf;

    //ENCODING
    //no fields will be changed later
    //leafs
    TreeNode(uint16_t sym, uint32_t fr):
        left(nullptr),
        right(nullptr),
        symbol(sym),
        freq(fr),
        leaf(true)
    {}

    //internal
    TreeNode(TreeNode* leftNode, TreeNode* rightNode) :
        left(leftNode),
        right(rightNode),
        symbol(0),
        freq(leftNode->freq + rightNode->freq),
        leaf(false)
    {}

    //DECODING
    //fields with links will be changed
    //leaf
    TreeNode(uint16_t sym) :
        left(nullptr),    //empty
        right(nullptr),   //empty
        symbol(sym),
        freq(0),          //does not matter
        leaf(true)
    {}

    //internal
    TreeNode() :
        left(nullptr),   //will be init later
        right(nullptr),  //also
        symbol(0),
        freq(0),         //does not matter
        leaf(false)
    {}

    virtual ~TreeNode() {
        if (!leaf) {
            delete left;
            delete right;
        }
    }
};



void DFS(const TreeNode* node, std::vector<std::vector<bool>>& table, std::vector<bool>& path) {
    if (node->leaf) {
        table[node->symbol] = path;
        return;
    }
    path.push_back(false);
    DFS(node->left, table, path);
    path.pop_back();
    path.push_back(true);
    DFS(node->right, table, path);
    path.pop_back();
}

void GetCodes(TreeNode* root, std::vector<std::vector<bool>>& table) {
    std::vector<bool> path;
    DFS(root, table, path);
}

void DFSSerialize(const TreeNode* node, BitsOutputBuffer& buffer) {
    if (node->leaf) {
        buffer.AddToBuffer(true);
        buffer.AddToBuffer(node->symbol);
        return;
    }
    buffer.AddToBuffer(false);
    DFSSerialize(node->left, buffer);
    DFSSerialize(node->right, buffer);
}

void SerializeTree(const TreeNode* root, BitsOutputBuffer& buffer) {
    DFSSerialize(root, buffer);
}

/////////////////


TreeNode* DFSDeserialize(BitsInputBuffer& buffer) {
    if (buffer.GetBits(1)) {
        //input bit is 1
        //leaf
        return new TreeNode((uint16_t)buffer.GetBits(16));
    } else {
        //input bit is 0
        //internal node
        TreeNode* node = new TreeNode();
        node->left  = DFSDeserialize(buffer);
        node->right = DFSDeserialize(buffer);
        return node;
    }
}

TreeNode* DeserializeTree(BitsInputBuffer& buffer) {
    TreeNode* ret = DFSDeserialize(buffer);
    return ret;
}

struct Comp {
    bool operator ()(const TreeNode* a, const TreeNode* b ) {
        return a->freq > b->freq;
    }
};

void EncodeHuffman(std::istream& inputData, std::ostream& outputData) {
    //HUFFMAN CODES

    //preparing codes
    std::vector<uint32_t> freqStats(0xff + 1, 0);
    uint8_t byte;
    while (!inputData.eof()) {
        inputData.read((char*) &byte, 1);
        freqStats[byte]++;
        inputData.peek();
    }
    std::priority_queue <TreeNode*, std::vector<TreeNode*>, Comp> pq;
    uint16_t sym = 0;
    for (uint32_t i = 0; i < freqStats.size(); ++i, ++sym)  {
        if (freqStats[i])
            pq.push(new TreeNode(sym, freqStats[i]));
    }
    uint16_t exitCode = 0xff + 1;
    pq.push(new TreeNode(exitCode, 0));
    freqStats.clear();//no longer needed
    TreeNode *left, *rigth;
    while (pq.size() > 1) {
        left = pq.top();
        pq.pop();
        rigth = pq.top();
        pq.pop();
        pq.push(new TreeNode(left, rigth));
    }
    TreeNode *root = pq.top();
    pq.pop();
    std::vector<std::vector<bool>> codeTable(0xff + 2);
    //some elements will be empty, buut [] works in O(1)
    GetCodes(root, codeTable);
    codeTable.shrink_to_fit();//no more changes in codeTable

    BitsOutputBuffer outputBuffer(outputData);
    SerializeTree(root, outputBuffer);
    delete root; //tree no longer needed - there is codeTable

    //second run through file
    inputData.clear();
    inputData.seekg(0, std::ios::beg); //seekg == seek get position
    //zero is offset from position

    while (!inputData.eof()) {
        inputData.read((char*)&byte, 1);
        outputBuffer.AddToBuffer(codeTable[byte]);
        inputData.peek();//to trigger eof
    }
    outputBuffer.AddToBuffer(codeTable[exitCode]);
    outputBuffer.Flush();
    codeTable.clear();
}

///////////////////////////////////////////////////////////////////////////

void DecodeHuffman(std::istream& encodedData, std::ostream& decodedData) {

    BitsInputBuffer inputBuffer(encodedData);
    //get tree from file
    TreeNode* root = DeserializeTree(inputBuffer);
    TreeNode* currNode = root;
    uint16_t exitCode = 0xff + 1;
    while (!inputBuffer.Empty()) {
        //if (encodedData.eof()) {
        //  byte += 2;
        //}
        if (inputBuffer.GetBits(1))
            currNode = currNode->right;
        else
            currNode = currNode->left;
        if (currNode->leaf) {
            if (currNode->symbol == exitCode)
                return;
            decodedData.write((char*)&currNode->symbol, 1);
            currNode = root;
        }
    }
}
