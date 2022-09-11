#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>

#include "fileio.h"
#include "linkedlist.h"

#define BUFSIZE 1024

#ifndef DEBUG
#define DEBUG 0
#endif

/*
stream_t open_raw_data(char * filename) {
    int fd = OPEN(filename, O_RDONLY);
    if(fd == -1) {
        // Uh oh, opening the file has gone awry.
        perror("open");

        stream_t stream;
        stream.data = NULL;
        stream.size = 0;
        return stream;
    }

    if(DEBUG) printf("File descriptor is %d", fd);

    stream_t stream;

    // Buffer that we keep writing to
    char buffer[BUFSIZE];

    // declare and initialize the linked list
    linkedlist_t data_list;
    ll_init(&data_list, BUFSIZE);

    node_t * last_node = data_list.head;

    // Keeps track of the number of bytes that we currently have
    int bytes;

    // Keeps a running total of the number of bytes in the file
    size_t total_data_size = 0;

    // FINISH THIS UP
    while((bytes = READ(fd, buffer, BUFSIZE)) > 0) {
        last_node = ll_insert_after_node(&data_list, last_node, buffer, bytes);
        total_data_size += bytes;
    }

    stream.data = malloc(total_data_size * sizeof(char));
    stream.size = total_data_size;

    // Keeps a running tally of all the bytes that were successfully copied
    int bytes_copied;

    for(; data_list.head; ll_delete_front(&data_list)) {
        // TODO: Finish code that copies data into stream
        memcpy(stream.data + bytes_copied,
                data_list.head->node_data,
                data_list.head->node_occupied);

        bytes_copied += data_list.head->node_occupied;
    }

    ll_destroy(&data_list);
    return stream;
}
*/

void stream_destroy(stream_t * stream) {
    free(stream->data);
    stream->data = NULL;
    stream->size = 0;
}

stream_t open_raw_data(char * filename) {
    int fd = OPEN(filename, O_RDONLY);

    // You cannot feed in an invalid file descriptor
    if(fd < 0) {
        printf("s\n");
        perror("Fed an invalid file descriptor into open_raw_data.");
        
        stream_t stream;
        stream.data = NULL;
        stream.size = 0;
        return stream;
    }
    stream_t stream;

    // Buffer we use with read()
    char buffer[BUFSIZE];

    // declare and initialize the linked list
    linkedlist_t orig_list;
    linkedlist_t * data_list  = &orig_list;
    ll_init(data_list, BUFSIZE);

    if(DEBUG) puts("Initialized linked list");
    
    node_t * last_node = data_list->head;
    
    // We use bytes to keep track of currently read no. of bytes and we use
    // total_data_size to keep track of the size of the data read.
    int bytes;
    size_t total_data_size = 0;

    while((bytes = READ(fd, buffer, BUFSIZE)) > 0) {
        last_node = ll_insert_after_node(data_list, last_node, buffer, bytes);
        total_data_size += bytes;
    }

    if(DEBUG) puts("FIlled up linked list");

    // Allocate a segment in memory equal to no. of chars read
    stream.data = malloc(total_data_size);
    stream.size = total_data_size;

    // For the elements of the linked list, dump the contents of the elements
    // sequentially into our stream
    size_t bytes_left = total_data_size;
    while(data_list->head != NULL) {
        memcpy(stream.data + (total_data_size - bytes_left), 
            data_list->head->node_data, data_list->head->node_occupied);

        bytes_left -= data_list->head->node_occupied;
        ll_delete_front(data_list);
    }

    // we do this to free the memory used by list
    ll_destroy(data_list);

    return stream;
}