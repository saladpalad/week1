#include "linked_list.h"

// Function pointers to (potentially) custom malloc() and
// free() functions.
//
static void * (*malloc_fptr)(size_t size) = NULL;
static void   (*free_fptr)(void* addr)    = NULL; 

struct linked_list * linked_list_create(void){
    struct linked_list* ll = (struct linked_list*) malloc_fptr(sizeof(struct linked_list));
    if(ll != NULL){
        ll->head = NULL;
        return ll;
    }
    return NULL;
}

bool linked_list_delete(struct linked_list * ll){
    if (ll == NULL) return false;
    struct node* head = ll->head;
    while (head != NULL){
        struct node* delete_node = head;
        head = head->next;
        free_fptr(delete_node);
    }
    free_fptr(ll);
    return true;
}

size_t linked_list_size(struct linked_list * ll){
    if (ll == NULL) return SIZE_MAX;
    size_t size = 0;
    struct node* head = ll->head;
    while (head != NULL){
        head = head->next;
        size++;
    }
    return size;
}

bool linked_list_insert_end(struct linked_list * ll,
                            unsigned int data){
    if (ll == NULL) return false;
    struct node* new_node = (struct node*) malloc_fptr(sizeof(struct node));
    if (new_node == NULL) return false;
    new_node->next = NULL;
    new_node->data = data;
    struct node* head = ll->head;
    if(head == NULL){
        ll->head = new_node;
        return true;
    }
    while(head->next != NULL){
        head = head->next;
    }
    head->next = new_node;
    return true;
}

bool linked_list_insert_front(struct linked_list * ll,
                              unsigned int data){
    if (ll == NULL) return false;
    struct node* head = ll->head;
    struct node* new_node = (struct node*) malloc_fptr(sizeof(struct node));
    if (new_node == NULL) return false;
    new_node->next = head;
    new_node->data = data;
    ll->head = new_node;
    return true;
}

bool linked_list_insert(struct linked_list * ll,
                        size_t index,
                        unsigned int data){
    if (ll == NULL) return false;
    struct node* head = ll->head;
    struct node* new_node = (struct node*) malloc_fptr(sizeof(struct node));
    if (new_node == NULL) return false;
    new_node->data = data;
    if (index == 0){
        new_node->next = head;
        ll->head = new_node;
        return true;
    }
    for (size_t i = 0; i < index-1 && head != NULL; i++){
        head = head->next;
    }
    if (head == NULL){ // oob index
        free_fptr(new_node);
        return false;
    }
    new_node->next = head->next;
    head->next = new_node;
    return true;
}

size_t linked_list_find(struct linked_list * ll,
                        unsigned int data){
    if (ll == NULL) return SIZE_MAX;
    struct node* head = ll->head;
    size_t index = 0;
    while (head != NULL){
        if (head->data == data) return index;
        head = head->next;
        index++;
    }
    return SIZE_MAX;
}

bool linked_list_remove(struct linked_list * ll,
                        size_t index){
    if (ll == NULL || ll->head == NULL) return false;
    struct node* head = ll->head;
    if (index == 0){
        struct node* remove_node = head;
        ll->head = head->next;
        free_fptr(remove_node);
        return true;
    }
    for (size_t i = 0; i < index - 1 && head != NULL; i++){
        head = head->next;
    }
    if (head == NULL || head->next == NULL) return false;
    struct node* remove_node = head->next;
    head->next = remove_node->next;
    free_fptr(remove_node);
    return true;
}

struct iterator * linked_list_create_iterator(struct linked_list * ll,
                                              size_t index){
    if (ll == NULL || ll->head == NULL) return NULL;
    struct node* head = ll->head;
    size_t count = 0;
    while (head != NULL){
        if (count == index) break;
        head = head->next;
        count++;
    }
    if (count == index){
        struct iterator* it = (struct iterator*) malloc_fptr(sizeof(struct iterator));
        if (it == NULL) return NULL;
        it->ll = ll;
        it->current_node = head;
        it->current_index = count;
        it->data = head->data;
        return it;
    }
    return NULL;
}

bool linked_list_delete_iterator(struct iterator * iter){
    if (iter == NULL) return false;
    free_fptr(iter);
    return true;
}

bool linked_list_iterate(struct iterator * iter){
    if (iter == NULL || iter->current_node == NULL || iter->current_node->next == NULL) return false;
    iter->current_node = iter->current_node->next;
    iter->current_index++;
    iter->data = iter->current_node->data;
    return true;
}

bool linked_list_register_malloc(void * (*malloc)(size_t)){
    if (malloc == NULL) return false;
    malloc_fptr = malloc;
    return true;
}

bool linked_list_register_free(void (*free)(void*)){
    if(free == NULL) return false;
    free_fptr = free;
    return true;
}
