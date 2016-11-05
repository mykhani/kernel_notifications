Simple kernel modules demo to understand the Linux notification chain API
=========================================================================

The module notifier.ko creates a miscellaneous device and publishes a notification chain 
for file operation events (open, close, read, write). Another module notified.ko subscribes
to this notification chain and provides callbacks for the above mentioned file operation
events. 

On each file operation on miscdevice device node, notifer module raises an event and correspondingly,
the callback/handler provided by notified module is executed. In callback, the type of event
(open, close, read and write) is printed to the console.

Sample Output
=============
```
# sudo insmod notifer.ko 
# sudo insmod notified.ko 

# dmesg | tail
[36647.313176] Initializing notifier module 
[36647.313178] Address of miscdev pointer (stack): ffffffffa0d88288 
[36647.313178] Value of miscdev pointer (heap): ffff8803ff9562a0 
[36667.282509] Initializing notified module 
[36667.282511] Subscribing to custom notifier chain

# sudo touch /dev/notifierdev
[36762.603680] Notifier open called 
[36762.603684] Open event received, ptr: 00000000deadbeaf 
[36762.603694] Notifier close called 
[36762.603695] Close event received, ptr: 00000000deadbeaf

# sudo dd if=/dev/notifierdev bs=1 count=1
[36904.274558] Notifier open called 
[36904.274561] Open event received, ptr: 00000000deadbeaf 
[36904.274618] Notifier read called 
[36904.274619] Read event received, ptr: 00000000deadbeaf 
[36904.274624] Notifier close called 
[36904.274625] Close event received, ptr: 00000000deadbeaf

# sudo dd if=/dev/zero of=/dev/notifierdev bs=1 count=1
[36985.345819] Notifier open called 
[36985.345822] Open event received, ptr: 00000000deadbeaf 
[36985.345828] Notifier write called 
[36985.345829] Write event received, ptr: 00000000deadbeaf 
[36985.345831] Notifier close called 
[36985.345831] Close event received, ptr: 00000000deadbeaf
```
