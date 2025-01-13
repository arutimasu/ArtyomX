extern struct ih_low_t {
    struct clist_head_t list_head; /* should be at first */
    int number; /* interrupt number */
    ih_low_cb_t handler; /* interrupt handler */
};