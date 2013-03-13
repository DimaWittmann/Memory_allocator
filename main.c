#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

#define numOfPoiters 100
#define heapSize 10000000

int main(){

    void** pointers[numOfPoiters];
	void* mem = malloc(heapSize);

    initHeap(mem, heapSize);
    int i;
    int j;

    int size;
    int action;
    int randValue;
    void* pointer;
    int countfree = 0;
    int countalloc = 0;
    int countrealloc = 0;

    for(i=0;i<numOfPoiters;i++){
        countalloc++;
        pointers[i] = mem_alloc(rand()%100);
        mem_fill(pointers[i]);
    }

    for(i=0;i<1000;i++){
        action = rand()%3;
        switch(action){
            case 0:           
                randValue = rand()%numOfPoiters;
                if (pointers[randValue] != NULL){
                    countfree++;
                    mem_free(pointers[randValue]);
                    pointers[randValue] = NULL;
                }
                break;
            case 1:            
                randValue = rand()%numOfPoiters;

                if (pointers[randValue] != NULL){

                    pointer = mem_relloc(pointers[randValue], rand()%200);

                    if (pointer != NULL){
                        countrealloc++;
                        pointers[randValue] = pointer;                       
                        mem_fill(pointers[randValue]);
                    }
                }
                mem_fill(pointers[randValue]);
                break;
            case 2:
            
                randValue = rand()%numOfPoiters;

                if (pointers[randValue] != NULL){
                    mem_free(pointers[randValue]);
                    pointers[randValue] = mem_alloc(rand()%200);
                    mem_fill(pointers[randValue]);
                }else{
                    pointers[randValue] = mem_alloc(rand()%100);
                    if (pointers[randValue] != NULL){
                        mem_fill(pointers[randValue]);
                    }
                }
                if(pointers[randValue]){
                    countalloc++;
                }
                break;
        }
    }    
    
    printf("free %d alloc %d relloc %d",countfree,countalloc,countrealloc);

/*
    FILE *test_file;
	test_file = fopen("test.bin", "wb");

    for(i=0; i<size; i++){
        unsigned char x = *(char*)(mem+ i);
        putc(x, test_file);
    } 
    
    fclose(test_file);
  */  
    return 0;
}

