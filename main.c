#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

#define numOfPoiters 10
#define heapSize 10000

int main(){

    void** pointers[numOfPoiters];
	void* mem = malloc(heapSize);

    initHeap(mem, heapSize);
    int i;
    int j;
/*
    int alloc1 = mem_alloc(24);
    int alloc2 = mem_alloc(10);
    int alloc3 = mem_alloc(24);

    mem_dump();
    printf("\n");
    mem_free(alloc1);
    mem_dump();
    printf("\n");
    int alloc4 = mem_relloc(alloc3, 60);
    mem_dump();
    printf("\n");
*/
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
    mem_dump();
    printf("\n");
    for(i=0;i<100;i++){
        action = rand()%3;
        switch(action){
            case 0:
                
                randValue = rand()%numOfPoiters;
                //printf("%s  %d\n", "free", randValue);
                if (pointers[randValue] != NULL){
                    countfree++;
                    mem_free(pointers[randValue]);
                    pointers[randValue] = NULL;

                }
                break;
            case 1:
                
                randValue = rand()%numOfPoiters;
                //printf("%s %d\n", "relloc", randValue);
                if (pointers[randValue] != NULL){
                        //mem_dump();
                        //printf("\n");
                    pointer = mem_relloc(pointers[randValue], rand()%200);
                        //mem_dump();
                        //printf("\n");
                    //printf("%d\n", pointer);
                    if (pointer != NULL){
                        countrealloc++;
                        pointers[randValue] = pointer;
                        mem_fill(pointers[randValue]);
                        //mem_dump();
                        //printf("\n");
                    }
                }
                mem_fill(pointers[randValue]);
  
                break;
            case 2:
            
                randValue = rand()%numOfPoiters;
                //printf("%s %d\n", "alloc", randValue);
                if (pointers[randValue] != NULL){
                    mem_free(pointers[randValue]);
                    pointers[randValue] = mem_alloc(rand()%200);
                    mem_fill(pointers[randValue]);
                }else{
                    pointers[randValue] = mem_alloc(rand()%100);
                    mem_fill(pointers[randValue]);
                }
                if(pointers[randValue]){
                    countalloc++;
                }
                //mem_dump();
                //printf("\n");
                break;

        }
        mem_dump();
        getc(stdin);

        for (j=0; j < numOfPoiters; j++){

            if(pointers[j]!=NULL){
                if(mem_check(pointers[j])){
                    printf("%s %d\n", "Oh God why!?", j);
                    //mem_dump();
                    getc(stdin);
                } 
            }
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
 