#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

/*Module metadata*/
MODULE_AUTHOR("Poonam Gupta");
MODULE_DESCRIPTION("Hello world driver");
MODULE_LICENSE("GPL");

static struct proc_dir_entry* proc_entry;

static ssize_t custom_read(struct file* file, char __user* user_buffer, size_t count, loff_t* offset)
{
	char msg[] = "Hello World! :)";
	int msg_len = strlen(msg);

	if(*offset > 0)
		return 0;

	copy_to_user(user_buffer, msg, msg_len);
	*offset = msg_len;

	return msg_len;
}

static struct proc_ops p_ops = {
	.proc_read = custom_read,
};

/*Custom init and exit methods*/
static int __init custom_init(void) {
	proc_entry = proc_create("helloworlddriver", 0666, NULL, &p_ops);
	printk(KERN_INFO "Hello world driver loaded.");
	return 0;
}

static void __exit custom_exit(void) {
	proc_remove(proc_entry);
	printk(KERN_INFO "Goodbye my friend, I shall miss you dearly...");
}

module_init(custom_init);
module_exit(custom_exit);
