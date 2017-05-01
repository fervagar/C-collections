/*
 * Copyright (C) 2017 Fernando Vañó García
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
 *	Fernando Vanyo Garcia <fernando@fervagar.com>
 */

#include <stdio.h>
#include "linkedlist.h"

// Simple Proof Of Concept showing an example of usage //

void print_list(flist *list) {
    flnode *p;
    char *c;
    int i;

    if(list->head == NULL) {
        printf("Empty list\n");
    }
    else{
        printf("# of nodes: %lu\n", list->nodes);

        for(i = 0, p = list->head; p != NULL; p = p->next, i++) {
            c = GETPAYLOAD(p, char);
            if(c != NULL) {
                printf("Element %d\t=>\t%c\n", i, *c);
            }
            else {
                printf("Element %d\t=>\tNULL\n", i);
            }
        }
    }
    return;
}

int main(int argc, char *argv[]) {
    char *string = "hello world";
    flist *l;
    int i;

    l = create_list();
    for(i = 0; string[i] != 0x00; i++) {
        add_tail(l, create_node((void *) &string[i]));
    }

    print_list(l);
    free_list(l);

    return 0;
}

