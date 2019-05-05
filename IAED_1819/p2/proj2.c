/*
 * File:  proj2.c
 * Author:  Rafael Rodrigues 89532
 * Description: Segundo Projeto da cadeira de IAED
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *next;
    char *name;
    char *local;
    char *domain;
    char *phone;
} *link;


/* -------------------------------------------------------------------------- */


link __a__(link head, char *name, char *local, char *domain, char *phone);

void __l__(link head);

void __p__(link head, char *name);

link __r__(link head, char *name);

void __e__(link head, char *name, char *new_local, char *new_domain);

void __c__(link head, char *domain);


/* -------------------------------------------------------------------------- */


char* readString();


link verifyName(link head, char *name);


void printContact(link head);


char* strdup (const char *s);


void freeNode(link head);


link freeList(link head);


/* -------------------------------------------------------------------------- */


int main(int argc, char *argv[]) {
    link head = NULL;
    char command, *input1, *input2, *input3, *input4;

    while (command != 'x') {

        if (argc > 1) {
            command = argv[1][0];
            if (command != 'l' && command != 'x') {
                input1 = strdup(argv[2]);
                if (command != 'p' && command != 'r' && command != 'c' ) {
                    input2 = strtok(argv[3], "@");
                    input3 = strtok(NULL, "");
                    if (command != 'e') {
                        input4 = strdup(argv[4]);
                    }
                }
            }
            argc = 0;
        } else {
            scanf("%c", &command);
            getchar();
            if (command != 'l' && command != 'x') {
                input1 = readString();
                if (command != 'p' && command != 'r' && command != 'c' ) {
                    input2 = readString();
                    input3 = readString();
                    if (command != 'e') {
                        input4 = readString();
                    }
                }
            }
        }

        switch (command) {
            case 'a':
                head = __a__(head, input1, input2, input3, input4);
                break;
            case 'l':
                __l__(head);
                break;
            case 'p':
                __p__(head, input1);
                break;
            case 'r':
                head = __r__(head, input1);
                break;
            case 'e':
                __e__(head, input1, input2, input3);
                break;
            case 'c':
                __c__(head, input1);
                break;
        }
    }

    freeList(head);
    return 0;
}


/* -------------------------------------------------------------------------- */


link __a__(link head, char *name, char *local, char *domain, char *phone) {
    link aux, newnode = (link) malloc(sizeof(struct node));

    newnode->next = NULL;
    newnode->name = strdup(name);
    newnode->local = strdup(local);
    newnode->domain = strdup(domain);
    newnode->phone = strdup(phone);

    if (head == NULL) {
        return newnode;
    } else {
        if (verifyName(head, name) == NULL) {
            for(aux = head; aux->next != NULL; aux = aux->next);
            aux->next = newnode;
        } else {
            freeNode(newnode);
            puts("Nome existente.");
        }
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
    link contact;

    contact = verifyName(head, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        printContact(contact);
    }
}


link __r__(link head, char *name) {
    link contact, aux;

    contact = verifyName(head, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
        return head;
    } else {
        if (contact == head) {
            head = contact->next;
        } else {
            for(aux = head; aux->next != NULL &&
                aux->next != contact; aux = aux->next);
            aux->next = aux->next->next;
        }
        freeNode(contact);
        return head;
    }
}


void __e__(link head, char *name, char *new_local, char *new_domain) {
    link contact;

    contact = verifyName(head, name);
    if (contact == NULL) {
        puts("Nome inexistente.");
    } else {
        contact->local = strdup(new_local);
        contact->domain = strdup(new_domain);
    }
}


void __c__(link head, char *domain) {
    link aux;
    int count = 0;

    for(aux = head; aux != NULL; aux = aux->next) {
        if (strcmp(aux->domain, domain) == 0) {
            count++;
        }
    }
    printf("%s:%d\n", domain, count);
}


/* -------------------------------------------------------------------------- */


char* readString() {
    int length = 1, count = 0;
    char * string = (char*) malloc(sizeof(char) * length), c;

    while((c = getchar()) != '\n' && c != EOF && c != ' ' && c != '@'){
        string = realloc(string, sizeof(char) * length++);
        string[count++] = c;
    } string[count] = '\0';
    return string;
}


link verifyName(link head, char *name) {
    link aux;

    for (aux = head; aux != NULL; aux = aux->next) {
        if (strcmp(aux->name, name) == 0) {
            return aux;
        }
    }
    return NULL;
}


void printContact(link head) {
    printf("%s %s@%s %s\n", head->name, head->local, head->domain, head->phone);
}


char* strdup (const char *s) {
    int slen = strlen(s);
    char *result = malloc(slen + 1);

    if(result == NULL) {
        return NULL;
    }
    memcpy(result, s, slen+1);
    return result;
}


void freeNode(link head) {
    free(head->name);
    free(head->local);
    free(head->domain);
    free(head->phone);
    free(head);
}


link freeList(link head) {
    link temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        freeNode(temp);
    }

    return head;
}


/* -------------------------------------------------------------------------- */
