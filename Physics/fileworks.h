#pragma once

#include "stddef.h"
#include "stdint.h"

const char* askFilename(char mode);

class DataStorageWriter {
    public:
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

    public:
    FileReadInterface(const char* filename);
    ~FileReadInterface();
};

class FileWriteInterface : public DataStorageWriter {
    int buff_size;
    int buff_offs;
    void* buff;

    int fd;

    public:
    FileWriteInterface(const char* filename);
    void* PrepareDataBuf(size_t size);
    ~FileWriteInterface();
};
