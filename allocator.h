struct header {
    int free;
    unsigned int size;
    unsigned int predSize;
};
/*
 * Ініціалізація пам'яті, якою управлятиме алокатор
 */
void initHeap(void* addr, size_t size);
void* mem_alloc(size_t size);
void mem_free(void* addr);
void* mem_relloc(void* addr, size_t size);
void mem_dump();
/*
 * Заповнення пам'яті інтами, нульовий елемент - xor інших 	елементів
 */
void mem_fill(void* addr);
/*
 * Перевірка пам'яті на цілісність
 */
int mem_check(void* addr);