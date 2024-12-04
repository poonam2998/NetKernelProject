#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/proc_fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pooanm Gupta");
MODULE_DESCRIPTION(" Packet Counting Kernel Module");
MODULE_VERSION("1.0");

/*Packet counters*/

static unsigned int tcp_count = 0;
static unsigned int udp_count = 0;
static unsigned int icmp_count = 0;

/*Hook function*/
static unsigned int packet_counter_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
	struct iphdr *ip_header;

	if(!skb)
		return NF_ACCEPT;
	
	ip_header = ip_hdr(skb);
	if(!ip_header)
		return NF_ACCEPT;
	
	switch(ip_header->protocol){
		case IPPROTO_TCP:
			printk(KERN_INFO "TCP Packet: %d\n", ++tcp_count);
			break;
		case IPPROTO_UDP:
			printk(KERN_INFO "UDP Packet: %d\n", ++udp_count);
			break;
		case IPPROTO_ICMP:
			printk(KERN_INFO "ICMP Packet: %d\n", ++icmp_count);
			break;
		default:
			printk(KERN_INFO "Other Packet");
			break;
	}
	return NF_ACCEPT; 
}

/*Hook Structure*/
static struct nf_hook_ops netfilter_ops = {
	.hook = packet_counter_hook,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_PRE_ROUTING,
	.priority = NF_IP_PRI_FIRST,
};

/*Proc read function*/
static ssize_t proc_read(struct file* file, char __user* user_buffer, size_t count, loff_t* offset){
	char data[128];
	int data_len;

	if(*offset > 0)
		return 0;
	data_len = snprintf(data, sizeof(data), "TCP packets: %d\nUDP packets: %d\nICMP packets: %d\n", tcp_count, udp_count, icmp_count);

	if(copy_to_user(user_buffer, data, data_len))
		return -EFAULT;
	*offset = data_len;
	return data_len;
}

/*Proc write function*/
static ssize_t proc_write_options(struct file* file, const char __user *buffer, size_t len, loff_t *offset){
	char input[16];

	if(*offset > 0)
		return 0;

	if(len > sizeof(input)-1)
		return -EINVAL;
	
	if(copy_from_user(input, buffer, len))
		return -EFAULT;

	input[len] = '\0';
	printk(KERN_INFO "INPUT: %s", input);

	if(strncmp(input, "reset",5) == 0){
		tcp_count = 0;
		udp_count = 0;
		icmp_count = 0;
		printk(KERN_INFO "Packet counters reset\n");

	} else
		printk(KERN_WARNING "Invalid command: %s\n",input);

	return len;
}

/*Proc file struct*/
static const struct proc_ops p_ops = {
	.proc_read = proc_read,
	.proc_write = proc_write_options,
};

static int __init packet_counter_init(void){
	int ret;

	printk(KERN_INFO "Packet Counter Module loaded\n");
	
	/*Register Netfilter hook*/
	ret = nf_register_net_hook(&init_net, &netfilter_ops);
	if(ret){
		printk(KERN_ERR "Failed to reister Netfilter hook. Err %d \n", ret);
		return ret;
	}

	/*Craete Proc file*/
	if(!proc_create("packet_counter", 0666, NULL, &p_ops)){
		nf_unregister_net_hook(&init_net, &netfilter_ops);
		printk(KERN_ERR "Failed to craete proc file. Unregistered net_hook");
		return -ENOMEM;
	}
	printk(KERN_INFO "Netfilter hook registered. Proc file init done\n");
	return 0;
}

static void __exit packet_counter_exit(void){
	remove_proc_entry("packet_counter", NULL);
	nf_unregister_net_hook(&init_net, &netfilter_ops);
	printk(KERN_INFO "Packet Counter Module un-loaded\n");
}

module_init(packet_counter_init);
module_exit(packet_counter_exit);
