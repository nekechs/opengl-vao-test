#ifndef _FILE_IO_
#define _FILE_IO_

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#define OPEN(pathname, flags) open(pathname, flags) 
#define OPEN_MODE(pathname, flags, mode) open(pathname, flags, mode)
#define READ(fd, buf, count) read(fd, buf, count)

typedef struct {
    char * data;
    size_t size;
} stream_t;

/**
 * @brief Takes in a string that represents a filename and then outputs the raw data into a stream_t,
 * which also contains the size of the data. What is important is that the stream cannot actually be treated
 * as a string, so this function does not add a null terminator to the data.
 * 
 * Note: MUST FREE THE POINTER AT SOME POINT AFTER USE 
 * 
 * @param filename - The file that is to be read out from. If this is not a valid file, this function returns
 * a stream with size of 0 and data storing NULL
 * @return stream_t - stores pointer to a section in the heap that contains the raw dump from the file
 */
stream_t open_raw_data(char * filename);

/**
 * @brief This function deallocates a stream from the heap. Please ensure that the data for the stream
 * is already on the heap.
 * 
 * This function will free the data, set the pointer to NULL, and then set the size to 0.
 * 
 * @param stream the stream object that is to be freed.
 */
void stream_destroy(stream_t * stream);
#endif