/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "contact.h"

/* -------------------------------------------------------------------------- */


link __a__(link head, char *name, char *local, char *domain, char *phone);

void __l__(link head);

void __p__(link head, char *name);

link __r__(link head, char *name);

link __e__(link head, char *name, char *new_local, char *new_domain);

void __c__(link head, char *domain);


/* -------------------------------------------------------------------------- */


char* readString(int max_size);


int verifyEmail(char *local, char *domain);


void printContact(link head);


int listLength(link head);


char* strdup (const char* s);

/* -------------------------------------------------------------------------- */

int main() {
    char command;
    char *name, *local, *domain, *phone;
    link head = NULL;

    while (scanf("%c", &command) && command != 'x') {

        getchar();
        switch (command) {

            case 'a':
                name = readString(MAX_NAME);
                local = readString(MAX_LOCDOM);
                domain = readString(MAX_LOCDOM);
                phone = readString(MAX_PHONE);
                if (verifyEmail(local, domain) == 0) {
                    return 1;
                }
                head = __a__(head, name, local, domain, phone);
                break;

            case 'l':
                __l__(head);
                break;

            case 'p':
                name = readString(MAX_NAME);
                __p__(head, name);
                break;

            case 'r':
                name = readString(MAX_NAME);
                __r__(head, name);
                break;

            case 'e':
                name = readString(MAX_NAME);
                local = readString(MAX_LOCDOM);
                domain = readString(MAX_LOCDOM);
                if (verifyEmail(local, domain) == 0) {
                    return 1;
                }
                __e__(head, name, local, domain);
                break;

            case 'c':
                domain = readString(MAX_LOCDOM);
                __c__(head, domain);
                break;
        }
    }
    return 0;
}


/* -------------------------------------------------------------------------- */


link __a__(link head, char *name, char *local, char *domain, char *phone) {
    link temp, newnode = (link) malloc(sizeof(struct node));

    newnode->next = NULL;
    newnode->name = strdup(name);
    newnode->local = strdup(local);
    newnode->domain = strdup(domain);
    newnode->phone = strdup(phone);

    if (head == NULL) {
        return newnode;
    } else {
        for(temp = head; temp->next != NULL; temp = temp->next) {
            if (strcmp(temp->name, name) == 0) {
                puts("Nome existente.");
                return head;
            }
        }
        temp->next = newnode;
        return head;
    }
}


void __l__(link head) {
    while (head != NULL) {
        printContact(head);
        head = head->next;
    }
}


void __p__(link head, char *name) {
    link temp;

    for(temp = head; temp != NULL; temp = temp->next) {
        if (strcmp(temp->name, name) == 0) {
            printContact(temp);
            return;
        }
    }
    puts("Nome inexistente.");
}


link __r__(link head, char *name) {
    link temp, aux;

    for(temp = head; temp != NULL; temp = temp->next) {
        if (strcmp(temp->name, name) == 0) {
            aux = temp;
            free(aux);
            return head;
        }
    }
    puts("Nome inexistente.");
    return head;
}


link __e__(link head, char *name, char *new_local, char *new_domain) {
    link temp;

    for(temp = head; temp != NULL; temp = temp->next) {
        if (strcmp(temp->name, name) == 0) {
            temp->local = strdup(new_local);
            temp->domain = strdup(new_domain);
            return head;
        }
    }
    puts("Nome inexistente.");
    return head;
}


void __c__(link head, char *domain) {
    link temp;
    int count = 0;

    for(temp = head; temp != NULL; temp = temp->next) {
        if (strcmp(temp->domain, domain) == 0) {
            count++;
        }
    }
    printf("%s:%d\n", domain, count);
}

/* -------------------------------------------------------------------------- */


char* readString(int max_size) {
    int length = 1, count = 0;
    char * string = (char*) malloc(sizeof(char) * length), c;

    while(count < max_size && (c = getchar()) != '\n' && c != ' ' && c != '@'){
       string = realloc(string, sizeof(char) *  length++);
       string[count++] = c;
   } string[count] = '\0';
    return string;
}


int verifyEmail(char *local, char *domain) {
    int length;

    length = strlen(local) + strlen(domain);
    if (length > MAX_LOCDOM) {
        return 0;
    } else {
        return 1;
    }
}


void printContact(link head) {
    printf("%s %s@%s %s\n", head->name, head->local, head->domain, head->phone);
}


int listLength(link head) {
    int count = 0;
    link temp;
    for (temp = head; temp != NULL; temp = temp->next) {
        count++;
    }
    return count;
}


char* strdup (const char* s) {
    size_t slen = strlen(s);
    char* result = malloc(slen + 1);
    if(result == NULL) {
        return NULL;
    }

    memcpy(result, s, slen+1);
    return result;
}

/* -------------------------------------------------------------------------- */
