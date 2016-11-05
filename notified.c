#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/notifier.h>
#include "custom_notification_events.h"

extern struct blocking_notifier_head file_op_event_chain;

int file_op_event_handler(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	int ret = 0;

	switch (event) {
	case CUSTOM_EVENT_OPEN:
		printk("Open event received, ptr: %p \n", ptr);
		break;
	case CUSTOM_EVENT_CLOSE:
		printk("Close event received, ptr: %p \n", ptr);
		break;
	case CUSTOM_EVENT_READ:
		printk("Read event received, ptr: %p \n", ptr);
		break;
	case CUSTOM_EVENT_WRITE:
		printk("Write event received, ptr: %p \n", ptr);
		break;
	default:
		printk("Invalid event received \n");
		ret = -EINVAL;
	}

	return ret;
}

struct notifier_block file_op_event = {
	.notifier_call = file_op_event_handler,
};

static int __init notified_init(void)
{
	int ret = 0;
	
	printk("Initializing notified module \n");
	
	printk("Subscribing to custom notifier chain \n");

	ret = blocking_notifier_chain_register(&file_op_event_chain, &file_op_event);	

	if (ret) {
		printk(KERN_ERR "Failed to subscribe to notificatons \n");
	}	

        return ret;

}

static void __exit notified_exit(void)
{
	printk("Exiting notified module \n");
	
	printk("Un-subscribing to custom notifier chain \n");
	blocking_notifier_chain_unregister(&file_op_event_chain, &file_op_event);

}

module_init(notified_init);
module_exit(notified_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yasir Khan <yasir_electronics@yahoo.com>");
