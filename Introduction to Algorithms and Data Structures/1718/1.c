/* -------------------------------------------------------------------------- */
/*                   Nome: Rafael Rodrigues - Numero: 89532                   */
/*                                                                            */
/*                                 1º Projeto                                 */
/*              Introducao aos Algoritmos e Estruturas de Dados               */
/* -------------------------------------------------------------------------- */

/* ------------------------------ BIBLIOTECAS ------------------------------- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/* --------------------------------- MACROS --------------------------------- */

#define MAX_ELEMENTS 10000
#define MAX_FILENAME 81
#define MAX_OFFSET 20000
#define MAX_LINES (maxi + 1 - mini)
#define MAX_COLUMNS (maxj + 1 - minj)

/* --------------------------- VARIAVEIS GLOBAIS ---------------------------- */

unsigned long mini, minj, maxi, maxj, nelem, size;
double zero, dens;
char parameter[10], temporary_filename[MAX_FILENAME], filename[MAX_FILENAME];
int filename_control;

typedef struct {
    unsigned long line;
    unsigned long column;
    double value;
} values;

values elements[MAX_ELEMENTS];

typedef struct {
    unsigned long elements;
    unsigned long index;
} lines;

/* -------------------- DECLARACAO DAS FUNCOES PRINCIPAIS ------------------- */

void add_element(unsigned long line, unsigned long column, double value);

void list_elements();

void info_matrix();

void list_line(unsigned long line);

void list_column(unsigned long column);

void sort_matrix();

void redefine_zero(double value);

void compress_matrix();

void save_matrix();

/* ------------------- DECLARACAO DAS FUNCOES AUXILIARES -------------------- */

void calc_mini();

void calc_minj();

void calc_maxi();

void calc_maxj();

void calc_size();

void calc_dens();

void refresh_info();

void sort(values array[MAX_ELEMENTS], int option, unsigned long size);

/* ---------------------------------- MAIN ---------------------------------- */

int main(int argc, char const *argv[]) {
    /*
     * Funcao: main
     * ------------
     * A linha de comandos permite a indicação opcional do nome do ficheiro de
     * entrada com os elementos da matriz a ler. Durante a execução do programa
     * as instruções são lidas do terminal (standard input) na forma de um
     * conjunto de linhas iniciadas por um carácter, que se passa a designar
     * por comando, seguido de um número de informações dependente do comando
     * a executar. Os comandos disponíveis são a, p, i, l, c, o, z, s, w, q.
     */
    char command;
    unsigned long line, column;
    double value;
    zero = 0; nelem = 0; filename_control = 0;
    if (argc == 2) {
        FILE *read = fopen(argv[1], "r");
        if (read != NULL) {
            strcpy(temporary_filename, argv[1]); filename_control = 1;
            while (3 == fscanf(read, "%lu %lu %lf", &line, &column, &value)) {
                add_element(line, column, value);
            }
            fclose(read);
        }
    }
    while (command != 'q') {
        scanf("%c", &command);
        switch (command) {
            case 'a':
                scanf("%lu %lu %lf", &line, &column, &value);
                add_element(line, column, value); break;
            case 'p':
                list_elements(); break;
            case 'i':
                info_matrix(); break;
            case 'l':
                scanf("%lu", &line);
                list_line(line); break;
            case 'c':
                scanf("%lu", &column);
                list_column(column); break;
            case 'o':
                fgets(parameter, sizeof parameter, stdin);
                sort_matrix(); break;
            case 'z':
                scanf("%lf", &value);
                redefine_zero(value); break;
            case 's':
                compress_matrix(); break;
            case 'w':
                fgets(filename, sizeof filename, stdin);
                save_matrix(filename_control); break;
        }
    }
    return 0;
}

/* -------------------- DEFINICAO DAS FUNCOES PRINCIPAIS -------------------- */

void add_element(unsigned long line, unsigned long column, double value) {
    /*
     * Funcao: add_element
     * -------------------
     * Adiciona o elemento na referida posicao da matriz. Se ja
     * existir um elemento nessa posicao, este sera substituido pelo novo valor.
     * Se ja existir um elemento nessa posicao e o valor a substituir for igual
     * a zero o elemento e removido.
     *
     * Argumentos: a linha, a coluna e o valor do elemento a adicionar.
     * Output: Nao tem.
     */
    int i, j, control = 0;
    refresh_info();
    if (value == zero) {
        for (i = 0; i < nelem; i++) {
            if (elements[i].line == line && elements[i].column == column) {
                for (j = i; j < nelem - 1; j++) {
                    elements[j] = elements[j + 1];
                }
                elements[nelem - 1].value = zero;
                nelem--;
            }
        }
    } else {
        if (nelem == 0) {
            elements[nelem].line = line;
            elements[nelem].column = column;
            elements[nelem].value = value;
            nelem++;
        } else {
            for (i = 0; i < nelem; i++) {
                if (elements[i].line == line && elements[i].column == column) {
                    elements[i].value = value; control = 1;
                }
            }
            if (control == 0) {
                elements[nelem].line = line;
                elements[nelem].column = column;
                elements[nelem].value = value;
                nelem++;
            }
        }
    }
}

