#include <asm-generic/errno-base.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Erik Jelezarov");
MODULE_DESCRIPTION("Basic module that registers a character device.");

static int major_dev_no;

static struct file_operations fops = {};

//constructor function for kernel module
static int mymodule_init(void){
	printk("CHR_DEV: mymodule_init enter\n");

	major_dev_no = register_chrdev(0, "mydev", &fops);

	if (!major_dev_no) {
	    printk("CHR_DEV: Device registration failed!\n");
		return major_dev_no;
	}

	printk("CHR_DEV: Device registration sucess! Major device no. is %d\n", major_dev_no);

	printk("CHR_DEV: mymodule_init exit\n");

	return 0;

}

//destructor function for kernel module
static void mymodule_exit(void){
	printk("CHR_DEV: mymodule_exit enter\n");

	unregister_chrdev(major_dev_no, "mydev");

	printk("CHR_DEV: mymodule_exit exit\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);
