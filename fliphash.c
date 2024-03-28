/* Name:        Prabin Kumar Sabat
   Regd. No:    211220 (Sri Sathya Sai Institute of Higher Learning)
   Task:        C Implementation of FlipHash using xxHash family as the hashing function.
*/

#include <stdint.h>
#include <xxhash.h>
#include <stdio.h>
#include <string.h>
#define ValueSize 64
#define maxCacheSizeFliphash 640000
#define maxQueueSize 10000
   //All the Values used for generating the keys should be of the size 64 bits.

// int32_t seeding(int16_t, int16_t);
// int64_t keyGenerator(FILE*);
// void takeFixedValues(FILE*, char*);
// void checkFileSize(FILE*);
// void checkFile(const char*);
int64_t flipHashForFixedValues(int argc,const char*);
   //Declarations

typedef struct queue{
   char * pointer;
   int frontPos;
   int endPos;
}Queue;
typedef Queue* qptr;

int main(int argc, char* args[]){

   return 0;
}
int64_t flipHashForFixedValues(int argc, const char *args, qptr q){
   //FixedValues means the values used for generating the keys
   //..are given in fixed sizes.
   if(argc==1){
      #pragma omp paralle sections
      {
         #pragma omp section{
            readFixedSizeInputsStd(q);
         }
         #pragma omp section
         {
            flipHash(q);
         }
   }
}

void readFixedSizeInputsStd(qptr q){
   while(1){
      char packet = (char *)malloc(sizeof(char)*ValueSize);
      if(packet==NULL){
         perror(" Not enough Memory!! :: From flipHashForFixedValues");
         exit(1);
      }
      do{}
      while(fgets(packet,(ValueSize+1), stdin) == NULL);
      enque(packet,q); 
   }
}

void createQueue(qptr Q){
   char *Queue = (char*)malloc(sizeof(char *)*(maxQueueSize+1));
   if(Queue==NULL){
      perror(" Not Enough Storage To create the Queue!");
      exit(2);
   }
   Q->pointer=Queue;
   Q->frontPos=-1;
   Q->endPos=-1;
   return;
}

void enque(char* input, qptr q){
    q->frontPos=++q->frontPos%maxQueueSize;
    while(q->frontPos==q->endPos);
    q->pointer[q->frontPos]=input;
    if(q->endPos==-1)
        q->endPos=0;
    return;
}

char* deque(qptr q){
    if(q->endPos==-1){
        printf(" The queue is empty.\n");
        return NULL;
    }
    char * temp= q->pointer[q->endPos--];
    q->endPos=++q->endPos%maxQueueSize;
    if(q->endPos==q->frontPos){
        q->frontPos=-1;
        q->endPos=-1;
    }
    return temp;
}

void disposeQueue(qptr q){
   int count=0;
   while(q->pointer[count])
      free(q->pointer[count]);
   free(q->pointer);
   free(q);
   return;
}

// int32_t seeding(int16_t a, int16_t b){
//    return (int32_t)(a + (b << 16)); // Multiplying by 2^16 by using left shifting.
// }

// int64_t keyGenerator(FILE* pC){
//    //pC stands for pointer to Cache
//    char line[ValueSize];
//       // Buffer to store each line from the file   
//    if(fgets( line, sizeof(line), pC) != NULL ){
//       return (uint64_t)XXH3_64bits(line, ValueSize);
//          //returns the key for Consistent Hashing
//    }
//    exit(1);
// }

// void takeFixedValues(FILE* pC, char* location){
//    //excepts the values to be of a fixed size. (In this case to be
//    //.. of size ValueSize).
//    char line[ValueSize+1];
//       //+1 for the scanf to store the \0 at the end.
//    scanf("%s",line);
//    fwrite(line, sizeof(line)-1, 1, pC);
//       //write the contents of the line to the cache file.(Skips the end \0)
//    return;
// }

// void checkFileSize(FILE* pC){
//    if(ftell(pC) >= maxCacheSizeFliphash)
//       return 1;
//    else 
//       return 0;
// }

// void checkFile(const char* filename){
//    FILE* pC = fopen(filename, "r");
//    fseek(pC,0,2);
//    if(checkFileSize(pC)){
//       rename(filename,strcat(filename,"_copy"));
//          //Renames the current cache file, so that next time we start a new cache file.
//          //.. this maintains the size of the cache file(while storing twice the data :)- )
//          //.. works as making a copy of the current file and emptying it when the file is open agained next for writing.
//    }

//    fclose(pC);
//    return;
// }