void list_elements() {
    /*
     * Funcao: list_elements
     * ---------------------
     * Argumentos: Nao tem.
     * Output: Lista todos os elementos representaveis da matriz, ou seja, nao
     * zero, pela ordem em que se encontrem. Caso a matriz nao tenha nenhum
     * elemento diferente de zero a funcao imprime a mensagem "empty matrix".
     */
    int i;
    refresh_info();
    if (nelem == 0) {
        printf("empty matrix\n");
    } else {
        for (i = 0; i < nelem; i++) {
            printf("[%lu;%lu]=%.3lf\n", elements[i].line,
            elements[i].column, elements[i].value);
        }
    }
}

void info_matrix() {
    /*
     * Funcao: info_matrix
     * -------------------
     * Argumentos: Nao tem.
     * Output: Imprime numa linha as caracteristicas da matriz, onde <mini>,
     * <minj>, <maxi>, <maxj> sao os menores e maiores indices de todos os
     * elementos representados, ou seja os limites da matriz; <nelem> e o numero
     * de elementos efectivamente representados e <size> e a dimensao da matriz
     * completa. A densidade <dens> e o racio entre <nelem> e <size>.
     * Caso a matriz nao tenha nenhum elemento diferente de zero a funcao
     * imprime a mensagem "empty matrix".
     */
    refresh_info();
    if (nelem == 0) {
        printf("empty matrix\n");
    } else {
        printf("[%lu %lu] [%lu %lu] %lu / %lu = %.3f%%\n",
        mini, minj, maxi, maxj, nelem, size, dens * 100);
    }
}

void list_line(unsigned long line) {
    /*
     * Funcao: list_line
     * -----------------
     * Argumentos: linha a ser imprimida.
     * Output: Imprime a linha completa da matriz, incluindo os elementos
     * zero, numa so linha do terminal. Se so houver elementos zero na linha,
     * ou a linha indicada estiver fora dos limites, e impressa a mensagem
     * "empty line".
     */
    int i, counter = 0, control;
    refresh_info();
    for (i = 0; i < nelem; i++) {
        if (elements[i].line == line && elements[i].value != zero) {
            counter++;
        }
    }
    if (counter == 0) {
        printf("empty line\n");
    } else {
        while (minj <= maxj) {
            control = 0;
            for (i = 0; i < nelem; i++) {
                if ((elements[i].line == line)
                && (elements[i].column == minj)
                && (elements[i].value != zero)) {
                    printf(" %.3lf", elements[i].value); control = 1;
                }
            }
            if (control == 0) {
                printf(" %.3lf", zero);
            }
            minj++;
        }
        printf("\n");
    }
}

void list_column(unsigned long column) {
    /*
     * Funcao: list_column
     * -------------------
     * Argumentos: coluna a ser imprimida.
     * Output: Imprime a coluna completa da matriz, incluindo os elementos
     * zero. Se so houver elementos zero na coluna, ou a coluna indicada estiver
     * fora dos limites, e impressa a mensagem "empty column".
     */
    int j, counter = 0, control;
    refresh_info();
    for (j = 0; j < nelem; j++) {
        if (elements[j].column == column  && elements[j].value != zero) {
            counter++;
        }
    }
    if (counter == 0) {
        printf("empty column\n");
    } else {
        while (mini <= maxi) {
            control = 0;
            for (j = 0; j < nelem; j++) {
                if ((elements[j].line == mini)
                && (elements[j].column == column)
                && (elements[j].value != zero)) {
                    printf("[%lu;%lu]=%.3lf\n",
                    elements[j].line, elements[j].column, elements[j].value);
                    control = 1;
                }
            }
            if (control == 0) {
                printf("[%lu;%lu]=%.3lf\n", mini, column, zero);
            }
            mini++;
        }
    }
}

void sort_matrix() {
    /*
     * Funcao: sort_matrix
     * -------------------
     * Se nao for indicado o parametro, ordena por ordem crescente as linhas e,
     * dentro de cada linha, por ordem crescente as colunas. Se for indicado o
     * parâmetro column, ordena por ordem crescente as colunas e, dentro de cada
     * coluna, por ordem crescente as linhas.
     *
     * Argumentos: pode ou nao ter um parametro.
     * Output: Nao tem.
     */
    refresh_info();
    if (parameter[0] == '\n') {
        sort(elements, 1, nelem); sort(elements, 0, nelem);
    } else {
        sort(elements, 0, nelem); sort(elements, 1, nelem);
    }
}

