#include <asm-generic/errno-base.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Erik Jelezarov");
MODULE_DESCRIPTION("Basic module that reacts to read and write ops to procfs file.");

//static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

static char msg[255];

//implemented read proc_op
static ssize_t mymodule_read(struct file *file, char __user *ubuf, size_t ubuf_len, loff_t *offset){
    pr_info("PROC_FS: mymodule_read\n");
    //printk(KERN_INFO "PROC_FS: mymodule_read\n");

	int not_copied, to_copy, delta;

	if (*offset >= sizeof(msg)){
	    return 0;
	}

	to_copy = min(ubuf_len, sizeof(msg) - *offset);
	not_copied = copy_to_user(ubuf, &msg[*offset], to_copy);

	delta = to_copy - not_copied;
	*offset += delta;

	return delta;
}

//implemented write proc_op
//must return size of input to prevent repeated/infinite attempts at writing
static ssize_t mymodule_write(struct file *file, const char __user *ubuf, size_t ubuf_len, loff_t *offset){
    pr_info("PROC_FS: mymodule_write\n");
    //printk(KERN_INFO "PROC_FS: mymodule_write\n");

    int not_copied, to_copy, delta;

    memset(msg, 0, sizeof(msg));

    if (*offset >= sizeof(msg)){
        return -ENOSPC;
    }

    to_copy = min(ubuf_len, sizeof(msg) - *offset);
    not_copied = copy_from_user(&msg[*offset], ubuf, to_copy);

    delta = to_copy - not_copied;
    *offset += delta;

    return delta;
}

//linking implemented proc operations
const struct proc_ops procfs_ops = {
	.proc_read = mymodule_read,
	.proc_write = mymodule_write
};

//constructor function for kernel module
static int mymodule_init(void){
    pr_debug("PROC_FS: mymodule_init enter\n");
	//printk(KERN_DEBUG "PROC_FS: mymodule_init enter\n");

	//creating parent folder, if unsuccessful return error
	// proc_folder = proc_mkdir("testing", NULL);

	// if (proc_folder == NULL){
	//     printk("PROC_FS: mymodule_init - failed to create procfs folder.\n");
	// 	return -ENOMEM;
	// }

	//creating proc file within parent folder, if unsuccessful remove parent folder and return error
	// set third parameter to name of parent folder if exists
	proc_file = proc_create("mymodule", 0666, NULL, &procfs_ops);

	if (proc_file == NULL){
	    pr_err("PROC_FS: mymodule_init - failed to create procfs file.\n");
		//printk(KERN_ERR "PROC_FS: mymodule_init - failed to create procfs file.\n");
		//proc_remove(proc_folder);
		return -ENOMEM;
	}

	memset(msg, 0, sizeof(msg));
	strcpy(msg, "default message\n");

	pr_debug("PROC_FS: mymodule_init exit\n");
	//printk(KERN_DEBUG "PROC_FS: mymodule_init exit\n");

	return 0;

}

//destructor function for kernel module
static void mymodule_exit(void){
    pr_debug("PROC_FS: mymodule_exit enter\n");
    //printk(KERN_DEBUG "PROC_FS: mymodule_exit enter\n");

	//removing file entity
	// if parent folder exists, you can remove just the parent folder and it will
	// recursively remove everything inside
	proc_remove(proc_file);

	pr_debug("PROC_FS: mymodule_exit exit\n");
	//printk(KERN_DEBUG "PROC_FS: mymodule_exit exit\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);
