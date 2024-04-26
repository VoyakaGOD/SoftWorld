#pragma once

#include "stddef.h"
#include "stdint.h"
#include <stdexcept>

#ifdef unix
#define FILEWORKS_UNIX
#else
#define FILEWORKS_FALLBACK
#endif


#ifdef FILEWORKS_FALLBACK
#include <stdio.h>
#endif

typedef uint8_t saved_obj_id_t;
typedef uint16_t obj_fixed_data_len_t;

#define PTR_MOVE_BYTES(ptr, offs) (ptr) = ((uint8_t*)(ptr)) + (offs);

#define PTR_APPEND(ptr, type, val) {(*(type*)(ptr)) = val; (ptr) = ((type*)(ptr)) + 1;}
#define WRITER_APPEND(writer, type, val) {  \
    if (((type*)((writer).data)) + 1 > (writer).end) \
        throw std::out_of_range("data write exceeds length of the buffer"); \
    (*(type*)(writer.data)) = val; (writer.data) = ((type*)writer.data) + 1;    \
}

#define WRITER_CHECK_BYTES(writer, size) {\
    if (((uint8_t*)(writer.data)) + (size) > writer.end) \
        throw std::out_of_range("data write exceeds length of the buffer"); \
}

#define WRITER_MOVE_BYTES(writer, offs) {\
    PTR_MOVE_BYTES(writer.data, offs)    \
    WRITER_CHECK_BYTES(writer, 0)        \
}

#define READER_CHECK_BYTES(reader, size, err) { \
    if (((uint8_t*)(reader.data)) + (size) > reader.end) \
        err \
}

#define READER_MOVE_BYTES(reader, size, err) { \
    PTR_MOVE_BYTES(reader.data, size)          \
    READER_CHECK_BYTES(reader, 0, err)         \
}   \

#define PTR_READVAL(ptr, type, val) {val = (*(type*)(ptr)); (ptr) = ((type*)ptr) + 1;}

#define READER_READVAL(reader, type, val, err) { \
    READER_CHECK_BYTES(reader, sizeof(type), err) \
    PTR_READVAL(reader.data, type, val)          \
}

class DataStorageWriter {
    public:
    void* data = nullptr;
    void* end  = nullptr;
    virtual void* PrepareDataBuf(size_t size) = 0;
};

class DataStorageReader {
    public:
    void* data;
    void* end;
};

class FileReadInterface : public DataStorageReader {
    void* begin_ptr;
    size_t size;

    #ifdef FILEWORKS_FALLBACK
        FILE* file;
    #endif

    public:
    FileReadInterface(const char* filename);
    ~FileReadInterface();
};

class FileWriteInterface : public DataStorageWriter {
    int buff_size = 0;
    void* buff = nullptr;

    #ifdef FILEWORKS_UNIX
        int file_offs = 0;
        int fd;
    #endif
    #ifdef FILEWORKS_FALLBACK
        FILE* file;
    #endif

    public:
    FileWriteInterface(const char* filename);
    void* PrepareDataBuf(size_t size);
    ~FileWriteInterface();
};