void redefine_zero(double value) {
    /*
     * Funcao: redefine_zero
     * ---------------------
     * Altera o valor do elemento zero. Se existir algum elemento com o valor
     * do novo elemento zero, este elemento e eliminado.
     *
     * Argumentos: valor do novo elemnto zero.
     * Output: Nao tem.
     */
    int i, j, counter = 0;
    refresh_info();
    if (nelem > 0) {
        for (i = 0; i < nelem; i++) {
            if (elements[i].value == value) {
                counter++;
            }
        }
        while (counter > 0) {
            for (i = 0; i < nelem; i++) {
                if (elements[i].value == value) {
                    for (j = i; j < nelem; j++) {
                        elements[j] = elements[j + 1];
                    }
                    nelem--;
                    counter--;
                }
            }
        }
    }
    zero = value;
}

void compress_matrix() {
    /*
     * Funcao: compress_matrix
     * -----------------------
     * Comprime a matriz.
     *
     * Argumentos: Nao tem.
     * Output: Imprime uma linha de valores, uma linha de indices, e uma linha
     * de offsets, que representam a compactacao da matriz esparsa. Caso a
     * densidade da matriz seja superior a 50% nao e aplicado o algoritmo de
     * compressao e imprime a mensagem "dense matrix".
     */
    refresh_info();
    lines list_lines[MAX_LINES], swap[1];
    values temporary[MAX_ELEMENTS];
    double list_values[MAX_OFFSET];
    unsigned long list_offset[MAX_LINES], i, j, k, lines_count, control,
    counter, line_index, line_elements, offset, larger_offset, position;
    long long list_index[MAX_OFFSET], l;
    if (dens > 0.5) {
        printf("dense matrix\n");
    } else {
        i = 0; lines_count = 0;
        for (j = mini; j < maxi + 1; j++) {
            control = 0; list_lines[i].elements = 0;
            for (k = 0; k < nelem; k++) {
                if (elements[k].line == j) {
                    list_lines[i].index = j;
                    list_lines[i].elements++;
                    control = 1;
                }
            }
            if (control > 0) {
                lines_count++;
                i++;
            }
        }
        for (i = 1; i < lines_count; i++) {
            swap[0] = list_lines[i]; l = i - 1;
            while ((l >= 0) && (swap[0].elements > list_lines[l].elements)) {
                list_lines[l + 1] = list_lines[l]; l--;
            }
            list_lines[l + 1] = swap[0];
        }
        for (i = 0; i < MAX_OFFSET; i++) {
            list_values[i] = 0;
            list_index[i] = -1;
        }
        for (i = 0; i < MAX_LINES; i++) {
            list_offset[i] = 0;
        }
        larger_offset = 0;
        for (i = 0; i < lines_count; i++) {
            line_index = list_lines[i].index;
            line_elements = list_lines[i].elements;
            if (line_elements > 0) {
                j = 0;
                while (j < line_elements) {
                    for (k = 0; k < nelem; k++) {
                        if (elements[k].line == line_index) {
                            temporary[j] = elements[k];
                            j++;
                        }
                    }
                }
                sort(temporary, 1, line_elements);
                counter = 0; offset = 0;
                while (counter < line_elements) {
                    for (j = 0; j < line_elements; j++) {
                        position = temporary[j].column - minj;
                        if (list_index[position + offset] == -1) {
                            counter++;
                        } else if ((list_index[position + offset] != -1)) {
                            offset++;
                        }
                    }
                }
                for (j = 0; j < line_elements; j++) {
                    position = temporary[j].column - minj;
                    list_values[position + offset] = temporary[j].value;
                    list_index[position + offset] = line_index;
                }
                list_offset[line_index - mini] = offset;
                if (larger_offset < offset) {
                    larger_offset = offset;
                }
            }
        }
        printf("value =");
        for (i = 0; i < MAX_COLUMNS + larger_offset; i++) {
            if (list_index[i] == -1) {
                printf(" %.3f", zero);
            } else {
                printf(" %.3f", list_values[i]);
            }
        } printf("\n");
        printf("index =");
        for (i = 0; i < MAX_COLUMNS + larger_offset; i++) {
            if (list_index[i] == -1) {
                printf(" 0");
            } else {
                printf(" %llu", list_index[i]);
            }
        } printf("\n");
        printf("offset =");
        for (i = 0; i < MAX_LINES; i++) {
            printf(" %lu", list_offset[i]);
        } printf("\n");
    }
}

