#pragma once

#include "stddef.h"
#include "stdint.h"

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

#define PTR_MOVE_BYTES(ptr, offs) (ptr) = ((uint8_t*)(ptr)) + offs;

#define PTR_APPEND(ptr, type, val) {(*(type*)(ptr)) = val; (ptr) = ((type*)ptr) + 1;}

#define PTR_READVAL(ptr, type, val) {val = (*(type*)(ptr)); (ptr) = ((type*)ptr) + 1;}

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
