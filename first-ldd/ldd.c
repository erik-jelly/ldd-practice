#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Erik Jelezarov");
MODULE_DESCRIPTION("Basic interactable linux device driver that reacts to read and write ops");

static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

//implemented read proc_op
static ssize_t ldd_read(struct file *file, char __user *ubuf, size_t count, loff_t *offset){
	printk("PROC_FS_LDD: ldd_read\n");
	return 0;
}

//implemented write proc_op
//must return size of input to prevent repeated/infinite attempts at writing
static ssize_t ldd_write(struct file *file, const char __user *ubuf, size_t count, loff_t *offset){
    printk("PROC_FS_LDD: ldd_write\n");
    return count;
}

//linking implemented proc operations
const struct proc_ops ldd_proc_ops = {
	.proc_read = ldd_read,
	.proc_write = ldd_write
};

//constructor function for kernel module
static int ldd_init(void){
	printk("PROC_FS_LDD: ldd_init enter\n");

	//creating parent folder, if unsuccessful return error
	proc_folder = proc_mkdir("testing", NULL);

	if (proc_folder == NULL){
	    printk("PROC_FS_LDD: ldd_init - failed to create procfs folder.\n");
		return -ENOMEM;
	}

	//creating proc file within parent folder, if unsuccessful remove parent folder and return error
	proc_file = proc_create("ldd", 0666, proc_folder, &ldd_proc_ops);

	if (proc_file == NULL){
		printk("PROC_FS_LDD: ldd_init - failed to create procfs file.\n");
		proc_remove(proc_folder);
		return -ENOMEM;
	}

	printk("PROC_FS_LDD: ldd_init exit\n");

	return 0;

}

//destructor function for kernel module
static void ldd_exit(void){
	printk("PROC_FS_LDD: ldd_exit enter\n");

	//removing folder entity recursively removes all other files within
	proc_remove(proc_folder);

	printk("PROC_FS_LDD: ldd_exit exit\n");
}

module_init(ldd_init);
module_exit(ldd_exit);