void save_matrix() {
    /*
     * Funcao: save_matrix
     * -------------------
     * Salvaguarda a lista de elementos representados num ficheiro.
     * O nome do ficheiro pode ser omitido caso ja tenha sido definido por um
     * comando w anterior ou caso tenha sido indicado na linha de comandos.
     * O ficheiro sera criado, caso nao exista, ou re-criado, caso ja exista.
     *
     * Argumentos: pode ou nao ser dado um nome para o ficheiro.
     * Output: Nao tem.
     */
    int i;
    char default_filename[MAX_FILENAME] = "matrix.out";
    memmove(filename, filename + 1, strlen(filename));
    filename[strlen(filename) - 1] = 0;
    switch (filename_control) {
        case 0:
            if (strlen(filename) == 0) {
                strcpy(temporary_filename, default_filename);
                break;
            } else {
                strcpy(temporary_filename, filename);
                filename_control = 1;
                break;
            }
        case 1:
            if (strlen(filename) != 0) {
                strcpy(temporary_filename, filename);
                break;
            }
    }
    FILE *write = fopen(temporary_filename, "w");
    for (i = 0; i < nelem; i++) {
        if (elements[i].value != zero) {
            fprintf(write, "%lu %lu %f\n", elements[i].line,
            elements[i].column, elements[i].value);
        }
    }
    fclose(write);
}

/* -------------------- DEFINICAO DAS FUNCOES AUXILIARES -------------------- */

void calc_mini() {
    /*
     * Funcao: calc_mini
     * -----------------
     * Calcula o limite inferior das linhas representadas da matriz.
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    int i;
    mini = ULONG_MAX;
    for (i = 0; i < nelem; i++) {
        if (mini > elements[i].line && elements[i].value != zero) {
            mini = elements[i].line;
        }
    }
}

void calc_minj() {
    /*
     * Funcao: calc_minj
     * -----------------
     * Calcula o limite inferior das colunas representadas da matriz.
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    int j;
    minj = ULONG_MAX;
    for (j = 0; j < nelem; j++) {
        if (minj > elements[j].column && elements[j].value != zero) {
            minj = elements[j].column;
        }
    }
}

void calc_maxi() {
    /*
     * Funcao: calc_maxi
     * -----------------
     * Calcula o limite superior das linhas representadas da matriz.
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    int i;
    maxi = 0;
    for (i = 0; i < nelem; i++) {
        if (maxi < elements[i].line && elements[i].value != zero) {
            maxi = elements[i].line;
        }
    }
}

void calc_maxj() {
    /*
     * Funcao: calc_maxj
     * -----------------
     * Calcula o limite superior das colunas representadas da matriz.
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    int j;
    maxj = 0;
    for (j = 0; j < nelem; j++) {
        if (maxj < elements[j].column && elements[j].value != zero) {
            maxj = elements[j].column;
        }
    }
}

void calc_size() {
    /*
     * Funcao: calc_size
     * -----------------
     * Calcula a dimensao da matriz completa.
     * Uma matriz diz-se completa se todas as posicoes de todas as colunas
     * (entre <minj> e <maxj>) e de todas as linhas (entre <mini> e <maxi>)
     * estiverem representadas.
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    calc_maxi(); calc_mini(); calc_maxj(); calc_minj();
    size = ((maxi - mini + 1) * (maxj - minj + 1));
}

void calc_dens() {
    /*
     * Funcao: calc_size
     * -----------------
     * Calcula a densidade da matriz, ou seja, o racio entre o numero de
     * elementos representadas e a dimensao da matriz completa.
     * A densidade e representada em percentagem.
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    calc_size();
    dens = (double)nelem / (double)size;
}

void refresh_info() {
    /*
     * Funcao: refresh_info
     * --------------------
     * Atualiza todosos valores relativos as caracteristicas da matriz,
     * isto e, mini, minj, maxi, maxj, size e dens;
     *
     * Argumentos: Nao tem.
     * Output: Nao tem.
     */
    calc_mini(); calc_minj(); calc_maxi(); calc_maxj();
    calc_size(); calc_dens();
}

void sort(values array[MAX_ELEMENTS], int option, unsigned long size) {
    /*
     * Funcao: sort
     * ------------
     * Algoritmo de ordenacao (insertion sort).
     *
     * Argumentos: lista a ordenar, opcao* e tamanho da lista.
     * Output: Nao tem.
     *
     * *a opcao permite escolher entre ordenar linhas(0) ou colunas(1).
     */
    long long i, j;
    values swap[1];
    switch (option) {
        case 0:
            for (i = 1; i < size; i++) {
                swap[0] = array[i]; j = i - 1;
                while ((j >= 0) && (swap[0].line < array[j].line)) {
                    array[j + 1] = array[j]; j--;
                }
                array[j + 1] = swap[0];
            } break;
        case 1:
            for (i = 1; i < size; i++) {
                swap[0] = array[i]; j = i - 1;
                while ((j >= 0) && (swap[0].column < array[j].column)) {
                    array[j + 1] = array[j]; j--;
                }
                array[j + 1] = swap[0];
            } break;
    }
}

/* -------------------------------------------------------------------------- */
