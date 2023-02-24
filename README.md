
# String-Pattern-Matching-Algorithm<br>
Authored by Shahar Amram

## ==Description==

The program is pattern matching algorithm.<br>
This program implement the pattern matching algorithm.<br>
when we do search for a string ot text pattern matching algorithm used to show the search result, that mean if we found the wanted pattern.<br>
the time complexity of the algorithm is linear.<br>

program contain 2 files:

### ***list.c***:

The file contain the execute of double linked list.
in this file we use to create lists that will represent for each state (node) in our tree the "transions" and the "output".

### ***pattern matching.c***:

The file conatain the execute of our FDA tree.<br>
The program simulates tree of states with edges, that contain words/data on the edges.<br>
first, we initalize the tree, then during our program we will add words to the tree, then we will do the failure function of each node that represnt the last show of the word in the tree if there was.<br>
After,we will search on the tree to look the wanted pattern. if we found the same wanted pattern we will add it to list and return it. after all,the profram will make sure to destroy each allocated memory of the program including destroy the FDA tree.<br>

 
## ==Program DATABASE==

1.FSM -type of struct pm_t, the tree, represent by each state (node) and edges between them.

2.States - type of struct pm_state, represent each node of the tree.

3.edge - type of struct pm_labeled_edge_t, represnt the edge between the states as each edge contain word.

4.pm_match - type of struct pm_match_t, represent the matched (if there was) pattern from the text.



## Functions:

1.pm_init(pm_t *) - Initializes the fsm parameters (the fsm itself should be allocated).Returns 0 on success, -1 on failure.this function should init zero state.<br>

2.pm_addstring(pm_t *,unsigned char *, _size_t n) - Adds a new string to the fsm, given that the string is of length n.
Returns 0 on success, -1 on failure.<br>

3.pm_makeFSM(pm_t *) - Finalizes construction by setting up the failrue transitions, as well as the goto transitions of the zerostate.Returns 0 on success, -1 on failure.<br>

4.pm_goto_set(pm_state_t *from_state,unsigned char symbol,pm_state_t *to_state) - Set a transition arrow from this from_state, via a symbol, to a to_state. will be used in the pm_addstring and pm_makeFSM functions. Returns 0 on success, -1 on failure.<br>

5.pm_goto_get(pm_state_t *state,unsigned char symbol) - Returns the transition state.  If no such state exists, returns NULL. will be used in pm_addstring, pm_makeFSM, pm_fsm_search, pm_destroy functions.<br>

6.pm_fsm_search(pm_state_t *,unsigned char *,_size_t) - Search for matches in a string of size n in the FSM. if there are no matches return empty list.<br>

7.pm_destroy(pm_t *) - Destroys the fsm, deallocating memory, using the pm_destroy_rec.<br>

8.pm_destroy_rec(pm_state_t* state) - private function, destroy each node of the tree, include the lists.<br>



## ==Program Files==

slist.c - the file contain the execute of the double linked list.<br>
pattern_matching.c - the file contain the execute of the FSM tree using the 'slist'.<br>


## ==How to compile?==<br>

compile : gcc slist.c pattern_matching.c -o test

run: ./test

## ==Input==<br>

patterns that tree contains.
text that we will search in if there is a match from the tree patterns.

## ==Output==<br>
1. Each time you create new state in the FSM print:
"Allocating state i\n".
2. Each time you create an edge for the goto function from state i to state j in the FSM
print: "I -> a -> j\n", where 'a' is the character that generates this edge.
3. Each time you create an edge for the failure function from state i with depth > 1 to
any state j in the FSM print: "Setting f(i) = j\n"

