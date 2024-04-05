/* Name:        Prabin Kumar Sabat
   Regd. No:    211220 (Sri Sathya Sai Institute of Higher Learning)
   Task:        C Implementation of FlipHash using xxHash family as the hashing function.
*/

#include <stdint.h>
#include "xxHash/xxh3.h" //download the xxHash repository from https://github.com/Cyan4973/xxHash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define ValueSize 64
#define keySizeFliphash 64
   //All the Values used for generating the keys should be of the size 64 bits.

typedef struct{
   int64_t count;
}resource;
typedef resource* resPtr;

int32_t seeding(int16_t, int16_t);
int64_t fliphashPow2(char *, int64_t);
int64_t fliphashGeneral(char*, resPtr);
int64_t keyGenerator(FILE*);
   //Declarations

int main(int argc, const char* args[]){
   clock_t start_time, end_time;
   double total_time;
   resPtr res = (resPtr)malloc(sizeof(resource)); //right now there is no way of increasing the value in between execution.
      //stop execution and start again to increase resources.
   printf(" Please enter the total number of resources:\n");
   scanf(" %ld", &res->count);
   char temp[ValueSize+1];
   char *temp2;
   int64_t result;
   while(1){
      scanf(" %s",temp);
      if(temp!=NULL){
         start_time = clock();
         printf(" %s\n", temp);
         result = fliphashGeneral(temp,res);
         printf(" result: %ld\n", result);
         end_time = clock();
         total_time = ((double)(end_time - start_time)) /  CLOCKS_PER_SEC; 
         printf(" Time taken: %f\n", total_time);
      }   
   }
   return 0;
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