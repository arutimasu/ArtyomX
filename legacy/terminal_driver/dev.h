extern struct dev_t {
    struct clist_head_t list_head; /* should be at first */
    char name[8]; /* device name */
    void* base_r; /* base read address */
    void* base_w; /* base write address */
    dev_read_cb_t read_cb; /* read handler */
    dev_write_cb_t write_cb; /* write handler */
    dev_ioctl_cb_t ioctl_cb; /* device specific command handler */
    struct clist_definition_t ih_list; /* low half interrupt handlers */
};