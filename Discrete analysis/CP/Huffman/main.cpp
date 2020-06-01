#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <experimental/filesystem>
#include <algorithm>
#include <iterator>

#include "lzw.hpp"
#include "huffman.hpp"

//namespace fs = std::experimental::filesystem;


int Compress(std::string& filename, bool fromStdin, bool toStdout, bool keepFiles, bool fastest) {
    std::ifstream inputData;
    std::ofstream outputTmp;
    std::ifstream inputTmp;
    std::ofstream outputData;

    uint32_t maxCodeLen;
    if (fastest) {
        maxCodeLen = 16;
    } else {
        maxCodeLen = 12;
    }
    std::string tmpname = filename + "~.tmpPetrosyan";
    outputTmp.open(tmpname, std::ofstream::out | std::ofstream::binary);
    if (!(outputTmp.is_open() && outputTmp.good())) {
        std::cerr << "Error while creating temporary file" << std::endl;
        outputTmp.close();
        return 1;
    }

    if (fromStdin) {
        std::istream& inputStd = std::cin;
        EncodeLZW(inputStd, outputTmp, maxCodeLen);
        outputTmp.close();
    } else {
        inputData.open(filename, std::ifstream::in | std::ifstream::binary);
        if (!(inputData.is_open())) {
            std::cerr << "Error while opening file " << filename << std::endl;
            inputData.close();
            return 1;
        }
        EncodeLZW(inputData, outputTmp, maxCodeLen);
        outputTmp.close();
        inputData.close();
    }

    inputTmp.open(tmpname, std::ifstream::in | std::ifstream::binary);
    if (!(inputTmp.is_open() )) {
        std::cerr << "Error while opening temporary file" << std::endl;
        inputTmp.close();
        return 1;
    }
    if (toStdout) {
        std::ostream& outputStd = std::cout;
        uint8_t byte = 0xee;
        outputStd.write((char*)&byte, 1);
        EncodeHuffman(inputTmp, outputStd);
        inputTmp.close();
    } else {
        outputData.open(filename + ".Petros", std::ofstream::out | std::ofstream::binary);
        if (!(outputData.is_open() && outputData.good())) {
            std::cerr << "Error while creating archive file" << std::endl;
            outputData.close();
            return 1;
        }
        uint8_t specialByte = 0xee;
        outputData.write((char*)&specialByte, 1);

        EncodeHuffman(inputTmp, outputData);

        //write down uncompressed size
        outputData.write((char*)&specialByte, 1);
        uint64_t uncompressedSize = std::experimental::filesystem::file_size(filename);
        outputData.write((char*)&uncompressedSize, 8);

        inputTmp.close();
        outputData.close();
    }
    if (remove(tmpname.data())) {
        std::cerr << "Error while removing temporary file" << std::endl;
        return 1;
    }
    if (!keepFiles && !fromStdin) {
        if (remove(filename.data())) {
            std::cerr << "Error while removing file " << filename << std::endl;
            return 1;
        }
    }

    return 0;
}
int Decompress(std::string& filename, bool fromStdin, bool toStdout, bool keepFiles) {
    std::ifstream inputData;
    std::ofstream outputTmp;
    std::ifstream inputTmp;
    std::ofstream outputData;
    uint32_t ret = 0;

    std::string tmpname = filename + "~.tmpPetrosyan";
    std::string unpackedname = "";
    outputTmp.open(tmpname, std::ofstream::out | std::ofstream::binary);
    if (!(outputTmp.is_open() && outputTmp.good())) {
        std::cerr << "Error while creating temporary file" << std::endl;
        outputTmp.close();
        return 1;
    }

    if (fromStdin) {
        std::istream& inputStd = std::cin;
        uint8_t specialByte = 0xee;
        uint8_t byte;
        inputData.read((char*)&byte, 1);
        if (byte == specialByte) {
            DecodeHuffman(inputStd, outputTmp);
        }
        else {
            ret = 1;
        }
        outputTmp.close();

    } else {
        unpackedname = filename.substr(0, filename.length() - 2);
        if (filename.substr(filename.length() - 2, filename.length() - 1) != ".Petros") {
            std::cerr << "File does not have extension .Petros :" << filename << std::endl;
            return 1;
        }
        inputData.open(filename, std::ifstream::in | std::ifstream::binary);
        if (!(inputData.is_open())) {
            std::cerr << "Error while opening archive file " << filename << std::endl;
            inputData.close();
            return 1;
        }

        uint8_t specialByte = 0xee;
        uint8_t byte;
        inputData.clear();
        inputData.read((char*)&byte, 1);
        if (byte == specialByte) {
            DecodeHuffman(inputData, outputTmp);
        }
        else {
            ret = 1;
        }
        inputData.close();
        outputTmp.close();

    }
    if (ret) {
        return ret;
    }
    inputTmp.open(tmpname, std::ifstream::in | std::ifstream::binary);
    if (!(inputTmp.is_open() )) {
        std::cerr << "Error while opening temporary file" << std::endl;
        inputTmp.close();
        return 1;
    }
    if (toStdout) {
        std::ostream& outputStd = std::cout;
        ret = DecodeLZW(inputTmp, outputStd);
        inputTmp.close();
    } else {
        outputData.open(unpackedname , std::ofstream::out | std::ofstream::binary);
        if (!(outputData.is_open() && outputData.good())) {
            std::cerr << "Error while creating file: " << unpackedname << std::endl;
            outputData.close();
            return 1;
        }
        ret = DecodeLZW(inputTmp, outputData);
        outputData.close();
        inputTmp.close();
    }

    if (remove(tmpname.data())) {
        std::cerr << "Error while removing temporary file" << std::endl;
        return 1;
    }

    if (!keepFiles && !fromStdin) {
        if (remove(filename.data())) {
            std::cerr << "Error while removing file " << filename << std::endl;
            return 1;
        }
    }

    if (ret) {
        std::cerr << "Wrong archive structure:" << std::endl;
    }
    return ret;
}
void checkIntegrity(std::string& filename) {
    //so there is special byte 0xee in the beggining
    //next goes bytes is serialized huffman tree
    //basically any input data can be interpreterted as tree
    std::ifstream inputData;
    if (filename.substr(filename.length() - 2, filename.length() - 1) != ".Petros") {
        std::cerr << filename << " is not a .Petros archive" << std::endl;
        return;
    }
    inputData.open(filename, std::ifstream::in | std::ifstream::binary);
    uint8_t byte;
    bool corruption = false;
    inputData.read((char*)&byte, 1);
    if (byte != 0xee)
        corruption = true;
    else {
        inputData.seekg(-9, inputData.end);
        inputData.read((char*)&byte, 1);
        if (byte != 0xee)
            corruption = true;
    }

    inputData.close();
    if (corruption)
        std::cout << "Archive file " << filename << " corrupted" << std::endl;
    else
        std::cout << "Archive file " << filename << " is consistent" << std::endl;
    //archive also should end with 0xee and uncomressed size
}
void getInfo(std::string& filename) {

    uint64_t uncompressedSize = 0, compressedSize;
    std::string unpackedname = filename.substr(0, filename.length() - 2);
    if (filename.substr(filename.length() - 2, filename.length() - 1) != ".Petros") {
        std::cerr << filename << " is not a .Petros archive" << std::endl;
    }
    else {
        std::cout << filename << ":" << std::endl;
        compressedSize = std::experimental::filesystem::file_size(filename);
        std::cout << "\tcompressed " << compressedSize << std::endl;

        std::ifstream inputData;
        inputData.open(filename, std::ifstream::in | std::ifstream::binary);
        if (!(inputData.is_open())) {
            std::cerr << "Error while opening file " << filename << std::endl;
            inputData.close();
        }

        inputData.seekg(-8, inputData.end);
        inputData.read((char*)&uncompressedSize, 8);
        inputData.close();
        std::cout << "\tuncompressed " << uncompressedSize << std::endl;
        std::cout << "\tuncompressed_name " << unpackedname << std::endl;
    }
}
int main(int argc, char const * argv[]) {
    bool flagWriteToStdout = false;
    bool flagDecompress = false;
    bool flagKeepFiles = false;
    bool flagListProperties = false;
    bool flagRecursive = false;
    bool flagTestIntegrity = false;

    bool flagFastest = true; //default -9

    bool flagCompress = false;
    bool flagReadFromStdin = false;
    std::string filename;
    for (int i = 1; i <= argc - 1; ++i)  {//for (int i = 1; i <= argc - 1; ++i)  {
        std::string arg = argv[i];
        if (arg == "-c") {
            flagWriteToStdout = true;
        } else if (arg == "-d") {
            flagDecompress = true;
        } else if (arg == "-k") {
            flagKeepFiles = true;
        } else if (arg == "-l") {
            flagListProperties = true;
        } else if (arg == "-r") {
            flagRecursive = true;
        } else if (arg == "-t") {
            flagTestIntegrity = true;
        } else if (arg == "-1") {
            flagFastest = true;
        } else if (arg == "-9") {
            flagFastest = false;
        }
        else{
            filename = arg;
        }
    }
    //std::string filename = argv[argc - 1];
    if ( filename == "-" ) {
        flagWriteToStdout = true;
        flagReadFromStdin = true;
    }

    if (!flagTestIntegrity && !flagDecompress && !flagListProperties)
        flagCompress = true;

    if (flagRecursive) {
        std::experimental::filesystem::recursive_directory_iterator dir(filename), end;
        std::vector<std::string> files;
        while (dir != end) {
            if (std::experimental::filesystem::is_regular_file(dir->path())) {
                files.push_back(dir->path());
            }
            ++dir;
        }

        if (flagCompress) {
            for (std::string& file : files)
                if (Compress(file, false, false, flagKeepFiles, flagFastest))
                    return 1;
        }
        else if (flagDecompress) {
            for (std::string& file : files)
                if (Decompress(file, false, false, flagKeepFiles))
                    return 1;
        } else if (flagListProperties && !flagReadFromStdin) {
            getInfo(filename);
        } else if (flagTestIntegrity) {
            checkIntegrity(filename);
        }
        return 0;
    }
    if (flagCompress) {
        return Compress(filename, flagReadFromStdin, flagWriteToStdout, flagKeepFiles , flagFastest);
    } else if (flagDecompress) {
        return Decompress(filename, flagReadFromStdin, flagWriteToStdout, flagKeepFiles);

    } else if (flagListProperties && !flagReadFromStdin) {
        getInfo(filename);
    } else if (flagTestIntegrity) {
        checkIntegrity(filename);
    }
    return 0;
}