struct io_buf_t {
    int fd; /* file descriptor */
    char* base; /* buffer beginning */
    char* ptr; /* position in buffer */
    bool is_eof; /* whether end of file */
    void* file; /* file definition */
};

#define FILE struct io_buf_t