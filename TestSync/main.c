#include <stdio.h>
#include <stdlib.h>
#define HASH_SIZE		13

int target;

typedef struct threadArg {
	int node_count;
	int thread_num;
} thread_arg;

typedef struct q_node {
	struct q_node* next;
	struct q_node* prev;
	int data;
} queue_node;

queue_node* front;
queue_node* rear;

typedef struct hash_list {
	struct hash_list* next;
	struct q_node* q_loc;
} hlist_node;

hlist_node* hashlist[HASH_SIZE];

void init_queue() {
	front = NULL;
	rear = NULL;
}

void init_hlist_node(){
	int i;
	for (i=0; i<HASH_SIZE-1; i++){
		hashlist[i]->q_loc=NULL;
		hashlist[i]->next=NULL;
	}
}

int hash(int val){
	int bucket=val%HASH_SIZE;
	return bucket;
}


int value_exist(int val){
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

void enqueue(queue_node* new_node) {
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

void dequeue(queue_node* del_node) {
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
				else if(newNode->next == NULL) {
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

void hash_queue_add(hlist_node *hashtable, int val){
	int num = hash(val);
	hlist_node *newhashtable = (hlist_node*)malloc(sizeof(hlist_node));
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

void hash_queue_insert_by_target() {
	hlist_node* hashtable = (hlist_node*)malloc(sizeof(hlist_node));
	hash_queue_add(hashtable,target);
}

void hash_queue_delete_by_target() {
	hlist_node* hashtable = (hlist_node*)malloc(sizeof(hlist_node));
	queue_node* newNode = (queue_node*)malloc(sizeof(queue_node));
	newNode->data = target;
	int find=value_exist(newNode->data);
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


int main() {
	init_queue();
	queue_node* newNode1 = (queue_node*)malloc(sizeof(queue_node));
	queue_node* newNode2 = (queue_node*)malloc(sizeof(queue_node));
	queue_node* newNode3 = (queue_node*)malloc(sizeof(queue_node));

	newNode1->data = 1;
	newNode2->data = 2;
	newNode3->data = 3;


	target = 5;
	hash_queue_insert_by_target();
	target = 3;
	hash_queue_insert_by_target();
	target = 16;
	hash_queue_insert_by_target();
	target = 3;
	hash_queue_delete_by_target();

}