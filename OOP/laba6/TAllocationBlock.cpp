#include "TAllocationBlock.h"
#include <iostream>


TAllocationBlock::TAllocationBlock(size_t size, size_t count) : 
_size(size),_count(count) {
    _used_blocks = (char*)malloc(_size*_count);
    _free_blocks = (void**)malloc(sizeof(void*) * _count);
    
    for(size_t i=0;i<_count;i++) _free_blocks[i] = _used_blocks+i*_size;
    _free_count = _count;
    std::cout << "TAllocationBlock: Memory init" << std::endl;
}

void *TAllocationBlock::allocate() {
    void *result = nullptr;

    if(_free_count > 0) { 
        result = _free_blocks[_free_count-1];
        _free_count--;
        std::cout << "TAllocationBlock: Allocate " << (_count-_free_count) <<
        " of " << _count << std::endl;
    } else {
        std::cout << "TAllocationBlock: No memory exception kek" << std::endl;
    }
    return result;
}

void TAllocationBlock::deallocate(void *pointer) {
    std::cout << "TAllocationBlock: Deallocate block "<< std::endl;
    _free_blocks[_free_count] = pointer;
    _free_count ++;
}

bool TAllocationBlock::has_free_blocks() {
    return _free_count>0;
}

TAllocationBlock::~TAllocationBlock() {
    if(_free_count<_count) {
        std::cout << "TAllocationBlock: Memory leak?" << std::endl;
    } else {
        std::cout << "TAllocationBlock: Memory freed" << std::endl;
    }
    free(_free_blocks);
    free(_used_blocks);
}