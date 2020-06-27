#include <stdlib.h>
#include <cstring>

class Allocator{
    public:
        Allocator(size_t chunkCapacity = defaultChunkCapacity):
            head_(NULL), chunkCapacity_(chunkCapacity)
        {

        }

        ~Allocator(){
            while(head_){
                Chunk* next = head_->next;
                free((void*)head_); //check if Chunk* can transfer to void* in this way;
                head_ = next;
            }
        }
        size_t Capacity() const{
            size_t capacity = 0;
            for(Chunk* c = head_;c!=0;c = c->next)
                capacity += c->capacity;
            return capacity;
        }

        size_t Size() const{
            size_t size = 0;
            for(Chunk* c = head_;c!=0;c = c->next)
                size += c->size;
            return size;
        }

        void* Alloc(size_t size){
            if(!size)
                return NULL;
            size = Align(size);
            if(head_==NULL || head_->size + size > head_->capacity)
            {
                if(!AddChunk(chunkCapacity_ > size ? chunkCapacity_ : size))
                    return NULL;
            }
            void* buffer = reinterpret_cast<char*>(head_) + Align(sizeof(Chunk)) + head_->size;
            head_->size += size;
            return buffer;
        }

        void* Realloc(void* prePtr,size_t preSize,size_t newSize){
            if(prePtr == 0)  
                return Alloc(newSize);
            
            if(newSize == 0)
                return NULL;
            
            preSize = Align(preSize);
            newSize = Align(newSize);

            if(preSize >= newSize)
                return prePtr;

            if(prePtr == reinterpret_cast<char*>(head_) + Align(sizeof(head_)) + head_->size - preSize){
                size_t increment = static_cast<size_t>(newSize - preSize);
                if(head_->size + increment <= head_->capacity){
                    head_->size += increment;
                    return prePtr;
                }
            }

            if(void* newBuffer = Alloc(newSize)){
                if(preSize)
                    std::memcpy(newBuffer,prePtr,preSize);
                return newBuffer;
            }
            else
                return NULL;
        }

        size_t Align(size_t size){
            return (((size) + static_cast<size_t>(7u)) & ~static_cast<size_t>(7u));
        }

    private:
        static const int defaultChunkCapacity =  1024 * 64 * 1024 ;

        bool AddChunk(size_t capacity){   //capacity must be aligned
            Chunk* chunk;
            if(chunk = reinterpret_cast<Chunk*>(malloc(Align(sizeof(Chunk)) + capacity))){  
                chunk->capacity = capacity;
                chunk->size = 0;
                chunk->next = head_;
                head_ = chunk;
                return true;
            }
            else
                return false;
        }

        struct Chunk{
            size_t capacity;
            size_t size;
            Chunk* next;
        };

        Chunk* head_;
        size_t chunkCapacity_;
};

 