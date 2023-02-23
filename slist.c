
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "slist.h"

/** Initialize a double linked list
	\param list - the list to initialize */
void dbllist_init(dbllist_t *list){
    //initialize the list
    dbllist_head(list) = NULL;
    dbllist_tail(list) = NULL;
    dbllist_size(list) = 0;
}

/** Destroy and de-allocate the memory hold by a list
	\param list - a pointer to an existing list
	\param dealloc flag that indicates whether stored data should also be de-allocated */
void dbllist_destroy(dbllist_t *list,dbllist_destroy_t dealloc){
    if(list == NULL){
        return;
    }
    dbllist_node_t * node_ptr;
//run on the list as long head is not null
    while(dbllist_head(list) != NULL){
        node_ptr = dbllist_head(list);
        dbllist_head(list) = dbllist_next(dbllist_head(list));

        //want to free the data of the node
        if(dealloc == DBLLIST_FREE_DATA){
            free(node_ptr->data);
            node_ptr->data = NULL;}
        free(node_ptr);
        node_ptr = NULL;
    }
        dbllist_size(list) = 0;
}


/** Append data to list (add as last node of the list)
	\param list - a pointer to a list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure */
int dbllist_append(dbllist_t *list, void *data) {
    dbllist_node_t *Node = (dbllist_node_t*)malloc(sizeof (dbllist_node_t));
    if(Node == NULL){
        perror("malloc fail!");
        return -1;
    }
    dbllist_data(Node) = data;
    dbllist_next(Node) = NULL;

    if(dbllist_head(list) == NULL) { //if the list is empty
        dbllist_prev(Node) = NULL;
        dbllist_head(list) = Node;
        dbllist_tail(list) = Node;
        dbllist_size(list)++;
        return 0;
    }
    dbllist_prev(Node) = dbllist_tail(list);
    dbllist_next(dbllist_tail(list)) = Node;
    dbllist_tail(list) = Node;
    dbllist_size(list)++;
    return 0;
}

/** Prepend data to list (add as first node of the list)
	\param list - a pointer to list
	\param data - the data to place in the list
	\return 0 on success, or -1 on failure
*/
int dbllist_prepend(dbllist_t *list,void *data){
    dbllist_node_t *Node = (dbllist_node_t*)malloc(sizeof (dbllist_node_t));
    if(Node == NULL){
        perror("malloc fail!");
        return -1;
    }
    dbllist_data(Node) = data;
    if(dbllist_head(list) == NULL){ //if the list is empty
        dbllist_prev(Node) = NULL;
        dbllist_next(Node) = NULL;
        dbllist_head(list) = Node;
        dbllist_tail(list) = Node;
        dbllist_size(list)++;
        return 0;
    }
    dbllist_prev(Node) = NULL;
    dbllist_next(Node) = dbllist_head(list);
    dbllist_prev(dbllist_head(list)) = Node;
    dbllist_head(list) =Node;
    dbllist_size(list)++;
    return 0;

}

/** \brief Remove the specific node from the list.
	\param list a pointer to the list
	\param pointer to the node that should be removed.
	\param dealloc flag that indicates whether to de-allocated the data in the node
	\return 0 on success, or -1 on failure
*/
int dbllist_remove(dbllist_t *list, dbllist_node_t* pointer,dbllist_destroy_t dealloc) {
    if(list == NULL)
        return -1;
    if (dbllist_head(list) == NULL )
        return -1;

    if(pointer == NULL)
        return -1;

    //first spot
    if (dbllist_data(dbllist_head(list)) == dbllist_data(pointer)){
        //first spot and only one node
         if (dbllist_size(list) == 1) {
            dbllist_data(dbllist_head(list)) = NULL;
            dbllist_prev(dbllist_head(list)) = NULL;
            dbllist_next(dbllist_head(list)) = NULL;
             dbllist_head(list) = NULL;
            dbllist_tail(list) = NULL;
            dbllist_size(list)--;
            if (dealloc == DBLLIST_FREE_DATA){
                free(dbllist_data(pointer));
                free(pointer);
                pointer = NULL;}
             if (dealloc == DBLLIST_LEAVE_DATA){
                 free(pointer);
                 pointer = NULL;}
            return 0;
        }
        dbllist_head(list) = dbllist_next(dbllist_head(list));
        dbllist_prev(dbllist_head(list)) = NULL;
        dbllist_next(dbllist_head(list)) = dbllist_next(dbllist_head(list));
        dbllist_size(list)--;
        if (dealloc == DBLLIST_FREE_DATA){
            free(pointer->data);
            free(pointer);
            pointer = NULL;}
        if (dealloc == DBLLIST_LEAVE_DATA){
            free(pointer);
            pointer = NULL;}
        return 0;
}

//the list is not size 1
    int flag = 0;
    dbllist_node_t* node_ptr = dbllist_head(list);
    while(dbllist_next(node_ptr) != NULL){
        if(dbllist_data(dbllist_next(node_ptr)) == dbllist_data(pointer) && dbllist_next(dbllist_next(node_ptr)) == NULL){ //last spot
            dbllist_next(node_ptr) = NULL;
            dbllist_next(pointer) = NULL;
            dbllist_prev(pointer) = NULL;
            dbllist_size(list)--;
            flag = 1;
            if (dealloc == DBLLIST_FREE_DATA){
                free(pointer->data);
                free(pointer);
                pointer = NULL;
                break;}
            if (dealloc == DBLLIST_LEAVE_DATA){
                free(pointer);
                pointer = NULL;
                break;}
        }
        if(dbllist_data(dbllist_next(node_ptr)) == dbllist_data(pointer)){
            dbllist_next(node_ptr) =  dbllist_next(dbllist_next(node_ptr));
            dbllist_prev(dbllist_next(pointer)) = node_ptr;
            dbllist_next(pointer) = NULL;
            dbllist_prev(pointer) = NULL;
            dbllist_data(pointer) = NULL;
            dbllist_size(list)--;
            flag = 1;
            if (dealloc == DBLLIST_FREE_DATA){
                free(pointer->data);
                free(pointer);
                pointer = NULL;
                break;}
            if (dealloc == DBLLIST_LEAVE_DATA){
                free(pointer);
                pointer = NULL;
                break;}
        }
        node_ptr = dbllist_next(node_ptr);
    }
    if(flag == 1)
        return 0;
    //the node not found!
    else
        return -1;
}
