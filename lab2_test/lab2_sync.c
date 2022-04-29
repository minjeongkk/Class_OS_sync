/*
*   DKU Operating System Lab
*           Lab2 (Hash Queue Lock Problem)
*           Student id :
*           Student name :
*
*   lab2_sync.c :
*       - lab2 main file.
*		- Thread-safe Hash Queue Lock code
*		- Coarse-grained, fine-grained lock code
*       - Must contains Hash Queue Problem function's declations.
*
*   Implement thread-safe Hash Queue for coarse-grained verison and fine-grained version.
*/

#include <aio.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab2_sync_types.h"

pthread_mutex_t mutex_lock;
/*
 * TODO
 *  Implement function which init queue nodes for front and rear
 *  ( refer to the ./include/lab2_sync_types.h for front and rear nodes)
 */
void init_queue() {
	// You need to implement init_queue function.
	front = NULL;
	rear = NULL;
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue.
 */
void enqueue(queue_node* new_node) {
	// You need to implement enqueue function.
	if (front == NULL) {
		front = new_node;
		rear = new_node;
	}
	else {
		rear->next = new_node;
		new_node->prev = rear;
		rear = new_node;
	}
	
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in coarse-grained manner.
 */
void enqueue_cg(queue_node* new_node) {
	// You need to implement enqueue_cg function.
	pthread_mutex_lock(&mutex_lock);
	if (front == NULL) {
		front = new_node;
		rear = new_node;
	}
	else {
		rear->next = new_node;
		new_node->prev = rear;
		rear = new_node;
	}
	pthread_mutex_unlock(&mutex_lock);
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in fine-grained manner.
 */
void enqueue_fg(queue_node* new_node) {
	// You need to implement enqueue_fg function.
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue.
 */
void dequeue(queue_node* del_node) {
	// You need to implement dequeue function.
	if (del_node->prev == NULL) {
		del_node->next->prev = NULL;
		front = del_node->next;
	}
	else if (del_node->next == NULL) {
		del_node->prev->next = NULL;
		rear = del_node->prev;
	}
	else {
		del_node->prev->next = del_node->next;
		del_node->next->prev = del_node->prev;
	}
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in coarse-grained manner.
 */
void dequeue_cg(queue_node* del_node) {
	// You need to implement dequeue_cg function.
	pthread_mutex_lock(&mutex_lock);
	if (del_node->prev == NULL) {
		del_node->next->prev = NULL;
		front = del_node->next;
	}
	else if (del_node->next == NULL) {
		del_node->prev->next = NULL;
		rear = del_node->prev;
	}
	else {
		del_node->prev->next = del_node->next;
		del_node->next->prev = del_node->prev;
	}
	pthread_mutex_unlock(&mutex_lock);
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in fine-grained manner.
 */
void dequeue_fg(queue_node* del_node) {
	// You need to implement dequeue_fg function.
}

/*
 * TODO
 *  Implement function which init hashlist(same as hashtable) node.
 */
void init_hlist_node() {
	// You need to implement init_hlist_node function.
	int i;
	for (i = 0; i < HASH_SIZE; i++) {
		hashlist[i] = NULL;
		//hashlist[i]->q_loc = NULL;
		//hashlist[i]->next = NULL;
	}
}

/*
 * TODO
 *  Implement function which calculate hash value with modulo operation.
 */
int hash(int val) {
	// You need to implement hash function.
	int bucket = val % HASH_SIZE;
	return bucket;
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add(hlist_node* hashtable, int val) {
	// You need to implement hash_queue_add function.
	int bucket = hash(val);

	if (hashlist[bucket] == NULL) {
		hashlist[bucket] = hashtable;
	}
	else {
		hlist_node* newhashtable;
		newhashtable = hashlist[bucket];
		while (newhashtable != NULL) {
			if (newhashtable->next == NULL) {
				newhashtable->next = hashtable;
				break;
			}
			newhashtable = newhashtable->next;
		}
	}
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = val;
	newNode->prev = NULL;
	newNode->next = NULL;
	enqueue(newNode);
	hashtable->q_loc = newNode;
	
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add_cg(hlist_node* hashtable, int val) {
	// You need to implement hash_queue_add_cg function.
	pthread_mutex_lock(&mutex_lock);
	int bucket = hash(val);

	if (hashlist[bucket] == NULL) {
		hashlist[bucket] = hashtable;
	}
	else {
		hlist_node* newhashtable;
		newhashtable = hashlist[bucket];
		while (newhashtable != NULL) {
			if (newhashtable->next == NULL) {
				newhashtable->next = hashtable;
				break;
			}
			newhashtable = newhashtable->next;
		}
	}
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = val;
	newNode->prev = NULL;
	newNode->next = NULL;
	enqueue(newNode);
	hashtable->q_loc = newNode;
	pthread_mutex_unlock(&mutex_lock);
}

/*
 * TODO
 *  Implement function which insert the resilt of finding the location
 *  of the bucket using value to the entry and hashtable
 *
 *  @param hlist_node *hashtable		: A pointer variable containing the bucket
 *  @param int val						: Data to be stored in the queue node
 */
void hash_queue_add_fg(hlist_node* hashtable, int val) {
	// You need to implement hash_queue_add_fg function.
}

/*
 * TODO
 *  Implement function which check if the data(value) to be stored is in the hashtable
 *
 *  @param int val						: variable needed to check if data exists
 *  @return								: status (success or fail)
 */
int value_exist(int val) {
	// You need to implement value_exist function.
	int bucket = hash(val); //bucket 값
	hlist_node* newhashtable;
	newhashtable = hashlist[bucket];
	while (newhashtable) {
		if (newhashtable->q_loc->data == val) { 
			return 1; // 값을 찾은 경우
		}
		newhashtable = newhashtable->next;
	}
	return 0; // 값을 못 찾은 경우
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target() {
	// You need to implement hash_queue_insert_by_target function.
	hlist_node* newhashtable = (hlist_node*)malloc(sizeof(hlist_node));
	newhashtable->next = NULL;
	newhashtable->q_loc = NULL;
	hash_queue_add(newhashtable, target);

}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_cg() {
	// You need to implement hash_queue_insert_by_target_cg function.
	hlist_node* newhashtable = (hlist_node*)malloc(sizeof(hlist_node));
	pthread_mutex_lock(&mutex_lock);
	newhashtable->next = NULL;
	newhashtable->q_loc = NULL;
	hash_queue_add(newhashtable, target);
	pthread_mutex_unlock(&mutex_lock );
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_fg() {
	// You need to implement hash_queue_insert_by_target_fg function.
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target() {
	// You need to implement hash_queue_delete_by_target function.
	int bucket = hash(target);
	int find = value_exist(target);
	hlist_node* del_hashlist;
	del_hashlist = hashlist[bucket];
	hlist_node* prev_hashlist = (hlist_node*)malloc(sizeof(hlist_node));
	queue_node* delNode = (queue_node*)malloc(sizeof(queue_node));
	if (find == 1) {
		while (del_hashlist != NULL) {
			if (del_hashlist->q_loc->data == target) {
				prev_hashlist->next = del_hashlist->next;
				delNode = del_hashlist->q_loc;
				break;
			}
			prev_hashlist = del_hashlist;
			del_hashlist = del_hashlist->next;
		}

		dequeue(delNode);
	}
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_cg() {
	// You need to implement hash_queue_delete_by_target_cg function.
	pthread_mutex_lock(&mutex_lock);
	int bucket = hash(target);
	int find = value_exist(target);
	hlist_node* del_hashlist;
	del_hashlist = hashlist[bucket];
	hlist_node* prev_hashlist = (hlist_node*)malloc(sizeof(hlist_node));
	queue_node* delNode = (queue_node*)malloc(sizeof(queue_node));
	if (find == 1) {
		while (del_hashlist != NULL) {
			if (del_hashlist->q_loc->data == target) {
				prev_hashlist->next = del_hashlist->next;
				delNode = del_hashlist->q_loc;
				break;
			}
			prev_hashlist = del_hashlist;
			del_hashlist = del_hashlist->next;
		}

		dequeue(delNode);
	}
	pthread_mutex_unlock(&mutex_lock);
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_fg() {
	// You need to implement hash_queue_delete_by_target_fg function.
}

