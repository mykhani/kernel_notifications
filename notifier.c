#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/notifier.h>

#include "custom_notification_events.h"

struct miscdevice *miscdev;

struct blocking_notifier_head file_op_event_chain;

EXPORT_SYMBOL_GPL(file_op_event_chain);

int notifier_open(struct inode *i, struct file *f)
{
        printk("Notifier open called \n");
	blocking_notifier_call_chain(&file_op_event_chain, CUSTOM_EVENT_OPEN, (void *)0xDEADBEAF);
        return 0;
}

int notifier_close(struct inode *i, struct file *f)
{
        printk("Notifier close called \n");
	blocking_notifier_call_chain(&file_op_event_chain, CUSTOM_EVENT_CLOSE, (void *)0xDEADBEAF);
        return 0;
}

ssize_t notifier_read(struct file *f, char __user *buf, size_t len, loff_t *offset)
{
        printk("Notifier read called \n");

	/* call the functions registered with read notifier */
	blocking_notifier_call_chain(&file_op_event_chain, CUSTOM_EVENT_READ, (void *)0xDEADBEAF);

        return len;
}

ssize_t notifier_write(struct file *f, const char __user *buf, size_t len, loff_t *offset)
{
        /* use copy_from_user to copy data from userspace and send over USB */
        printk("Notifier write called \n");
	blocking_notifier_call_chain(&file_op_event_chain, CUSTOM_EVENT_WRITE, (void *)0xDEADBEAF);
        return len;
}

static const struct file_operations notifier_fops = {
        .open = notifier_open,
        .release = notifier_close,
        .read = notifier_read,
        .write = notifier_write,
};

static int __init notifier_init(void)
{
	int ret;

	printk("Initializing notifier module \n");

	miscdev = kzalloc(sizeof(struct miscdevice), GFP_KERNEL);
	if (!miscdev) {
		printk(KERN_ERR "Failed to allocated miscdevice \n");
		return -ENOMEM;
	}

	printk("Address of miscdev pointer (stack): %p \n", &miscdev);
	printk("Value of miscdev pointer (heap): %p \n", miscdev);

	BLOCKING_INIT_NOTIFIER_HEAD(&file_op_event_chain);

	/* Initialize misc device */
        miscdev->minor = MISC_DYNAMIC_MINOR;
        miscdev->name = kasprintf(GFP_KERNEL, "notifierdev");

        miscdev->fops = &notifier_fops;

        /* register misc device */
        ret = misc_register(miscdev);
        if (ret) {
                printk(KERN_ERR "Unable to register misc device \n");
        }

        return ret;

}

static void __exit notifier_exit(void)
{
	printk("Exiting notifer module \n");

	if (miscdev->name)
                kfree(miscdev->name);

        misc_deregister(miscdev);
}

module_init(notifier_init);
module_exit(notifier_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yasir Khan <yasir_electronics@yahoo.com>");
