#include <stdlib.h>
#include <stdio.h>
#include "allocator.h"

#define head struct header*


static size_t heapSize;
static void* heap;
static int memFrame = 4;
static int headerSize = sizeof (struct header);

void initHeap(void* addr, size_t size) {

    heap = addr;
    if(size % memFrame != 0){
       heapSize = size - size % memFrame ; 
    }else{
       heapSize = size;
    }

    struct header firstHeader = {0, 0, 0};
    struct header midHeader = {0x1, heapSize - headerSize * 3, 0};
    struct header lastHeader = {0, 0, heapSize - headerSize * 3};
    *((head) heap) = firstHeader;
    *((head) (heap+headerSize)) = midHeader;
    *((head) (heap+heapSize-headerSize)) = lastHeader;
}

void* mem_alloc(size_t size) {
    void* addr = NULL;
    if (size % memFrame != 0) {
        size = size - (size % memFrame) + memFrame;
    }

    size_t position = 0;
    head curFrame;

    while ((position < heapSize - headerSize) && addr == NULL) {
        curFrame = ((head) (heap + position));

        if (curFrame->free && curFrame->size >= size) {
            
            addr = heap + position;
            curFrame->free = 0;

            int oldSize = ((head) addr)->size;
            ((head) addr)->size = size;
            int newSize = oldSize - size - headerSize;

            if (newSize > memFrame) {
                void* pos = heap + position + headerSize + size;
                ((head) pos) -> free = 1;
                ((head) pos) -> predSize = size;
                ((head) pos) -> size = newSize;
                ((head) (pos+headerSize+newSize))->predSize = newSize;
            }else{
                ((head) addr)->size = oldSize;
            }
        } else {
            position += headerSize + curFrame->size; 
        }
    }
    if (addr !=NULL){
        return addr+headerSize; 
    }else{
        return NULL;
    }
    
}

void mem_free(void* addr) {
    
  
    addr -= headerSize;

    if (addr != NULL){
        if (!((head) addr)->free) {

            ((head) addr)->free = 1;
            void* rightHeader = addr + headerSize + ((head) addr)->size;

            do {
                if (((head) rightHeader)->free) {
                    int newSize =((head) addr)->size + headerSize + ((head) rightHeader)->size;
                    ((head) addr)->size = newSize;
                    ((head) (addr+newSize+headerSize))->predSize = newSize;
                }
                rightHeader = addr;
                addr -= ((head) addr)->predSize + headerSize;
            } while (((head) addr)->free);
        }
    }
    addr = NULL;
    
}


void* mem_relloc(void* addr, size_t size){
    addr -= headerSize;

    if (addr != NULL){
        int i;
        void* result = NULL;
        if (size % memFrame != 0) {
            size = size - (size % memFrame) + memFrame;
        }

        void* rightHeader = addr + headerSize + ((head) addr)->size;
        void* leftHeader = addr - ((head) addr)->predSize - headerSize;
        int maxSize = 0;
        int newSize = 0 ;
        
        maxSize = ((head)(rightHeader))->size + ((head)(addr))->size + headerSize;
        if((((head)(rightHeader))->free && size <= maxSize) ||
                ((!((head)(rightHeader))->free) && size<((head) addr)->size) ){   //розширення вправо
            
            if((!((head)(rightHeader))->free) && size<((head) addr)->size){
                maxSize = ((head)(addr))->size;
                newSize = maxSize - size - headerSize;
                if (newSize >= 0){
                    ((head)(addr))->size = size;
                    
                    ((head)(addr+headerSize+size))->free = 1;
                    ((head)(addr+headerSize+size))->size = newSize;
                    ((head)(addr+headerSize+size))->predSize = size;
                    
                    ((head)(addr+headerSize+size+headerSize+newSize))->predSize = newSize;
                    
                }//else нічого змінювати і не потрібно
                
            }else{
                
                newSize = maxSize - size - headerSize;  
                if (newSize >= 0){
                    ((head)(addr+size+headerSize))->free = 1;
                    ((head)(addr+size+headerSize))->size = newSize;
                    ((head)(addr+size+headerSize))->predSize = size;

                    ((head)(addr+headerSize+size+headerSize+newSize))->predSize = newSize;

                    ((head)(addr))->size = size;
                }else{
                    ((head)(addr))->size = maxSize;
                    ((head)(addr+headerSize+maxSize))->predSize = maxSize;
                }
                return addr + headerSize;
            }
        }
        maxSize = ((head)(leftHeader))->size + ((head)(addr))->size + headerSize; 
        if(((head)(leftHeader))->free && size <= maxSize){   //зсув вліво

            newSize = maxSize - size - headerSize; 
            int length = ((head)(addr))->size/sizeof(int);
            for(i=0; i<length; i++){
                ((int*)(leftHeader+headerSize))[i]=((int*)(addr+headerSize))[i];
            }
            ((head)leftHeader)->free = 0;

            if (newSize >= 0){
                ((head)(leftHeader+headerSize+size))->free = 1;
                ((head)(leftHeader+headerSize+size))->size = newSize;
                ((head)(leftHeader+headerSize+size))->predSize = size;
                
                ((head)(leftHeader+headerSize+size+headerSize+newSize))->predSize = newSize;

                ((head)leftHeader)->size = size;
            }else{
                ((head)(leftHeader+headerSize+maxSize))->predSize = maxSize;
                ((head)leftHeader)->size = maxSize;
            }
            return leftHeader +headerSize;

        }else{   // пошук вільного кадру

            result = mem_alloc(size);
            
            if (!result){
                return NULL;
            }else{
                result -= headerSize;
                int length;

                if (size < ((head)addr)->size){
                    length = size/sizeof(int);
                }else{
                    length = (((head)addr)-> size)/sizeof(int);
                }
                for (i=0; i<length;i++){
                    ((int*)(result+headerSize))[i] = ((int*)(addr+headerSize))[i];
                }
                addr += headerSize;
                mem_free(addr);
            }
        }

        
        return result + headerSize;
    }
    
    return NULL;
}

void mem_dump() {
    int pos = 0;
    int i = 0;
    while (pos < heapSize) {
        char state = 'F';

        if (!(((head) (pos + heap))->free)) {
            state = 'B';
        }

        printf("@%d N%d %c %d %d \n", pos, i, state, ((head) (pos + heap))->size,((head)(pos + heap))->predSize);
        i++;
        pos += headerSize + ((head) (pos + heap))->size;
    }
}

void mem_fill(void* addr){
    
    if (addr == NULL){
        return;
    }
    int SUM = 0;
    int i;
    int next;
    int length = ((head)(addr-headerSize))->size / sizeof(int);

    for(i=1;i<length;i++){

        next = rand();
        SUM ^= next;
        ((int* )addr)[i] = next;
    }
    ((int* )addr)[0] = SUM;
}

int mem_check(void* addr){
    if (addr ==NULL){
        return 0;
    }

    int SUM = ((int* )addr)[0];
    int i;

    int length = ((head)(addr-headerSize))->size / sizeof(int);


    for(i=1;i<length;i++){
        SUM ^= ((int* )addr)[i];
    }
    return SUM;
}
