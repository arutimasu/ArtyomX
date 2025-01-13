extern void dev_register(struct dev_t* dev)
{
    struct clist_head_t* entry;
    struct dev_t* device;

    /* create list entry */
    entry = clist_insert_entry_after(&dev_list, dev_list.head);
    device = (struct dev_t*)entry->data;

    /* fill data */
    strncpy(device->name, dev->name, sizeof(dev->name));
    device->base_r = dev->base_r;
    device->base_w = dev->base_w;
    device->read_cb = dev->read_cb;
    device->write_cb = dev->write_cb;
    device->ioctl_cb = dev->ioctl_cb;
    device->ih_list.head = dev->ih_list.head;
    device->ih_list.slot_size = dev->ih_list.slot_size;
}