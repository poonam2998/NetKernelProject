#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/proc_fs.h>

#include "../include/packet_handler.h"
#include "../include/mac_table.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pooanm Gupta");
MODULE_DESCRIPTION("L2 Switch");
MODULE_VERSION("1.0");

static int __init packet_counter_init(void){
	int ret;

	printk(KERN_INFO "[l2_switch_main] L2 Switch Module loaded\n");
	
	/*Register Netfilter hook*/
	ret = packet_handler_init();

	return ret;
}

static void __exit packet_counter_exit(void){
	packet_handler_deinit();
	printk(KERN_INFO "[l2_switch_main] L2 Switch Module un-loaded\n");
}

module_init(packet_counter_init);
module_exit(packet_counter_exit);
