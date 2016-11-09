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

#include <stddef.h>                     // For 'offsetof' macro

#define PAYLOAD(node)                   (node->payload)
#define SETPAYLOAD(node, ptr)           (PAYLOAD(node) = ((void*) ptr))
#define GETPADDR(node, type)            ((type*)(&(node->payload)))
#define GETPAYLOAD(node, type)          ((type*)(node->payload))
#define GETBASEADDR(ptr, member)        (flnode *) ((char *)ptr - offsetof(flnode, member))

#define ITERATE(__list, __fn0, __code)                  \
    do {                                                \
        if(__list != NULL) {                            \
            for(__fn0 = __list->head;                   \
            __fn0 != NULL; __fn0 = __fn0->next) __code  \
        }                                               \
    } while(0)

#define ITERATE_AND_FREE(__list, __fn0, __code) \
    do {                                        \
        flnode *__fnextn;                       \
        if(__list != NULL) {                    \
            for(__fn0 = __list->head;           \
            __fn0 != NULL; __fn0 = __fnextn) {  \
                __fnextn = __fn0->next;         \
                __code                          \
                free(__fn0);                    \
            }                                   \
        }                                       \
    } while(0)


/**
 * Fervagar LinkedList v2
 * This is an implementation of a doubly linked list of generic payload.
 * It means that you can save whatever, any type of data. Each node contains
 * a void * pointer and we can assign to it a pointer to any data type. 
 * 
 * These are the explanations of the macros:
 *  PAYLOAD(node)         -> Get the direct pointer to the payload field
 *
 *  SETPAYLOAD(node, ptr) -> Set the pointer passed as argument to the payload
 *
 *  GETPADDR(node, type)    -> Get the address of the payload field (of the struct)
 *                           This is useful in certain situations. See below.
 *
 *  GETPAYLOAD(node, type)  -> Get the actual pointer address.
 *
 *  GETBASEADDR(ptr, member) -> This macro is borrowed from the Linux kernel.
 *      This is a custom version of the 'container_of' macro, useful to get
 *      the base address of a node. This is an example of usage:
 *          flnode node = GETBASEADDR(GETPADDR(list->head, char *), payload);
 *      Obviously the example is useless since we have 'list->head', but
 *      we can get the node base address from a pointer of any of the node members.
 *
 *  -----------------------------------------------------------------------------
 *
 *  The difference between GETPADDR and GETPAYLOAD macros are the following:
 *  Imagine we have a string variable: [ char *string = "some_string"; ]
 *  We can create a linked list, so each node points to a char of this string...
 *      struct flist *l = create_list();
 *      add_tail(l, create_node(&string[i])); <- For each index of 'string'
 *  In this example, GETPADDR will give us the address of the 'payload' field
 *  INSIDE the struct. On the other hand, GETPAYLOAD will give us the actual
 *  stored pointer (i.e. the pointer * to the char value).
 *  
 *  In other words, the following statements are equivalent:
 *      GETPAYLOAD(node, char) <==> *GETPADDR(p, char *)
 */

typedef struct flnode_ flnode;
typedef struct flist_ flist;

struct flnode_ {
        flnode *prev;
        flnode *next;
        flist *list;
        void *payload;
};

struct flist_ {
        flnode *head;
        flnode *tail;
        size_t nodes;
};

flist *create_list();
flnode *create_node(void *ptr);
flnode *delete_node(flnode * node);
flnode *add_before(flist * list, flnode * new_node, flnode * existing_node);
flnode *add_head(flist * list, flnode * node);
flnode *add_tail(flist * list, flnode * node);
flnode *set_payload(flnode * node, void *ptr);
void free_list(flist * list);
void free_list_and_all_payloads(flist * list);
flist *free_nodes(flist * list);
