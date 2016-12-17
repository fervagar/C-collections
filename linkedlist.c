/*
 * Copyright (C) 2016 Fernando Vañó García
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *      Fernando Vanyo Garcia <fernando@fervagar.com>
 */

#include <stdlib.h>
#include "linkedlist.h"

static void init_list(flist *list) {
        if(list) {
                list->head = list->tail = NULL;
                list->nodes = 0;
        }
}

flist *create_list() {
        flist *p;
        p = malloc(sizeof(*p));
        init_list(p);

        return p;
}

flnode *create_node(void *ptr) {
        flnode *new_node;
        new_node = malloc(sizeof(*new_node));
        new_node->prev = new_node->next = NULL;
        new_node->list = NULL;
        SETPAYLOAD(new_node, ptr);

        return new_node;
}

flnode *remove_node(flnode *node) {
        flnode *aux;
        flist *list;

        if((node) && (node->list)) {
                list = node->list;
                if(!(node->prev)) {        // HEAD node //
                        aux = node;
                        node = node->next;
                        if(node) {
                                node->prev = NULL;
                        }
                        list->head = node;
                }
                else {
                        aux = node->prev;
                        aux->next = node->next;
                        if(aux->next) {
                                (aux->next)->prev = aux;
                        }
                        else {
                                list->tail = aux;
                        }
                        aux = node;
                        node = node->next;
                }
                list->nodes--;
                free(aux);
        }

        return node;
}

/**
 *  Wrapper of 'remove_node()': Extract a single node; the head or the tail of the given list.
 *  The node to pop() is indicated by the 2nd argument: 
 *      If Zero     -> remove head node
 *      If Non-Zero -> remove tail node
 */
flnode *flist_pop(flist *list, int head) {
        if(!(list)) return NULL;
        return remove_node((head)? list->head : list->tail);
}

flnode *add_before(flist *list, flnode *new_node, flnode *existing_node) {
        flnode *aux;

        if((new_node) && (existing_node)) {
                new_node->list = list;
                aux = existing_node->prev;
                existing_node->prev = new_node;
                new_node->prev = aux;
                new_node->next = existing_node;
                aux->next = new_node;
                list->nodes++;
        }

        return new_node;
}

flnode *add_head(flist *list, flnode *node) {
        node->prev = NULL;
        node->next = list->head;
        node->list = list;
        list->head = node;

        if(!(list->tail)) {
                list->tail = node;
        }
        else {
                (node->next)->prev = node;
        }

        list->nodes++;

        return node;
}

flnode *add_tail(flist *list, flnode *node) {
        node->prev = list->tail;
        node->next = NULL;
        node->list = list;
        list->tail = node;

        if(!(list->head)) {
                list->head = node;
        }
        else {
                (node->prev)->next = node;
        }

        list->nodes++;

        return node;
}

int is_tail_node(flnode *node) {
        return ((node) && !(node->next));
}

flnode *set_payload(flnode *node, void *ptr) {
        if(node) {
                SETPAYLOAD(node, ptr);
                return node;
        }

        return NULL;
}

void free_list(flist *list) {
        flnode *p;

        if(list) {
                ITERATE_AND_FREE(list, p, ;);
                free(list);
        }
}

/**
 *  This funtion is a specific case, when all the payloads of the nodes
 *  are allocated and must be freed. A simple example of this is a list
 *  which all the nodes are strings built with asprintf().
 *  WARNING: This function may be dangerous. Do not use if < 203% sure
 */
void free_list_and_all_payloads(flist *list) {
        flnode *p;

        if(list) {
                ITERATE_AND_FREE(list, p, {
                        free(PAYLOAD(p));      /* /!\ */
                });
                free(list);
        }
}

/**
 *  The difference between free_nodes() and free_list() is that
 *  free_nodes() does not free the struct of the list (flist node)
 *  so, we can reuse this for other completely different list
 */
flist *free_nodes(flist * list) {
        flnode *p;

        if(list) {
                ITERATE_AND_FREE(list, p, ;);
                init_list(list);
        }

        return list;
}

