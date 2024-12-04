#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>	/*copy_to/from_user*/
#include <linux/device.h>

#define DEVICE_NAME "char_device"
#define CLASS_NAME "char_class"

static int major_number;
static struct class *char_class = NULL;
static struct device *char_device = NULL;
static char message[256] = {0};
static size_t message_len = 0;
static struct cdev char_cdev;

/*File Operations*/
static int dev_open(struct inode *inidep, struct file *filep){
	printk(KERN_INFO "CharDevice: Device opened");
	return 0;
}

static ssize_t dev_read(struct file *filep, char __user *buffer, size_t len, loff_t *offset){
	int ret;

	if(*offset >=message_len)
		return 0; /*EOF*/
	
	if(len > message_len - *offset)
		len = message_len - *offset;

	ret = copy_to_user(buffer, message+*offset, len);
	if(ret==0){
		printk(KERN_INFO"CharDevice: Sent %zu chars to user", len);
		*offset = *offset + len;
		return len;
	} else{
		printk(KERN_INFO"CharDevice: Failed to send %zu chars to user", len);
		return -EFAULT;
	}
}

static ssize_t dev_write(struct file* filep, const char __user *buffer, size_t len, loff_t *offset){

	if(len >= sizeof(message)){
		len = sizeof(message)-1;
	}

	if(copy_from_user(message, buffer, len))
		return -EFAULT;

    message[len] = '\0'; // Null-terminate
    message_len = len;
    printk(KERN_INFO "CharDevice: Received %zu characters from user\n", len);
    return len;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "CharDevice: Device closed\n");
    return 0;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init char_driver_init(void) {
    printk(KERN_INFO "CharDevice: Initializing\n");

    // Dynamically allocate a major number
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "CharDevice: Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "CharDevice: Registered with major number %d\n", major_number);

    // Create device class
    char_class = class_create(CLASS_NAME);
    if (IS_ERR(char_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "CharDevice: Failed to register device class\n");
        return PTR_ERR(char_class);
    }

    // Create device node
    char_device = device_create(char_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(char_device)) {
        class_destroy(char_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "CharDevice: Failed to create device\n");
        return PTR_ERR(char_device);
    }

    printk(KERN_INFO "CharDevice: Device created successfully\n");
    return 0;
}

static void __exit char_driver_exit(void) {
    device_destroy(char_class, MKDEV(major_number, 0));
    class_unregister(char_class);
    class_destroy(char_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "CharDevice: Goodbye\n");
}

module_init(char_driver_init);
module_exit(char_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Poonam Gupta");
MODULE_DESCRIPTION("Linux char driver");
MODULE_VERSION("1.0");

