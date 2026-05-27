

Whenever User executes Driver.ko (Driver.o), internally it calls the function specified in module_init();

then we can see the Printk messages by typing "dmesg" 
i.e----> "dmesg"  prints kernel ring buffer.


Every Device contains one Major Number and Specific Name.

"file operation" structure is the structure what kind of actions to be performed on Device.
