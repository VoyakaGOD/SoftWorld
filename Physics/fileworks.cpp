#include "fileworks.h"
#include <assert.h>
#include <errno.h>
#include <system_error>

#ifdef unix

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>


#define MAX_FILENAME_LEN 128

static char filename_buffer[MAX_FILENAME_LEN];

static bool has_zenity = system("which zenity") == 0;

const char* askFilename(char mode) {
    if (has_zenity) {
        FILE* pipe = popen(mode == 'r'  ? "zenity --file-selection --title=\"Choose file\""
                                        : "zenity --file-selection --save --title=\"Save file\"", "r");
        if (fgets(filename_buffer, MAX_FILENAME_LEN, pipe) == nullptr) {
            return nullptr;
        }
        pclose(pipe);
    }
    else {
        return nullptr;
    }
    char* newl_chr = strchr(filename_buffer, '\n');
    if (newl_chr) {*newl_chr = '\0';}
    return filename_buffer;
}

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
    this->fd  = open(filename, O_RDWR | O_CREAT | 066);
    if (this->fd < 0) {
        throw std::system_error(errno, std::system_category(), "cannot open file");
    }

    this->buff_size = PAGESIZE;
    this->buff = mmap(nullptr, PAGESIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    this->buff_offs = 0;

    if (!(this->buff)) {
        close(this->fd);
        throw std::system_error(errno, std::system_category(), "cannot map buffer");
    }
}

void* FileWriteInterface::PrepareDataBuf(size_t size) {
    assert(size <= PAGESIZE);
    if (this->buff_size < this->buff_offs + size) {
        if (write(this->fd, this->buff, this->buff_offs) < 0) {
            throw std::system_error(errno, std::system_category(), "error while writing data");
        }
    }
    this->buff_offs = size;
    return ((uint8_t*)(this->buff)) + this->buff_offs - size;
}

FileWriteInterface::~FileWriteInterface() {
    if (this->buff_offs) {
        write(fd, this->buff, this->buff_offs);
    }
    close(this->fd);
    munmap(this->buff, this->buff_size);
    return;
}

#else

#endif
