/* -------------------------------------------------------------------------- */
/*                   Nome: Rafael Rodrigues - Numero: 89532                   */
/*                                                                            */
/*                                 2º Projeto                                 */
/*              Introducao aos Algoritmos e Estruturas de Dados               */
/* -------------------------------------------------------------------------- */

/* ------------------------------ BIBLIOTECAS ------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/* --------------------------------- MACROS --------------------------------- */

#define DESC_MAX 8001

/* --------------------------- VARIAVEIS GLOBAIS ---------------------------- */

typedef struct {
    unsigned long id, duration, dependencies[10000];
    char description[DESC_MAX];
} task;

/* -------------------- DECLARACAO DAS FUNCOES PRINCIPAIS ------------------- */

void command_add();

void command_duration();

void command_depend();

void command_remove();

void command_path();

/* ------------------- DECLARACAO DAS FUNCOES AUXILIARES -------------------- */

void clear(int *command_control);

/* ---------------------------------- MAIN ---------------------------------- */

int main() {
    /*
     * Funcao: main
     * ------------
     *
     */
    char command[9];
    int command_control;
    while (strcmp(command, "exit") != 0) {
        command_control = 0;
        scanf("%s", command);
        while (command_control == 0) {
            if (strcmp(command, "add") == 0) {
                command_add(); clear(&command_control);
            } else if (strcmp(command, "duration") == 0) {
                command_duration(); clear(&command_control);
            } else if (strcmp(command, "depend") == 0) {
                command_depend(); clear(&command_control);
            } else if (strcmp(command, "remove") == 0) {
                command_remove(); clear(&command_control);
            } else if (strcmp(command, "path") == 0) {
                command_path(); clear(&command_control);
            } else if (strcmp(command, "exit") != 0) {
                printf("illegal arguments\n"); clear(&command_control);
            }
            command_control = 1;
        }
    }
    return 0;
}

/* -------------------- DEFINICAO DAS FUNCOES PRINCIPAIS -------------------- */

void command_add() {
    int command_control = 0, contador = 0;
    unsigned long id, duration;
    char description[DESC_MAX];
    if (scanf(" %lu", &id) == 1) {
        contador++;
    } else {
        clear(&command_control);
    }
    if (scanf(" %s", description) == 1) {
        contador++;
    } else {
        clear(&command_control);
    }
    if (scanf(" %lu", &duration) == 1) {
        contador++;
    } else {
        clear(&command_control);
    }
    printf("Add %lu %s %lu\n", id, description, duration);
}

void command_duration() {
    int command_control = 0;
    char espaco;
    unsigned long value;
    espaco = getchar();
    if (espaco == '\n') {
        value = 0;
    } else {
        scanf("%lu", &value);
    }
    clear(&command_control);
    if (command_control == 0) {
        printf("%lu\n", value);
    } else if (command_control == 1) {
        printf("illegal arguments\n");
    }

}

void command_depend() {
    unsigned long id;
    scanf("%lu", &id);
    printf("%lu\n", id);
}

void command_remove() {
    unsigned long id;
    scanf("%lu", &id);
    printf("%lu\n", id);
}

void command_path() {
    printf("Path\n");
}

/* -------------------- DEFINICAO DAS FUNCOES AUXILIARES -------------------- */

void clear(int *command_control) {
    char nova_linha;
    nova_linha = getchar();
    if (nova_linha == '\n') {
        *command_control = 0;
    } else {
        while (getchar() != '\n') {
            *command_control = 1;
        }
    }
}

/* -------------------------------------------------------------------------- */
