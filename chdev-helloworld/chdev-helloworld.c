#include <asm-generic/errno-base.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Erik Jelezarov");
MODULE_DESCRIPTION("Basic module that registers a character device.");

static int major_dev_no;

static ssize_t read (struct file *f, char __user *ubuf, size_t ubuf_len, loff_t *offset) {
    pr_info("CHR_DEV: read op called\n");
    //printk(KERN_INFO "CHR_DEV: read op called\n");
    return 0;
}

static ssize_t write (struct file *f, const char __user *ubuf, size_t ubuf_len, loff_t *offset){
    pr_info("CHR_DEV: write op called\n");
    //printk(KERN_INFO "CHR_DEV: write op called\n");
    return ubuf_len;
}

static struct file_operations fops = {
    .read = read,
    .write = write
};

//constructor function for kernel module
static int mymodule_init(void){
    pr_debug("CHR_DEV: mymodule_init enter\n");
	//printk(KERN_DEBUG "CHR_DEV: mymodule_init enter\n");

	major_dev_no = register_chrdev(0, "mydev", &fops);

	if (!major_dev_no) {
	    pr_err("CHR_DEV: Device registration failed!\n");
	    //printk(KERN_ERR "CHR_DEV: Device registration failed!\n");
		return major_dev_no;
	}

	pr_info("CHR_DEV: Device registration sucess! Major device no. is %d\n", major_dev_no);
	//printk(KERN_INFO "CHR_DEV: Device registration sucess! Major device no. is %d\n", major_dev_no);

	pr_debug("CHR_DEV: mymodule_init exit\n");
	//printk(KERN_DEBUG "CHR_DEV: mymodule_init exit\n");

	return 0;

}

//destructor function for kernel module
static void mymodule_exit(void){
    pr_debug("CHR_DEV: mymodule_exit enter\n");
	//printk(KERN_DEBUG "CHR_DEV: mymodule_exit enter\n");

	unregister_chrdev(major_dev_no, "mydev");

	pr_debug("CHR_DEV: mymodule_exit exit\n");
	//printk(KERN_DEBUG "CHR_DEV: mymodule_exit exit\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);
