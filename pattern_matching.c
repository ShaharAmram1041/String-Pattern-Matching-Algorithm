
#include "pattern_matching.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



//private function, recursive delete the allocated memory
void pm_destroy_rec(pm_state_t* );

/* Initializes the fsm parameters (the fsm itself should be allocated).  Returns 0 on success, -1 on failure.
*  this function should init zero state
*/
int pm_init(pm_t *FSM){
    if(FSM == NULL)
        return -1;
    //the id of the next state
    FSM->newstate  = 1;
    //alloc the root
    FSM->zerostate = (pm_state_t*) malloc(sizeof (pm_state_t));
    if(FSM->zerostate == NULL){
        perror("malloc fail!");
        return  -1;
    }
    //state S0
    FSM->zerostate->id = 0;
    //depth is in line 0
    FSM->zerostate->depth = 0;
    //create the output list
    FSM->zerostate->output = (dbllist_t*) malloc(sizeof (dbllist_t));
    if(FSM->zerostate->output == NULL){
        perror("malloc fail!");
        free(FSM->zerostate);
        return  -1;
    }
    //fail function of the root state
    FSM->zerostate->fail = NULL;
    //create the list of transitions
    FSM->zerostate->_transitions = (dbllist_t*) malloc(sizeof (dbllist_t));
    if(FSM->zerostate->_transitions == NULL){
        perror("malloc fail!");
        free(FSM->zerostate);
        free(FSM->zerostate->output);
        return  -1;
    }
    //initialize the two lists
    dbllist_init(FSM->zerostate->_transitions);
    dbllist_init(FSM->zerostate->output);
    return  0;
}


/* Adds a new string to the fsm, given that the string is of length n.
   Returns 0 on success, -1 on failure.*/
int pm_addstring(pm_t *FSM,unsigned char *c,_size_t n) {
    if(FSM == NULL)
        return -1;

    if(n == 0)
        return -1;

    //edge case: check if the size of n is not equal to the pattern
    int j;
    for(j = 0; c[j] != '\0' ;j++){
    }
    if(j != n)
        return -1;

    int i = 0;
    //pointer to the root
    pm_state_t *t_ptr = FSM->zerostate;
    //check if there is the same letter in the tree already
    while(pm_goto_get(t_ptr,c[i]) != NULL){
        t_ptr = pm_goto_get(t_ptr,c[i]);
        i++;
    }
    //create new nodes for the tree
    for (; i < n; i++) {
        //edge case, if one of the letters is ' '
        if(c[i] == ' ')
            continue;
        pm_state_t* next_state = (pm_state_t*) malloc(sizeof (pm_state_t));
        if(next_state == NULL){
            perror("malloc fail!");
            return -1;
        }
        printf("Allocating state %d",FSM->newstate);
        next_state->id = FSM->newstate;
        FSM->newstate = FSM->newstate + 1;
        //create for each node 2 lists
        next_state->_transitions = (dbllist_t*) malloc(sizeof (dbllist_t));
        if(next_state->_transitions == NULL){
            perror("malloc fail!");
            return -1;
        }
        next_state->output = (dbllist_t*) malloc(sizeof (dbllist_t));
        if(next_state->output == NULL){
            perror("malloc fail!");
            return -1;
        }
        //initialize the lists
        dbllist_init(next_state->_transitions);
        dbllist_init(next_state->output);
        //set the state with the letter on the tree
        if(pm_goto_set(t_ptr,c[i],next_state) != -1)
            t_ptr = next_state;
}
    //add the entire word to the output list in the last node :in get state
        dbllist_append(t_ptr->output,c);
    return 0;
}



/* Set a transition arrow from this from_state, via a symbol, to a
   to_state. will be used in the pm_addstring and pm_makeFSM functions.
   Returns 0 on success, -1 on failure.*/
int pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state){
    if(from_state == NULL || to_state == NULL){
        return -1;
    }
    //allocate edge between the states
    pm_labeled_edge_t* edge = (pm_labeled_edge_t*) malloc(sizeof (pm_labeled_edge_t));
    if(edge == NULL){
        perror("malloc fail!");
        return -1;
    }
    //update the 'to_state' node
    to_state->depth = from_state->depth + 1;
    to_state->fail = NULL;
    //update the edge between the states
    edge->state = to_state;
    edge->label = symbol;
    //add the edge to the transitions list
    if(dbllist_append(from_state->_transitions, edge) == -1)
        return -1;
    printf("\n%d -> %c -> %d\n",from_state->id, symbol, to_state->id);
    return 0;
}

/* Returns the transition state.  If no such state exists, returns NULL.
   will be used in pm_addstring, pm_makeFSM, pm_fsm_search, pm_destroy functions. */
pm_state_t* pm_goto_get(pm_state_t *state,unsigned char symbol){
    if(state == NULL){
        return NULL;
    }
    pm_state_t * result = NULL;
    //pointer to the state transitions list
    dbllist_node_t* t_ptr = dbllist_head(state->_transitions);
    pm_labeled_edge_t* edge;
    //run on the tree and search for the symbol
    while(t_ptr != NULL){
        edge = (pm_labeled_edge_t*)dbllist_data(t_ptr);
        //if find the same letter
        if(edge->label == symbol){
            result = edge->state;
            break;
        }
    t_ptr = dbllist_next(t_ptr);
}
    return result;
}

/* Finalizes construction by setting up the failure transitions, as
   well as the goto transitions of the zerostate.
   Returns 0 on success, -1 on failure.*/
int pm_makeFSM(pm_t * FSM) {
    if (FSM == NULL) {
        return -1;
    }
    //alloc the failure list
    dbllist_t *fail = (dbllist_t *) malloc(sizeof(dbllist_t));
    if (fail == NULL) {
        perror("malloc fail!\n");
        return -1;
    }
    //initialize the fail list
    dbllist_init(fail);
    //create pointer to each information structures
    pm_state_t *t_ptr = FSM->zerostate;
    dbllist_node_t *trans_head = dbllist_head(FSM->zerostate->_transitions);
    pm_labeled_edge_t *edge;
    int dbl_size = dbllist_size(t_ptr->_transitions);

    //first step,depth 1, all the fail of the nodes point to the root
    while (dbl_size > 0) {
        edge = dbllist_data(trans_head); //edge is the data between the states
        dbllist_append(fail, dbllist_data(trans_head)); //insert the states into the fail list
        edge->state->fail = FSM->zerostate;
        edge = dbllist_data(trans_head);
        trans_head = dbllist_next(trans_head);
        dbl_size--;
    }
    pm_state_t *p_state;
    pm_state_t *r;

//run on the fail list according to the algorithm
    while (dbllist_size(fail) > 0) {
        //r is the next state in the queue
        r = ((pm_labeled_edge_t *) dbllist_data(dbllist_head(fail)))->state;

        //dequeue the r from the list but leave the data if we want to use it
        if (dbllist_remove(fail, dbllist_head(fail), DBLLIST_LEAVE_DATA) == -1)
            return -1;
        //start to run on the fail list
        dbllist_node_t * head_of_trans = dbllist_head(r->_transitions);

        //run on the transitions list of each variable of the fail list
        while (head_of_trans != NULL) {
            dbllist_node_t * next_of_trans = dbllist_next(head_of_trans);
            if (dbllist_append(fail, dbllist_data(head_of_trans)) == -1) {
                return -1;
            }
            p_state = r->fail;
            pm_state_t *s = ((pm_labeled_edge_t *) (dbllist_data(head_of_trans)))->state;
            while (pm_goto_get(p_state, ((pm_labeled_edge_t *) (dbllist_data(head_of_trans)))->label) == NULL) {
                p_state = p_state->fail;
                if(p_state == NULL){ //point to the root
                    s->fail = FSM->zerostate;
                    break;
                }
            }
            if(p_state != NULL){
            s->fail = pm_goto_get(p_state, ((pm_labeled_edge_t *) (dbllist_data(head_of_trans)))->label);
            //now we append each of the fail data into the output list
            dbllist_node_t * output_node = dbllist_head(s->fail->output);
            for(int i = 0; i < dbllist_size(s->fail->output);i++){
                if(dbllist_append(s->output, dbllist_data(output_node)) == -1)
                    return -1;
                output_node = dbllist_next(output_node);
            }}
            printf("Setting f(%d) = %d\n", s->id, s->fail->id);
            head_of_trans = next_of_trans;
        }
    }
    //free the failure list
    free(fail);
        return 0;
    }

