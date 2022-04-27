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
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = new_node->data;
	newNode->next = NULL;
	newNode->prev = rear;

	if (front == NULL) { //비었을 때 
		front = newNode;
		rear = newNode;
	}
	else {
		rear->next = newNode;
		rear = newNode;
	}
	printf("data enqueue : %d\n", newNode->data);
	printf("front: %d, rear: %d\n", front->data, rear->data);
}

/*
 * TODO
 *  Implement function which add new_node at next rear node
 *
 *  @param queue_node *new_node		: Node which you need to insert at queue in coarse-grained manner.
 */
void enqueue_cg(queue_node* new_node) {
	// You need to implement enqueue_cg function.
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
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = del_node->data;

	if (front == NULL) {
		printf("Can't dequeue\n");
	}
	else {
		queue_node* checknode = (queue_node*)malloc(sizeof(queue_node));
		checknode = front;
		while (checknode != NULL) {
			if (checknode->data == newNode->data) {
				newNode = checknode;
				if (newNode->prev == NULL) {
					front = newNode->next;
					newNode->next->prev = newNode->prev;
				}
				else if (newNode->next == NULL) {
					rear = newNode->prev;
					newNode->prev->next = newNode->next;
				}
				else {
					rear = newNode->prev;
					newNode->prev->next = newNode->next;
					newNode->next->prev = newNode->prev;
				}
				break;
			}
			checknode = checknode->next;
		}
	}

	printf("data dequeue : %d\n", newNode->data);
	printf("front: %d, rear: %d\n", front->data, rear->data);
}

/*
 * TODO
 *  Implement function which delete del_node at location that contains target key
 *
 *  @param queue_node *del_node		: Node which you need to delete at queue in coarse-grained manner.
 */
void dequeue_cg(queue_node* del_node) {
	// You need to implement dequeue_cg function.
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
	for (i = 0; i < HASH_SIZE - 1; i++) {
		hashlist[i]->q_loc = NULL;
		hashlist[i]->next = NULL;
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
	int num = hash(val);
	hlist_node* newhashtable = (hlist_node*)malloc(sizeof(hlist_node));
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = val;
	newhashtable->next = NULL;
	newhashtable->q_loc = newNode;

	if (hashlist[num] == NULL) {    // 큐가 비어있을 때
		hashlist[num] = newhashtable;
		hashlist[num]->next = newhashtable;
	}
	else {    // 큐가 비어있지 않을 때
		hashlist[num]->next->next = newhashtable; //맨 뒤의 다음이 newNode
		hashlist[num]->next = newhashtable;
	}

	enqueue(newNode);
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
	int rv = 1;
	int bucket = hash(val);
	hlist_node* newhashtable = (hlist_node*)malloc(sizeof(hlist_node));
	newhashtable = hashlist[bucket];
	while (newhashtable) {
		if (newhashtable->q_loc->data == val) {
			rv = 0;
			break;
		}
		newhashtable = newhashtable->next;
	}
	return rv;
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target() {
	// You need to implement hash_queue_insert_by_target function.
	hlist_node* hashtable = (hlist_node*)malloc(sizeof(hlist_node));
	hash_queue_add(hashtable, target);
}

/*
 * TODO
 *  Implement function which find the bucket location using target
 */
void hash_queue_insert_by_target_cg() {
	// You need to implement hash_queue_insert_by_target_cg function.
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
	hlist_node* hashtable = (hlist_node*)malloc(sizeof(hlist_node));
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = target;
	int find = value_exist(newNode->data);
	int bucket = hash(newNode->data);
	hashtable = hashlist[bucket];

	hlist_node* beforetable = (hlist_node*)malloc(sizeof(hlist_node));
	if (find == 0) {
		dequeue(newNode);
		while (hashtable) {
			if (hashtable->q_loc->data == newNode->data) {
				beforetable->next = hashtable->next;
				break;
			}
			hashtable = hashtable->next;
			beforetable = hashtable;
		}
		printf("hashlist[%d] dequeue :%d", bucket, hashtable->q_loc->data);
	}

	else {
		printf("hashlist None! Can't delete\n");
	}
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_cg() {
	// You need to implement hash_queue_delete_by_target_cg function.
}

/*
 * TODO
 *  Implement function which find the bucket location and stored data
 *  using target and delete node that contains target
 */
void hash_queue_delete_by_target_fg() {
	// You need to implement hash_queue_delete_by_target_fg function.
}

