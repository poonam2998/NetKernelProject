#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/proc_fs.h>
#include <linux/types.h>

#include "../include/mac_table.h"

/* MAC table hash function 
 * (XOR based hash function)
 */
unsigned int mac_hash(unsigned char* mac_address){
	unsigned int hash_value = 0;
	
	hash_value = mac_address[0] ^ mac_address[1] ^ mac_address[2] ^ mac_address[3] ^ mac_address[4] ^ mac_address[5];
	printk(KERN_INFO "Hash Function value for mac %s: %d\n", mac_address, hash_value);
	
	return hash_value;
}

#ifdef MAC_TABLE_V2
/*Define MAC Table*/
static DEFINE_HASHTABLE(mac_table, MAC_TABLE_BITS);

void mac_table_init()
{
	hash_init(mac_table);
	printk(KERN_INFO "MAC table Init\n");
}

void mac_address_learn(unsigned char *mac_address, const char* port){
	
	struct mac_table_entry* entry_lookup = NULL;
	/*Lookup mac_address*/
	entry_lookup = mac_address_lookup(mac_address);
	
	/*if not present, then add entry to hash table*/
	if(!entry_lookup){
		/*Generate hash value*/
		int hash_value = mac_hash(mac_address);
		
		/*Create new hash entry and init*/
		struct mac_table_entry new_entry;
		strncpy(new_entry.mac_addr, mac_address, strlen(mac_address));
		strncpy(new_entry.port, port, strlen(port));

		/*add to linked list*/
		hash_add(mac_table, &new_entry.next, hash_value);
	}
}

/*MAC address Lookup*/
struct mac_table_entry* mac_address_lookup(unsigned char *mac_address){
	/*Check if hash table empty, Return NULL.*/
	if(hash_empty(mac_table)){
		printk(KERN_INFO "MAC table Empty\n");
		return NULL;
	}

	/*search for entry in Linked list*/
	struct mac_table_entry *entry; 
	int bkt;

	hash_for_each(mac_table, bkt, entry, next) {
		printk(KERN_INFO "[mac_table] entry %s %s \n", entry->mac_addr, entry->port);
		if(strcmp(entry->mac_addr, mac_address)){
			printk(KERN_INFO "Found matching entry in mac_table");
			return entry;
		}
	}

	/*Reach here if entry not found. return NULL */
	return NULL;
}

#elif
/*Init MAC table*/
int mac_table_init(struct mac_table* table){

	int i = 0;
	/*Init all buckets as NULL*/
	for(i = 0;i<MAC_TABLE_SIZE;i++){
		table->bucket[i] = NULL;
	}

	printk(KERN_INFO "MAC table Init\n");

	return 0;
}



/*MAC address learn*/
void mac_address_learn(struct mac_table* table, unsigned char *mac_address, const char* port){
	
	struct mac_entry* entry_lookup = NULL;
	/*Lookup mac_address*/
	entry_lookup = mac_address_lookup(table, mac_address);
	
	/*if not present, then add entry to hash table*/
	// if(!entry_lookup){
	// 	/*Generate hash value*/
	// 	int hash_value = mac_hash(mac_address);
		
	// 	/*Create new hash entry and init*/
	// 	struct mac_entry* new_node = kmalloc(sizeof(struct mac_entry),GFP_KERNEL);
	// 	memcpy(new_node->mac_addr, mac_address, ETH_ALEN);
	// 	strcpy(new_node->port, port);
	// 	new_node->next = NULL;

	// 	/*add to linked list*/
	// 	new_node = table->bucket[hash_value]->next;
	// 	table->bucket[hash_value]->next = new_node;
	// 	printk(KERN_INFO "[mac_table] Added entry %s, %d\n", new_node->mac_addr, new_node->port);
	// }
}

/*MAC address Lookup*/
struct mac_entry* mac_address_lookup(struct mac_table*  table, unsigned char *mac_address){
	
	/*Generate hash value*/
	int hash_value = mac_hash(mac_address);

	/*search for entry in Linked list*/
	struct mac_entry* current_entry = (table->bucket[hash_value])->next;
	while(current_entry)
	{
		printk(KERN_INFO "[mac_table] entry %s, %s\n", current_entry->mac_addr, current_entry->port);
		if(memcmp(current_entry->mac_addr, mac_address,6)==0){
			/*entry found, return entry*/
			printk(KERN_INFO "[mac_table] Found entry %s, %s \n", current_entry->mac_addr, current_entry->port);
			return current_entry;
		}
		current_entry = current_entry->next;
	}
	return NULL;
}

/*Deinit MAC table*/
void mac_table_deinit(struct mac_table* table){

	int i = 0;
	struct mac_entry *entry, *temp;

	/*Deinit all entries*/
	for(i = 0;i<MAC_TABLE_SIZE;i++){
		entry = table->bucket[i];
		while (entry){
			temp = entry->next;
			kfree(entry);
			entry = temp;		
		}
		table->bucket[i] = NULL;
	}
	kfree(table);
	printk(KERN_INFO "MAC table Deinit done\n");
}
#endif
