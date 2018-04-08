#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define A_SIZE 128
#define TERMINATOR 1

typdef struct index_range {
    int start;
    int end;
} index_range_t;

typedef struct trie_node {
    unsigned int is_eow;
    struct trie_node *children[A_SIZE];
} trie_node_t;

typedef struct compressed_trie_node {
    unsigned int is_eow;
    unsigned int no_children;
    index_range_t *children;
} compressed_trie_node_t;

trie_node_t *insert_word(trie_node_t *trie, char *word) {
    if (!word)
        return trie;
    int i=0, l = strlen(word) + TERMINATOR;
    trie_node_t *new_trie;
    
    if (!trie) { 
        new_trie = (trie_node_t *)calloc(1, sizeof(trie_node_t));
        if (!new_trie)
            return NULL;
    } else {
        new_trie = trie;
    }
    
    trie_node_t *current = new_trie;
    while (i<l) {
        if (!current->children[(unsigned int)word[i]]) {
            current->children[(unsigned int)word[i]] = (trie_node_t *)calloc(1, sizeof(trie_node_t));
        }
        current = current->children[(unsigned int)word[i]];
        i++;
    }
    current->is_eow = 1;
    
    return new_trie;
}

int search_word(trie_node_t *trie, char *word) {
    if (!trie || !word)
        return -1;
    int i=0, l = strlen(word) + TERMINATOR;
    trie_node_t *current = trie;
    
    while (i<l && current && current->children[(unsigned int)word[i]]) {
        current = current->children[(unsigned int)word[i]];
        i++;
    }
    
    if (current->is_eow && i==l)
        return 1;
    
    return 0;
}

int delete_word(trie_node_t *trie, char *word, int where) {
    if (!trie || !word)
        return -1;
    int i, l = strlen(word) + TERMINATOR;
    int has_children = 0;
    
    if (where == l) {
        if (trie->is_eow) {
            trie->is_eow = 0;
            
            for (i=0; i<A_SIZE; i++)
                if (trie->children[i]) {
                    has_children = 1;
                    break;
                }
                
            if (!has_children)
                return 1;
        }
        return 0;
    }
    
    if (delete_word(trie->children[(unsigned int)word[where]], word, where+1)) {
        free(trie->children[(unsigned int)word[where]]);
        trie->children[(unsigned int)word[where]] = NULL;
        
        for (i=0; i<A_SIZE; i++)
            if (trie->children[i]) {
                has_children = 1;
                break;
            }
            
        if (!has_children)
            return 1;
    }
    return 0;
}

void print_node(trie_node_t *node, int level) {
    if (!node)
        return;
    int i;
    
    for (i=0; i<A_SIZE; i++)
        if (node->children[i]) {
            if (node->children[i]->is_eow)
                printf("level %d: char '%c' - EOW\n", level, (char)i);
            else
                printf("level %d: char '%c'\n", level, (char)i);
            print_node(node->children[i], level+1);
        }
}

void free_trie(trie_node_t *node) {
    if (!node || node->is_eow) {
        free(node);
        return;
    }
    int i;
    
    for (i=0; i<A_SIZE; i++)
        if (node->children[i]) {
            free_trie(node->children[i]);
        }
    free(node);
}

compressed_trie_node_t *insert_word_c(compressed_trie_node_t *ctrie, char *word) {
    if (!word)
        return ctrie;
    
    int i, j=0, l = strlen(word) + TERMINATOR;
    if (!ctrie) {
        ctrie = (compressed_trie_node_t *)calloc(1, sizeof(compressed_trie_node_t));
        if (!ctrie)
            return NULL;
        ctrie->children[no_children++]->end = l-1;
    } else {
        compressed_trie_node_t *current = ctrie;
        
        while (j<l) {
            for (i=0; i<ctrie->no_children; i++) {
                if (ctrie->children[i]-> == word)
                    break;
            }
            
        }
    }
    
    return ctrie;
    
    
}

int main() {
    
    trie_node_t *trie = insert_word(NULL, "baluba");
    trie = insert_word(trie, "balerina");
    print_node(trie, 0);
    
    printf("baluba=%d\n",search_word(trie, "baluba"));
    printf("baluban=%d\n",search_word(trie, "baluban"));
    printf("bal=%d\n",search_word(trie, "bal"));
    printf("balerina=%d\n",search_word(trie, "balerina"));
    printf("bbalerina=%d\n",search_word(trie, "bbalerina"));
    printf("balerin=%d\n",search_word(trie, "balerin"));
    
    trie_node_t *trie2 = insert_word(NULL, "the");
    trie2 = insert_word(trie2, "a");
    trie2 = insert_word(trie2, "there");
    trie2 = insert_word(trie2, "answer");
    trie2 = insert_word(trie2, "any");
    trie2 = insert_word(trie2, "by");
    trie2 = insert_word(trie2, "bye");
    trie2 = insert_word(trie2, "their");
    delete_word(trie2, "answer", 0);
    print_node(trie2, 0);
    
    free_trie(trie);
    free_trie(trie2);
    
    return 0;
}
