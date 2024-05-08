#include <string.h>
#include <assert.h>
#include <errno.h>
#include <system_error>

#include "fileworks.h"

#ifdef FILEWORKS_UNIX

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <stddef.h>
#include <unistd.h>

FileReadInterface::FileReadInterface(const char* filename) {


    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        throw std::system_error(errno, std::system_category(), "cannot open file");
    }

    struct stat statbuf = {};
    if (fstat(fd, &statbuf)) {
        close(fd);
        throw std::system_error(errno, std::system_category(), "cannot open read file size");
    }

    this->size = statbuf.st_size;
    this->begin_ptr = mmap(nullptr, this->size, PROT_READ, MAP_SHARED, fd, 0);

    if (!(this->begin_ptr)) {
        close(fd);
        throw std::system_error(errno, std::system_category(), "cannot map file");
    }

    close(fd);

    this->data = this->begin_ptr;
    this->end  = ((uint8_t*)(this->begin_ptr)) + this->size;
}

FileReadInterface::~FileReadInterface() {
    munmap(begin_ptr, size);
}

#define PAGESIZE 4 * 1024

FileWriteInterface::FileWriteInterface(const char* filename) {
    this->fd  = open(filename, O_RDWR | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    if (this->fd < 0) {
        throw std::system_error(errno, std::system_category(), "cannot open file");
    }
    /*
    if (!(this->buff)) {
        close(this->fd);
        throw std::system_error(errno, std::system_category(), "cannot map buffer");
    }*/
}

void* FileWriteInterface::PrepareDataBuf(size_t size) {
    if (this->buff_size) {
        munmap(this->buff, this->buff_size);
    }

    this->file_offs = this->file_offs + this->buff_size;
    this->buff_size = size + (this->file_offs % PAGESIZE);

    ftruncate(this->fd, this->file_offs + size);
    this->buff = mmap(nullptr, this->buff_size
        , PROT_READ | PROT_WRITE, MAP_SHARED
        , this->fd, this->file_offs / PAGESIZE);

    if (!(this->buff)) {
        throw std::system_error(errno, std::system_category(), "cannot map file");
    }

    this->data = ((uint8_t*)(this->buff)) + (this->file_offs % PAGESIZE);
    this->end = ((uint8_t*)this->data) + size;

    return this->data;
}

FileWriteInterface::~FileWriteInterface() {
    close(this->fd);
    munmap(this->buff, this->buff_size);
    return;
}

#endif
#ifdef FILEWORKS_FALLBACK

FileReadInterface::FileReadInterface(const char* filename) {


    this->file = fopen(filename, "rb");

    if (!this->file) {
        throw std::system_error(errno, std::system_category(), "cannot open file");
    }

    fseek(this->file, 0, SEEK_END);
    this->size = ftell(this->file);
    rewind(this->file);

    this->begin_ptr = malloc(this->size);

    if (!(this->begin_ptr)) {
        fclose(this->file);
        throw std::system_error(errno, std::system_category(), "out of memory");
    }

    size_t ret = fread(this->begin_ptr, 1, this->size, this->file);
    if (ret != this->size) {
        fclose(this->file);
        throw std::system_error(errno, std::system_category(), "failed to read file");
    }

    this->data = this->begin_ptr;
    this->end  = ((uint8_t*)(this->begin_ptr)) + this->size;
}

FileReadInterface::~FileReadInterface() {
    fclose(this->file);
    free(this->begin_ptr);
}

FileWriteInterface::FileWriteInterface(const char* filename) {

    this->file  = fopen(filename, "wb");
    if (!(this->file)) {
        throw std::system_error(errno, std::system_category(), "cannot open file");
    }
}

void* FileWriteInterface::PrepareDataBuf(size_t size) {
    if (this->buff_size) {
        size_t ret = fwrite(this->buff, 1, this->buff_size, this->file);
        if (ret != this->buff_size) {
            this->buff_size = 0;
            throw std::system_error(errno, std::system_category(), "failed to write file");
        }
        free(this->buff);
    }

    if (!size) {
        this->buff_size = 0;
        this->data = nullptr;
        this->end = nullptr;
        return nullptr;
    }

    this->buff = malloc(size);
    this->buff_size = size;

    if (!(this->buff)) {
        throw std::system_error(errno, std::system_category(), "out of memory");
    }

    this->data = ((uint8_t*)(this->buff));
    this->end = ((uint8_t*)this->data) + size;

    return this->data;
}

FileWriteInterface::~FileWriteInterface() {
    if (this->buff_size) {
        fwrite(this->buff, 1, this->buff_size, this->file);
    }

    fclose(this->file);
    free(this->buff);
}
#endif