/* Search for matches in a string of size n in the FSM.
   if there are no matches return empty list */
dbllist_t * pm_fsm_search(pm_state_t * FSM,unsigned char *c,_size_t n){
    if(n == 0)
        return NULL;
    if(FSM == NULL)
        return NULL;
    if(c[0] == ' ' || c[0] == '\0')
        return NULL;
    //initialize the list and the pm_match struct
    dbllist_t *result = (dbllist_t*) malloc(sizeof (dbllist_t));//the matched list
    if(result == NULL){
        perror("malloc fail!");
        return NULL;
    }
    dbllist_init(result);
    int j;
    pm_state_t * state = FSM ; //state = 0

    //run on the states of each output list according the algorithm
    for(j = 0;j < n; j++) {
        while (pm_goto_get(state, c[j]) == NULL) {
            state = state->fail;
            if (state == NULL)
                break;
        }
        if (state == NULL) {
            state = FSM;
            continue;
        }
        pm_match_t *res = NULL;
        state = pm_goto_get(state, c[j]);

        int output_size = dbllist_size(state->output);
        //if the state is 'get state'
        if (output_size > 0) {
            dbllist_node_t *output_node = dbllist_head(state->output);
            int counter = 0;
            //running on the word and return after to the same location
            while (counter < dbllist_size(state->output)) {
                //create the specific struct of the search match
                res = (pm_match_t *) malloc(sizeof(pm_match_t));
                if (res == NULL) {
                    perror("malloc fail!");
                    return NULL;
                }
                //initialize each field of the struct
                res->fstate = state;
                res->pattern = dbllist_data(output_node);
                res->start_pos = (int) (j - strlen(res->pattern) + 1);
                res->end_pos = j;
                dbllist_append(result, res);
                output_node = dbllist_next(output_node);
                counter++;
            }
        }
    }
    return result;
}

/* Destroys the fsm, deallocating memory. */
void pm_destroy(pm_t * FSM){
    if (FSM == NULL){
        fprintf(stderr,"FSM is null");
        return;
    }
    //using the recursive function
    pm_destroy_rec(FSM->zerostate);
}


//post order recursive run
void pm_destroy_rec(pm_state_t* state) {
    if(state == NULL)
        return ;

//if there is a junction or the there is continuance to the edge
    if(dbllist_size(state->_transitions) >= 1){
        dbllist_node_t * current = dbllist_head(state->_transitions);
        dbllist_node_t * next;
        pm_state_t * edge_state;
        //run on the state transitions of each node
        while (current != NULL){
            next = dbllist_next(current);
            edge_state = ((pm_labeled_edge_t *) (dbllist_data(current)))->state;
            pm_destroy_rec(edge_state);
            //free the states in the transitions list
            free(current->data);
            free(current);
            current = next;
    }
    }

//free the allocated memory lists
    if(dbllist_size(state->_transitions) >= 1){
        free(state->_transitions);
        state->_transitions = NULL;}
    else{
        dbllist_destroy(state->_transitions, DBLLIST_FREE_DATA);}

    dbllist_destroy(state->output,DBLLIST_LEAVE_DATA);
    free(state->output);
    if(state->_transitions != NULL){
        free(state->_transitions);
        state->_transitions = NULL;}
    free(state);
    state = NULL;
}
