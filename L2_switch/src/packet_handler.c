#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/proc_fs.h>

/*Custom Header files*/
#include "../include/packet_handler.h"

/*Hook function*/
static unsigned int packet_handler_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state){
	struct ethhdr *eth_header;

	/*Is packet valid*/
	if(!skb)
		return NF_ACCEPT;

	/*Extract eth header*/
	eth_header = eth_hdr(skb);
	printk(KERN_INFO "[packet_handler] Dest MAC:%pM, Src MAC: %pM, Proto:%d\n", eth_header->h_dest, eth_header->h_source, eth_header->h_proto); 

	return NF_ACCEPT; 
}

/*Hook Structure*/
static struct nf_hook_ops netfilter_ops = {
	.hook = packet_handler_hook,
	.pf = NFPROTO_IPV4,
	.hooknum = NF_INET_PRE_ROUTING,
	.priority = NF_IP_PRI_FIRST,
};

/*Register Netfilter Hook*/
int packet_handler_init(void){
	int ret;

	/*Register Netfilter hook*/
	ret = nf_register_net_hook(&init_net, &netfilter_ops);
	if(ret){
		printk(KERN_ERR "Failed to register Netfilter hook. Err %d \n", ret);
		return ret;
	}

	printk(KERN_INFO "[packet_handler] Netfilter hook registered.\n");
	return 0;
}

/*Unregister Netfilter hook*/
void packet_handler_deinit(void){
	nf_unregister_net_hook(&init_net, &netfilter_ops);
	printk(KERN_INFO "[packet_handler] Netfilter hook un-registered.\n");
}
