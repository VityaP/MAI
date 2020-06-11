#ifndef TALLOCATIONBLOCK_H
#define TALLOCATIONBLOCK_H
#include <cstdlib>

class TAllocationBlock {
public:
    // Конструктор блока
    TAllocationBlock(size_t size,size_t count);
    // Аллоцирование под запрос
    void *allocate();
    // Возврат памяти
    void deallocate(void *pointer);
    // Есть ли свободные блоки?
    bool has_free_blocks();
    // Деконструктор блока
    virtual ~TAllocationBlock();
private:
    // сколько выделено
    size_t _size;
    // текущий размер
    size_t _count;
    // используемые блоки
    char *_used_blocks;
    // свободные блоки
    void **_free_blocks;
    // число свободных
    size_t _free_count;
};
#endif /* TALLOCATIONBLOCK_H */