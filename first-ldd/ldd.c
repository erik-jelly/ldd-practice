#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GLP");
MODULE_AUTHOR("Erik Jelezarov");
MODULE_DESCRIPTION("Basic interactable linux device driver that reacts to read and write ops");


static struct proc_dir_entry *proc_entry;

static ssize_t ldd_read(struct file *, char __user *, size_t, loff_t *){
        printk("LDD: ldd_read");
        return 0;
}

const struct proc_ops ldd_proc_ops = {
        .proc_read = ldd_read
};

static int ldd_init(void){
        printk("LDD: ldd_init enter\n");

        proc_entry = proc_create("ldd", 0, NULL, &ldd_proc_ops);

        if (proc_entry == NULL){
                printk("LDD: ldd_init - proc_create failed");
                return -1;
        }

        printk("LDD: ldd_init exit\n");

        return 0;
