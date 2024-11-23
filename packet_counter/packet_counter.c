#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

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

static int __init packet_counter_init(void){
	int ret;

	printk(KERN_INFO "Packet Counter Module loaded\n");
	
	/*Register Netfilter hook*/
	ret = nf_register_net_hook(&init_net, &netfilter_ops);
	if(ret){
		printk(KERN_ERR "Failed to reister Netfilter hook. Err %d \n", ret);
		return ret;
	}
	printk(KERN_INFO "Netfilter hook registered\n");
	return 0;
}

static void __exit packet_counter_exit(void){
	nf_unregister_net_hook(&init_net, &netfilter_ops);
	printk(KERN_INFO "Packet Counter Module un-loaded\n");
}

module_init(packet_counter_init);
module_exit(packet_counter_exit);
