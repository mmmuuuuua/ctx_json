#include <cstdio>

class FileStream{
    public:
        FileStream(std::FILE* fp, char* buffer, size_t bufferSize):
            fp_(fp), buffer_(buffer), bufferSize_(bufferSize), end_(0), current_(buffer_), readCount_(0), count_(0), eof_(false)
        {
            assert(fp_!=0);
            Read();
        }

        char Peek() const { 
            return *current_; 
        }

        char Next() { 
            char c = *current_; 
            Read(); 
            return c; 
        }

        size_t Count() const { 
            return count_ + static_cast<size_t>(current_ - buffer_); 
        }

        char* Current() { 
            return current_; 
        }

    private:
        void Read(){
            if(current_<end_)
                current_++;
            else if(!eof_)
            {
                count_+= readCount_;
                readCount_ = std::fread(buffer_,1,bufferSize_,fp_);
                end_ = buffer_ + readCount_ - 1;
                current_ = buffer_;
                if(readCount_<bufferSize_){
                    buffer_[readCount_] = '\0';
                    ++end_;
                    eof_ = true;
                }
            }
        }
        std::FILE* fp_;
        char* buffer_;
        char* current_;
        char* end_;
        size_t bufferSize_;
        size_t readCount_;
        size_t count_;
        bool eof_;
};


class StringStream {
    public:
        StringStream(const char *src) : current_(src), head_(src) {}

        char Peek() const { return *current_; }
        char Next() { return *current_++; }
        const char* Current() { return current_; }
        size_t Count() const { return static_cast<size_t>(current_ - head_); }
    private:
        const char* current_;     //!< Current read position.
        const char* head_;    //!< Original head of the string.
};