/* Name:        Prabin Kumar Sabat
   Regd. No:    211220 (Sri Sathya Sai Institute of Higher Learning, Brindavan Campus, Bengaluru)
   Task:        C Implementation of FlipHash using xxHash family as the hashing function.
*/

#include <stdint.h>
#include "xxHash/xxh3.h" //download the xxHash repository from https://github.com/Cyan4973/xxHash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define ValueSize 64
#define maxCacheSizeFliphash 64000
#define maxQueueSize 1000
#define keySizeFliphash 64
#define cfile "cachefile.txt"
   //All the Values used for generating the keys should be of the size 64 bits.
typedef struct{
   char **pointer;
   int frontPos;
   int endPos;
}queue;
typedef queue* qptr;


typedef struct{
   int64_t count;
}resource;
typedef resource* resPtr;
   //why a struct? A: So that u can store more things if you want to in the future.


int64_t flipHashForFixedValues(int argc,const char**, qptr, resPtr);
void executeFliphash(qptr, resPtr);
void readFixedSizeInputsStd(qptr);
void createQueue(qptr);
void enque(char *, qptr);
char *deque(qptr);
void disposeQueue(qptr);
int32_t seeding(int16_t, int16_t);
int64_t fliphashPow2(char *, int64_t);
int64_t fliphashGeneral(char*, resPtr);
int64_t keyGenerator(FILE*);
int checFileSize(FILE*);
//checkFile(const char*);
   //Declarations

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, const char* args[]){
   qptr q = (qptr)malloc(sizeof(queue));
   createQueue(q);
   resPtr res = (resPtr)malloc(sizeof(resource)); //right now there is no way of increasing the value in between execution.
      //stop execution and start again to increase resources.
   printf(" Please enter the total number of resources:\n");
   scanf(" %ld", &res->count);
   getc(stdin);
   
   flipHashForFixedValues(argc, args, q, res);
   return 0;
}

int64_t flipHashForFixedValues(int argc, const char *args[], qptr q, resPtr res){
   //FixedValues means the values used for generating the keys
   //..are given in fixed sizes.
   if(argc==1)
      #pragma omp parallel sections
      {
         #pragma omp section
         {
            readFixedSizeInputsStd(q);
         }
         #pragma omp section
         {
            executeFliphash(q, res);
         }
      }
}
   //right now we are considering only to take inputs from the stdin
   //.. planning to take inputs from file whose name will be given during run time.

void executeFliphash(qptr q, resPtr res){
   char * temp;
   int64_t result;
   while(1){
      temp = deque(q);
      if(temp!=NULL){
         result = fliphashGeneral(temp,res);
         printf(" %ld",result); // you can modify this to write to a file instead of stdin.
         free(temp);
         temp=NULL;
         printf(" Freed\n");
      }
   }
}


void readFixedSizeInputsStd(qptr q){
   while(1){
      char* packet = (char *) malloc( sizeof(char) * (ValueSize+1));
      if(packet==NULL){
         perror(" Not enough Memory!! :: From flipHashForFixedValues");
         exit(1);
      }
      if(fgets(packet,(ValueSize+1), stdin) != NULL){
         enque(packet,q);
      }
   }
}

void createQueue(qptr Q){
   Q->pointer=(char**)malloc(sizeof(char *)*(maxQueueSize+1));
   if(Q->pointer==NULL){
      perror(" Not Enough Storage To create the Queue!");
      exit(2);
   }
   Q->frontPos=0;
   Q->endPos=0;
   return;
}

void enque(char* input, qptr q){
   pthread_mutex_lock(&queue_mutex);
   q->frontPos=++q->frontPos%maxQueueSize;
   while(q->frontPos==q->endPos);
   q->pointer[q->frontPos]=input;
   pthread_mutex_unlock(&queue_mutex);
   
   return;
}

char* deque(qptr q){
   if(q->endPos==q->frontPos){
      return NULL;
   }
   pthread_mutex_lock(&queue_mutex);

   char * temp = q->pointer[++q->endPos];
   q->endPos =(q->endPos)%maxQueueSize;
   printf(" frontPos:%d, endPos:%d", q->frontPos, q->endPos);

   pthread_mutex_unlock(&queue_mutex);

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

int32_t seeding(int16_t a, int16_t b){
   return (int32_t)(a + (b << 16)); // Multiplying by 2^16 by using left shifting.
}

int64_t fliphashPow2(char * key, int64_t twoPower){ //number of resources = 2^n
   int64_t a = (XXH3_64bits_withSeed(key, keySizeFliphash, seeding(0,0))) & ((1ULL << twoPower) - 1);
   int64_t temp = a;
   int16_t b = 0;
   for(;temp>1;temp=(temp>>1))
      b++;
   int64_t c = (XXH3_64bits_withSeed(key, keySizeFliphash, seeding(b,0))) & ((1ULL << b) - 1);
   return (int64_t)(a+c);
}

int64_t fliphashGeneral(char * key, resPtr resource){
   int64_t numberOfResources = (int64_t)resource->count;
   int64_t r = 0;
   for(; numberOfResources > 0 ; numberOfResources = (numberOfResources >> 1))
      r++;
   int64_t d = fliphashPow2(key,  r); //first hash result
   if(d < numberOfResources){
      return d;
   }
   else{
      int64_t e;
      int64_t rNegative1 = 1ULL << (r-1); //count of r-1 resources
      for(int i=0; i<64; i++){
         e = (XXH3_64bits_withSeed(key, keySizeFliphash, seeding(r-1,i))) & ((1ULL << r) - 1);
         if(e < rNegative1){
            return fliphashPow2(key, (r-1) );
         }
         else if( e < numberOfResources ){
            return e;
         }
      }
      return fliphashPow2(key, (r-1) );
      } 
}

int64_t keyGenerator(FILE* pC){
   //pC stands for pointer to Cache
   char line[ValueSize];
      // Buffer to store each line from the file   
   if(fgets( line, sizeof(line), pC) != NULL ){
      return (uint64_t)XXH3_64bits(line, ValueSize);
         //returns the key for Consistent Hashing
   }
   exit(1);
}

int checkFileSize(FILE* pC){
   if(ftell(pC) >= maxCacheSizeFliphash)
      return 1;
   else 
      return 0;
}

void checkFile(char* filename){
   FILE* pC = fopen(filename, "r");
   fseek(pC,0,2);
   if(checkFileSize(pC)){
      rename(filename,strcat(filename,"_copy"));
         //Renames the current cache file, so that next time we start a new cache file.
         //.. this maintains the size of the cache file(while storing twice the data :)- )
         //.. works as making a copy of the current file and emptying it when the file is open agained next for writing.
   }

   fclose(pC);
   return;
}