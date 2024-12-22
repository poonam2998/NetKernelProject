#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/proc_fs.h>
#include <linux/types.h>


#define MAC_TABLE_SIZE	256

struct mac_entry{
	unsigned char mac_addr[ETH_ALEN];
	char port[IFNAMSIZ];
	struct mac_entry* next;
};

struct mac_table{
	struct mac_entry* bucket[MAC_TABLE_SIZE];
};

/*Init MAC table*/
int mac_table_init(struct mac_table* table);

/*MAC table hash function*/
unsigned int mac_hash(unsigned char* mac);

/*MAC address learn*/
void mac_address_learn(struct mac_table* table, unsigned char *mac_address, const char* port);

/*MAC address Lookup*/
struct mac_entry* mac_address_lookup(struct mac_table*  table, unsigned char *mac_address);

void mac_table_deinit(struct mac_table* table);
